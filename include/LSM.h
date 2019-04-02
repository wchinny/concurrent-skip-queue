#ifndef __LSM_H
#define __LSM_H
#pragma once
#include <bits/stdc++.h>
#include "SkipList.h"
#include "BTree.h"
using namespace std;

class LSM {
public:
	SkipList *s;
	BTree *b;
	int numInserts;

	LSM(int _numInserts, int pRelaxation, int numThreads) {
		s = new SkipList(pRelaxation,numThreads);
		b = new BTree(4096);
		numInserts = _numInserts;
	};

	void insert(int data, int priority);
	void traverse();
	bool contains(int data);
	int delete_min();
};

#endif /*  __LSM_H*/