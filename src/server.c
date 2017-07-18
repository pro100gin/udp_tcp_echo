#include "../include/server_core.h"


int main() {
	/*var for epoll*/
	struct epoll_event ev_udp, ev_tcp, events[1];
	int epollfd = 0, nfds;
	char bufin[MAX_MSG_SIZE];
	int n;
	/*var for udp*/
	int ld;
	struct sockaddr_in skaddr, remote;
	socklen_t length;

	/*var for tcp*/
	int sd_srv = 0, sd_cln = 0, rtn = 0;
	struct sockaddr_in srv_addr, cln_addr;
	struct hostent* cln_hst = NULL;
	socklen_t sock_size = 0;
	pthread_t cln_hndl_tid = 0;
	CLIENT_INFO* cln_info = NULL;

	/*initialaize TCP server*/
	sock_size = sizeof(struct sockaddr_in);

	memset(&srv_addr, 0, sock_size);
	memset(&cln_addr, 0, sock_size);

	sd_srv = socket(AF_INET, SOCK_STREAM, 0);
	if (sd_srv == -1) {
		perror("Server TCP: socket(server)");

		close(sd_srv);
		exit(EXIT_FAILURE);
	}

	printf("Server start\n");

	srv_addr.sin_family = AF_INET;
	srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	srv_addr.sin_port = htons(SERVER_PORT);

	rtn = bind(sd_srv, (struct sockaddr*)&srv_addr, sock_size);
	if (rtn == -1) {
		perror("Server TCP: bind(server)");

		close(sd_srv);
		exit(EXIT_FAILURE);
	}

	rtn = listen(sd_srv, 1);
	if (rtn == -1) {
		perror("Server TCP: listen(server)");

		close(sd_srv);
		exit(EXIT_FAILURE);
	}
	/*initialize UDP server*/
	if ((ld = socket( AF_INET, SOCK_DGRAM, 0 )) < 0) {
		printf("Problem creating UDP socket\n");
		exit(1);
	}

	skaddr.sin_family = AF_INET;
	skaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	skaddr.sin_port = htons(7777);

	if (bind(ld, (struct sockaddr *) &skaddr, sizeof(skaddr))<0) {
		printf("Problem binding UDP\n");
		exit(0);
	}

	length = sizeof( skaddr );
	if (getsockname(ld, (struct sockaddr *) &skaddr, &length)<0) {
		printf("Error getsockname UDP\n");
		exit(1);
	}

	printf("The server UDP ip is %s port number is %d\n",
		inet_ntoa(skaddr.sin_addr), ntohs(skaddr.sin_port));

	/*initialize epoll*/
	epollfd = epoll_create(10);
	if (epollfd == -1) {
		perror("epoll_create");
		exit(EXIT_FAILURE);
	}
	ev_tcp.events = EPOLLIN;
	ev_tcp.data.fd = sd_srv;

	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, sd_srv, &ev_tcp) == -1) {
		perror("epoll_ctl: listen_sock TCP");
		exit(EXIT_FAILURE);
	}
	ev_udp.events = EPOLLIN;
	ev_udp.data.fd = ld;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, ld, &ev_udp) == -1) {
		perror("epoll_ctl: listen_sock UDP");
		exit(EXIT_FAILURE);
	}

	/*main loop*/
	while(1) {
		nfds = epoll_wait(epollfd, events, 1, -1);
		if (nfds == -1) {
			perror("epoll_pwait");
			exit(EXIT_FAILURE);
		}
		if(events[0].data.fd == sd_srv){
				sd_cln = accept(sd_srv, (struct sockaddr*)&cln_addr, &sock_size);
			if (sd_cln == -1) {
				perror("Server TCP: accept(client)");
				close(sd_srv);
				close(sd_cln);
				exit(EXIT_FAILURE);
			}

			cln_hst = gethostbyaddr((char *)&cln_addr.sin_addr.s_addr, 4, AF_INET);
			printf("Server TCP: incoming connection from %s:%d (hs: %s)\n\n",
			       inet_ntoa(cln_addr.sin_addr), ntohs(cln_addr.sin_port),
			                  ((cln_hst != NULL) ? cln_hst->h_name : ""));

			cln_info = malloc(sizeof(CLIENT_INFO));
			cln_info->sd_cln = sd_cln;
			memcpy(&cln_info->cln_addr, &cln_addr, sock_size);

			pthread_create(&cln_hndl_tid, NULL, cln_hndl,
							      (void*)cln_info);
		}
		if(events[0].data.fd == sd_srv){
			n = recvfrom(ld, bufin, MAX_MSG_SIZE, 0,
		                             (struct sockaddr *)&remote, &length);
			if (n<0){
				perror("Error receiving UDP data");
			}
			printf("UDP Got a datagram %s from %s port %d\n", bufin,
			   inet_ntoa(remote.sin_addr), ntohs(remote.sin_port));

			n = sendto(ld, bufin, strlen(bufin), 0,
			        (struct sockaddr *)&remote, length);
			if(n < 0){
				perror("Error sending to UDP client");
			}
			printf("Sending datagram to UDP client");
		}
	}
	close(sd_srv);
	close(sd_cln);

	exit(EXIT_SUCCESS);
}