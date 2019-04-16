#include "SkipList.h"
#include "BTree.h"
#include "DLSM.h"
#include <bits/stdc++.h>

using namespace std;

int main() {

	srand(time(0));

	//Initialize our LSM with the following variables:
	// int num_inserts = 100;
	// int pRelaxation = 6;
	// int numThreads = 1;

	// LSM *lsm_Tree = new LSM(num_inserts,pRelaxation,numThreads);

	// for(int i = 1; i <= num_inserts; i++) {
	// 	lsm_Tree->insert(i, i);
	// }

	DLSM *test = new DLSM(4, 4);

	vector<thread> threads;

	for(int i = 0; i < 4; i++) {
		threads.emplace_back(&DLSM::ops, test, i);
	}

	for(auto &t : threads) {
		t.join();
	}

	(test->skip_array[0])->print(0, "output_test.txt");
	(test->skip_array[1])->print(0, "output_test1.txt");
	(test->skip_array[2])->print(0, "output_test2.txt");
	(test->skip_array[3])->print(0, "output_test3.txt");

	(test->shared)->print();
}