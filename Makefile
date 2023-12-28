HOME=/home/debian/open62541/
CC=gcc
CFLAGS1=-g -std=c99 -v
CFLAGS2=-I$(HOME) -I$(HOME)include/ -I$(HOME)plugins/ -I$(HOME)src/ -I$(HOME)build/src_generated/ \
-I$(HOME)arch/ -I$(HOME)deps/ -I$(HOME)plugins/include/ -I/usr/local/include/ -I/usr/include/libxml2
CFLAGS3=-g -pass-exit-codes
DEPS=
LIBS=-lm -lrt -lwebsockets -ljson-c \
-lmariadbclient -lpthread -lxml2 -lcrypto -lssl -lmbedtls -lmbedx509 -lmbedcrypto
OBJ=SV_Misc.o SV_ExtractXMLElementNames.o SV_Alarm.o SV_Event.o SV_Historizing.o \
SV_Monitor.o SV_Method.o SV_PopulateOPCUANodes.o SV_CreateOPCUANodes.o \
SV_StartOPCUAServer.o json_checker.o SV_PubSub.o SV_mainOPCUAServer.o

ODIR=obj
LDIR1=-L/usr/local/lib/
LDIR2=-L/usr/lib/
LDFLAGS=-L$(HOME)build/bin/ -l:libopen62541.a

all: myNewServer

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS1) $(CFLAGS2)

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS1) $(CFLAGS2) $(CFLAGS3) -c  $< -o $@

myNewServer: $(OBJ)
	$(CC) $(LDIR1) $(LDIR2) -o $@ $^ $(LDFLAGS) $(LIBS)


#LIBS=-lm -lrt -lmodbus -lqpid-proton-core -lqpid-proton-proactor -lqpid-proton -lwebsockets -ljson-c \
#-lmariadbclient -lpthread -lxml2 -lcrypto -lssl -lmbedtls -lmbedx509 -lmbedcrypto -lopen62541
#OBJ=open62541.o ua_amqp_adaptor.o ua_pubsub_amqp.o SV_Misc.o SV_ExtractXMLElementNames.o SV_Alarm.o SV_Event.o \
#SV_Monitor.o SV_Method.o SV_WebSockets.o SV_Historizing.o SV_PopulateOPCUANodes.o SV_CreateOPCUANodes.o \
#SV_StartOPCUAServer.o json_checker.o SV_PubSub.o SV_mainOPCUAServer.o
