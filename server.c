#if defined(HAVE_CONFIG_H)
#include "config.h"
#endif

#include <stdio.h>
#include <sys/uio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#if !defined(SERVER_ADDR)
#define SERVER_ADDR "127.0.0.1"
#endif

#if !defined(SERVER_PORT)
#define SERVER_PORT 12345
#endif

enum {
  NQUEUESIZE = 5,
};

char *message = "Hello!\nGood-bye!!\n";

int main(void)
{
  int s, ws, soval, cc;
  struct sockaddr_in sa, ca;
  socklen_t ca_len;

  if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    fprintf(stderr, "error: socket\n");
    exit(1);
  }

  soval = 1;
  if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &soval, sizeof(soval)) == -1) {
    fprintf(stderr, "error: setsockopt\n");
    exit(1);
  }

  memset(&sa, 0, sizeof(sa));
  sa.sin_family = AF_INET;
  sa.sin_port = htons(SERVER_PORT);
  sa.sin_addr.s_addr = htonl(INADDR_ANY);
  if (bind(s, (struct sockaddr *)&sa, sizeof(sa)) == -1) {
    fprintf(stderr, "error: bind\n");
    exit(1);
  }

  if (listen(s, NQUEUESIZE)) {
    fprintf(stderr, "error: listen\n");
    exit(1);
  }

  fprintf(stderr, "Ready.\n");

  for (;;) {
    fprintf(stderr, "Waiting for a connection..\n");

    ca_len = sizeof(ca);
    if ((ws = accept(s, (struct sockaddr *)&ca, &ca_len)) == -1) {
      fprintf(stderr, "error: accept\n");
      exit(1);
    }
    fprintf(stderr, "Connection established.\n");

    fprintf(stderr, "Sending the message...\n");
    if ((cc = write(ws, message, strlen(message))) == -1) {
      fprintf(stderr, "error: write\n");
      exit(1);
    }
    fprintf(stderr, "Message sent.\n");

    if (shutdown(ws, SHUT_RDWR) == -1) {
      fprintf(stderr, "error: shutdown\n");
      exit(1);
    }

    if (close(ws) == -1) {
      fprintf(stderr, "error: close\n");
      exit(1);
    }
  }
}
