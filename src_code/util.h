//
// Latest edit by Mxxxxx on 2018/1/23.
//

#ifndef UTIL_H
#define UTIL_H

#define PATHLEN 128
#define LISTENQ 1024
#define BUFLEN 8192
#define DELIM "="

#define HS_CONF_OK 0
#define HS_CONF_ERROR -1

#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef struct hs_conf{
    char root[PATHLEN];
    int port;
    int thread_num;
}hs_conf_t;

int read_conf(char* filename, hs_conf_t* conf);
void handle_for_sigpipe();
int socket_bind_listen(int port);
int make_socket_non_blocking(int fd);
void accept_connection(int listen_fd, int epoll_fd, char* path);

#endif
