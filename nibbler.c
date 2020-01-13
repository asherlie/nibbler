#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* TODO: host should be extracted from url */
int read_bytes(char* host_addr, char* url){
      int sock = socket(AF_INET, SOCK_STREAM, 0);
      struct hostent* host = gethostbyname(host_addr);
      if(!host){
            puts("host lookup failed");
            return -1;
      }

      struct sockaddr_in addr = {.sin_family = AF_INET, .sin_port = htons(80)};
      memcpy(&addr.sin_addr.s_addr, host->h_addr, host->h_length);

      if(connect(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) == -1){
            perror("connect");
            return -1;
      }
      char req[500] = {0}, buf[5000] = {0};
      sprintf(req, "GET / HTTP/1.0\r\nHost: %s\r\nConnection: close\r\n\r\n", url);
      /* TODO: don't rely on strlen(), param can be url_len and we can add that
       * to the constant len of the request
       */
      write(sock, req, strlen(req));
      int nb, b_read = 0;
      while((nb = read(sock, buf, 4999)))
            b_read += nb;
      close(sock);
      puts(buf);
      return b_read;
}

int main(){
      printf("read %i bytes\n", read_bytes("example.com", "example.com"));
      return 0;
}
