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

#define PORT 8383
#define BUFLEN 1024
char buffer[BUFLEN];

static void die (int line_number, const char * format, ...) {
    va_list vargs;
    va_start (vargs, format);
    fprintf (stderr, "%d: ", line_number);
    vfprintf (stderr, format, vargs);
    fprintf (stderr, ".\n");
    va_end (vargs);
    exit (1);
}

int main(int argc, char *argv[]) {
  struct sockaddr_in server_addr;

  int s, in_len, slen = sizeof(server_addr);

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

  if (sendto(s, buffer, strlen(buffer), MSG_CONFIRM, (struct sockaddr*) &server_addr, slen) == -1)
    die(__LINE__, "ERROR: Receiving request");

  if ((in_len = recvfrom(s, buffer, BUFLEN, MSG_WAITALL, (struct sockaddr *) &server_addr, &slen)) == -1)
    die(__LINE__, "ERROR: Sending response");

  printf("%s\n", buffer);

  return 0;
}
