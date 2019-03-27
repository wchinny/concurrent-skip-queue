#include "SkipList.h"
#include "BTree.h"
#include <bits/stdc++.h>

using namespace std;

int main() {
	BTree b(4096);
	for(int i = 0; i < 100; i++) {
		b.insert(i);
	}
	b.traverse();
}