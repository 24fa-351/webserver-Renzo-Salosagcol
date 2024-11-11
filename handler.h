#ifndef HANDLER_H
#define HANDLER_H

typedef struct {
    int total_requests;
    int total_received_bytes;
    int total_sent_bytes;
} ServerInfo;

void handle_request(int client_sock, ServerInfo *stats);

#endif
