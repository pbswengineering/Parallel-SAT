#ifndef __LITERAL_HPP__
#define __LITERAL_HPP__

class Literal
{
private:
    int literal;
public:
    Literal(int);
    bool is_negated() const;
    bool is_satisfied_by(bool) const;
    int get_atom() const;
    int to_int() const;
    Literal opposite() const;
    void print() const;
    friend bool operator< (const Literal&, const Literal&);
    friend bool operator!= (const Literal&, const Literal&);
};

bool operator< (const Literal&, const Literal&);
bool operator!= (const Literal&, const Literal&);

#endif // __LITERAL_HPP__

