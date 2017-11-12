OBJS:=$(wildcard *.c)
server:$(OBJS)
	gcc $^ -g -o $@ -I ../include_server -lcrypt -lmysqlclient
clean:
	rm server
