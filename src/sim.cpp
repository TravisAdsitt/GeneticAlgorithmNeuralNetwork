#include <math.h>
#include <iostream>
#include <random>
#include <ctime>
#include <vector>
#include "../include/utils.hpp"
#include "../include/agents.hpp"
#include "../include/config.hpp"

void run_sim(std::vector<Agent *> agents, Position *goal)
{
    for (int tick = 0; tick < NUM_TICKS_PER_GEN; tick++)
    {
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

std::vector<Agent *> *setup_agent_generation(Position *start_pos = NULL, std::vector<AgentDistancePair *> *based_on = NULL, MergeType mt = SingleSplit, float mutation_chance = 0.01)
{
    // Storage for our agents
    std::vector<Agent *> *agents = new std::vector<Agent *>;

    for (int agent_i = 0; agent_i < NUM_AGENTS_PER_GEN; agent_i++)
    {

        // Check if we are basing our agents on anything
        if (based_on)
        {
            int choice1 = get_rand_int(0, based_on->size() - 1), choice2 = get_rand_int(0, based_on->size() - 1);
            while (choice2 == choice1)
                choice2 = get_rand_int(0, based_on->size() - 1);
            // Using default mutation delta
            agents->push_back(new Agent(new Position(start_pos->x, start_pos->y), based_on->at(choice1)->agent, based_on->at(choice2)->agent, mt, mutation_chance));
        }
        else
        {
            // Create agents with two sensors, distance from goal x and y
            agents->push_back(new Agent(new Position(start_pos->x, start_pos->y), 2));
        }
    }

    return agents;
}

int main()
{
    generator.seed(time(0));
    std::vector<AgentDistancePair *> *closest = NULL;
    std::vector<Agent *> *agents = NULL;

    // Goal Location
    Position *new_pos = get_random_position(BOUNDARY_EDGE_LENGTH - 1, BOUNDARY_EDGE_LENGTH - 1);
    Position *goal = get_random_position(BOUNDARY_EDGE_LENGTH - 1, BOUNDARY_EDGE_LENGTH - 1);

    float max_distance = get_distance(Position(0, 0), Position(BOUNDARY_EDGE_LENGTH, BOUNDARY_EDGE_LENGTH));

    float mutation_chance = 0;
    float dist_perc = 0.10;

    for (int generation = 0; generation < NUM_GEN; generation++)
    {
        // Setup our generations Agents
        if (!closest)
        {
            // Setup our initial set of agents
            agents = setup_agent_generation(new_pos);
        }
        else
        {
            // Distance Percentage, approaches 0 as the best performing agent gets closer to the goal
            dist_perc = (*closest).at(0)->distance / max_distance;
            // Base our mutation chance on how close we are to the goal.
            mutation_chance = std::min(MUTATION_CHANCE_C_VALUE * pow(2, (dist_perc * MUTATION_CHANCE_LIMIT)), MAX_MUTATION_CHANCE);
            // Setup our next generation
            std::vector<Agent *> *new_agents = setup_agent_generation(new_pos, closest, AGENT_MERGE_STRATEGY, mutation_chance);

            // Clean up our previous generation
            for (int a = 0; a < agents->size(); a++)
                delete (*agents)[a];
            for (int adp = 0; adp < closest->size(); adp++)
                delete (*closest)[adp];

            delete agents;
            delete closest;

            agents = new_agents;
        }

        // Run our simulation
        run_sim(*agents, goal);

        // Rank our agents and take the configured number of top performers
        closest = get_closest_agents(*agents, goal, generation, NUM_AGENTS_SELECTED_EACH_GENERATION);

        if (PRINT_GENERATION_PERFORMANCE)
            std::cout << closest->at(0)->distance << "," << mutation_chance << std::endl;
    }

    return 0;
}