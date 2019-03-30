#include "SkipList.h"
#include "BTree.h"
#include "LSM.h"
#include <bits/stdc++.h>

using namespace std;

int main() {

	srand(time(NULL));

	LSM *lsm_Tree = new LSM(100000);


	int num_inserts = 100000;

	for(int i = 1; i <= num_inserts; i++) {
		lsm_Tree->insert(i, i);
	}

	lsm_Tree->b->traverse();

	lsm_Tree->s->print(0);


	
}