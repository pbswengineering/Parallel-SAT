#include <iostream>

#include "literal.hpp"

using namespace std;

Literal::Literal(int l)
{
    this->literal = l;
}

bool Literal::is_negated() const
{
    return this->literal < 0;
}

bool Literal::is_satisfied_by(bool v) const
{
    return ((this->literal < 0) && (!v)) || ((this->literal > 0) && v);
}

int Literal::get_atom() const
{
    return (this->literal > 0 ? this->literal : -this->literal);
}

int Literal::to_int() const
{
    return this->literal;
}

Literal Literal::opposite() const
{
    return Literal(-this->literal);
}

bool operator< (const Literal &a, const Literal &b)
{
    return a.literal < b.literal;
}

bool operator!= (const Literal &a, const Literal &b)
{
    return a.literal != b.literal;
}

void Literal::print() const
{
    cout << this->literal;
}

// That's all, folks!

