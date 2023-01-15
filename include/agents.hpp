#include "neural_network.hpp"
#include "utils.hpp"
#include "config.hpp"
#include <SFML/Graphics.hpp>
#include <cstdio>
#include <iostream>

#ifndef AGENT_H
#define AGENT_H

/**
 * @brief Agents have brains and can be based on other Agents.
 *
 */
struct Agent
{
private:
    /**
     * @brief Mutate the Neural Network of this Agent.
     *
     * @param mutation_chance The chance any given weight will be mutated.
     */
    void mutate(float mutation_chance)
    {
        if (mutation_chance == 0)
            return;

        // Mutate our NN
        nn->mutate(mutation_chance);
    }

public:
    std::vector<Position *> positions;
    NeuralNetwork *nn;
    int num_sensors, num_controls;

    /**
     * @brief Construct a new Agent object
     *
     * Number of controls is hardcoded here, all of the movement logic is contained in the Agent
     *
     * @param pos The starting position for this Agent
     * @param num_sensors The number of sensors this agent will have, basically the number of inputs to our Neural Network
     */
    Agent(Position *pos, int num_sensors) : num_sensors(num_sensors)
    {
        num_controls = 4; // X-Delta, Y-Delta, X-Positive, Y-Positive
        nn = new NeuralNetwork(num_sensors, num_controls);
        positions.push_back(pos);
    }

    /**
     * @brief Construct a new Agent object
     *
     * @param pos The start position for this Agent
     * @param ancestor1 Ancestor 1 for NN
     * @param ancestor2 Ancestor 2 for NN
     * @param mt The merge strategy for merging the two agents
     * @param mutation_chance Mutation chance for any given weight in the NN
     */
    Agent(Position *pos, Agent *ancestor1, Agent *ancestor2, MergeType mt = SingleSplit, float mutation_chance = MAX_MUTATION_CHANCE) : num_controls(ancestor1->num_controls), num_sensors(ancestor1->num_sensors)
    {
        nn = new NeuralNetwork(ancestor1->nn, ancestor2->nn, mt);
        positions.push_back(pos);
        mutate(mutation_chance);
    }

    ~Agent()
    {
        for (int pos = 0; pos < positions.size(); pos++)
            delete positions[pos];
        delete nn;
    }

    /**
     * @brief Allow this agent to make its next move
     *
     * @param sensor_input The input for the Agent to base its decision off of
     */
    void move(float *sensor_input)
    {
        // Get how much this agent wants to move
        float *move_deltas = nn->predict(sensor_input);

        // Get and store its next position
        Position *curr_pos = positions.back();
        float next_pos_x = move_deltas[2] > 0.5 ? (curr_pos->x + move_deltas[0]) : (curr_pos->x - move_deltas[0]);
        float next_pos_y = move_deltas[3] > 0.5 ? (curr_pos->y + move_deltas[1]) : (curr_pos->y - move_deltas[1]);

        // Check for boundaries
        if (next_pos_x < 0)
            next_pos_x = 0;
        if (next_pos_x > BOUNDARY_EDGE_LENGTH - DRAW_OBJECT_SIZE)
            next_pos_x = BOUNDARY_EDGE_LENGTH - DRAW_OBJECT_SIZE;
        if (next_pos_y < 0)
            next_pos_y = 0;
        if (next_pos_y > BOUNDARY_EDGE_LENGTH - DRAW_OBJECT_SIZE)
            next_pos_y = BOUNDARY_EDGE_LENGTH - DRAW_OBJECT_SIZE;

        // Store location
        positions.push_back(new Position(next_pos_x, next_pos_y));

        // Free our float array
        free(move_deltas);
    }
};

/**
 * @brief This is used at the end of a generation to help sort out how well all Agents performed
 *
 */
struct AgentDistancePair
{
    Agent *agent;
    float distance;

    AgentDistancePair(Agent *agent, float distance) : agent(agent), distance(distance) {}
};

/**
 * @brief Used in the sort function to base the performance of all Agents on their final positions distance from the goal
 *
 * @param first
 * @param second
 * @return true
 * @return false
 */
bool compare_agent_distance_pair(AgentDistancePair *first, AgentDistancePair *second)
{
    return bool(first->distance < second->distance);
}

/**
 * @brief Helper function to draw a generations moves
 *
 * @param agents_and_distances Agent and Distance pairs
 * @param goal The position of the goal they are trying to get to
 */
void draw_agent_path(std::vector<AgentDistancePair *> *agents_and_distances, Position *goal, int generation_number)
{
    // create the window
    sf::RenderWindow window(sf::VideoMode(BOUNDARY_EDGE_LENGTH, BOUNDARY_EDGE_LENGTH), "Generation " + std::to_string(generation_number));
    sf::RectangleShape shape;
    sf::Clock clock;
    sf::Time accumulator = sf::Time::Zero;
    

    while (window.isOpen())
    {
        int num_moves = (*agents_and_distances).at(0)->agent->positions.size();

        for (int move = 0; move < num_moves; move++)
        {
            bool first_drawn = false;
            shape.setSize(sf::Vector2f(DRAW_OBJECT_SIZE, DRAW_OBJECT_SIZE));

            sf::Event event;
            window.clear(sf::Color::Black);

            // Draw our Goal
            shape.setFillColor(sf::Color::Yellow);
            shape.setPosition(sf::Vector2f(goal->x, goal->y));
            window.draw(shape);

            for (AgentDistancePair *adp : *agents_and_distances)
            {
                // Draw our Agent
                Agent *agent = adp->agent;
                shape.setFillColor(sf::Color::Red);
                shape.setPosition(sf::Vector2f(agent->positions[move]->x, agent->positions[move]->y));
                window.draw(shape);
            }

            // Re-Draw the top performer
            shape.setFillColor(sf::Color::Green);
            shape.setPosition(sf::Vector2f(agents_and_distances->at(0)->agent->positions[move]->x, agents_and_distances->at(0)->agent->positions[move]->y));
            window.draw(shape);
            

            window.display();

            while(accumulator.asSeconds() < DRAW_SECONDS_PER_FRAME){
                accumulator += clock.restart();
                while (window.pollEvent(event))
                {
                    if (event.type == sf::Event::Closed){
                        window.close();
                        return;
                    }
                }
            }
            accumulator = sf::Time::Zero;

        }
    }
}

/**
 * @brief Get the agent pointer and float distance from the goal so we can sort them.
 *
 * @param agents The agents that were a part of a generation
 * @param goal The position of the goal that was used in the generation
 * @param num_to_find The number of final agent distance pairs that the caller would like returned
 * @return std::vector<AgentDistancePair *>*
 */
std::vector<AgentDistancePair *> *get_closest_agents(std::vector<Agent *> agents, Position *goal, int generation_number, int num_to_find = 2)
{
    std::vector<AgentDistancePair *> *agent_distance_pairs = new std::vector<AgentDistancePair *>;

    for (Agent *a : agents)
        agent_distance_pairs->push_back(new AgentDistancePair(a, get_distance(*(a->positions.back()), *goal)));

    sort(agent_distance_pairs->begin(), agent_distance_pairs->end(), compare_agent_distance_pair);

    if (DRAW_GENERATION_PERFORMANCE && (generation_number % DRAW_EVERY_NTH_GENERATION) == 0 && DRAW_FULL_POPULATION)
        draw_agent_path(agent_distance_pairs, goal, generation_number);

    while (agent_distance_pairs->size() > num_to_find)
    {
        delete agent_distance_pairs->back();
        agent_distance_pairs->pop_back();
    }

    if (DRAW_GENERATION_PERFORMANCE && (generation_number % DRAW_EVERY_NTH_GENERATION) == 0 && !DRAW_FULL_POPULATION)
        draw_agent_path(agent_distance_pairs, goal, generation_number);

    return agent_distance_pairs;
}
#endif