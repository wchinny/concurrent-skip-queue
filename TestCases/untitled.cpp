#include <bits/stdc++.h>

int main() {

	SkipList *s = new SkipList(3);

	s->insert(10, 1);
	s->insert(20, 1);
	s->insert(3, 2);
	s->insert(15, 1);
	s->insert(5, 1);

	int level;
	bool success = true;

	referenceCheck(s, 0, {3, 5, 10, 15, 20});
	referenceCheck(s, 1, {3});
	referenceCheck(s, 2, {});
	referenceCheck(s, 20, {});
}