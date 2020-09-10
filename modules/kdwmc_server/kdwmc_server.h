#include <pthread.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <stdio.h>

#define KDWMC_SERVER_PORT 8383
#define KDWMC_SERVER_BUFLEN 1024

char kdwmc_server_buf[KDWMC_SERVER_BUFLEN];
bool kdwmc_server_running = false;
pthread_t kdwmc_server_thread_id;
bool kdwmc_server_has_subscriber = false;
struct sockaddr kdwmc_server_subscriber;

void kdwmc_server_start();
void *kdwmc_server_thread(void *vargp);
int kdwmc_server_handler(int len, struct sockaddr *client);
void kdwmc_server_subscribe();
void kdwmc_server_stop();
