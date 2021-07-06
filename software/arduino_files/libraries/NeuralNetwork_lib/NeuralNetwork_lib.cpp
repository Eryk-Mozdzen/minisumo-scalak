// NeuralNetwork_lib.cpp
// Eryk Mozdzen 2019

#include "NeuralNetwork_lib.h"

#define E 2.718281828459045235360287471352

Row::Row() {}

Row::Row(int columns) {
	elements = new float[columns];
	
	for(int i=0; i<columns; i++)
		elements[i] = 0;
}

float relu(float input) {
	if(input>0) return input;
	else return 0;
}

float sigmoid(float input) {
	input = 1/(1+pow(E,-input));
	return input;
}

float tanh(float input) {
	input = (2*sigmoid(2*input))-1;
	return input;
}

Matrix::Matrix() {}

Matrix::Matrix(byte rows_a,byte columns_a) {
	rows = rows_a;
	columns = columns_a;
	elements = new Row[rows];
	
	for(int i=0; i<rows; i++)
		elements[i] = Row(columns);
}

Matrix::Matrix(byte rows_a,byte columns_a,float* input) {
	rows = rows_a;
	columns = columns_a;
	elements = new Row[rows];

	for(int i=0; i<rows; i++)
		elements[i] = Row(columns);

	for(int i=0; i<rows; i++)
		for(int j=0; j<columns; j++)
			elements[i].elements[j] = input[i*columns+j];
}

void Matrix::print() {
	Serial.print("---------------\n");
	for(int i=0; i<rows; i++) {
		for(int j=0; j<columns; j++) {
			Serial.print(elements[i].elements[j]);
			Serial.print("\t");
		}
		Serial.print("\n");
	}
	Serial.print("---------------\n");
}

void Matrix::fill(float number) {
	for(int i=0; i<rows; i++)
		for(int j=0; j<columns; j++)
			elements[i].elements[j] = number;
}

void Matrix::randomize(float a,float b) {
	a *=100;
	b *=100;
	b++;
	for(int i=0; i<rows; i++)
		for(int j=0; j<columns; j++)
			elements[i].elements[j] = (float)(random(a,b)/100.0);
}

void Matrix::funcAll(float(*func)(float)) {
	for(int i=0; i<rows; i++)
		for(int j=0; j<columns; j++)
			elements[i].elements[j] = func(elements[i].elements[j]);
}

void Matrix::multiply(float number) {
	for(int i=0; i<rows; i++)
		for(int j=0; j<columns; j++)
			elements[i].elements[j] *=number;
}

void Matrix::add(Matrix second) {
	if(rows==second.rows && columns==second.columns) {
		for(int i=0; i<rows; i++)
			for(int j=0; j<columns; j++)
				elements[i].elements[j] +=second.elements[i].elements[j];
	} else Serial.println("Wrong number of rows or columns in adding one matrix to another...");
}

static void Matrix::multiply(Matrix first,Matrix second,Matrix *result) {
  if(first.columns==second.rows && first.rows==result->rows && second.columns==result->columns) {

		result->fill(0);

    for(int i=0; i<first.rows; i++)
      for(int j=0; j<second.columns; j++)
        for(int k=0; k<first.columns; k++)
          result->elements[i].elements[j] +=first.elements[i].elements[k]*second.elements[k].elements[j];

  } else Serial.println("Wrong number of rows or columns in multiplication two matrices...");
}

NeuralNetwork::NeuralNetwork() {}

NeuralNetwork::NeuralNetwork(int layers_a, ...) {
	layers = layers_a;
	weights = new Matrix[layers];
	values = new Matrix[layers];
	bias = new Matrix[layers];

	int pattern[layers];
	va_list arguments;
  va_start(arguments, layers);
	for(int i=0; i<layers; i++)
		pattern[i] = va_arg(arguments, int);
	
	for(int i=0; i<layers; i++) {
		values[i] = Matrix(pattern[i],1);
		bias[i] = Matrix(pattern[i],1);
		if(i!=0) weights[i] = Matrix(pattern[i],pattern[i-1]);
		else weights[i] = Matrix(1,1);
	}
	/*
	float weights0[1][1] = {{0}};
	float weights1[5][7] = {{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0},{0,0,0,0,0,0,0}};
	float weights2[2][5] = {{0,0,0,0,0},{0,0,0,0,0}};
	float bias0[7][1] = {{0},{0},{0},{0},{0},{0},{0}};
	float bias1[5][1] = {{0},{0},{0},{0},{0}};
	float bias2[2][1] = {{0},{0}};
	
	weights[0] = Matrix(1,1,*weights0);
	weights[1] = Matrix(5,7,*weights1);
	weights[2] = Matrix(2,5,*weights2);
	bias[0] = Matrix(7,1,*bias0);
	bias[1] = Matrix(5,1,*bias1);
	bias[2] = Matrix(2,1,*bias2);
	*/
}

Matrix NeuralNetwork::calculate(Matrix input) {

	if(values[0].rows!=input.rows) {
		Serial.println("Wrong input...");
		return Matrix(0,0);
	}

	for(int i=1; i<layers; i++) values[i].fill(0);
	
	for(int i=0; i<layers; i++) {
		if(i==0) {
			for(int m=0; m<input.rows; m++)
				for(int n=0; n<input.columns; n++)
					values[i].elements[m].elements[n] = input.elements[m].elements[n];
		} else Matrix::multiply(weights[i],values[i-1],&values[i]);
		
		values[i].add(bias[i]);

		values[i].funcAll(relu);
		//values[i].funcAll(sigmoid);
		//values[i].funcAll(tanh);
	}

	return values[layers-1];
}

void NeuralNetwork::randomize(int seed) {
	randomSeed(seed);
	for(int i=0; i<layers; i++) weights[i].randomize(-1,1);
  for(int i=0; i<layers; i++) bias[i].randomize(0,2);
}