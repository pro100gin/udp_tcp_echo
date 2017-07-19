#ifndef SERVER_CORE_H
#define SERVER_CORE_H
#define _SVID_SOURCE
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>

#include <netdb.h>

#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/epoll.h>
#include <sys/poll.h>

#include <arpa/inet.h>
#include <time.h>

#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <netinet/in.h>
#include <time.h>
#include <sys/select.h>

#define SERVER_IP "192.168.2.83"
#define SERVER_PORT 7778

#define MAX_MSG_SIZE 256

typedef struct ci {
	int sd_cln;
	struct sockaddr_in cln_addr;
} CLIENT_INFO;

void* cln_hndl(void* args);

#endif
