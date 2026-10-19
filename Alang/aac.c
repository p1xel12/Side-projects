#include "types.h"
#include "scan.h"
#include "../list.h"

int main(int argc, char *argv[]) {
    printf("AAC main\n");
    token_table_t tokens = scan_file(argv[1]);

    for (int i = 0; i < tokens.tokenc; i++) {
        printf("%d\n", tokens.tokens[i].token);
    }

    free_token_table(tokens);

    return 0;
}