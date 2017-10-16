//
// Created by Adam Hansson on 2017-10-12.
//

#ifndef SLOTTED_ALOHA_RESENDCALCULATOR_H
#define SLOTTED_ALOHA_RESENDCALCULATOR_H

#include "feedback.h"

class ResendCalculator {
public:
    ResendCalculator(double arrivalRate);
    void update(Feedback status);
    double getProbability();
    double getEstimatedBacklog();

private:
    double n_estimate; //Estimate of the backlog
    double arrivalRate; //The arrival rate of each node
    double resendProbability; //The probability of retransmission
};


#endif //SLOTTED_ALOHA_RESENDCALCULATOR_H
