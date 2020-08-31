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
struct sockaddr kdwmc_server_tag_mask_observer;
bool kdwmc_server_has_tag_mask_observer = false;

void kdwmc_server_start();
void *kdwmc_server_thread(void *vargp);
int kdwmc_server_handler(int len, struct sockaddr *client);
void kdwmc_server_observe_tag_mask(int tag_mask);
void kdwmc_server_stop();
