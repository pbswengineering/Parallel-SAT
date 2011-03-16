#ifndef __COMMUNICATIONS_HPP__
#define __COMMUNICATIONS_HPP__

#include <queue>

using namespace std;

#include "node.hpp"

class Message
{
public:
    // Master -> Workers signals
    static const int PREPARE = 1;
    static const int ALL_DONE = 2;

    // Workers -> Master signals
    static const int SAT = 3;
    static const int UNSAT = 4;

    int type;
    int source;

    Message(int, int);
};

class Communications
{
private:
    int procs;
    queue<int> available_procs;
public:
    bool has_done;
    static const int MASTER = 0;

    int rank; // It should become private
    Communications(int, int);
    bool is_master();
    bool give_away(Node&);
    void set_available(int);
    Node receive_node();
    Message wait_for_message();
    bool any_message_waiting();
    void tell_result(int);
    int busy_workers();
    void halt();
};

#endif // __COMMUNICATIONS_HPP__

