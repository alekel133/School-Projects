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

    while(input != "STOP")
    {
        cin >> input;
        if(input != "STOP")
            list.insert(input); 
    }

    cout << endl << "Reversed: " << endl;

    list.reverse(cout);

    return 0;
}
