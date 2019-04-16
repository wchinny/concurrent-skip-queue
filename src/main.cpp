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
		threads.emplace_back(&DLSM::ops, test, i, 0);
	}

	for(auto &t : threads) {
		t.join();
	}

	vector<thread> threads2;

	for(int i = 0; i < 4; i++) {
		threads2.emplace_back(&DLSM::ops, test, i, 1);
	}

	for(auto &t : threads2) {
		t.join();
	}

	// for(int i = 0; i < 1000000; i++) {
	// 	(test->shared)->remove((int)rand() % 10000 + 1);
	// }

	(test->skip_array[0])->print(0, "output_test.txt");
	(test->skip_array[1])->print(0, "output_test1.txt");
	(test->skip_array[2])->print(0, "output_test2.txt");
	(test->skip_array[3])->print(0, "output_test3.txt");

	(test->shared)->print();
}