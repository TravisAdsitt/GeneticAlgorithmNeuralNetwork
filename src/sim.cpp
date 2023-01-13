#include <math.h>
#include <iostream>
#include <random>
#include <ctime>
#include <vector>
#include "../include/utils.hpp"
#include "../include/agents.hpp"
#include "../include/config.hpp"

void run_sim(std::vector<Agent *> agents)
{
    // Goal Location
    Position *goal = get_random_position(BOUNDARY_EDGE_LENGTH, BOUNDARY_EDGE_LENGTH);

    for (int tick = 0; tick < NUM_TICKS_PER_GEN; tick++)
    {
        std::cout << "Tick " << tick << "!" << std::endl;
        for (Agent *a : agents)
        {
            // Sense the Agents distance from the goal
            float sensors[2] = {
                (a->positions.back()->x - goal->x) / BOUNDARY_EDGE_LENGTH,
                (a->positions.back()->y - goal->y) / BOUNDARY_EDGE_LENGTH};
            // Ask the Agent what it wants to do
            a->move(sensors);
        }
    }
}

void change_my_int(int &changeme)
{
    changeme = 5;
}

int main()
{
    generator.seed(time(0));
    // Storage for our agents
    std::vector<Agent *> agents;

    for (int agent_i = 0; agent_i < NUM_AGENTS_PER_GEN; agent_i++)
    {
        // Create agents with two sensors, distance from goal x and y
        agents.push_back(new Agent(get_random_position(BOUNDARY_EDGE_LENGTH, BOUNDARY_EDGE_LENGTH), 2));
    }

    run_sim(agents);

    return 0;
}