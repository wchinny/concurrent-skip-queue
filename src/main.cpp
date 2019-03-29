#include "SkipList.h"
#include "BTree.h"
#include <bits/stdc++.h>

using namespace std;

int main() {

	srand(time(NULL));

	// int NUM_THREADS = 1;
	// int p_relaxation = 4;

	// SkipList *s = new SkipList(p_relaxation, NUM_THREADS);

	// s->k_priorities.push_back(new Node(13, 7, 1));
	// // s->k_priorities.push_back(420);

	// for(auto k : s->k_priorities) {
	// 	cout << k->value << endl;
	// }

	SkipList *s = new SkipList();

	BTree Btree(4096);

	int num_inserts = 1000000;

	Node *deletedNode;

	for(int i = 1; i <= num_inserts; i++) {

		s->insert(i, i);

		if(s->getHeight() >= log2(num_inserts)) {
			while(true) {
				deletedNode = s->delete_min();

				Btree.insert(deletedNode->value);

				if(s->height <= log2(num_inserts) - 3){
					break;
				}
			}
		}
	}

	// Btree.traverse();

	cout << Btree.search(1) << endl;

	// int temp = s->delete_k_min(s->head, 5);

	// cout << temp << endl;

	s->print(0);


	
}