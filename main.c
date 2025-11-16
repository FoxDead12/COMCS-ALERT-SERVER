/*
 
 COMCS ALERT SERVER
 
 Requirements:
    -> UDP Server
    -> Best QoS in UDP communications
    -> Ability responde multi clients

 Client UPD request body:
    -> temperature: { "current": value, "average": value };
    -> humidity:    { "current": value, "average": value };
 
*/

#include "main.h"
#include <stdlib.h>
#include <stdio.h>

#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <fcntl.h>

#define PORT 3001
#define BUFFER_SIZE 1024

int
main(int argc, const char * argv[]) {
    
  int socket_fd = 0;
  int optval    = 0;
  struct sockaddr_in serveraddr;
  
  
  // ... create socket file descriptor ...
  socket_fd = socket(AF_INET, SOCK_DGRAM, 17); // 17 - is protocol UDP (come from MAC mini M4, check linux)
  if ( socket_fd < 0 ) {
    perror("error opening socket");
  }
  
  // ... make socket async ...
  int flags = fcntl(socket_fd, F_GETFL, 0);
  fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK);
  
  /* setsockopt: Handy debugging trick that lets
   * us rerun the server immediately after we kill it;
   * otherwise we have to wait about 20 secs.
   * Eliminates "ERROR on binding: Address already in use" error.
   */
  optval = 1;
  setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int));
  
  // ... set address of server ...
  bzero((char *)&serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons((unsigned short) PORT);
  
  // ... bind socket to port ...
  if( bind(socket_fd, (struct sockaddr*) &serveraddr, sizeof(serveraddr)) < 0 ) {
    perror("error binding socket");
  }
  
  // ... UDP server start here ...
  udp_start(socket_fd);
  
  return EXIT_SUCCESS;
}


int
udp_start (int socket_fd) {
  
  // ... make UDP server ...
    
  while (1) {
    
    // ... reset reader of file descriptors ...
    fd_set read_fds;
    FD_ZERO(&read_fds);
    FD_SET(socket_fd, &read_fds);
  
    
    // ... wait for events in file descriptors ...
    int activity = select(socket_fd + 1, &read_fds, NULL, NULL, NULL);
    if ( activity < 0 ) {
      perror("error select");
      break;
    }
    
    
    // ... read event ...
    if ( FD_ISSET(socket_fd, &read_fds) ) {
      
      char buffer[BUFFER_SIZE + 1] = {0};
      size_t n = recvfrom(socket_fd, buffer, BUFFER_SIZE, 0, NULL, NULL);
      
      
    }
    
    
  }
  
  
  return 1;
}
