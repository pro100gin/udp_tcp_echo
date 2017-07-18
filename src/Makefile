CFLAGS = -g -O2 -Wall -pedantic -ansi
LFLAGS =

vpath %.h ../include/
vpath %.c ../lib/

server.o: server.c server_core.c
		gcc -c $< $(CFLAGS) $(LFLAGS)

client_udp.o: client_udp.c client_core.c
		gcc -c $< $(CFLAGS) $(LFLAGS)

client_tcp.o: client_tcp.c client_core.c
		gcc -c $< $(CFLAGS) $(LFLAGS)

clean:                                                                                 
		rm -rf *.o *.out
