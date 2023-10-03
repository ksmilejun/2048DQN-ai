#include <iostream>
#include <cmath>
#define sigmoid(x) ( 1.0/(1.0+exp(-(x))) )
#pragma once

#ifndef _NEURON_H_
#define _NEURON_H_

using namespace std;
class Neuron
{
private:
    int num_of_input;		// Number of input synapse
    double alpha;			// Sensitivity

public:
    double vdelta;
    double* input_weight; // Chemical signal weight
    double* weight_error; // Cumulative weight error

    Neuron()
    {

    }
    void Setdata(int num_of_input, double alpha) {
        this->num_of_input = num_of_input;
        this->alpha = alpha;
        input_weight = new double[num_of_input + 1];	// The last one is for constant input!
        weight_error = new double[num_of_input + 1];

        for (int i = 0; i < num_of_input + 1; i++)
        {
            input_weight[i] = ((double)rand() / RAND_MAX) * 2 - 1; // -1 ~ 1 Random
            weight_error[i] = 0.0;
        }
    }
    ~Neuron()
    {
        delete[] input_weight;
        delete[] weight_error;
    }

    double work(double input[])
    {
        double sum = 0;
        for (int i = 0; i < num_of_input; i++)
        {
            sum += input_weight[i] * input[i];
        }
        sum += input_weight[num_of_input] * 1.0;	// Constant input

        return sigmoid(sum);
    }

    void learn(double input[], double target)
    {
        double output = work(input);
        double output_error = output - target;

        for (int i = 0; i < num_of_input; i++)
        {
            weight_error[i] += output_error * input[i] * output * (1 - output);
        }
        weight_error[num_of_input] += output_error * 1.0 * output * (1 - output);
    }

    void fix()
    {
        for (int i = 0; i < num_of_input + 1; i++)
        {
            input_weight[i] -= alpha * weight_error[i];
            weight_error[i] = 0.0;
        }

    }

};
class layer {
private:
    int neuronnum;
public:
    Neuron* neuron;
    double* outputdata;
    layer(int neuronnum) {
        this->neuronnum = neuronnum;
        neuron = new Neuron[neuronnum];
        outputdata = new double[neuronnum];
    }
    ~layer() {
        delete[] neuron;
        delete[] outputdata;
    }
    void MakeLayer(int inputnum) {
        for (int i = 0; i < neuronnum; i++) {
            neuron[neuronnum].Setdata(inputnum, 0.1);
        }
    }
    void processlayer(double inputdata[]) {
        int sum = 0;
        for (int i = 0; i < neuronnum; i++) {
            outputdata[i] = sum += neuron[i].work(inputdata);
        }
    }
};

#endif