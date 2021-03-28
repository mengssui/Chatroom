.PHONY : run clean kill

ALL=./common/head.h ./bin/wechat.o ./bin/udp_client.o ./bin/udp_server.o ./bin/reactor.o ./bin/wechat.o ./bin/common.o ./bin/client_discover.o ./bin/heart_beat.o ./bin/send_chat.o

CFLAG=-DDEBUG

./bin/all: $(ALL) 
	gcc ./bin/*.o -o ./obj/wechat -lpthread $(CFLAG)

./bin/wechat.o: ./common/head.h wechat.c
	gcc -c -I ./common/ wechat.c -o ./bin/wechat.o -lpthread $(CFLAG)

./bin/udp_server.o: ./common/head.h ./common/udp_server.c
	gcc -c -I ./common/ ./common/udp_server.c -o ./bin/udp_server.o -lpthread $(CFLAG)

./bin/reactor.o: ./common/head.h ./common/reactor.c
	gcc -c -I ./common/ ./common/reactor.c -o ./bin/reactor.o -lpthread $(CFLAG)

./bin/udp_client.o: ./common/udp_client.h ./common/udp_client.c
	gcc -c -I ./common/ ./common/udp_client.c -o ./bin/udp_client.o $(CFLAG)

./bin/common.o: ./common/common.h ./common/head.h ./common/common.c
	gcc -c -I ./common/ ./common/common.c -o ./bin/common.o $(CFLAG)

./bin/client_discover.o: ./common/head.h ./common/client_discover.h ./common/client_discover.c
	gcc -c -I ./common/ ./common/client_discover.c -o ./bin/client_discover.o $(CFLAG)

./bin/heart_beat.o: ./common/head.h ./common/heart_beat.h ./common/heart_beat.c
	gcc -c -I ./common/ ./common/heart_beat.c -o ./bin/heart_beat.o $(CFLAG)

./bin/send_chat.o: ./common/head.h ./common/send_chat.h ./common/send_chat.c
	gcc -c -I ./common/ ./common/send_chat.c -o ./bin/send_chat.o $(CFLAG)

clean:
	rm ./bin/*  ./obj/*

runback:
	nohup ./obj/wechat &
run:
	./obj/wechat 
kill:
	kill `pidof wechat`

