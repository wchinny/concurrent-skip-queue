#include <bits/stdc++.h>

using namespace std;

bool referenceCheck(SkipList* s, int level, vector<int> values) {
	Node *temp = s->getHead();

	for(int i = 0; i < values.size(); i++) {
		temp = temp->getNext(level);
		if(temp->getValue() != values[i]) {
			return false;
		}
	}

	if(temp->getNext(level) == NULL) {
		cout << "Reference check: PASS" << endl;
		return true;
	}

	cout << "Reference check: fail whale :(" << endl;
	return false;
} 

bool testCase01() {
	SkipList *s = new SkipList(3);

	s->insert(10, 1);
	s->insert(20, 1);
	s->insert(3, 2);
	s->insert(15, 1);
	s->insert(5, 1);

	int level;
	bool success = true;

	success &= referenceCheck(s, 0, {3, 5, 10, 15, 20});
	success &= referenceCheck(s, 1, {3});
	success &= referenceCheck(s, 2, {});
	success &= referenceCheck(s, 20, {});

	return success;
}

bool testCase02() {
	SkipList *s = new SkipList();

	s->insert(10);
	s->insert(20);
	s->insert(3);
	s->insert(15);
	s->insert(5);

	int level;
	bool success = true;

	success &= referenceCheck(s, 0, {3, 5, 10, 15, 20});
	success &= referenceCheck(s, 4, {});
	success &= (s->getSize() == 5);
	success &= (s->getHeight() == 3);

	return success;
}

int main() {
	testCase01();
}