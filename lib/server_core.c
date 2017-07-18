#include "../include/server_core.h"

void* cln_hndl(void* args) {
	CLIENT_INFO* cln_info = NULL;
	ssize_t bts = 0;
	char msg[MAX_MSG_SIZE];

	cln_info = (CLIENT_INFO*) args;

	while(1) {
		memset(msg, 0, MAX_MSG_SIZE);

		bts = recv(cln_info->sd_cln, msg, MAX_MSG_SIZE, 0);
		if (bts == -1) {
			perror("#TCP# Server: recv(client)");

			close(cln_info->sd_cln);
			free(cln_info);

			pthread_exit((void*)EXIT_FAILURE);
		}

		printf("#TCP# Server: recived message from client: '%s' (%d bytes)\n", msg, (int) bts);

		if (!strcmp(msg, "exit")) {
			printf("#TCP# Server: client message handler thread bye\n");

			close(cln_info->sd_cln);
			free(cln_info);

			pthread_exit((void*)EXIT_SUCCESS);
		}

		bts = send(cln_info->sd_cln, msg, bts, 0);
		if (bts == -1) {
			perror("#TCP# Server: send(client)");

			close(cln_info->sd_cln);
			free(cln_info);

			pthread_exit((void*)EXIT_FAILURE);
		}

		printf("#TCP# Server: send message to client: '%s' (%d bytes)\n\n", msg, (int)bts);
	}

	close(cln_info->sd_cln);
	free(cln_info);

	pthread_exit((void*)EXIT_SUCCESS);
}
