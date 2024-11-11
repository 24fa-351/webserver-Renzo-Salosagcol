#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include "handler.h"
#include "protocol.h"

#define BUFFER_SIZE 1024
#define DEFAULT_PORT 80

void *client_handler(void *arg) {
    int client_sock = *(int*)arg;
    free(arg);
    ServerInfo stats = {0, 0, 0};
    handle_request(client_sock, &stats);
    return NULL;
}

int main(int argc, char *argv[]) {
    int port = DEFAULT_PORT;
    if (argc == 3 && strcmp(argv[1], "-p") == 0) {
        port = atoi(argv[2]);
    }

    int server_sock = socket(PF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr, client_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_sock, 5);

    while (1) {
        socklen_t client_addr_size = sizeof(client_addr);
        int *client_sock = malloc(sizeof(int));
        *client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &client_addr_size);

        pthread_t t_id;
        pthread_create(&t_id, NULL, client_handler, client_sock);
        pthread_detach(t_id);
    }

    close(server_sock);
    return 0;
}
