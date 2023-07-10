#include<iostream>
#include<vector>
#include<memory>
#include<cmath>
#include<random>

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

	Matrix(int rows, int colms, int random_state=clock()){
		this->rows = rows;
		this->colms = colms;
		this->data = new vector<float>(rows*colms);
				
		std::default_random_engine generator;
		std::uniform_real_distribution<float> dist(0.0, 1.0);

		generator.seed(random_state);

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
		
		if(m->rows != this->colms){
			cout << "INVALID OPERATION ! WRONG MATRIX DIMENSIONS\n";
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

	vector<float>* get_data(){
		return this->data;
	}


	Matrix& operator+(Matrix& m){
		
		if(this->colms != m.colms || this->rows != m.rows){
			cout << "WRONG DIMENSIONS FOR ADITTION!";
			exit(1);
		}

		Matrix *res = new Matrix(this->rows, this->colms);
		
		for(int i = 0; i < this->data->size(); i++){
			res->data->at(i) = this->data->at(i) + m.data->at(i);
		}

	
		return *res;

	}

};


class Layer: public Matrix{


	private:

	Matrix* (*activation)(Matrix*);

	Matrix* bias;

	public:
	
	Layer(int n_rows, int n_colms) : Matrix(n_rows, n_colms){

	}

	Layer(int n_neurons, int previous_layer, Matrix* (*activation)(Matrix*)) : Matrix(n_neurons, previous_layer){
		this->activation = activation;
		
		bias = new Matrix(n_neurons, 1);	
	}

	
	Layer(int rows, int colms, float values[], int n) : Matrix(rows, colms, values, n){
	
	}

	Matrix* (*get_activation())(Matrix*){
		return this->activation;
	}

	Matrix* get_bias(){
		return this->bias;
	}

};

float l2_loss(Matrix& pred, Matrix& target){
	
	if(pred.get_rows() != target.get_rows()){
		cout << "WRONG MATRIX DIMENSIONS!" << endl;
		exit(1);
	}

	float loss = 0;

	for(int i = 0; i < pred.get_data()->size(); i++){
		loss += pow(pred.get_data()->at(i) - target.get_data()->at(i), 2);
	}

	return (float) loss/(pred.get_data()->size());

}

Matrix* sigmoid(Matrix * m){
	
	Matrix *res = new Matrix(m->get_rows(), m->get_colms());

	for(int i = 0; i < res->get_rows(); i++){
		float n_value = 1/(1 + exp(m->at(0, i)));
		res->set(0, i, n_value);
	}

	return res;

}

Matrix * linear(Matrix *m){
	
	return m;

}

class NN{
	
	vector<Layer*> *layers;
	
	float (*loss)(Matrix&, Matrix&);
	
	Matrix* targets;

	public:

	NN(int size_input, float inputs[], int size_target, float targets[], float (*loss)(Matrix&, Matrix&)=l2_loss){
		this->layers = new vector<Layer*>();

		//Create initial layer - inputs
		
		Layer *input = new Layer(size_input, 1, inputs, size_input);

		this->add_layer(input);

		this->targets = new Matrix(size_target, 1, targets, size_target);
		this->loss = loss;
		
	}

	void add_layer(Layer *layer){
		layers->push_back(layer);
	}

	void add_layer(int n_neurons, Matrix* (*activation)(Matrix*)){
		
		int n_colms = this->layers->back()->get_rows();
			
		Layer *new_layer = new Layer(n_neurons, n_colms, activation);

		this->layers->push_back(new_layer);
		
	}


	void forward_prop(){

		Matrix *result;
		
		Matrix *ant;

		ant = this->layers->at(0);
		

		for(int i = 1; i < this->layers->size(); i++){
			
			//*result = (Layer) *(*(this->layers->at(i)->get_activation()))(this->layers->at(i)->mul(result));
			
			ant->print();
			this->layers->at(i)->get_bias()->print();
			this->layers->at(i)->print();

			cout<<"\n=============\n";
			
			Matrix* (*activation)(Matrix*) = this->layers->at(i)->get_activation();
			Matrix m = *(this->layers->at(i)->mul(ant)) + *(this->layers->at(i)->get_bias());
			
			cout << "\n----------res:\n";
			result =  (*activation)(&m);

			ant = result;

		}

		result->print();
		
		cout << "------ Current loss : " << this->loss(*result, *targets) << "\n";
	}

	void back_prop(){
	
	}



	void print(){
		
		cout << "\n================= LAYERS ===================\n";
		
		int i = 1;
		

		for(vector<Layer*>::iterator itr = this->layers->begin(); itr != this->layers->end(); itr++){
			
			cout << "-----------layer : " << std::to_string(i) << " -----------\n";
			(*itr)->print();
		}
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
	
	/*
	Matrix *a = new Matrix(2, 2);
	a->print();
	
	cout << "--------------" << endl;
	
	Layer *l = new Layer(10, 10, &sigmoid);

	l->print();
	*/

	float inputs[] = {1.0, 2.0};
	
	float targets[] = {2.0};

	NN *neural_net = new NN(2, inputs, 1, targets); 
	
	neural_net->add_layer(2, &sigmoid);
	
	neural_net->add_layer(2, &sigmoid);
	
	neural_net->add_layer(1, &linear);
	
	neural_net->forward_prop();
	
	return 0;
}
	
