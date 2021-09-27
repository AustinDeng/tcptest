#ifndef SERVER__H
#define SERVER__H

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#include "append.h"
#include "config.h"

int server_main();

#endif