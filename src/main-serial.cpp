#include <iostream>

using namespace std;

#include "node.hpp"
#include "dpll-serial.hpp"

int main(int argc, char *argv[])
{
    cout << "DPLL based SAT solver, by Paolo Bernardi" << endl << endl;

    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << " FILE" << endl;
        return 1;
    }

    Node n(argv[1]);

    if (dpll(n))
        cout << "SAT" << endl;
    else
        cout << "UNSAT" << endl;

    return 0;
}

// That's all, folks!

