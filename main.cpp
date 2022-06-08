#include <set>
#include <vector>
#include <iostream>

using namespace std;
void search();
double leave_one_out_cross_validation();

int main() {

}

void search(vector<vector<int>>& data) {
	set<int> current_set_of_features; 
	for (int i = 0; i < data[0].size(); i++) {
		cout << "On the " << i << "th level of the search tree" << endl;
		int feature_to_add_at_this_level;
		double best_so_far_accuracy = 0;
		for (int j = 0; j < data[0].size(); j++) {
			if (current_set_of_features.find(j) == current_set_of_features.end()) {
				double accuracy = leave_one_out_cross_validation(data,current_set_of_features,j+1);
				if (accuracy > best_so_far_accuracy) {
					best_so_far_accuracy = accuracy;
					feature_to_add_at_this_level = j;
				}
			}
		}
	}
}
double leave_one_out_cross_validation() {
	//https://stackoverflow.com/questions/2704521/generate-random-double-numbers-in-c
	double f = (double)rand() / RAND_MAX;
    	return 0 + f * (100 - 0);
}