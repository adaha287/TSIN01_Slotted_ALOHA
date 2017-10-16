//
// Created by Adam Hansson on 2017-09-19.
//

#include <iostream>
#include <vector>
#include "node.h"
#include <cmath>
#include "ResendCalculator.h"
#include "Feedback.h"
#include <fstream>
#include <map>

using namespace std;

int main() {
    int const slots = 1000;
    int const m{100}; //Number of nodes in the system
    double q_r{0.01}; //Probability of retransmission
    double arrival_rate{0.35}; //Overall arrival rate
    double q_a = 1-exp(-arrival_rate/m); //Probability of arrival for each node
    int totalDelay{0};
    ResendCalculator bayesianProbability(arrival_rate); //Calculator to keep track of estimate of backlog and resend
    // probability

    vector<BasicNode*> nodes{}; //Hold all nodes in the system
    bool pseudoBayesianStabilization = true; //Set to true to enable pseudo-Bayesian stabilization
    BasicNode::initRandomizer(); //Seed the randomizer

    //Create and initialize nodes
    for(int i{}; i < m; ++i){
        if(pseudoBayesianStabilization){
            nodes.push_back(new PseudoBayesianNode(q_a));
        }
        else{
            nodes.push_back(new Node(q_a));
        }
    }

    vector<BasicNode*> sendList{}; //All nodes that transmit a certain slot.

    //Open files for data printing
    ofstream delayFile;
    ofstream backlogFile;
    ofstream enteredPacketsFile;
    ofstream leftPacketsFile;
    ofstream stateDiagramFile;
    ofstream attemptRateFile;
    ofstream estimatedBacklogFile;
    delayFile.open("delay.data", ofstream::out | ofstream::trunc);
    enteredPacketsFile.open ("entered.data", ofstream::out | ofstream::trunc);
    leftPacketsFile.open ("left.data", ofstream::out | ofstream::trunc);
    backlogFile.open ("backlog.data", ofstream::out | ofstream::trunc);
    stateDiagramFile.open ("state.data", ofstream::out | ofstream::trunc);
    attemptRateFile.open("attemptRate.data", ofstream::out | ofstream::trunc);
    estimatedBacklogFile.open("estimatedBacklog.data", ofstream::out | ofstream::trunc);

    int entered_packets{}; //Number of packets that have entered the system
    int left_packets{}; //Number of packets that have left the system
    map<int, int> states{}; //Save current state for histogram calculation

    //Simulation loop runs for 'slots' number of times
    for (int i{1}; i <= slots; ++i) {
        Feedback status = Feedback::IDLE;

        if(pseudoBayesianStabilization){
            q_r = bayesianProbability.getProbability(); //Get qr
        }

        int number_of_backlogged_nodes{};
        sendList.clear();

        //Loop through all nodes  each slot
        for (auto node : nodes) {
            if (node->get_backlog()) {
                ++number_of_backlogged_nodes;
            }
            if (node->step(q_r, i)) { //step the current node, it returns whether it transmits a packet or not
                sendList.push_back(node);  //Add node to list with nodes that transmits this slot
            }
            if (node->gotNewPacket()) {
                ++entered_packets;
            }
        }

        //If successful transmission
        if (sendList.size() == 1) {
            status = Feedback::SUCCESS;
            ++left_packets;
            int delay = sendList.back()->getDelay(i);
            totalDelay += delay;
            delayFile << i << " " << delay << endl; //Print delay to file

        } else if (sendList.size() > 1) { //collision
            status = Feedback::COLLISION;
        }


        if(pseudoBayesianStabilization){
            bayesianProbability.update(status); //Update estimate of backlog and retransmission probability
            estimatedBacklogFile << i << " " << bayesianProbability.getEstimatedBacklog() << endl;  //Save estimated backlog
        }

        double attemptRate = (m - number_of_backlogged_nodes)*q_a + number_of_backlogged_nodes*q_r; //Calculate
        // attempt rate

        states[number_of_backlogged_nodes]++; //Save how many times each state appears
        enteredPacketsFile << i << " " << entered_packets << endl; //Totally how many packets have entered the system
        leftPacketsFile << i << " " << left_packets << endl; //Totally how many packets have left the system
        backlogFile << i <<" " << number_of_backlogged_nodes << endl; //How many currently backlogged nodes
        attemptRateFile << i << " " << attemptRate << endl; // Attempt rate
    }

    // Ps theoretical = (Qa(1,n)*Qr(0,n) + Qa(0,n)*Qr(1,n)) * stateProbability[n]
    double Ps_theoretical{0};


    //Calculate theoretical value for the probability of success
    if(!pseudoBayesianStabilization){
        // Ps theoretical = Qa(1,n)*Qr(0,n) + Qa(0,n)*Qr(1,n) * P(state==n)
        for(int n{0}; n <= m ; ++n) {
            double QaOn = pow(1 - q_a, m - n);
            double Qa1n = (m - n) * pow(1 - q_a, m - n - 1) * q_a;
            double QrOn = pow(1 - q_r, n);
            double Qr1n = n * pow(1 - q_r, n - 1) * q_r;
            Ps_theoretical += (Qa1n * QrOn + QaOn * Qr1n) * states[n] / 1000;
        }
        cout << "Ps theoretical: " << Ps_theoretical << endl;

    }

    //Simulated value for the probability of success
    double Ps_simulation = double(left_packets)/double(slots);
    cout << "Ps simulated: " << Ps_simulation << endl;

    cout << "Average delay:" << double(totalDelay) / double(left_packets) << endl;

    //Send histogram data to file
    for(int i{}; i <= m; ++i){
        if(states.find(i) != states.end()){
            stateDiagramFile << i << " " << states[i] << endl;
        } else{
            stateDiagramFile << i << " " << 0 << endl;
        }
    }

    //Close open files
    estimatedBacklogFile.close();
    delayFile.close();
    enteredPacketsFile.close();
    leftPacketsFile.close();
    backlogFile.close();
    stateDiagramFile.close();
    attemptRateFile.close();

    //Free allocated memory
    while(!nodes.empty()){
        delete nodes.back();
        nodes.pop_back();
    }

    return 0;
}
