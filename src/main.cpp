#include "SkipList.h"
#include "BTree.h"
#include "LSM.h"
#include <bits/stdc++.h>

using namespace std;

int main() {

	srand(time(NULL));

	int num_inserts = 100;

	LSM *lsm_Tree = new LSM(num_inserts);

	for(int i = 1; i <= num_inserts; i++) {
		lsm_Tree->insert(i, i);
	}

	lsm_Tree->delete_k_min();

	// cout << lsm_Tree->contains(10008) << endl;

	lsm_Tree->traverse();

	// lsm_Tree->b->traverse();

	// lsm_Tree->s->print(0);


	
}