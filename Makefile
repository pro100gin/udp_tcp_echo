IDIR = include
LDIR = lib
SDIR = src
LFLAGS = -lpthread
CFLAGS = -g -O2 -Wall -pedantic -ansi
CC = gcc

SUBDIRS = $(LDIR) $(SDIR)
OBJSERV = $(SDIR)/server.o $(LDIR)/server_core.o
OBJCLIENT_UDP = $(SDIR)/client_udp.o $(LDIR)/client_core.o
OBJCLIENT_TCP = $(SDIR)/client_tcp.o 


vpath %.c $(SDIR) : $(LDIR)
vpath %.o $(SDIR) : $(LDIR)
vpath %.h $(IDIR)

all: server client_udp client_tcp

server: $(OBJSERV)
		$(CC) -o $@ $^ $(CFLAGS) $(LFLAGS) 

$(OBJSERV): prepare

client_udp: $(OBJCLIENT_UDP)
		$(CC) -o $@ $^ $(CFLAGS) $(LFLAGS) 
client_tcp: $(OBJCLIENT_TCP)
		$(CC) -o $@ $^ $(CFLAGS) $(LFLAGS) 

$(OBJCLIENT_UDP): prepare

$(OBJCLIENT_TCP): prepare

.PHONY: prepare $(SUBDIRS)
prepare: $(SUBDIRS)

$(SUBDIRS):
		make -C $@
		
$(SDIR): $(LDIR)

clean:
		clear
		cd src	&& $(MAKE) clean
		cd lib	&& $(MAKE) clean
		rm -rf server client_udp client_tcp *.o
