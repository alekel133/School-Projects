/*
 * Author: Alexander Kellough
 * NetID: atk133
 *
 * Description: Program for sending a file from client to server.
 * Based on Example UDP by Maxwell Young.
 */

#include<iostream>
#include <sys/types.h>   // defines types (like size_t)
#include <sys/socket.h>  // defines socket class
#include <netinet/in.h>  // defines port numbers for (internet) sockets, some address structures, and constants
#include <time.h>        // used for random number generation
#include <string.h> // using this to convert random port integer to string
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <random>
#include <time.h>
#include <fcntl.h>

using namespace std;

int main(int argc, char *argv[]){

	int n_port = stoi(argv[1]);
  int t_port = stoi(argv[2]);
  
  struct sockaddr_in server;
  struct sockaddr_in client;
  int mysocket = 0;
  int i = 0;
  socklen_t clen = sizeof(client);
  char payload[4];
  
  if ((mysocket=socket(AF_INET, SOCK_DGRAM, 0))==-1)
    cout << "Error in socket creation.\n";
  
  memset((char *) &server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(n_port);
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  if (bind(mysocket, (struct sockaddr *)&server, sizeof(server)) == -1)
    cout << "Error in binding.\n";

  // Variables and objects for file transfer
  int fd = open("upload.txt", O_CREAT | O_WRONLY | O_RDONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IXUSR);
  off_t size;
  char ack[4];

  // Get File size from client
  if(recvfrom(mysocket, &size, sizeof(off_t), 0, (struct sockaddr *)&client, &clen) == -1){
		  cout << "Failed to recieve." << endl;
  }

  // While there is still data, continue transfer.
  while(size) {
	  // Get packet
	  if(recvfrom(mysocket, payload, 4, 0, (struct sockaddr *)&client, &clen) == -1){
		  cout << "Failed to recieve." << endl;
	  }
	
	  // Format acknowledgement.
	  for(int i = 0; i < 4; ++i) {
			ack[i] = toupper(payload[i]);
	  }
	  
	  // Send acknowledgement.
	  if (sendto(mysocket, ack, 4, 0, (struct sockaddr *)&client, clen)==-1){
        cout << "Error in sendto function.\n";
     }

	  // If there is still data left, write full packet, update size.
     if(size > 4) {
		  if(write(fd, payload, 4) == -1) {
				perror("Error in write.");
				exit(EXIT_FAILURE);
		  }
		  size = size -4;
	  }
	  // If there is no data left, write valid bytes, and set size to 0 (file empty).
	  else {
			for(int i = 0; i < size; ++i) {
				write(fd, &payload[i], 1);
			}
			size = 0;
	  }
  }

  // Close File Descriptors.
  close(fd);
  close(mysocket);
  return 0;
}
