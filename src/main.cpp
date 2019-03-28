#include "SkipList.h"
#include "BTree.h"
#include <bits/stdc++.h>

using namespace std;

int main() {

	srand(time(NULL));
	SkipList *s = new SkipList();

	Node *deletedNode = NULL;

	BTree Btree(4096);

	for(int i = 1; i <= 1000000; i++) {

		s->insert(123124, i);

		if(s->getHeight() >= 14) {
			while(true) {
				deletedNode = s->delete_min();

				Btree.insert(deletedNode->value);

				if(s->height == 12)
					break;
			}
		}
	}

	Btree.traverse();
	
}