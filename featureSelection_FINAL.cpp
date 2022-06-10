#include <set>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <limits>
#include <math.h>
#include <stdio.h>
#include <float.h>
#include <chrono>
#include <ctime>

using namespace std;
void ForwardSelection(vector<vector<double> >& data);
void BackwardElimination(vector<vector<double> >& data);
double leave_one_out_cross_validation(vector<vector<double> >& data, set<int>& features);
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
	chrono::time_point<chrono::system_clock> start,end;
	start = chrono::system_clock::now();
	//Forward Selection
	if (algorithmNum == 1) {
		ForwardSelection(data);
	}

	//Backward Elmination
	else if (algorithmNum == 2) {
		BackwardElimination(data);
	}

	//Incorrect User Input
	else {
		cout << "please enter valid number" << endl;
		return 0;
	}
	end = chrono::system_clock::now();
	chrono::duration<double> elapsed_seconds = end - start;
	cout << "elapsed time: " << elapsed_seconds.count() << "s" << endl;
}

void ForwardSelection(vector<vector<double> >& data) {
	ofstream f;
	f.open("forward_trace.txt");
	cout << "Beginning Search" << endl << endl;
	set<int> current_set_of_features; 
	//test accuracy of using no features first (default rate)
	double accuracy = leave_one_out_cross_validation(data,current_set_of_features);
	f << "Using feature(s) {";
	//cout << "Using feature(s) {";
	f << "} accuracy is " << accuracy << "%" << endl;
	//cout << "} accuracy is " << accuracy << "%" << endl;
	double overall_best_accuracy = accuracy;
	set<int> best_feature_set = current_set_of_features;
	f << "Feature set {";
	//cout << "Feature set {";
	f << "} was best, accuracy is " << overall_best_accuracy << "%" << endl;
	//cout << "} was best, accuracy is " << overall_best_accuracy << "%" << endl;
	for (int i = 1; i < data[0].size(); i++) {
		int feature_to_add_at_this_level;
		double best_so_far_accuracy = 0;
		for (int j = 1; j < data[0].size(); j++) {
			if (current_set_of_features.find(j) == current_set_of_features.end()) {
				set<int> new_set_of_features = current_set_of_features;
				new_set_of_features.insert(j);	
				double accuracy = leave_one_out_cross_validation(data,new_set_of_features);
				f << "Using feature(s) {";
				//cout << "Using feature(s) {";
				for (set<int>::iterator it = new_set_of_features.begin(); it != new_set_of_features.end(); ++it) {
					f << *it;
					//cout << *it;
					if (next(it) != new_set_of_features.end()) {
						f << ", ";
						//cout << ", ";
					}
				} 
				f << "} accuracy is " << accuracy << "%" << endl;
				//cout << "} accuracy is " << accuracy << "%" << endl;
				if (accuracy > best_so_far_accuracy) {
					best_so_far_accuracy = accuracy;
					feature_to_add_at_this_level = j;
				}
			}
		}
		//Quit Search if accuracy fails to improve (takes into account local minima)
		if (best_so_far_accuracy < overall_best_accuracy) {
			f << "(Warning, Accuracy has decreased! Continuing search in case of local maxima)" << endl;
			//cout << "(Warning, Accuracy has decreased! Continuing search in case of local maxima)" << endl;
		}
		else {
			overall_best_accuracy = best_so_far_accuracy;
			best_feature_set = current_set_of_features;
			best_feature_set.insert(feature_to_add_at_this_level);
		}

		current_set_of_features.insert(feature_to_add_at_this_level);
		f << "Feature set {";
		//cout << "Feature set {";
		for (set<int>::iterator it = current_set_of_features.begin(); it != current_set_of_features.end(); ++it) {
			f << *it;
			//cout << *it;
			if (next(it) != current_set_of_features.end()) {
				f << ", ";
				//cout << ", ";
			}
		}
		f << "} was best, accuracy is " << best_so_far_accuracy << "%" << endl;
		//cout << "} was best, accuracy is " << best_so_far_accuracy << "%" << endl;
		
	}
	f << "Finished Search!! The best feature subset is {";
	//cout << "Finished Search!! The best feature subset is {";
	for (set<int>::iterator it = best_feature_set.begin(); it != best_feature_set.end(); ++it) {
		f << *it;
		//cout << *it;
		if (next(it) != best_feature_set.end()) { 
			f << ", ";
			//cout << ", ";
		}
	}
	f << "}, which has an accuracy of " << overall_best_accuracy << "%" << endl; 
	//cout << "}, which has an accuracy of " << overall_best_accuracy << "%" << endl; 
}
void BackwardElimination(vector<vector<double> >& data) {
	ofstream f;
	f.open("backward_trace.txt");
	cout << "Beginning Search" << endl << endl;
	set<int> current_set_of_features; 
	for (int i = 1; i < data[0].size(); i++) {
		current_set_of_features.insert(i);
	}

	//test accuracy of using all possible features together first
	double accuracy = leave_one_out_cross_validation(data,current_set_of_features);
	f << "Using feature(s) {";
	//cout << "Using feature(s) {";
	for (set<int>::iterator it = current_set_of_features.begin(); it != current_set_of_features.end(); ++it) {
		f << *it;
		//cout << *it;
		if (next(it) != current_set_of_features.end()) {
			f << ", ";
			//cout << ", ";
		}
	} 
	f << "} accuracy is " << accuracy << "%" << endl;
	//cout << "} accuracy is " << accuracy << "%" << endl;
	double overall_best_accuracy = accuracy;
	set<int> best_feature_set = current_set_of_features;
	f << "Feature set {";
	//cout << "Feature set {";
	for (set<int>::iterator it = current_set_of_features.begin(); it != current_set_of_features.end(); ++it) {
		f << *it;
		//cout << *it;
		if (next(it) != current_set_of_features.end()) {
			f << ", ";
			//cout << ", ";
		}
	}
	f << "} was best, accuracy is " << overall_best_accuracy << "%" << endl;
	//cout << "} was best, accuracy is " << overall_best_accuracy << "%" << endl;
	
	for (int i = 1; i < data[0].size(); i++) {
		int feature_to_remove_at_this_level;
		double best_so_far_accuracy = 0;
		for (int j = 1; j < data[0].size(); j++) {
			if (current_set_of_features.find(j) != current_set_of_features.end()) {
				set<int> new_set_of_features = current_set_of_features;
				new_set_of_features.erase(j);
				double accuracy = leave_one_out_cross_validation(data,new_set_of_features);
				f << "Using feature(s) {";
				//cout << "Using feature(s) {";
				for (set<int>::iterator it = new_set_of_features.begin(); it != new_set_of_features.end(); ++it) {
					f << *it;
					//cout << *it;
					if (next(it) != new_set_of_features.end()) {
						f << ", ";
						//cout << ", ";
					}
				} 
				f << "} accuracy is " << accuracy << "%" << endl;
				//cout << "} accuracy is " << accuracy << "%" << endl;
				if (accuracy > best_so_far_accuracy) {
					best_so_far_accuracy = accuracy;
					feature_to_remove_at_this_level = j;
				}
			}
		}
		//Quit Search if accuracy fails to improve (takes into account local minima)
		if (best_so_far_accuracy < overall_best_accuracy) {
			f << "(Warning, Accuracy has decreased! Continuing search in case of local maxima)" << endl;
			//cout << "(Warning, Accuracy has decreased! Continuing search in case of local maxima)" << endl;
		}
		else {
			overall_best_accuracy = best_so_far_accuracy;
			best_feature_set = current_set_of_features;
			best_feature_set.erase(feature_to_remove_at_this_level);
		}

		current_set_of_features.erase(feature_to_remove_at_this_level);
		f << "Feature set {";
		//cout << "Feature set {";
		for (set<int>::iterator it = current_set_of_features.begin(); it != current_set_of_features.end(); ++it) {
			f << *it;
			//cout << *it;
			if (next(it) != current_set_of_features.end()) {
				f << ", ";
				//cout << ", ";
			}
		}
		f << "} was best, accuracy is " << best_so_far_accuracy << "%" << endl;
		//cout << "} was best, accuracy is " << best_so_far_accuracy << "%" << endl;
		
	}
	f << "Finished Search!! The best feature subset is {";
	//cout << "Finished Search!! The best feature subset is {";
	for (set<int>::iterator it = best_feature_set.begin(); it != best_feature_set.end(); ++it) {
		f << *it;
		//cout << *it;
		if (next(it) != best_feature_set.end()) {
			f << ", ";
			//cout << ", ";
		}
	}
	f << "}, which has an accuracy of " << overall_best_accuracy << "%" << endl; 
	//cout << "}, which has an accuracy of " << overall_best_accuracy << "%" << endl; 
}
double leave_one_out_cross_validation(vector<vector<double> >& data, set<int>& features) {
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
	return (double(number_correctly_classified)/double(data.size()))*100;
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





