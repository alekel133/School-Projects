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

using namespace std;

int main(int argc, char *argv[]){
  
  struct hostent *s; 
  s = gethostbyname(argv[1]);
  int n_port = stoi(argv[2]);
  char *filename = argv[3];

  struct sockaddr_in server;
  int mysocket = 0;
  socklen_t slen = sizeof(server);
  char payload[4]="ABC"; 
  cout << "Payload contains:  " << payload << endl;
  
  
  if ((mysocket=socket(AF_INET, SOCK_DGRAM, 0))==-1)
    cout << "Error in creating socket.\n";

  
  memset((char *) &server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(n_port);
  bcopy((char *)s->h_addr, 
	(char *)&server.sin_addr.s_addr,
	s->h_length);
 
  // obtained from https://stackoverflow.com/questions/32737083/extracting-ip-data-using-gethostbyname                                         
  struct in_addr a;
  printf("name: %s\n", s->h_name);
  while (*s->h_aliases)
      printf("alias: %s\n", *s->h_aliases++);
  while (*s->h_addr_list)
  {
      bcopy(*s->h_addr_list++, (char *) &a, sizeof(a));
      printf("address: %s\n", inet_ntoa(a));
  }

  if(sendto(mysocket, payload, 4, 0, (struct sockaddr *)&server, slen) == -1) {
	 cout << "Error in sendto function." << endl;
  }

  int r_port;
  recvfrom(mysocket, &r_port, sizeof(r_port), 0, (struct sockaddr *)&server, &slen); 

  cout << "Random Port: " << r_port << endl;

  close(mysocket);

  if ((mysocket=socket(AF_INET, SOCK_DGRAM, 0))==-1)
    cout << "Error in creating socket.\n";

  
  memset((char *) &server, 0, sizeof(server));
  server.sin_family = AF_INET;
  server.sin_port = htons(r_port);

  // Declaring variables and objects for file exchange
  int fd = open(filename, O_RDONLY);
  int state = 0;
  struct stat st;

  // Get file size
  fstat(fd, &st);
  off_t size = st.st_size;

  // Send filesize to server
  if(sendto(mysocket, &size, sizeof(off_t), 0, (struct sockaddr *)&server, slen) == -1) {
		 cout << "Error in sendto function." << endl;
  }

  // While the EOF has not been reached, continue transmitting data.
  state = 4;
  while(state == 4){

	  // Read packet from file
	  state = read(fd, payload, 4);
	  if(state == -1) {
			perror("Error in read.\n");
			exit(EXIT_FAILURE);
	  }

	  // Send packet
	  if(sendto(mysocket, payload, 4, 0, (struct sockaddr *)&server, slen) == -1) {
		 cout << "Error in sendto function." << endl;
	  }

	  // Wait for acknowledgment
	  if(recvfrom(mysocket, payload, 4, 0, (struct sockaddr *)&server, &slen) == -1) {
			cout << "Failed to recieve." << endl;
		}

	   // If there is still data, update size and output acknowledgement.
		if(size > 4) {
	      cout << payload << endl;
         size = size - 4;
		}
		// If EOF has been reached, only print valid characters of acknowledgment.
		else {
			for(int i = 0; i < size; ++i) {
				cout << payload[i] << endl;
			}
			size = 0;
		}
  }

  // Close file descriptors.
  close(fd);
  close(mysocket);
  return 0;
}
