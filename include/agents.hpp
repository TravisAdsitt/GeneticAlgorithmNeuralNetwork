#include "neural_network.hpp"
#include "utils.hpp"
#include "config.hpp"

struct Agent
{
    std::vector<Position *> positions;
    NeuralNetwork *nn;
    int num_sensors, num_controls;

    Agent(Position *pos, int num_sensors) : num_sensors(num_sensors)
    {
        num_controls = 2;
        nn = new NeuralNetwork(num_sensors, num_controls);
        positions.push_back(pos);
    }
    ~Agent()
    {
        delete nn;
    }

    void move(float *sensor_input)
    {
        // Get how much this agent wants to move
        float *move_deltas = nn->predict(sensor_input);

        // Get and store its next position
        Position *curr_pos = positions.back();
        float next_pos_x = (curr_pos->x + move_deltas[0]);
        float next_pos_y = (curr_pos->y + move_deltas[1]);

        // Check for boundaries
        if (next_pos_x < 0)
            next_pos_x = 0;
        if (next_pos_x > BOUNDARY_EDGE_LENGTH)
            next_pos_x = BOUNDARY_EDGE_LENGTH;
        if (next_pos_y < 0)
            next_pos_y = 0;
        if (next_pos_y > BOUNDARY_EDGE_LENGTH)
            next_pos_y = BOUNDARY_EDGE_LENGTH;

        // Store location
        positions.push_back(new Position(next_pos_x, next_pos_y));

        // Free our float array
        free(move_deltas);
    }
};