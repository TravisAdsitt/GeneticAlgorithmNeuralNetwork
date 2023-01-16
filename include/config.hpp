#include "utils.hpp"

/**
 * @brief World controls
 */
#define BOUNDARY_EDGE_LENGTH 800

/**
 * @brief Mutation Controls
 */
#define MAX_MUTATION_CHANCE 0.99
#define MUTATION_CHANCE_C_VALUE 0.01
#define MUTATION_CHANCE_LIMIT 6
#define AGENT_MERGE_STRATEGY SingleSplit

/**
 * @brief Generation Controls
 */
#define NUM_GEN 10000
#define NUM_TICKS_PER_GEN 1000
#define NUM_AGENTS_PER_GEN 100
#define NUM_AGENTS_SELECTED_EACH_GENERATION 10

/**
 * @brief Display Options
 */
#define PRINT_GENERATION_PERFORMANCE false
#define DRAW_GENERATION_PERFORMANCE true
#define DRAW_SECONDS_PER_FRAME 0.01
#define DRAW_OBJECT_SIZE 5
#define DRAW_EVERY_NTH_GENERATION 5 
#define DRAW_FULL_POPULATION true