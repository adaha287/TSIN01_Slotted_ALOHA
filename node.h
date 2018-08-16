//
// Created by Adam Hansson, adaha287, 950517-4517 on 2017-09-15.
//

#ifndef SLOTTED_ALOHA_NODE_H
#define SLOTTED_ALOHA_NODE_H

#include <random>

class BasicNode{
public:
    BasicNode(double qa);
    virtual bool step(double qr, int step) = 0;
    static void initRandomizer();
    bool get_backlog();
    bool gotNewPacket();
    int getDelay(int departureSlot);

protected:
    double qa;
    bool backlog;
    int arrivalSlot;
    bool newPacket;
    static std::uniform_real_distribution<double> randomUniDistr;
    static std::default_random_engine engine;

private:
};

//Normal node used in slotted ALOHA without PB stabilization
class Node : public BasicNode{
public:
    Node(double qa) : BasicNode(qa) {}
    virtual bool step(double qr, int slotNumber);

protected:

private:
};

//Node with PB stabilization implemented
class PseudoBayesianNode : public BasicNode{
public:
    PseudoBayesianNode(double qa) : BasicNode(qa) {}
    virtual bool step(double qr, int slotNumber);

protected:

private:
};

#endif //SLOTTED_ALOHA_NODE_H
