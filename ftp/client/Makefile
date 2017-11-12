OBJS:=$(wildcard *.c)
client:$(OBJS)
	gcc $^ -o $@ -I ../include_client -lcrypt
clean:
	rm client
