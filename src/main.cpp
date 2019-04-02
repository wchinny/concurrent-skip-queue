#include "SkipList.h"
#include "BTree.h"
#include "LSM.h"
#include <bits/stdc++.h>

using namespace std;

int main() {

	srand(time(0));

	//Initialize our LSM with the following variables:
	int num_inserts = 100;
	int pRelaxation = 6;
	int numThreads = 1;

	LSM *lsm_Tree = new LSM(num_inserts,pRelaxation,numThreads);

	for(int i = 1; i <= num_inserts; i++) {
		lsm_Tree->insert(i, i);
	}

   //cout << lsm_Tree->contains(10008) << endl;

   lsm_Tree->traverse();

}