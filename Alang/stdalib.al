#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef __GNUC__
    #define bool _Bool
    #define void* any*
#endif

extern int dynlen(void *ptr);