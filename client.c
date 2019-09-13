#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/uio.h>
#include <unistd.h>

#if !defined(SERVER_ADDR)
#define SERVER_ADDR "127.0.0.1"
#endif

#if !defined(SERVER_PORT)
#define SERVER_PORT 12345
#endif

int main(int argc, char *argv[])
{
  int s, cc;
  struct sockaddr_in sa;
  char buf[1024];

  if (argc != 2) {
    fprintf(stderr, "usage: %s ipaddr\n", argv[0]);
    exit(1);
  }

  if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    fprintf(stderr, "error: socket\n");
    exit(1);
  }

  memset(&sa, 0, sizeof(sa));
  sa.sin_family = AF_INET;
  sa.sin_port = htons(SERVER_PORT);
  sa.sin_addr.s_addr = inet_addr(argv[1]);

  fprintf(stderr, "Connecting to the server...\n");
  if (connect(s, (struct sockaddr *)&sa, sizeof(sa)) == -1) {
    fprintf(stderr, "error: connect\n");
    exit(1);
  }
  fprintf(stderr, "Connected.\n");

  fprintf(stderr, "Message from the server:\n\n");
  while ((cc = read(s, buf, sizeof(buf))) > 0) {
    write(1, buf, cc); /* 1: stdout */
  }
  if (cc == -1) {
    fprintf(stderr, "error: read\n");
    exit(1);
  }
  fprintf(stderr, "\n\nFinished receiving.\n");

  if (shutdown(s, SHUT_RDWR) == -1) {
    fprintf(stderr, "error: shutdown\n");
    exit(1);
  }

  if (close(s) == -1) {
    fprintf(stderr, "error: close\n");
    exit(1);
  }

  return 0;
}
