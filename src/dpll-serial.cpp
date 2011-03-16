#include <iostream>
#include <stack>

using namespace std;

#include "dpll-serial.hpp"

bool dpll(Node &initial)
{
    stack<Node> s;
    // Make a copy, the original is needed later for the check
    s.push(Node(initial));

    while (!s.empty())
    {
        Node n = s.top();
        s.pop();

        // First, check if the procedure is over
        if (n.contains_zero_clauses())
        {
            cout << "Final assignments: " << endl;
            n.print_assignments();
            if (initial.is_satisfied_by(n))
                cout << "Ok, the solution looks good." << endl;
            else
                cout << "Ooops, the solution isn't really that good!!" << endl;
            return true;
        }

        else if (n.contains_empty_clause())
        {
            continue; // This clause is already false in the model...
        }

        // Otherwise, check if the clause set can be reduced (first 2 rules) or
        // if the assignment set can be expanded (last 2 rules).
        // WARNING: 1) this exploits short circuit logic
        //          2) n is modified by the calls, if they return true
        else if (n.apply_subsumption() || \
                n.apply_unit_resolution() || \
                n.apply_assertion() || \
                n.apply_pure_literal())
        {
            s.push(n);
        }
     
        // Otherwise, let's go with splitting   
        else
        {
            Node n2 = n;
            int a = n.find_unassigned_atom();
            n.assign(a, true);
            n2.assign(a, false);
            s.push(n);
            s.push(n2);
        }
    }

    return false;
}

// That's all, folks!

