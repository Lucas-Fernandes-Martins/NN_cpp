#include<iostream>
#include<vector>
#include<memory>

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
	
	}

	Matrix(int rows, int colms, float values[], int n){
		this->rows = rows;
		this->colms = colms;
		this->data = new vector<float>(values, values + n);
	
	}

	public:

	float at(int row, int colm){
		return data->at(row*(this->colms) + colm);
	}

	void set(int row, int colm, float v){
		data->at(row*(this->rows) + colm) = v;
	}

	void add(int row, int colm, float v){
		data->at(row*(this->rows) + colm) += v;
	}

	void sum(Matrix *m){
		
		for(int i = 0; i < rows; i++){
			
			for(int j = 0; j < colms; j++){
				this->add(i, j, m->at(i,j));
			}
		}

	}


	void print(){
		cout << "M = " << endl;	
		for(int i = 0; i < this->rows; i++){
			
			for(int j = 0; j < this->colms; j++){
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

int main(){
	

	float v[] = {1.0, 2.0, 3.0, 4.0};
    	int n = sizeof(v) / sizeof(v[0]);	

	float *p = &(v[0]);
	
	Matrix *m = new Matrix(2, 2, v, n);

	m->print();
	
	//m->print_data();

	
	return 0;
}

