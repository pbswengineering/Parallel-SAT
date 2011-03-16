#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <cmath>
#include <iostream>

using namespace std;

#include "node.hpp"

Node::Serialized::~Serialized()
{
    if (unassigned_atoms != 0) delete unassigned_atoms;
    if (assignments != 0) delete assignments;
    if (clauses != 0) delete clauses;
}

Node::Node(Node::Serialized& s)
{
    int i;

    // Someday I'll find out why copy doesn't work here
    this->unassigned_atoms.clear();
    for (i = 0; i < s.unassigned_atoms_count; ++i)
        this->unassigned_atoms.insert(s.unassigned_atoms[i]);

    this->assignments.clear();
    for (i = 0; i < s.assignments_count; ++i)
    {
        int atom = abs(s.assignments[i]);
        this->assignments[atom] = (s.assignments[i] > 0);
    }

    // The following takes care of both clauses and literals list
    this->clauses.clear();
    this->literals.clear();
    i = 0;
    while (i < s.clauses_count)
    {
        Clause c;
        while (s.clauses[i] != 0)
        {
            Literal l(s.clauses[i]);
            c.add_literal(l);
            this->literals[l]++;
            ++i;
        }
        this->add_clause(c);
        ++i;
    }
}

class Node::Serialized Node::serialize()
{
    Node::Serialized s;
    int pos;

    // The unassigned atoms list can be just copied into a vector
    s.unassigned_atoms_count = this->unassigned_atoms.size();
    s.unassigned_atoms = new int[s.unassigned_atoms_count];
    copy(this->unassigned_atoms.begin(),
         this->unassigned_atoms.end(),
         s.unassigned_atoms);

    // The assignments keys are atoms, so, positive integers
    // Here a true atom will become (remain) a positive number, a false one a
    // negative number. Such number is of course the atom's index.
    pos = 0; // Why here? It's not that easy to put it in the for init section!
    s.assignments_count = this->assignments.size();
    s.assignments = new int[s.assignments_count];
    for (map<int, bool>::iterator i = this->assignments.begin();
        i != this->assignments.end();
        ++i, ++pos)
    {
        s.assignments[pos] = i->first * (i->second ? 1 : -1);
    }

    // I'm not going to serialize the literal list because it can be built
    // back when the receiver will deserialize the clauses.

    // The clauses are going to be "flattened" into a vector... Each literal
    // will be encoded as integer (negative, in case of negation) and each
    // clause will be separated by a 0 value.
    vector<Clause>::iterator i;
    int len = 0;
    // First, count how many literals there are in every clause
    for (i = this->clauses.begin(); i != this->clauses.end(); ++i)
        len += i->size();
    len += this->clauses.size(); // The 0's take space too!

    s.clauses_count = len;
    s.clauses = new int[len];
    pos = 0;
    for (i = this->clauses.begin(); i != this->clauses.end(); ++i)
    {
        for (vector<Literal>::iterator k = i->begin(); k != i->end(); ++k)
        {
            s.clauses[pos] = k->to_int();
            ++pos;
        }
        s.clauses[pos] = 0;
        ++pos;
    }

    return s;
}

Node::Node(char *input_file)
{
	ifstream f(input_file);
	char c;
    string s;
    int atoms;
    int clauses;
	while (f >> s)
	{
        if (s == "p") // Prefix line, with file format, atoms and clauses
        {
            f >> s;
            if (s != "cnf")
            {
                cerr << "Sorry, I can only handle the 'cnf' format." << endl \
                     << "I don't know what '" << s << "' is, my apologies." \
                     << endl;
                exit(1);
            }
            f >> atoms;
            f >> clauses;
            cout << "Atoms: " << atoms << endl;
            cout << "Clauses: " << clauses << endl;
            continue;
        }
        else if (s == "c") // Comment line
        {
            while (f.get(c) && (c != '\n'))
                ; // Just skipping the comment line...
        }
        else // It's a clause!
        {
            int n = atoi(s.c_str()); // The current token is a literal already!
            Clause cl;
            while (n != 0)
            {
                Literal l = Literal(n);
                cl.add_literal(l);
                this->literals[l]++;
                int atom = l.get_atom();
                this->unassigned_atoms.insert(atom);
                if (!(f >> n)) // Could leave the loop also because of EOF...
                    break;
            }

            this->add_clause(cl);
            
            if (n != 0) // Did I leave the loop because of EOF?
            {
                cerr << "I am sorry but I really need every clause to end " \
                     << "with a 0. " << endl << "Can you check the input file" \
                     << " please?" << endl;
                exit(1);
            }
        }
	}
	f.close();
}

Node::Node(const Node &n)
{
    this->clauses = n.clauses;
    this->literals = n.literals;
    this->assignments = n.assignments;
    this->unassigned_atoms = n.unassigned_atoms;
}

void Node::add_clause(Clause c)
{
	this->clauses.push_back(c);
}

void Node::remove_literal(Literal l)
{
    if (this->literals[l] > 0)
        --this->literals[l];
}

/// Node's assignmens set management functions

void Node::assign(int a, bool v)
{
    this->assignments[a] = v;
    this->unassigned_atoms.erase(a);
}

bool Node::is_assigned(int a)
{
    return this->assignments.find(a) != this->assignments.end();
}

bool Node::assignment(int a)
{
    return this->assignments[a];
}

int Node::find_unassigned_atom()
{
    return *(this->unassigned_atoms.begin());
}

/// Node's DPLL helper functions

bool Node::contains_zero_clauses()
{
    return this->clauses.empty();
}

bool Node::contains_empty_clause()
{
    for (vector<Clause>::iterator i = this->clauses.begin();
         i != this->clauses.end();
         ++i)
    {
        if ((*i).is_empty())
            return true;
    }

    return false;
}

bool Node::apply_subsumption()
{
    // Is there any literal that can evaluate its clause to true?
    map<int, bool>::iterator i;
    vector<Clause>::iterator k;
    for (i = this->assignments.begin(); i != this->assignments.end(); ++i)
        for (k = this->clauses.begin(); k != this->clauses.end(); ++k)
        {
            Literal l(i->second ? i->first : -i->first);
            if (k->find(l))
            {
                for (vector<Literal>::iterator x = k->begin();
                    x != k->end();
                    ++x)
                {
                    this->remove_literal(*x);
                }
                this->clauses.erase(k);
                return true;
            }
        }

    return false;
}

bool Node::apply_unit_resolution()
{
    // Is there any literal that cannot evaluate its clause to true?
    map<int, bool>::iterator i;
    vector<Clause>::iterator k;
    for (i = this->assignments.begin(); i != this->assignments.end(); ++i)
        for (k = this->clauses.begin(); k != this->clauses.end(); ++k)
        {
            Literal l(i->second ? -i->first : i->first);
            if (k->erase(l))
            {
                this->remove_literal(l);
                return true;
            }
        }

    return false;
}

bool Node::apply_assertion()
{
    // Is there any single literal clause?
    vector<Clause>::iterator i;
    for (i = this->clauses.begin(); i != this->clauses.end(); ++i)
        if (i->size() == 1)
        {
            this->assign(i->get_literal(0).get_atom(), \
                        i->get_literal(0).is_negated() ? false : true);
            return true;
        }

    return false;
}

bool Node::apply_pure_literal()
{
    // Is there any pure literal?
    map<Literal, int>::iterator i;
    for (i = this->literals.begin(); i != this->literals.end(); ++i)
        if ((i->second != 0) && (this->literals[i->first.opposite()] == 0))
        {
            this->assign(i->first.get_atom(), \
                        i->first.is_negated() ? false : true);
            return true;
        }
    
    return false;
}

///// For extra safety...

bool Node::satisfies(Clause &c)
{
    for (vector<Literal>::iterator i = c.begin();
        i != c.end();
        ++i)
    {
        if (i->is_satisfied_by(this->assignment(i->get_atom())))
            return true;
    }

    return false;
}

bool Node::is_satisfied_by(Node &n)
{
    // Every clause must be satisfiable
    for (vector<Clause>::iterator i = this->clauses.begin();
        i != this->clauses.end();
        ++i)
    {
        if (!(n.satisfies(*i)))
            return false;
    }

    return true;
}

void Node::print_clauses()
{
    for (vector<Clause>::iterator i = this->clauses.begin();
        i != this->clauses.end();
        ++i)
    {
        cout << "=> ";
        i->print();
        cout << endl;
    }
}

void Node::print_literals()
{
    for (map<Literal, int>::iterator i = this->literals.begin();
        i != this->literals.end();
        ++i)
    {
        i->first.print();
        cout << "(" << i->second << ") ";
    }
    cout << endl;
}

void Node::print_assignments()
{
    for (map<int, bool>::iterator i = this->assignments.begin();
        i != this->assignments.end();
        ++i)
    {
        cout << i->first << " = " << i-> second << endl;
    }
}

// That's all, folks!

