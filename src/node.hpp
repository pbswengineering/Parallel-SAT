#ifndef __NODE_HPP__
#define __NODE_HPP__

#include <map>
#include <set>
#include <vector>

using namespace std;

#include "clause.hpp"
#include "literal.hpp"

// A node of the DPLL's search tree; contains both a clauses' set and
// an assignments' set. Dividing them would be way too messy.
class Node
{
private:
    vector<Clause> clauses;
    map<Literal, int> literals; // How many times it appears in the clauses
    map<int, bool> assignments;
    set<int> unassigned_atoms;
public:
    // Serialization related stuff
    class Serialized {
    public:
        int unassigned_atoms_count;
        int *unassigned_atoms;
        int assignments_count;
        int *assignments;
        int clauses_count;
        int *clauses;
        Serialized():unassigned_atoms(0),
                     assignments(0),
                     clauses(0)
        { };
        ~Serialized();
    };
    Node(Node::Serialized&);
    Node::Serialized serialize();

    Node() { };
    Node(char*);
    Node(const Node&);
    
	void add_clause(Clause);
    void remove_literal(Literal);

    // Checks for DPLL's termination
    bool contains_zero_clauses();
    bool contains_empty_clause();

    // DPLL rules
    bool apply_subsumption();
    bool apply_unit_resolution();
    bool apply_assertion();
    bool apply_pure_literal();

    // Assignements set management
    void assign(int, bool);
    bool is_assigned(int);
    bool assignment(int);
    int find_unassigned_atom();

    // Debug stuff, to be deleted sometime, maybe...
    void print_clauses();
    void print_assignments();
    void print_literals();
    void print_stats() {
	cout << "Literals: " << literals.size()
             << " (" << zeros() << ")" << endl
             << "Clauses: " << clauses.size() << endl
             << "Assignments: " << assignments.size() << endl
             << "Unassigned atoms: " << unassigned_atoms.size() << endl;
    }
	int zeros()
	{
		int z = 0;
		for (map<Literal, int>::iterator i = literals.begin();
		     i != literals.end();
		     ++i)
		{
			if (i->second == 0) ++z;
		}
		return z;
	}
    bool satisfies(Clause&);
    bool is_satisfied_by(Node&);
};

#endif // __NODE_HPP__

