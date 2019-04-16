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

	void test_remove();
	void ops(int idx, int flag);
};

#endif /* _DLSM_ */