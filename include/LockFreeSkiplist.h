#ifndef _LOCK_FREE_SKIPLIST_H
#define _LOCK_FREE_SKIPLIST_H
#pragma once 

#include <bits/stdc++.h>
using namespace std;

class SharedNode {
public:

	int key;
	int topLayer;
	int threadID;
	SharedNode** nexts;
	bool marked;
	bool fullyLinked;
	mutex lock;


	SharedNode(int _key, int _height) {
		fullyLinked = false; // check this if we fuck up
		marked = false; 
		threadID = -1;
		key = _key;
		topLayer = _height;
		nexts = new SharedNode*[_height + 1];
	}

	SharedNode(int _key, int _height, int _threadID) {
		fullyLinked = false; // check this if we fuck up
		marked = false; 
		threadID = _threadID;
		key = _key;
		topLayer = _height;
		nexts = new SharedNode*[_height + 1];
	}
};

class SharedSkipList {
public:

	SharedNode *LSentinel;
	SharedNode *RSentinel;

	int pRelaxation;

	int MaxHeight; // current max height

	SharedSkipList(int _MaxHeight, int _pRelaxation) 
	{	
		pRelaxation = _pRelaxation;
		MaxHeight = _MaxHeight;
		LSentinel = new SharedNode(INT_MIN + 1, _MaxHeight);
		RSentinel = new SharedNode(INT_MAX - 1, _MaxHeight);
		
		for(int i = 0; i < _MaxHeight; i++) 
		{
			LSentinel->nexts[i] = RSentinel;
			RSentinel->nexts[i] = NULL;
		}
	}

    //Function prototypes;
    int findNode(int v, SharedNode* preds [], SharedNode* succs []);
    bool add(int v, int threadID); 
 	bool remove(int v);
    bool okToDelete(SharedNode* candidate, int lFound); 
 	bool contains(int v); 
	void print(string fileName); 
	bool isEmpty();
	int minTraverse(int k, int threadID);
};

#endif /* _LOCK_FREE_SKIPLIST */