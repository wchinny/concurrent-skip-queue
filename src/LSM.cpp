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