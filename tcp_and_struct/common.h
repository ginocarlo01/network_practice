#ifndef COMMON_H
#define COMMON_H

#include <stddef.h> 

int recv_full(int sock, void *buffer, size_t size);
int send_full(int sock, const void *buffer, size_t size);

#endif 
