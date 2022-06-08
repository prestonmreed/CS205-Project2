#include <set>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;
void search(vector<vector<double> >& data);
double leave_one_out_cross_validation(vector<vector<double> >& data, set<int>& features, int feature);
void collectData(string& filename, vector<vector<double> >& data);

int main() {
	/*
	-----------------------------USER INTERFACE------------------------
	*/
	string filename;
	int algorithmNum = 0;
	cout << "Welcome to Preston Reed's Feature Selection Algorithm" << endl;
	cout << "Type in the name of the file to test: ";
	getline(cin,filename);
	cout << filename << endl;
	vector<vector<double> > data;
	collectData(filename,data);
	cout << endl;
	cout << "Type the name of the Algorithm you want to run." << endl;
	cout << "1) Forward Selection" << endl;
	cout << "2) Backward ELimination" << endl;
	cin >> algorithmNum;
	cout << "This dataset has " << data[0].size()-1 << " features (not including the class attribute), with " << data.size() << " instances." << endl;
	/*
	-------------------------------------------------------------------
	*/

	//Forward Selection
	if (algorithmNum == 1) {
		search(data);
	}

	//Backward Elmination
	else if (algorithmNum == 2) {
		search(data);
	}

	//Incorrect User Input
	else {
		cout << "please enter valid number" << endl;
		return 0;
	}
}

void search(vector<vector<double> >& data) {
	set<int> current_set_of_features; 
	for (int i = 1; i < data[0].size(); i++) {
		cout << "On the " << i << "th level of the search tree" << endl;
		int feature_to_add_at_this_level;
		double best_so_far_accuracy = 0;
		for (int j = 1; j < data[0].size(); j++) {
			if (current_set_of_features.find(j) == current_set_of_features.end()) {
				cout << "--Considering adding the " << j <<  " feature" << endl;
				double accuracy = leave_one_out_cross_validation(data,current_set_of_features,j+1);
				if (accuracy > best_so_far_accuracy) {
					best_so_far_accuracy = accuracy;
					feature_to_add_at_this_level = j;
				}
			}
		}
		current_set_of_features.insert(feature_to_add_at_this_level);
		cout << "On level " << i << " I added feature " << feature_to_add_at_this_level << " to the current set" << endl;
	}
}
double leave_one_out_cross_validation(vector<vector<double> >& data, set<int>& features, int feature) {
	//https://stackoverflow.com/questions/2704521/generate-random-double-numbers-in-c
	double f = (double)rand() / RAND_MAX;
    	return 0 + f * (100 - 0);
}

void collectData(string& filename, vector<vector<double> >& data) {
	string row;
	ifstream f(filename);
	while (getline(f,row)) {
		vector<double> curr_row;
		double val;
		stringstream ss(row);
		while (ss >> val) {
			curr_row.push_back(val);
		} 
		data.push_back(curr_row);	
	}
	f.close();
}





