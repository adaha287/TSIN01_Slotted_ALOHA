//
// Created by Adam Hansson on 2017-09-15.
//


#include <iostream>
#include "node.h"

std::uniform_real_distribution<double> BasicNode::randomUniDistr{0, 1}; //Random generator
std::default_random_engine BasicNode::engine{}; //Used with random generator

BasicNode::BasicNode(double qa)
    : qa{qa}, backlog{false}, arrivalSlot{0}, newPacket{false} {
}

void BasicNode::initRandomizer() {
    std::random_device rd{};
    engine.seed(rd());
}

//Calculate the delay of a packet at successful transmission
int BasicNode::getDelay(int departureSlot) {
    int delay = departureSlot - arrivalSlot;
    backlog = false;
    newPacket = false;
    return delay;
}

bool BasicNode::get_backlog() {
    return backlog;
}

bool BasicNode::gotNewPacket() {
    return newPacket;
}

//Calculate whether this node transmits a packet current slot or not
bool Node::step(double qr, int slotNumber){
    double randomNumber = randomUniDistr(engine);

    newPacket = false;
    if(backlog){
        return randomNumber < qr;
    }
    else{
        if(randomNumber < qa){ //Receive new packet
            arrivalSlot = slotNumber;
            newPacket = true;
            backlog = true;
            return true;
        }
    }
    return false;
}

//Calculate whether this node transmits a packet current slot or not
bool PseudoBayesianNode::step(double qr, int slotNumber) {
    //check if we have no backlogged packet and get a new packet
    newPacket = !backlog && randomUniDistr(engine) < qa;

    //If a new packet arrive, set backlog to true and save current slot number as arrivalSlot
    if(newPacket){
        arrivalSlot = slotNumber;
        backlog = true;
    }

    //Return whether the node have and tries to transmit current packet
    return backlog && randomUniDistr(engine) < qr;
}
