#include "game.h"
#include "utilities.h"
// Standard Includes for MPI, C and OS calls
#include <mpi.h>

// C++ standard I/O and library includes
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// C++ stadard library using statements
using std::cout ;
using std::cerr ;
using std::endl ;

using std::vector ;
using std::string ;

using std::ofstream ;
using std::ifstream ;
using std::ios ;

void Server(int argc, char *argv[]) {

  // Check to make sure the server can run
  if(argc != 3) {
    cerr << "two arguments please!" << endl ;
    MPI_Abort(MPI_COMM_WORLD,-1) ;
  }

  // Input case filename
  ifstream input(argv[1],ios::in) ;

  // Output case filename
  ofstream output(argv[2],ios::out) ;

  int count = 0 ;
  int NUM_GAMES = 0 ;
  int client_space, world_size, k, stop, send_size, recv_size, tag, size, NUM_COMPLETE;
  unsigned char rbuf[IDIM*JDIM];
  MPI_Status status;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  client_space = world_size - 1;
  send_size = IDIM * JDIM;
  recv_size = 1;
  stop = client_space * 2;
  // get the number of games from the input file
  input >> NUM_GAMES ;
  for(int i=0;i<NUM_GAMES;++i) {  // for each game in file...
    string input_string ;
    input >> input_string ;

    if(input_string.size() != IDIM*JDIM) {
      cerr << "something wrong in input file format!" << endl ;
      MPI_Abort(MPI_COMM_WORLD,-1) ;
    }
    // read in the initial game state from file
    unsigned char buf[IDIM*JDIM] ;
    for(int j=0;j<IDIM*JDIM;++j)
      buf[j] = input_string[j] ;

    // Send every process 2 subproblems to work on while problems are dispensed
    if(i < stop + 1)
    {
        k = (i % client_space) + 1;
        MPI_Send(&buf, send_size+1, MPI_UNSIGNED_CHAR, k, 1, MPI_COMM_WORLD);
    }

    // Wait on clients to request more subproblems, then send these subproblems. This essentially functions like a priority queue
    // that assigns tasks to the fastest processors.
    else
    {
        MPI_Recv(&size, recv_size, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status); // How many subproblem-steps are being sent
        k = status.MPI_SOURCE; // Process sending the subproblems
        if(size > 0)
        {
           output << "found solution = " << endl;
           game_state s;
           for(int t = 0; t < size; ++t)
           {
               MPI_Recv(&rbuf, send_size+1, MPI_UNSIGNED_CHAR, k, 1, MPI_COMM_WORLD, &status); // Get solution step
               s.Init(rbuf);
               s.Print(output); // Record solution step
               output << "-->" << endl;
           }
           Output << "solved" << endl;
           Count++;
        }
        MPI_Send(&buf, send_size+1, MPI_UNSIGNED_CHAR, k, 1, MPI_COMM_WORLD); // Send new subproblem
        NUM_COMPLETE++;
    }
 }

 // Once all subproblems have been dispersed allow clients to finish
 while(NUM_COMPLETE < NUM_GAMES)
 {
      MPI_Recv(&size, recv_size, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
      k = status.MPI_SOURCE;
      if(size > 0)
      {
         output << "found solution = " << endl;
         game_state s;
         for(int t = 0; t < size; ++t)
         {
             MPI_Recv(&rbuf, send_size+1, MPI_UNSIGNED_CHAR, k, 1, MPI_COMM_WORLD, &status);
             s.Init(rbuf);
             s.Print(output);
             output << "-->" << endl;
         }
         output << "solved" << endl;
         count++;
      }
      MPI_Send(&rbuf, send_size+1, MPI_UNSIGNED_CHAR, k, 0, MPI_COMM_WORLD);  // Send tag 0, telling clients that there is no more information to process
      NUM_COMPLETE++;
  }
  // Report how cases had a solution.
  cout << "found " << count << " solutions" << endl ;
}

// Put the code for the client here
void Client() {
   unsigned char rec[IDIM*JDIM];
   MPI_Status status;
   Int rank, recv_size, send_size, tag, size;
   Tag = 1;
   Recv_size = IDIM * JDIM;
   send_size = 1;
   rank = MPI_Comm_rank(MPI_COMM_WORLD, &rank);

   // While there is information to process
   while(tag != 0)
   {
      // Recieve new buffer
      mPI_Recv(&rec, recv_size+1, MPI_UNSIGNED_CHAR, 0,  MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      tag = status.MPI_TAG;  // Make sure that this is valid information (if tag is 0, then this should be a previously processed problem).
      if(tag == 0)
         break;

      // initialize the game
      game_state game_board ;
      game_board.Init(rec) ;

      // If we find a solution to the game, put the results in
      // solution
      move solution[IDIM*JDIM] ;
      int size = 0 ;
      // Search for a solution to the puzzle
      bool found = depthFirstSearch(game_board,size,solution) ;

      // If the solution is found we want to output how to solve the puzzle
      // in the results file.
      if(found)
      {
         MPI_Send(&size, send_size, MPI_INT, 0, 1, MPI_COMM_WORLD); // Let Server know the size of the solution.
         game_state s ;
         s.Init(rec) ;
         for(int i=0;i<size;++i) {
            s.makeMove(solution[i]) ;
            s.SaveBoard(rec); // Load mobve into buffer
            MPI_Send(&rec, recv_size+1, MPI_UNSIGNED_CHAR, 0, 1, MPI_COMM_WORLD); // Send move to server for recording
          }
      }

      // If no solution was found, send size 0.
      else
      {
         size = 0;
         MPI_Send(&size, send_size, MPI_INT, 0, 1, MPI_COMM_WORLD);
      }
   }
}


int main(int argc, char *argv[]) {
  // This is a utility routine that installs an alarm to kill off this
  // process if it runs to long.  This will prevent jobs from hanging
  // on the queue keeping others from getting their work done.
  chopsigs_() ;

  // All MPI programs must call this function
  MPI_Init(&argc,&argv) ;


  int myId ;
  int numProcessors ;

  /* Get the number of processors and my processor identification */
  MPI_Comm_size(MPI_COMM_WORLD,&numProcessors) ;
  MPI_Comm_rank(MPI_COMM_WORLD,&myId) ;

  if(myId == 0) {
    // Processor 0 runs the server code
    get_timer() ;// zero the timer
    Server(argc,argv) ;
    // Measure the running time of the server
    cout << "execution time = " << get_timer() << " seconds." << endl ;
  } else {
    // all other processors run the client code.
    Client() ;
  }

  // All MPI programs must call this before exiting
  MPI_Finalize() ;
}
