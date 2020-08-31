#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define PORT 8383
#define BUFLEN 1024
char buffer[BUFLEN];
struct sockaddr_in server_addr;
int in_len, slen;

static void die (int line_number, const char * format, ...) {
  va_list vargs;
  va_start (vargs, format);
  fprintf (stderr, "%d: ", line_number);
  vfprintf (stderr, format, vargs);
  fprintf (stderr, ".\n");
  va_end (vargs);
  exit (1);
}

void sig_handler(int signo) {
  int s;

  strcpy(buffer, "reset tag_mask_observer");

	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    die(__LINE__, "ERROR: Creating socket");

  if (sendto(s, buffer, strlen(buffer)+1, MSG_CONFIRM, (struct sockaddr*) &server_addr, slen) == -1)
    die(__LINE__, "ERROR: Sending request");

  exit(0);

}

int main(int argc, char *argv[]) {
  int s;
  slen = sizeof(server_addr);

  if (signal(SIGINT, sig_handler) == SIG_ERR)
    die(__LINE__, "ERROR: While initializing signal handler");

	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    die(__LINE__, "ERROR: Creating socket");

  server_addr.sin_family = AF_INET; // IPv4
  server_addr.sin_addr.s_addr = htonl(0x7F000001); // Ip-Adresse
  server_addr.sin_port = htons(PORT); // Port

  if (argc > 1)
    strcpy(buffer, argv[1]);
  for(int i = 2; i < argc; ++i) {
    strcat(buffer, " ");
    strcat(buffer, argv[i]);
  }

  if (sendto(s, buffer, strlen(buffer)+1, MSG_CONFIRM, (struct sockaddr*) &server_addr, slen) == -1)
    die(__LINE__, "ERROR: Sending request");

  if ((in_len = recvfrom(s, buffer, BUFLEN, MSG_WAITALL, (struct sockaddr *) &server_addr, &slen)) == -1)
    die(__LINE__, "ERROR: Receiving response");

  if (argc > 1 && !strcmp(argv[1], "observe")) {
    if (strcmp(buffer, "0")) {
      printf("There is already an observer.\n");
      return 0;
    }
    while (1) {
      if ((in_len = recvfrom(s, buffer, BUFLEN, MSG_WAITALL, (struct sockaddr *) &server_addr, &slen)) == -1)
        die(__LINE__, "ERROR: Sending response");
      printf("%s\n", buffer);
    }
  }

  printf("%s\n", buffer);

  return 0;
}
