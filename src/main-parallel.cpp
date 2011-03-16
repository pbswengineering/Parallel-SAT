#include <iostream>

using namespace std;

#include "mpi.h"

#include "communications.hpp"
#include "node.hpp"
#include "dpll-parallel.hpp"

int main(int argc, char *argv[])
{
    MPI::Init(argc, argv);

    Communications c(MPI::COMM_WORLD.Get_rank(),
                     MPI::COMM_WORLD.Get_size());
    
    if (c.is_master())
    {
        if (argc != 2)
        {
            cout << "Usage: " << argv[0] << " FILE" << endl;
            return 1;
        }

        cout << "DPLL based SAT solver, by Paolo Bernardi" << endl << endl;

        Node n(argv[1]);
        double start_time = MPI::Wtime();

        if (dpll(n, c))
            cout << "SAT" << endl;
        else
            cout << "UNSAT" << endl;

        double end_time = MPI::Wtime();
        cout << "Execution time: " << end_time - start_time << " secs." << endl;

        c.halt();
    }
    else // Worker processor
    {
        bool the_end = false;
        Node n;
        while (!the_end && !c.has_done)
        {
            Message m = c.wait_for_message();
            switch(m.type)
            {
            case Message::PREPARE:
                cout << "PREPARE received " << c.rank << endl;
                n = c.receive_node();
                dpll(n, c);
                cout << c.rank << " returing IDLE" << endl;
                break;
            case Message::ALL_DONE:
                cout << "ALL_DONE received " << c.rank << endl;
                the_end = true;
                break;
            default:
                // This should not be reached
                cerr << "A worker received an unexpected message!" << endl;
                the_end = true;
            }
        }
        cout << "proc " << c.rank << " done!" << endl;
    }

    MPI::Finalize();
    return 0;
}

// That's all, folks!

