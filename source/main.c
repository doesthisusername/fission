#include <stdbool.h>
#include <stdio.h>
#include "backend.h"
#include "types.h"

s32 main() {
    struct nk_context* ctx;
    //struct nk_colorf bg;
    //struct nk_image img;

    if(!init(&ctx)) {
        fprintf(stderr, "failed to init nk_context\n");
    }

    shutdown();
    return 0;
}
