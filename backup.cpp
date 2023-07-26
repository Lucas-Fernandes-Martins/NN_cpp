	#include<iostream>
	#include<cstdlib>
	#include<ctime>
	#include<vector>
	#include<cmath>

	using namespace std;



	typedef pair<float, float> duo; 

	class model{

		private:
		int niter;
		float eps;
		float lrate;
		vector<duo> *train_set;
		
		float w;
		
		public:
		
		model(vector<duo> *train_set){
			this->train_set = train_set;
			this->niter = 500;
			this->eps = 0.01;
			this->lrate = 0.01;
			this->w = this->randf();
		}

		float randf(){
			srand(time(0));
		
			float random = ((float) rand()/RAND_MAX)*10;

			return random;
		}

		float qloss(float x, float y){
			return pow((x-y), 2);
		}

		float l1loss(float x, float y){
			return abs(x-y);
		}


		float loss(float p){

			float loss = 0;

			for(int i = 0; i < train_set->size(); i++){
				cout << p*(*train_set).at(i).first << " - " << (*train_set).at(i).second << endl;

				loss += pow((p*(*train_set).at(i).first - (*train_set).at(i).second), 2);
			}

			return loss;

		}

		float loss(float p, string type){

			float loss = 0;

			float (model::*floss)(float, float);

				if (type == "l1loss"){
					floss = &model::l1loss;
					
				}else if (type == "qloss"){
					floss = &model::qloss;
					
				}else{
					floss = &model::qloss;
			}

		for(int i = 0; i < train_set->size(); i++){
			cout << p*(*train_set).at(i).first << " - " << (*train_set).at(i).second << endl;

			loss += (this->*floss)(p*(*train_set).at(i).first, (*train_set).at(i).second);
		}

		return loss;

	}

	void train(){
		
		//loss
		cout << "----------- TRAINING --------------" << endl;
		for(int i = 0; i < this->niter; i++){
			float current_loss = loss(w);

			cout << "-----------------"<<"\n";

			cout << "Iteration: " << i << " w :" << this->w << "\n";

			cout << "current loss: " << current_loss << "\n";
			
			float del = (loss(this->w + this->eps) - loss(this->w));
			
			cout << "del :" << del << "\n";

			this->w -= this->lrate*del;

			cout << "New w: " << w << "\n"; 

		}


		

	}

};


int main(){

	int a[][2] = {{1,3}, {2,6}, {3,9}, {4,12}, {5,15}};

	vector<duo> *t = new vector<duo>(); 

	for(int i = 0; i < 10; i++)
		t->push_back(pair(i, 3*i));
	
	/*
	for(int i = 0; i < 10; i++)
		cout << t->at(i).first << " - " << t->at(i).second  << endl;

	*/

	cout << "Hello World!" << endl;
	
	model model1(t);

	cout << model1.randf() << endl;

	model1.train();

	return 0;

}
