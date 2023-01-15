#include <cmath>
#include <vector>
#include <memory>

#ifndef UTILS_H
#define UTILS_H

/**
 * @brief Used for randomness
 */
std::default_random_engine generator;

/**
 * @brief Used to encapsulate a position in the world
 */
struct Position
{
    float x;
    float y;
    Position(float x, float y) : x(x), y(y) {}
};

/**
 * @brief Get the distance between two positions
 *
 * @param pos1 Position 1
 * @param pos2 Position 2
 * @return float The distance between the two positions
 */
float get_distance(Position pos1, Position pos2)
{
    return sqrt(pow((pos2.x - pos1.x), 2) + pow((pos2.y - pos1.y), 2));
}

/**
 * @brief Get a random uniform float
 *
 * @param from From float
 * @param to To float
 * @return float Random float between from and to float
 */
float get_rand_uniform_float(float from, float to)
{
    std::uniform_real_distribution<float> distribution(from, to);
    return distribution(generator);
}

/**
 * @brief Get the random normal float
 *
 * @param mean Mean of the random float distribution
 * @param std_dev Standard deviation for the float distribution
 * @return float Random float given the inputs
 */
float get_rand_normal_float(float mean, float std_dev)
{
    std::normal_distribution<float> distribution(mean, std_dev);
    return distribution(generator);
}

/**
 * @brief Get a random integer
 *
 * @param from From Integer
 * @param to To Integer
 * @return int Random Integer between from and to
 */
int get_rand_int(int from, int to)
{
    std::uniform_real_distribution<float> distribution(from, to);
    return (int)distribution(generator);
}

/**
 * @brief Get a random boolean
 *
 * @param chance The float chance for true (must be between 0-1)
 * @return true
 * @return false
 */
bool get_rand_bool(float chance = 0.5)
{
    return get_rand_uniform_float(0.0, 1.0) < chance;
}

/**
 * @brief Find the dot product bewteen two float arrays
 *
 * @param a Float array 1
 * @param b Float array 2
 * @param length The length of each array (should be the same between both)
 * @return float The dot product resultant
 */
float dot_product(float *a, float *b, int length)
{
    float total = 0;

    for (int i = 0; i < length; i++)
    {
        total += a[i] * b[i];
    }

    return total;
}

/**
 * @brief Calculate the sigmoid value from a given float
 *
 * @param input
 * @return float
 */
float sigmoid(float input)
{
    return 1 / (1 + exp(-input));
}

/**
 * @brief Helper function to get a random position
 *
 * @param max_x Maximum value allowed in the x direction
 * @param max_y Maximum value allowed in the y direction
 * @return Position* The object to be returned
 */
Position *get_random_position(float max_x, float max_y)
{
    return new Position(get_rand_uniform_float(0, max_x), get_rand_uniform_float(0, max_y));
}
#endif