//
// Created by Adam Hansson on 2017-09-15.
//

#ifndef SLOTTED_ALOHA_NODE_H
#define SLOTTED_ALOHA_NODE_H


class Node {
private:
    double q_retransmit;
    bool backlog;

public:
    Node(){
        this->q_retransmit = 0.1;
        this->backlog = false;
    }

    Node(double const &qr){
        this->q_retransmit = qr;
        this->backlog = false;
    }

    double get_q_retransmit(){
        return this->q_retransmit;
    }

    bool backlogged(){
        return this->backlog;
    }

    //Return true if successful retransmission
    bool retransmit(){

    }

    //Return true if packet is transmitted successful the first time
    bool transmit(){

    }
    void set_backlog(bool backlogged){
        this->backlog = backlogged;
    }

};


#endif //SLOTTED_ALOHA_NODE_H
