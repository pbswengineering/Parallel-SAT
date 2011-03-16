#include <iostream>
#include <stack>

using namespace std;

#include "communications.hpp"
#include "dpll-parallel.hpp"

bool dpll(Node &initial, Communications &c)
{
    stack<Node> s;
    // Make a copy, the original is needed later for the check
    s.push(Node(initial));

    while (!s.empty())
    {
        // Before doing anything, check if someone else has already found the
        // solution to the problem
        if (c.any_message_waiting())
        {
            Message m = c.wait_for_message();
            cout << "Node " << c.rank << " has an awaiting message " 
                 << m.type << " from " << m.source << endl;
            if (c.is_master())
            {
                if (m.type == Message::SAT) // A worker did it!
                {
                    cout << "A worker did it! Which one?" << endl;
                    return true;
                }
                else // The worker didn't find a solution; it maybe used later.
                {
                    cout << "Worker " << m.source << " RESCHEDULED" << endl;
                    c.set_available(m.source);
                }
            }
            else // Worker
            {
                if (m.type == Message::ALL_DONE) // The master did it!
                {
                    cout << "The master did it!" << endl;
                    c.has_done = true;
                    return true;
                }
            }
        }

        Node n = s.top();
        s.pop();

        // First, check if the procedure is over
        if (n.contains_zero_clauses())
        {
            cout << " ########## SAT: " << c.rank << endl;

            if (!c.is_master())
                c.tell_result(Message::SAT);

            if (initial.is_satisfied_by(n))
                cout << "Ok, the solution looks good." << endl;
            else
                cout << "Ooops, the solution isn't really that good!!" << endl;
            return true;
        }
        else if (n.contains_empty_clause())
        {
            continue; // This wasn't the good leaf...
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
            
            // Can any other processor do some job for the master?
            if (!c.is_master() || !c.give_away(n2))
                s.push(n2);
        }
    }

    cout << " ########## UNSAT: " << c.rank << endl;
    
    if (!c.is_master())
    {
        c.tell_result(Message::UNSAT);
        return false;
    }
    else // Master
    {
        if (c.busy_workers() > 0)
            cout << "Must wait still " << c.busy_workers() << " workers." << endl;
        // Some worker may still be active and find out a solution...
        while (c.busy_workers() > 0)
        {
            Message m = c.wait_for_message();
            if (m.type == Message::SAT)
                return true;
            else
                c.set_available(m.source);
        }
        return false;
    }
}

// That's all, folks!

