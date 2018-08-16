//
// Created by Adam Hansson, adaha287, 950517-4517 on 2017-10-12.
//

#include <cmath>
#include <algorithm>
#include "ResendCalculator.h"
#include <iostream>

ResendCalculator::ResendCalculator(double arrivalRate)
    : arrivalRate{arrivalRate}, n_estimate{arrivalRate}, resendProbability{1/arrivalRate}
{
}

// Update estimate of backlog and retransmission probability
void ResendCalculator::update(Feedback status) {
    if(status == Feedback::COLLISION){
        n_estimate = n_estimate + arrivalRate + pow(exp(1)-2, -1);
    }
    else{
        n_estimate = std::max(arrivalRate, n_estimate + arrivalRate -1);
    }

    if(n_estimate >= 0 && n_estimate < 1){
        resendProbability = 1.0;
    } else if(n_estimate >= 1){
        resendProbability = 1.0 / n_estimate;
    }else{
        std::cout << "Error when calculating resendProbability, n_estimate is less than 0!!" << std::endl;
    }
}

double ResendCalculator::getProbability() {
    return resendProbability;
}

double ResendCalculator::getEstimatedBacklog() {
    return n_estimate;
}
