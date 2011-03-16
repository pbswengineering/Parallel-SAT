#include <iostream>
#include <vector>

#include "clause.hpp"

using namespace std;

void Clause::add_literal(Literal l)
{
    this->literals.push_back(l); 
}

bool Clause::is_empty()
{
    return this->literals.empty();
}

int Clause::size()
{
    return this->literals.size();
}

Literal Clause::get_literal(int index)
{
    return this->literals[index];
}

bool Clause::find(Literal l)
{
    vector<Literal>::iterator i;
    for (i = this->literals.begin();
        i != this->literals.end() && (*i) != l;
        ++i)
        ;

    return i != this->literals.end();
}

bool Clause::erase(Literal l)
{
    vector<Literal>::iterator i;
    for (i = this->literals.begin();
        i != this->literals.end() && (*i) != l;
        ++i)
        ;

    if (i != this->literals.end())
    {
        this->literals.erase(i);
        return true;
    }
    else
        return false;
}

void Clause::print()
{
    for (vector<Literal>::iterator i = this->literals.begin();
        i != this->literals.end();
        ++i)
    {
        i->print();
        cout << " ";
    }
}

// That's all, folks!

