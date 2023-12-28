CC=gcc
CFLAGS1=-g -std=c99 -v
CFLAGS2=-I/home/pi/open62541/ -I/home/pi/open62541/include -I/home/pi/open6251/plugins -I/usr/local/include
DEPS=open62541.h
LIBS=-lm -lrt -lmodbus -lqpid-proton-core -lqpid-proton-proactor -lqpid-proton -lwebsockets -ljson-c \
-lmariadbclient -lpthread -lxml2 -lcrypto -lssl -lmbedtls -lmbedx509 -lmbedcrypto -lopen62541
OBJ=open62541.o ua_amqp_adaptor.o ua_pubsub_amqp.o SV_Misc.o SV_ExtractXMLElementNames.o SV_Alarm.o SV_Event.o \
SV_Monitor.o SV_Method.o SV_WebSockets.o SV_Historizing.o SV_PopulateOPCUANodes.o SV_CreateOPCUANodes.o \
SV_StartOPCUAServer.o json_checker.o SV_PubSub.o SV_mainOPCUAServer.o

ODIR=obj
LDIR1=-L/usr/local/lib/
LDIR2=-L/usr/lib/
LDIR3=-L/home/pi/open62541/build/bin
LDFLAGS=-L/home/pi/open62541/build/bin/ -lopen62541 -static

all: myNewServer

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS1) $(CFLAGS2)

myNewServer: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS1) $(CFLAGS2) $(LDIR1) $(LDIR2) $(LDIR3) $(LIBS)
