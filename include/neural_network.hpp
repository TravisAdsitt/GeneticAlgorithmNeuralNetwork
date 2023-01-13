#include <cstdlib>
#include <bits/stdc++.h>
#include "utils.hpp"

enum MergeType{
    EveryOther,
    SingleSplit,
    RandomChoice
};

struct Layer{
    int num_inputs;
    int num_neurons;
    float* weights;

    Layer(int num_neurons, int num_inputs): num_neurons(num_neurons), num_inputs(num_inputs){
        weights = (float*) calloc(num_neurons * num_inputs, sizeof(float));
        for(int weight = 0; weight < num_neurons * num_inputs; weight++) weights[weight] = get_rand_float(0.0, 1.0);
    }
    ~Layer(){
        free(weights);
    }

    float* calculate_outputs(float* inputs){
        /*
            Calculates the outputs of all neurons with the given inputs
        */
        float* outputs = (float*) calloc(num_neurons, sizeof(float));

        for(int neuron_start_index = 0; neuron_start_index < num_inputs * num_neurons; neuron_start_index += num_inputs){
            // This helps us know which neuron we are calculating for
            int current_neuron = neuron_start_index / num_inputs;
            // Set the proper neurons output using dot product
            outputs[current_neuron] = dot_product(inputs, weights + neuron_start_index, num_inputs);
            outputs[current_neuron] = sigmoid(outputs[current_neuron]);
        }

        return outputs;
    }
};

struct NeuralNetwork{
private:
    void merge_every_other(NeuralNetwork a, NeuralNetwork b){
        // Merge Hidden Layer
        int num_weights = (hidden->num_neurons * hidden->num_inputs);
        for(int weight_index = 0; weight_index < num_weights; weight_index++){
            if(weight_index % 2 == 1){
                hidden->weights[weight_index] = b.hidden->weights[weight_index];
            }else{
                hidden->weights[weight_index] = a.hidden->weights[weight_index];
            }
        }
        // Merge Output Layer
        num_weights = (output->num_neurons * output->num_inputs);
        for(int weight_index = 0; weight_index < num_weights; weight_index++){
            if(weight_index % 2 == 1){
                output->weights[weight_index] = b.output->weights[weight_index];
            }else{
                output->weights[weight_index] = a.output->weights[weight_index];
            }
        }
    }
    void merge_single_split(NeuralNetwork a, NeuralNetwork b){
        // Merge Hidden Layer
        int num_weights = (hidden->num_neurons * hidden->num_inputs);
        int split_num = get_rand_int(0, num_weights - 1);
        for(int weight_index = 0; weight_index < num_weights; weight_index++){
            if(weight_index < split_num){
                hidden->weights[weight_index] = a.hidden->weights[weight_index];
            }else{
                hidden->weights[weight_index] = b.hidden->weights[weight_index];
            }
        }
        // Merge Output Layer
        num_weights = (output->num_neurons * output->num_inputs);
        split_num = get_rand_int(0, num_weights - 1);
        for(int weight_index = 0; weight_index < num_weights; weight_index++){
            if(weight_index < split_num){
                output->weights[weight_index] = a.output->weights[weight_index];
            }else{
                output->weights[weight_index] = b.output->weights[weight_index];
            }
        }
    }
    void merge_random_choice(NeuralNetwork a, NeuralNetwork b){
        // Merge Hidden Layer
        int num_weights = (hidden->num_neurons * hidden->num_inputs);
        for(int weight_index = 0; weight_index < num_weights; weight_index++){
            if(get_rand_bool()){
                hidden->weights[weight_index] = a.hidden->weights[weight_index];
            }else{
                hidden->weights[weight_index] = b.hidden->weights[weight_index];
            }
        }
        // Merge Output Layer
        num_weights = (output->num_neurons * output->num_inputs);
        for(int weight_index = 0; weight_index < num_weights; weight_index++){
            if(get_rand_bool()){
                output->weights[weight_index] = a.output->weights[weight_index];
            }else{
                output->weights[weight_index] = b.output->weights[weight_index];
            }
        }
    }
public:
    Layer *hidden, *output;
    int num_neurons, num_inputs, num_outputs;
    
    NeuralNetwork(int num_neurons, int num_inputs, int num_outputs): num_neurons(num_neurons), num_inputs(num_inputs), num_outputs(num_outputs){
        hidden = new Layer(num_neurons, num_inputs);
        output = new Layer(num_outputs, num_neurons);
    }
    NeuralNetwork(NeuralNetwork a, NeuralNetwork b, MergeType mt): num_neurons(a.num_neurons), num_inputs(a.num_inputs), num_outputs(a.num_outputs){
        /*
            Merging two Neural Networks assumes they are identical in their layer shapes.
        */
        hidden = new Layer(num_neurons, num_inputs);
        output = new Layer(num_outputs, num_neurons);

        switch(mt){
            case EveryOther:
                merge_every_other(a, b);
            case SingleSplit:
                merge_single_split(a, b);
            case RandomChoice:
                merge_random_choice(a, b);
        }
    }
    ~NeuralNetwork(){
        delete(hidden);
        delete(output);
    }

    float* predict(float* inputs){
        /*
            It is assumed the inputs array is the same length as 'num_inputs'
            in the constructor.
        */
        float* hidden_out = hidden->calculate_outputs(inputs);
        float* output_out = output->calculate_outputs(hidden_out);
        free(hidden_out);

        return output_out;
    }
};