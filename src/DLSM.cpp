#include "DLSM.h"

#define NUM_THREADS 4
#define NUM_INSERTS 500

using namespace std;

void DLSM::test_remove() {
		for(int i = 0; i < 100000; i++) {
			shared->remove((int)(rand() % 10000 + 1));
		}
	}

void DLSM::ops(int idx, int flag) {

	int k = (ceil(shared->pRelaxation / NUM_THREADS));

	printf("K: %d\n", k);

	int min_val;

	if(flag == 0) {
		// add only
		for(int i = 0; i < NUM_INSERTS / NUM_THREADS; i++) {
			this->skip_array[idx]->insert((int)(rand() % 1000000 + 1), (int)(rand() % 1000000 + 1));
		}
	} else if(flag == 1) {
		for(int i = 0; i < 1; i++) {
			if(shared->isEmpty()) {
				this->skip_array[idx]->delete_min();
			} else {
				min_val = shared->minTraverse(k, idx);
				shared->remove(min_val);
			}
		}
	}

	// else if(flag == 1) {
	// 	// delete only	
	// 	for(int i = 0; i < 10000; i++) {
	// 		this->skip_array[idx]->delete_min();
	// 	}	
	// }

	// else {
	// 	// both
	// 	for(int i = 0; i < 10000; i++) {
	// 		this->skip_array[idx]->insert((int)(rand() % 10000 + 1), (int)(rand() % 10000 + 1));
	// 	}
	// }



	//SHAVIT LIST

	// cout << this->skip_array[idx]->getHeight() << endl;

	Node *deletedNode;

	if(skip_array[idx]->getHeight() >= log2(NUM_INSERTS / NUM_THREADS)) {

		while(true) {

			deletedNode = skip_array[idx]->delete_min();

			shared->add(deletedNode->value, idx);

			if(skip_array[idx]->height <= log2(NUM_INSERTS / NUM_THREADS) - 3){
				break;
			}
		}
	}
}


