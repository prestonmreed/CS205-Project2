#include <set>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <limits>
#include <math.h>
#include <stdio.h>
#include <float.h>

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
		//		cout << "--Considering adding the " << j <<  " feature" << endl;
				double accuracy = leave_one_out_cross_validation(data,current_set_of_features,j);
				cout << "Using feature(s) {";
				for (set<int>::iterator it = current_set_of_features.begin(); it != current_set_of_features.end(); ++it) {
					cout << *it << ", ";
				} 
				cout << j << "} accuracy is " << accuracy*100 << "%" << endl;
				if (accuracy > best_so_far_accuracy) {
					best_so_far_accuracy = accuracy;
					feature_to_add_at_this_level = j;
				}
			}
		}
		current_set_of_features.insert(feature_to_add_at_this_level);
		//cout << "On level " << i << " I added feature " << feature_to_add_at_this_level << " to the current set" << endl;
	}
}
double leave_one_out_cross_validation(vector<vector<double> >& data, set<int>& features, int feature) {
	//https://stackoverflow.com/questions/2704521/generate-random-double-numbers-in-c
	//double f = (double)rand() / RAND_MAX;
    	//return 0 + f * (100 - 0);
	int number_correctly_classified = 0;
	for (int i = 0; i < data.size(); i++) {
		double label_object_to_classify = data[i][0];
//		cout << "Looping over i, at the " << i << "th location" << endl;
//		cout << "The " << i << "th object is in class " << label_object_to_classify << endl;
		double nearest_neighbor_distance = DBL_MAX;
		double nearest_neighbor_location = DBL_MAX;
		double nearest_neighbor_label = 0;
		for (int j = 0; j < data.size(); j++) {
			if (j != i) {
				double distance = 0;
				for (set<int>::iterator it = features.begin(); it != features.end(); ++it) {
					distance += pow(data[i][*it]-data[j][*it],2);
				}
				distance += pow(data[i][feature]-data[j][feature],2);
				distance = sqrt(distance);
				if (distance < nearest_neighbor_distance) {
					nearest_neighbor_distance = distance;
					nearest_neighbor_location = j;
					nearest_neighbor_label = data[j][0];
				}
			}
		}
		if (label_object_to_classify == nearest_neighbor_label) number_correctly_classified++;
	}
	return double(number_correctly_classified)/double(data.size());
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





