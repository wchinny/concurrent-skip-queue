#include <bits/stdc++.h>
#include "LSM.h"

using namespace std;

void LSM::insert(int data, int priority) {
	s->insert(data, priority);

	Node *deletedNode;

	if(s->getHeight() >= log2(numInserts)) {
		while(true) {
			deletedNode = s->delete_min();

			b->insert(deletedNode->value);

			if(s->height <= log2(numInserts) - 3){
				break;
			}
		}
	}
}

bool LSM::contains(int data) {
	bool in_SkipList = s->contains(data);
	bool in_BTree = false;

	if(!in_SkipList) {
		in_BTree = b->search(data);
	}

	return in_SkipList || in_BTree;
}

void LSM::traverse() {
	cout << "Skiplist traversal" << endl;
	for(int i = 0; i < 100; i++) {cout << "=";}
	cout << endl;
	s->print(0);
	cout << "\nBTree traversal" << endl; 
	for(int i = 0; i < 100; i++) {cout << "=";}
	cout << endl;
	b->traverse();
}

int LSM::delete_k_min() {
	return s->delete_k_min(s->head, s->k);
}
