/*
    Author: Alexander Kellough
    MSU NetID: atk133
    Compiler: GCC
    Description:
        Program that takes input, stores it in a linked list, and outputs the data in reverse.

 */
#include <iostream>
#include "List.h"

using std::string;
using std::cout;
using std::cin;
using std::endl;

int main()
{
    List list;
    string input; 

    cout << "Enter strings to be reversed. Enter [STOP] when finished:" << endl;

    // Input loop
    while(input != "STOP")
    {
        cin >> input;
        if(input != "STOP")
            list.insert(input); 
    }

    cout << endl << "Reversed: " << endl;
    
    // Call to linked list reverse function.
    list.reverse(cout);

    return 0;
}
