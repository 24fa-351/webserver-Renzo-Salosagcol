#ifndef HTTP_H
#define HTTP_H

#include "handler.h"

void send_file(const char *path, int client_sock, const char *content_type, ServerInfo *stats);
void send_response(int client_sock, const char *status, const char *content_type, const char *body, ServerInfo *stats);
void parse_request(char *buffer, int client_sock, ServerInfo *stats);

#endif
