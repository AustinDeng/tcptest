#include "append.h"
#include "server.h"

int main() {  
    append_data("Hello, World!\n");
    server_main();
    return 0;
}