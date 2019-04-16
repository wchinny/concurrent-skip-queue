#ifndef _DLSM_H
#define _DLSM_H
#pragma once 

#include <bits/stdc++.h>
#include "SkipList.h"
#include "LockFreeSkiplist.h"

using namespace std;

class DLSM {
public:

	SkipList** skip_array;
	SharedSkipList* shared;

	int num_threads;

	DLSM(int numThreads, int pRelaxation) {
		num_threads = numThreads;
		skip_array = new SkipList*[numThreads];
		shared = new SharedSkipList(20);

		for(int i = 0; i < numThreads; i++) {
			skip_array[i] = new SkipList(pRelaxation, numThreads, i);
		}
	}

	void ops(int idx) {

		for(int i = 0; i < 10000; i++) {
			this->skip_array[idx]->insert((int)(rand() % 10000 + 1), (int)(rand() % 10000 + 1));
			// this->skip_array[idx]->insert(i,i);
			// printf("%d\n", i);
		}

		cout << this->skip_array[idx]->getHeight() << endl;

		Node *deletedNode;

		if(skip_array[idx]->getHeight() >= log2(10000)) {

			while(true) {

				deletedNode = skip_array[idx]->delete_min();

				shared->add(deletedNode->value);

				if(skip_array[idx]->height <= log2(10000) - 3){
					break;
				}
			}
		}
	}
};

#endif /* _DLSM_ */