#include "../include/client_udp.h"

int main( int argc, char **argv ) {
  int sk;
  struct sockaddr_in server;
  char buf[MAXBUF];
  int n_sent;
  int n_read;


  if ((sk = socket( AF_INET, SOCK_DGRAM, 0 )) < 0)
    {
      printf("Problem creating socket\n");
      exit(1);
    }

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  server.sin_port = htons(7777);

  while(1){
      printf("#UDP# Input you message: ");
      if (fgets(buf, MAXBUF, stdin) == NULL) {
          perror("#UDP# Client: fgets(msg)");

          close(sk);
          exit(EXIT_FAILURE);
      }
      buf[strlen(buf) - 1] = '\0';
      printf("#UDP# Client send message to server: %s (%d bytes)\n", buf, (int)strlen(buf));

      n_sent = sendto(sk, buf, strlen(buf), 0,
                      (struct sockaddr*) &server, sizeof(server));

      if (n_sent<0) {
          perror("#UDP# Problem sending data");
          exit(1);
      }
      if(!strcmp(buf, "exit")){
           printf("#UDP# Client: main thread bye\n");
           exit(EXIT_SUCCESS);
      }

      memset(buf, 0, MAXBUF);

      n_read = recvfrom(sk,buf,MAXBUF,0,NULL,NULL);
      if (n_read<0) {
          perror("#UDP# Problem in recvfrom");
          exit(1);
      }

      printf("#UDP# Client receive message from server: %s (%d bytes)\n\n", buf, n_read);
  }
  return(0);
}
