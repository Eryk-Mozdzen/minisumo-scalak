// NeuralNetwork_lib.h
// Eryk Mozdzen 2019

#ifndef NeuralNetwork_lib_h
#define NeuralNetwork_lib_h

#include "Arduino.h"

class Row {
	public:
		float *elements;
		
		Row();
		Row(int);
};

class Matrix {
	public:
		Row *elements;
		byte rows;
		byte columns;

		Matrix();
		Matrix(byte,byte);
		Matrix(byte,byte,float*);
		void print();
		void fill(float);
		void randomize(float,float);
		void funcAll(float(*)(float));
		void add(Matrix);
		void multiply(float);
		static void multiply(Matrix,Matrix,Matrix*);
		static Matrix multiply(Matrix,Matrix);
};

class NeuralNetwork {
	public:
		Matrix *weights;
		Matrix *values;
		Matrix *bias;
		byte layers;
		
		NeuralNetwork();
		NeuralNetwork(int,...);
		Matrix calculate(Matrix);
		void randomize(int);
};

float tanh(float);
float relu(float);
float sigmoid(float);

#endif
