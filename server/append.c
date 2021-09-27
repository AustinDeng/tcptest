#include "append.h"

int append_data(char* data) {
    FILE *fb2c = NULL;

    fb2c = fopen(STORE_DIR, "a+");
    if(fb2c != NULL) {
        fprintf(fb2c, "%s", data);
        fclose(fb2c);
        printf("Done!\n");
    } else {
        printf(STORE_DIR);
        printf("error\n");
    }
    
    return 0;
}