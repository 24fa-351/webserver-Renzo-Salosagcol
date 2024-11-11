#include "handler.h"
#include "protocol.h"
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 1024

void handle_request(int client_sock, ServerInfo *stats) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read = read(client_sock, buffer, sizeof(buffer) - 1);
    if (bytes_read > 0) {
        stats->total_received_bytes += bytes_read;
        buffer[bytes_read] = '\0';
        stats->total_requests++;
        parse_request(buffer, client_sock, stats);
    }
    close(client_sock);
}
