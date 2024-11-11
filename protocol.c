#include "protocol.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

void send_file(const char *path, int client_sock, const char *content_type, ServerInfo *stats) {
    FILE *file = fopen(path, "rb");
    if (file) {
        char buffer[BUFFER_SIZE];
        size_t bytes_read;
        dprintf(client_sock, "HTTP/1.1 200 OK\r\nContent-Type: %s\r\n\r\n", content_type);

        while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
            stats->total_sent_bytes += write(client_sock, buffer, bytes_read);
        }
        fclose(file);
    } else {
        write(client_sock, "HTTP/1.1 404 Not Found\r\n\r\n", 26);
    }
}

void send_response(int client_sock, const char *status, const char *content_type, const char *body, ServerInfo *stats) {
    dprintf(client_sock, "HTTP/1.1 %s\r\nContent-Type: %s\r\n\r\n%s", status, content_type, body);
    stats->total_sent_bytes += strlen(body);
}

void parse_request(char *buffer, int client_sock, ServerInfo *stats) {
    char method[16], path[256], protocol[16];
    sscanf(buffer, "%s %s %s", method, path, protocol);

    if (strcmp(method, "GET") == 0) {
        if (strncmp(path, "/static/", 8) == 0) {
            char file_path[512];
            snprintf(file_path, sizeof(file_path), ".%s", path);
            send_file(file_path, client_sock, "application/octet-stream", stats);
        } else if (strncmp(path, "/stats", 6) == 0) {
            char response[512];
            snprintf(response, sizeof(response),
                     "<html><body><h1>Server Stats</h1>"
                     "<p>Total Requests: %d</p>"
                     "<p>Total Received Bytes: %d</p>"
                     "<p>Total Sent Bytes: %d</p>"
                     "</body></html>",
                     stats->total_requests, stats->total_received_bytes, stats->total_sent_bytes);
            send_response(client_sock, "200 OK", "text/html", response, stats);
        } else if (strncmp(path, "/calc?", 6) == 0) {
            int a = 0, b = 0;
            sscanf(strstr(path, "a="), "a=%d", &a);
            sscanf(strstr(path, "b="), "b=%d", &b);
            char response[256];
            snprintf(response, sizeof(response), "%d", a + b);
            send_response(client_sock, "200 OK", "text/plain", response, stats);
        } else {
            send_response(client_sock, "404 Not Found", "text/plain", "Not Found", stats);
        }
    } else {
        send_response(client_sock, "405 Method Not Allowed", "text/plain", "Method Not Allowed", stats);
    }
}
