#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <poll.h>

typedef struct socket {
    int fd;

    struct sockaddr_in addr;
    unsigned int addrlen;

    int yes;
} *socket_t;

typedef struct client {
    socket_t sock;
    struct pollfd fds[2];
} *client_t;

typedef struct server {
    socket_t sock;

    struct pollfd *fds;
    int fdc;
} *server_t;

server_t server;
client_t client;

socket_t create_socket(int port) {
    socket_t sock = (socket_t)malloc(sizeof(*sock));

    sock->fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock->fd == -1) {
        perror("Failed to create socket");
        exit(-1);
    }

    sock->addr.sin_family = AF_INET;
    if (port != 0)
        sock->addr.sin_port = htons(port);
    sock->addr.sin_addr.s_addr = htonl(INADDR_ANY);

    sock->addrlen = sizeof(sock->addr);

    sock->yes = 1;
    setsockopt(sock->fd, SOL_SOCKET, SO_REUSEADDR, &sock->yes, sizeof(int));
    setsockopt(sock->fd, SOL_SOCKET, SO_REUSEPORT, &sock->yes, sizeof(int));

    return sock;
}

void destroy_socket(socket_t sock) {
    free(sock);
}

server_t create_server(int port) {
    server_t server = (server_t)malloc(sizeof(*server));
    server->sock = create_socket(port);

    if (bind(server->sock->fd, (struct sockaddr*)&server->sock->addr, server->sock->addrlen) != 0) {
        perror("Failed to bind socket");
        exit(-1);
    }

    if (listen(server->sock->fd, SOMAXCONN) != 0) {
        perror("Failed to listen");
        exit(-1);
    }
    
    server->fds = (struct pollfd*)malloc(sizeof(struct pollfd));

    struct pollfd pfd = {
        .fd = server->sock->fd,
        .events = POLLIN
    };
    server->fds[0] = pfd;

    server->fdc = 1;

    return server;
}

void poll_server(server_t server) {
    poll(server->fds, server->fdc, -1);
    
    for (int i = 0; i < server->fdc; i++) {
        struct pollfd pfd = server->fds[i];

        if (pfd.revents & POLLIN) {
            if (pfd.fd == server->sock->fd) {
                /*
                * Accept new connections
                */
                int fd = accept(server->sock->fd, (struct sockaddr*)&server->sock->addr, &server->sock->addrlen);

                if (fd == -1) {
                    perror("Failed to connect client");
                    exit(-1);
                }

                server->fdc++;
                server->fds = (struct pollfd*)realloc(server->fds, server->fdc*sizeof(struct pollfd));

                struct pollfd pfd = {
                    .fd = fd,
                    .events = POLLIN
                };
                server->fds[server->fdc-1] = pfd;

                printf("Successfully connected client\n");
            } else {
                /*
                * Poll clients for data
                */
                char buf[256];
                if (recv(pfd.fd, buf, 256, 0) == -1) {
                    close(pfd.fd);
                    server->fds[i].fd = -1;
                    server->fdc--;
                    continue;
                }

                for (int n = 1; n < server->fdc; n++) {
                    if (server->fds[n].fd != pfd.fd) {
                        if (send(server->fds[n].fd, buf, 256, 0) == -1) {
                            perror("Failed to send data to clients");
                            exit(-1);
                        }
                    }
                }
            }
        }
    }
}

client_t create_client(int port) {
    client_t client = (client_t)malloc(sizeof(*client));

    client->sock = create_socket(port);
    
    if (connect(client->sock->fd, (struct sockaddr*)&client->sock->addr, client->sock->addrlen) == -1) {
        perror("Failed to connect to server");
        exit(-1);
    }
    
    struct pollfd fds[2] = {
        {
            .fd = STDIN_FILENO,
            .events = POLLIN
        },
        {
            .fd = client->sock->fd,
            .events = POLLIN
        }
    };
    memcpy(client->fds, fds, sizeof(fds));

    return client;
}

void poll_client(client_t client) {
    poll(client->fds, 2, -1);

    if (client->fds[0].revents & POLLIN) {
        char buf[256];
        int bred = read(STDIN_FILENO, buf, 255);

        buf[bred] = 0;

        if (send(client->sock->fd, buf, 256, 0) == -1) {
            perror("Failed to send data");
            exit(-1);
        }
    }

    if (client->fds[1].revents & POLLIN) {
        char buf[256];
        int brec = recv(client->sock->fd, buf, 255, 0);

        if (brec == -1) {
            perror("Failed to receive data");
            exit(-1);
        }

        buf[brec] = 0;

        printf("%s", buf);
    }
}

void cleanup_server() {
    free(server->fds);
    destroy_socket(server->sock);
    free(server);
}

void cleanup_client() {
    destroy_socket(server->sock);
    free(client);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Invalid arguments. Wanted 3, got %d.", argc);
        exit(-1);
    }

    if (strcmp(argv[1], "-s") == 0) {
        server = create_server(atoi(argv[2]));

        atexit(cleanup_server);

        while (1) {
            poll_server(server);
        }
    } else if (strcmp(argv[1], "-c") == 0) {
        client = create_client(atoi(argv[2]));

        atexit(cleanup_client);

        while (1) {
            poll_client(client);
        }
    } else {
        fprintf(stderr, "Invalid argument: %s", argv[1]);
        exit(-1);
    }

    return 0;
}