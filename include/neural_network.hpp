#include <cstdlib>
#include <bits/stdc++.h>
#include "utils.hpp"

#ifndef NEURALNET_H
#define NEURALNET_H

/**
 * @brief These are the different kinds of merge strategies when combining two Neural Networks
 *
 * EveryOther -> Every other weight will be picked from each Layer
 * SingleSplit -> A single index will be selected as the point to choose from Neural Network B
 * RandomChoice -> Each weight will be randomly chosen between the two
 */
enum MergeType
{
    EveryOther,
    SingleSplit,
    RandomChoice
};

/**
 * @brief Used to construct a layer for a Neural Network
 *
 */
struct Layer
{
    int num_inputs;
    int num_neurons;
    float *weights;

    /**
     * @brief Construct a new Layer object
     *
     * @param num_neurons The number of neurons to put in the layer
     * @param num_inputs The number of inputs each neuron will have, basically the number of weights
     */
    Layer(int num_neurons, int num_inputs) : num_neurons(num_neurons), num_inputs(num_inputs)
    {
        // The total number of weights we will need is equal to the number of neurons * the number of weights.
        weights = (float *)calloc(num_neurons * num_inputs, sizeof(float));
        for (int weight = 0; weight < num_neurons * num_inputs; weight++)
            weights[weight] = get_rand_uniform_float(-1.0, 1.0);
    }
    ~Layer()
    {
        free(weights);
    }

    /**
     * @brief Used to mutate the layers weights.
     *
     * @param mutation_chance A float to set the chance that determines the chance any weight might be mutated.
     */
    void mutate(float mutation_chance)
    {
        if (mutation_chance == 0)
            return;

        for (int weight = 0; weight < num_inputs * num_neurons; weight++)
        {
            if (get_rand_bool(mutation_chance))
            {
                weights[weight] = get_rand_uniform_float(-1.0, 1.0);
            }
        }
    }

    /**
     * @brief Calculate what the layer would output with the given inputs.
     *
     * @param inputs This is assumed to be the same size as our initialized num_inputs.
     * @return float* This is what the layer has output, it is expected to be freed by the caller.
     */
    float *calculate_outputs(float *inputs)
    {
        /*
            Calculates the outputs of all neurons with the given inputs
        */
        float *outputs = (float *)calloc(num_neurons, sizeof(float));

        for (int neuron_start_index = 0; neuron_start_index < num_inputs * num_neurons; neuron_start_index += num_inputs)
        {
            // This helps us know which neuron we are calculating for
            int current_neuron = neuron_start_index / num_inputs;
            // Set the proper neurons output using dot product
            outputs[current_neuron] = dot_product(inputs, weights + neuron_start_index, num_inputs);
            outputs[current_neuron] = sigmoid(outputs[current_neuron]);
        }

        return outputs;
    }
};

struct NeuralNetwork
{
private:
    /**
     * @brief See 'MergeType' enum documentation for more information
     *
     * @param a Neural Network A
     * @param b Neural Network B
     */
    void merge_every_other(NeuralNetwork *a, NeuralNetwork *b)
    {
        // Merge Hidden Layer
        int num_weights = (hidden->num_neurons * hidden->num_inputs);
        for (int weight_index = 0; weight_index < num_weights; weight_index++)
        {
            if (weight_index % 2 == 1)
            {
                hidden->weights[weight_index] = b->hidden->weights[weight_index];
            }
            else
            {
                hidden->weights[weight_index] = a->hidden->weights[weight_index];
            }
        }
        // Merge Output Layer
        num_weights = (output->num_neurons * output->num_inputs);
        for (int weight_index = 0; weight_index < num_weights; weight_index++)
        {
            if (weight_index % 2 == 1)
            {
                output->weights[weight_index] = b->output->weights[weight_index];
            }
            else
            {
                output->weights[weight_index] = a->output->weights[weight_index];
            }
        }
    }
    /**
     * @brief See 'MergeType' enum documentation for more information
     *
     * @param a Neural Network A
     * @param b Neural Network B
     */
    void merge_single_split(NeuralNetwork *a, NeuralNetwork *b)
    {
        // Merge Hidden Layer
        int num_weights = (hidden->num_neurons * hidden->num_inputs);
        int split_num = (int)get_rand_normal_float(num_weights / 2, 1);
        for (int weight_index = 0; weight_index < num_weights; weight_index++)
        {
            if (weight_index < split_num)
            {
                hidden->weights[weight_index] = a->hidden->weights[weight_index];
            }
            else
            {
                hidden->weights[weight_index] = b->hidden->weights[weight_index];
            }
        }
        // Merge Output Layer
        num_weights = (output->num_neurons * output->num_inputs);
        split_num = get_rand_int(0, num_weights - 1);
        for (int weight_index = 0; weight_index < num_weights; weight_index++)
        {
            if (weight_index < split_num)
            {
                output->weights[weight_index] = a->output->weights[weight_index];
            }
            else
            {
                output->weights[weight_index] = b->output->weights[weight_index];
            }
        }
    }
    /**
     * @brief See 'MergeType' enum documentation for more information
     *
     * @param a Neural Network A
     * @param b Neural Network B
     */
    void merge_random_choice(NeuralNetwork *a, NeuralNetwork *b)
    {
        // Merge Hidden Layer
        int num_weights = (hidden->num_neurons * hidden->num_inputs);
        for (int weight_index = 0; weight_index < num_weights; weight_index++)
        {
            if (get_rand_bool())
            {
                hidden->weights[weight_index] = a->hidden->weights[weight_index];
            }
            else
            {
                hidden->weights[weight_index] = b->hidden->weights[weight_index];
            }
        }
        // Merge Output Layer
        num_weights = (output->num_neurons * output->num_inputs);
        for (int weight_index = 0; weight_index < num_weights; weight_index++)
        {
            if (get_rand_bool())
            {
                output->weights[weight_index] = a->output->weights[weight_index];
            }
            else
            {
                output->weights[weight_index] = b->output->weights[weight_index];
            }
        }
    }

public:
    Layer *hidden, *output;
    int num_inputs, num_outputs, num_neurons;

    /**
     * @brief Construct a new Neural Network object
     *
     * @param num_inputs Number of inputs to the Neural Network
     * @param num_outputs Number of outputs from the Neural Network
     * @param num_neurons Number of neurons in the hidden layer
     */
    NeuralNetwork(int num_inputs, int num_outputs, int num_neurons = 10) : num_neurons(num_neurons), num_inputs(num_inputs), num_outputs(num_outputs)
    {
        hidden = new Layer(num_neurons, num_inputs);
        output = new Layer(num_outputs, num_neurons);
    }
    /**
     * @brief Construct a new Neural Network object
     *
     * This function will not free the two Neural Networks that are used as its basis. There is an inherit assumption that
     * both neural networks are identical, if they are not there is a high likely hood you will get a Seg Fault.
     *
     * @param a Neural Network A
     * @param b Neural Network B
     * @param mt The Merge Strategy to use when merging based on the two input Neural Networks
     */
    NeuralNetwork(NeuralNetwork *a, NeuralNetwork *b, MergeType mt) : num_neurons(a->num_neurons), num_inputs(a->num_inputs), num_outputs(a->num_outputs)
    {
        /*
            Merging two Neural Networks assumes they are identical in their layer shapes.
        */
        hidden = new Layer(num_neurons, num_inputs);
        output = new Layer(num_outputs, num_neurons);

        switch (mt)
        {
        case EveryOther:
            merge_every_other(a, b);
        case SingleSplit:
            merge_single_split(a, b);
        case RandomChoice:
            merge_random_choice(a, b);
        }
    }
    ~NeuralNetwork()
    {
        delete (hidden);
        delete (output);
    }

    /**
     * @brief Mutate this Neural Network
     *
     * @param mutation_chance The chance any given weight will be mutated.
     */
    void mutate(float mutation_chance)
    {
        hidden->mutate(mutation_chance);
        output->mutate(mutation_chance);
    }

    /**
     * @brief Given the inputs what would the neural network output.
     *
     * @param inputs The input to the Neural Network, expected to be the same length as num_inputs
     * @return float* The predictions from the Neural Network, expected to be freed by the caller
     */
    float *predict(float *inputs)
    {
        /*
            It is assumed the inputs array is the same length as 'num_inputs'
            in the constructor.
        */
        float *hidden_out = hidden->calculate_outputs(inputs);
        float *output_out = output->calculate_outputs(hidden_out);
        free(hidden_out);

        return output_out;
    }
};
#endif