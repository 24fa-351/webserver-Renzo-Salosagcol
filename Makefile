all: web_server.o

web_server.o: web_server.c
	gcc -o web_server web_server.c handler.c protocol.c -pthread

run: web_server.o
	./web_server -p 8080

clean:
	rm -f web_server