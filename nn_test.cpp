#include<iostream>
#include<vector>
#include<memory>
#include<cmath>
#include <random>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::unique_ptr;

class Matrix{
	
	int rows;
	int colms;
	
	vector<float> *data;
	
	public:

	Matrix(int rows, int colms){
		this->rows = rows;
		this->colms = colms;
		this->data = new vector<float>(rows*colms);
		
		std::default_random_engine generator;
		std::uniform_real_distribution<float> dist(0.0, 1.0);

		for(vector<float>::iterator it = this->data->begin(); it != this->data->end(); it++){
			*it = dist(generator);
		}
	}

	Matrix(int rows, int colms, float values[], int n){
		this->rows = rows;
		this->colms = colms;
		this->data = new vector<float>(values, values + n);
	
	}

	public:

	int get_rows(){ return this->rows;}
	int get_colms(){ return this->colms;}

	float at(int row, int colm){
		return data->at(row*(this->colms) + colm);
	}

	void set(int row, int colm, float v){
		data->at(row*(this->colms) + colm) = v;
	}

	void add(int row, int colm, float v){
		data->at(row*(this->colms) + colm) += v;
	}

	void sum(Matrix *m){
		
		for(int i = 0; i < rows; i++){
			
			for(int j = 0; j < colms; j++){
				this->add(i, j, m->at(i,j));
			}
		}

	}

	Matrix* append(Matrix *m){
		
		if(this->rows != m->get_rows()){
			cout << "ERROR - Cant append matrices with a different number of rows";
			return NULL;
		}

		int new_rows = this->rows + m->get_rows();
		int new_colms = this->colms + m->get_colms();

		Matrix *res = new Matrix(this->rows, new_colms);
		
		res->print();
		
		for(int i = 0; i < this->rows; i++){
			
			for(int j = 0; j < this->colms; j++){
				res->set(i, j, this->at(i,j));
			}
		}
		
		cout << "===============" << "\n";
		res->print();
		cout << "===============" << "\n";
		

		for(int i = 0; i < m->get_rows(); i++){
			
			for(int j = 0; j < m->get_colms(); j++){
				res->set(i, this->colms + j, m->at(i,j));
			}
		}

		res->print();

		return res;

}

	


	Matrix* mul(Matrix *m){
		Matrix *res = new Matrix(this->rows, m->colms); 
		
		if(m->colms != this->rows || m->rows != this->colms){
			cout << "INVALID OPERATION ! WRONG MATRIX DIMENSIONS";
			return NULL;
		}

		for(int i = 0; i < this->rows; i++){
			for(int j = 0; j < m->colms; j++){
				
				float line_sum = 0;

				for(int k = 0; k < this->colms; k++){

					line_sum += (this->at(i,k))*(m->at(k, j));
					
				}

				res->set(i,j, line_sum);
			}	


		}

		return res;

	}


	void print(){
		cout << "M = " << endl;	
		for(int i = 0; i < this->rows; i++){
			

			for(int j = 0; j < this->colms; j++){
				
				if(j == 0) cout << " ";

				cout << this->at(i, j) << " ";
			}
			cout << "\n";
		}
	}

	void print_data(){
	
		for(int i = 0; i < this->data->size(); i++){
			cout << this->data->at(i) << " ";
		}
		cout << endl;
	}

};


class Layer: public Matrix{
	
	public:

	Layer(int n_neurons) : Matrix(n_neurons, 1){

	}

};

Matrix* sigmoid(Matrix * m){
	
	Matrix *res = new Matrix(m->get_rows(), m->get_colms());

	for(int i = 0; i < res->get_rows(); i++){
		float n_value = 1/(1 + exp(m->at(0, i)));
		res->set(0, i, n_value);
	}

	return res;

}

class NN{

	int n_layers;
	
	Matrix* (*act_function)(Matrix*);


	public:

	NN(int n_layers, Matrix* (*act_function)(Matrix*)){
		this->n_layers = n_layers;
		this->act_function = act_function;
	}

};

int main(){
	
	/* Testing framework	
	float v[] = {1.0, 2.0, 3.0, 4.0};
    	int n = sizeof(v) / sizeof(v[0]);	

	float *p = &(v[0]);
	
	Matrix *m = new Matrix(2, 2, v, n);

	m->print();
	
	//m->print_data();

	float z[] = {1.0, 1.0, 1.0, 1.0};

	Matrix *b = new Matrix(2, 2, z, n);
	
	m = m->mul(b);

	cout << "\n";

	m->print();
	
	float a1_array[] = {1.0,2.0};
	float a2_array[] = {3.0,4.0};

	Matrix *a1 = new Matrix(2, 1, a1_array, n);
	Matrix *a2 = new Matrix(1, 2, a2_array, n);

	cout << "\n";

	a1 = a1->mul(a2);

	a1->print();
	
	*/
	
	/*
	float a1_array[] = {1.0,2.0, 3.0, 4.0};
	float a2_array[] = {5.0,6.0};
	
    	int n = sizeof(a1_array) / sizeof(a1_array[0]);	

	Matrix *a1 = new Matrix(2, 2, a1_array, n);
	Matrix *a2 = new Matrix(2, 1, a2_array, 2);
	
	//a1->print();
	//a2->print();

	//a1 = a1->append(a2);
	
	Matrix *a3 = new Matrix(2,2, a1_array, 4);
	
	Matrix *a4 = new Matrix(2,1, a2_array, 2);
	
	a3->set(1,1, 5);
	
	a3->print();
	a4->print();
	a3->append(a4);

	*/
	
	Matrix *a = new Matrix(2, 2);
	a->print();
	
	cout << "--------------" << endl;
	
	Layer *l = new Layer(10);

	l->print();

	return 0;
}

