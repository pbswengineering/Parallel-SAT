#ifndef __CLAUSE_HPP__
#define __CLAUSE_HPP__

#include <vector>

using namespace std;

#include "literal.hpp"

class Clause
{
private:
    vector<Literal> literals;
public:
    void add_literal(Literal);
    Literal get_literal(int);
    bool is_empty();
    int size();
    bool find(Literal);
    bool erase(Literal);
    vector<Literal>::iterator begin() { return literals.begin(); };
    vector<Literal>::iterator end() { return literals.end(); };
    void print();
};

#endif // __CLAUSE_HPP__

