#include <iostream>
#include <vector>
#include "node.h"
#include <math.h>
#include <random>

using namespace std;

int main() {
    int const m = 100;
    double q_r = 0.2;
    vector<Node> nodes(m, Node(q_r));
    int const slots = 1;
    vector<int> backlogged_nodes{};
    double arrival_rate = 1/exp(1);
    poisson_distribution<int> poisson(arrival_rate);
    default_random_engine generator;
    cout << "Testing poisson numbers:" << poisson(generator) << endl;


    //Simulation loop runs for 'slots' number of times
    for (int i{}; i < slots; ++i){
        int number_of_backlogged_nodes{};
        for(Node& node : nodes) {
            cout << "Testing poisson numbers:" << poisson(generator) << endl;
            bool success{};
            if (node.backlogged()) {
                success = node.retransmit();
            }
            else {
                success = node.transmit();
            }
            //If success set to not be backlogged
            node.set_backlog(!success);
            if(!success){
                ++number_of_backlogged_nodes;
            }
        }
        //Save number of backlogged nodes
        backlogged_nodes.push_back(number_of_backlogged_nodes);
    }

    return 0;
}