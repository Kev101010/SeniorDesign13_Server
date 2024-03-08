#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

char event[1024];

void change_event(char* event_string){
  if(strcmp(event_string, "Safe") == 0){
    bzero(event, 1024);
    strcpy(event, "Safe");
    printf("--== EVENT CHANGE: Safe ==--\n");
  }else if(strcmp(event_string, "Shelter") == 0){
    bzero(event, 1024);
    strcpy(event, "Shelter");
    printf("--== EVENT CHANGE: Shelter ==--\n");
  }else if(strcmp(event_string, "Drill") == 0){
    bzero(event, 1024);
    strcpy(event, "Drill");
    printf("--== EVENT CHANGE: Drill ==--\n");
  }else if(strcmp(event_string, "Emergency") == 0){
    bzero(event, 1024);
    strcpy(event, "Emergency");
    system("python3 ./email_script.py");
    printf("--== EVENT CHANGE: Emergency (Notification Sent) ==--\n");
  }
}

int main(){
  
  char *ip = "192.168.213.6";
  //char *ip = "127.0.0.1";
  int port = 5566;

  int server_sock, client_sock;
  struct sockaddr_in server_addr, client_addr;
  socklen_t addr_size;
  char buffer[1024];
  int n;

  bzero(event, 1024);
  strcpy(event, "Safe");

  //SOCK_STREAM = TCP Protocol
  //AF_INET = ipv4
  server_sock = socket(AF_INET, SOCK_STREAM, 0);
  if (server_sock < 0){
    perror("Socket error");
    exit(1);
  }
  printf("TCP server socket created.\n");

  //clear memory of server addr to make sure it is empty
  memset(&server_addr, '\0', sizeof(server_addr));
  server_addr.sin_family = AF_INET; //ipv4
  server_addr.sin_port = htons(port);
  server_addr.sin_addr.s_addr = inet_addr(ip); //translate string into integer ip address

  //binding socket
  n = bind(server_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if (n < 0){
    perror("Bind error");
    exit(1);
  }

  printf("Bind to the port number: %d\n", port);

  listen(server_sock, 5);
  printf("Listening...\n");
  //TODO, connect multiple esp32's at once

  while(1){
      addr_size = sizeof(client_addr);
      client_sock = accept(server_sock, (struct sockaddr*)&client_addr, &addr_size);
      //printf("Client connected\n");

      bzero(buffer, 1024);
      recv(client_sock, buffer, sizeof(buffer), 0);
      printf("Client: %s\n", buffer);

      change_event(buffer);
      //bzero(buffer, 1024);
      //strcpy(buffer, "HI THIS IS SERVER");

      printf("Server: %s\n", event);
      send(client_sock, event, strlen(event), 0);

      close(client_sock);
      //printf("client disconnected\n");
  }

  return 0;
}
