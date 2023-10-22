/*
 * Author: Alexander Kellough
 * NetID: atk133
 *
 * Description: Code for sending a file from client to server.
 * Based on Example UDP by Maxwell Young.
 */

#include <iostream>
#include <sys/types.h>   // defines types (like size_t)
#include <sys/socket.h>  // defines socket class
#include <netinet/in.h>  // defines port numbers for (internet) sockets, some address structures, and constants
#include <netdb.h> 
#include <iostream>
#include <fstream>
#include <arpa/inet.h>   // if you want to use inet_addr() function
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "packet.h"

using namespace std;

int main(int argc, char *argv[]){
  struct hostent *s;
  s = gethostbyname(argv[1]);
  int out_port = stoi(argv[2]);
  int in_port = stoi(argv[3]);
  char *filename = argv[4];

  struct sockaddr_in network;
  int mysocket = 0;
  socklen_t slen = sizeof(network);
  
  if((mysocket=socket(AF_INET, SOCK_DGRAM, 0))==-1)
	  cout << "Error in creating socket." << endl;

  memset((char *) &network, 0, sizeof(network));
  network.sin_family = AF_INET;
  network.sin_port = htons(out_port);
  bcopy((char *)s->h_addr,
		  (char *)&network.sin_addr.s_addr,
		  s->h_length);
  return 0;
}
