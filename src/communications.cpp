#include <iostream>
#include <cstdlib>

using namespace std;

#include "mpi.h"

#include "communications.hpp"

Message::Message(int type, int source)
{
    this->type = type;
    this->source = source;
}

Communications::Communications(int rank, int procs)
{
    this->has_done = false;
    this->rank = rank;
    this->procs = procs;
    for (int i = 1; i <= procs - 1; ++i)
        this->available_procs.push(i);

    cout << "----> Communicator " << rank << "/" << procs << " initialized." << endl;
}

bool Communications::is_master()
{
    return this->rank == 0;
}

bool Communications::give_away(Node &n)
{
    if (!this->available_procs.empty())
    {
        cout << "---- Sending node: " << endl;
        n.print_stats();
        int p = this->available_procs.front();
        int m = Message::PREPARE;
        this->available_procs.pop();

        // First tell the receiver that there's a node ready for it
        MPI::COMM_WORLD.Send(&m, 1, MPI::INTEGER, p, p);
        Node::Serialized s = n.serialize();

        // Then send the actual node's content.
        // Since the vectors length is variable it always sends the vector's
        // size before the actual vector's content.
        MPI::COMM_WORLD.Send(&s.unassigned_atoms_count, 1, MPI::INTEGER, p, p);
        MPI::COMM_WORLD.Send(s.unassigned_atoms,
                             s.unassigned_atoms_count,
                             MPI::INTEGER,
                             p,
                             p);
        MPI::COMM_WORLD.Send(&s.assignments_count, 1, MPI::INTEGER, p, p);
        MPI::COMM_WORLD.Send(s.assignments,
                             s.assignments_count,
                             MPI::INTEGER,
                             p,
                             p);
        MPI::COMM_WORLD.Send(&s.clauses_count, 1, MPI::INTEGER, p, p);
        MPI::COMM_WORLD.Send(s.clauses,
                             s.clauses_count,
                             MPI::INTEGER,
                             p,
                             p);
        cout << "---- Node given to " << p << endl;
        return true;
    }
    else
    {
        return false;
    }
}

void Communications::set_available(int worker)
{
    this->available_procs.push(worker);
}

Message Communications::wait_for_message()
{
    int m;
    MPI::Status s;
    MPI::COMM_WORLD.Recv(&m, 1, MPI::INTEGER, MPI::ANY_SOURCE, this->rank, s);
    return Message(m, s.Get_source());
}

bool Communications::any_message_waiting()
{
    return MPI::COMM_WORLD.Iprobe(MPI::ANY_SOURCE, MPI::ANY_TAG);
}

Node Communications::receive_node()
{
    Node::Serialized s;

    // The following code receives variable-length vectors from the master.
    // The pattern is: 1) receive the vector's length;
    //                 2) allocate the vector using such length;
    //                 3) receive the vector's data.

    MPI::COMM_WORLD.Recv(&s.unassigned_atoms_count, 
                         1, 
                         MPI::INTEGER, Communications::MASTER, 
                         this->rank);
    s.unassigned_atoms = new int[s.unassigned_atoms_count];
    MPI::COMM_WORLD.Recv(s.unassigned_atoms,
                         s.unassigned_atoms_count,
                         MPI::INTEGER, 
                         Communications::MASTER, 
                         this->rank);

    MPI::COMM_WORLD.Recv(&s.assignments_count,
                         1,
                         MPI::INTEGER,
                         Communications::MASTER,
                         this->rank);
    s.assignments = new int[s.assignments_count];
    MPI::COMM_WORLD.Recv(s.assignments,
                         s.assignments_count,
                         MPI::INTEGER,
                         Communications::MASTER,
                         this->rank);

    MPI::COMM_WORLD.Recv(&s.clauses_count,
                         1,
                         MPI::INTEGER,
                         Communications::MASTER,
                         this->rank);
    s.clauses = new int[s.clauses_count];
    MPI::COMM_WORLD.Recv(s.clauses,
                         s.clauses_count,
                         MPI::INTEGER,
                         Communications::MASTER,
                         this->rank);

    Node n(s);
    cout << "---- Received node: " << endl;
    n.print_stats();
    return n;
}

void Communications::tell_result(int result_code)
{
    MPI::COMM_WORLD.Send(&result_code,
                         1,
                         MPI::INTEGER,
                         Communications::MASTER,
                         Communications::MASTER);
}

void Communications::halt()
{
    int m = Message::ALL_DONE;
    for (int p = 1; p <= this->procs - 1; ++p)
    {
        MPI::COMM_WORLD.Send(&m, 1, MPI::INTEGER, p, p);
        cout << "Halting " << p << endl;
    }
}

int Communications::busy_workers()
{
    return (procs - 1) - this->available_procs.size();
}

// That's all, folks!

