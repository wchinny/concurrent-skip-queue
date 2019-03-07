#include <bits/stdc++.h>
#include <cmath>

using namespace std;

struct Node {

public:

	int priority;
	int value;
	int height;
	bool marked;

	vector<Node*> nextPointers;

	Node(int height) {
		this->height = height;
		for(int i = 0; i < height; i++) {
			(this->nextPointers).push_back(nullptr);
		}
	}

	Node(int data, int priority, int height) {
		this->value = data;
		this->height = height;
		this->priority = priority;
		for(int i = 0; i < height; i++) {
			(this->nextPointers).push_back(nullptr);
		}
	}

	int getValue() {
		return this->value;
	}

	int getPriority() {
		return this->priority;
	}

	int getHeight() {
		return this->height;
	}

	Node* getNext(int level) {
		if(level < 0 || level > this->height - 1) {
			return nullptr;
		}
		return this->nextPointers[level];
	}

	void setNext(int level, Node* node) {
		this->nextPointers[level] = node;
	}

	void grow() {
		(this->height)++;
		(this->nextPointers).push_back(nullptr);
	}

	void maybeGrow() {

		double prob = 50;
		int chance = rand() % 100 + 1;
		if(chance < prob) {
			(this->height)++;
			(this->nextPointers).push_back(nullptr);
		}
	}

	void trim(int height) {
		if(this->value != 0 && this->height == 1) {
			return;
		}
		if(height >= this->height) {
			return;
		}
		for(int i = (this->height) - 1; i >= height; i--) {
			(this->nextPointers).erase(this->nextPointers.begin() + i);
			this->height--;
		}
		return;
	}
};

struct SkipList {

public:

	Node* head;
	int height; 
	int size; 

	SkipList(void) {
		this->head = new Node(1);
		this->height = 1;
		this->size = 0;
	}

	SkipList(int height) {
		this->head = new Node(height);
		this->height = height;
		this->size = 0;
	}

	int getSize() {
		return this->size;
	}

	int getHeight() {
		return this->height;
	}

	Node* getHead() {
		return this->head;
	}

	int generateRandomHeight(int maxHeight) {

		int level = 1;
		double prob = 50.0;

		while((rand() % 100 + 1) < prob) {
			level++;
			if(level > maxHeight) {
				level = maxHeight;
				break;
			}
		}

		return level;
	}

	void growSkipList() {
		Node* curr = (this->getHead())->getNext(this->height);

		if(curr == NULL) {
			(this->head)->grow();
			this->height++;
			return;
		}

		do {
			curr->maybeGrow();
			curr = curr->getNext(this->height);
		} while(curr->getNext(this->height) != NULL);

		(this->head)->grow();
		this->height++;
	}

	void trimSkipList() {
		Node *curr = (this->getHead())->getNext(this->getHeight() - 1);
		Node* temp;

		if(curr == NULL) {
			this->getHead()->trim(this->getHeight() - 1);
			this->height--;
			return;
		}

		for(;;) {
			temp = curr->getNext(this->getHeight() - 1);
			curr->trim(this->getHeight() - 1);
			if(temp == NULL) {
				break;
			}
			curr = temp;
		}

		this->getHead()->trim(this->getHeight() - 1);
		this->height--;
	}

	void insert(int data, int priority, int height) {
		Node* curr = this->head;
		Node* temp = new Node(data, priority, height);
		vector<Node*> path;

		this->size++;

		for(int i = 0; i < this->height; i++) {
			path.push_back(nullptr);
		}

		for(int i = this->height - 1; i >= 0; i--) {
			if(curr->getNext(i) == nullptr) {
				path[i] = curr;
			}
			else if(priority < (curr->getNext(i))->getPriority()) {
				path[i] = curr;
			}
			else if(priority == (curr->getNext(i)->getPriority())) {
				path[i] = curr;
			}
			else {
				curr = curr->getNext(i);
				i++;
			}
		}

		for(int i = temp->getHeight() - 1; i >= 0; i--) {
			temp->setNext(i, path[i]->getNext(i));
			path[i]->setNext(i, temp);
		}
	}

	void insert(int data, int priority) {
		Node* curr;
		int newsize = this->size + 1;

		if(this->size == 0) {
			newsize++;
		}

		int log = (int)ceil((log2 (double(newsize))));
		int maxHeight = max(log, this->height);
		int randHeight = generateRandomHeight(maxHeight);
		int newheight = this->height+1;

		if(log > this->height) {
			growSkipList();
		}

		insert(data, priority, randHeight);
		return;

	}

	void sldelete(int data, int priority) {
		Node* curr = this->head;
		Node* want = NULL;
		vector<Node*> path;

		for(int i = 0; i < this->height; i++) {
			path.push_back(nullptr);
		}

		for(int i = this->height - 1; i >= 0; i--) {
			if(curr->getNext(i) == NULL) {
				path[i] = curr;
			}
			else if(data < (curr->getNext(i))->getValue()) {
				path[i] = curr;
			}
			else if(data == curr->getNext(i)->getValue()) {
				path[i] = curr;
				want = curr->getNext(i);
			}
			else {
				curr = curr->getNext(i);
				i++;
			}
		}

		if(want == NULL) {
			return;
		}

		for(int i = want->getHeight() - 1; i >= 0; i--) {
			path[i]->setNext(i, want->getNext(i));
		}

		this->size--;

		int newsize = this->size;
		int log = (int)ceil((log2 (double(newsize))));

		if(newsize == 1 || newsize == 0) {
			log = 1;
		}

		while(log < this->height) {
			trimSkipList();
		}

		return;
	}

	void delete_min() {

		Node* curr = this->head;
		Node* want = NULL;
		vector<Node*> path;

		for(int i = 0; i < this->height; i++) {
			path.push_back(nullptr);
		}

		int top_priority = INT_MAX;

		for(int i = this->height - 1; i >= 0; i--) {
			path[i] = curr;
		}

		want = this->head->getNext(0);

		for(int i = want->getHeight() - 1; i >= 0; i--) {
			path[i]->setNext(i, want->getNext(i));
		}

		this->size--;

		int newsize = this->size;
		int log = (int)ceil((log2 (double(newsize))));

		if(newsize == 1 || newsize == 0) {
			log = 1;
		}

		while(log < this->height) {
			trimSkipList();
		}

		return;
	}


	bool contains(int data) {
		Node* curr = this->head;
		Node* want = NULL;

		for(int i = this->getHeight() - 1; i >= 0; i--) {
			if(curr->getNext(i) == NULL) {

			}
			else if(data < (curr->getNext(i))->getValue()) {
			}
			else if(data == (curr->getNext(i))->getValue()) {
				return true;
			}
			else {
				curr = curr->getNext(i);
				i++;
			}
		}
		return false;
	}

	Node* get(int data) {
		Node* curr = this->getHead();
		Node* want = NULL;

		for(int i = this->getHeight() - 1; i >= 0; i--) {
			if(curr->getNext(i) == NULL) {
			}
			else if(data < (curr->getNext(i))->getValue()) {
			}
			else if(data == (curr->getNext(i))->getValue()) {
				want = curr->getNext(i);
				return want;
			}
			else {
				curr = curr->getNext(i);
				i++;
			}
		}

		return want;
	}

	void print(int mode) {

        ofstream myfile;

        if(mode == 0) {
        	myfile.open("outputCPP.txt");
        }
        else if(mode == 1) {
        	myfile.open("outputCPP.txt", ios_base::app);
        }
        

        int curr_level = head->height;

        for(int i = curr_level - 1; i >= 0; i--) {
            Node *curr = head->getNext(i);

            while(curr != NULL) {
                ((curr == NULL) ? myfile << "NULL" : myfile << "[" << curr->getPriority() << ", " << curr->getValue()) << "]" << " -> ";
                curr = curr->getNext(i);
            }
            myfile << endl;
        }
        myfile << "----" << endl;
       	myfile << "height: " << this->getHeight() << endl;
        myfile.close();
    }

    void printHeight() {
    	cout << "height: " << this->getHeight() << endl;
    }

};

struct RNG {
public:
	int maxRandomValue;
	unordered_set<int> usedIntegers;

	RNG() {
		this->maxRandomValue = INT_MAX;
	}

	int getUniqueRandomInteger() {

		srand(time(NULL));

		if((this->usedIntegers).size() == maxRandomValue) {
			cout << "too many random integers generated" << endl;
			exit(0);
		}

		int r = (int)(rand() % this->maxRandomValue + 1);

		while((this->usedIntegers).count(r) != 0)
			r = (int)(rand() % this->maxRandomValue + 1);

		(this->usedIntegers).insert(r);
		return r;
	}

	void setMaxRandomValue(int max) {
		this->maxRandomValue = max;
	}

	void clear() {
		(this->usedIntegers).clear();
	}

	int getRandomUsedInteger() {

		srand(time(NULL));

		if((this->usedIntegers).size() == 0) {
			return -1;
		}

		int i = 0; 
		int index = (int)(rand() % (this->usedIntegers).size());

		for(auto used : this->usedIntegers) {
			if(i == index) {
				return used;
			}

			++i;

			return -1;
		}

		return -1;
	}

	int getRandomUnusedInteger() {
		srand(time(NULL));

		int r = (int)(rand() % this->maxRandomValue + 1);

		while((this->usedIntegers).count(r) != 0) {
			r = (int)(rand() % this->maxRandomValue + 1);
		}

		return r;
	}

};

void checkList(SkipList* s, int expectedSize, int expectedHeight) {
	if(s->getSize() != expectedSize || s->getHeight() != expectedHeight) {
		cout << "Check failed!" << endl;
		exit(1);
	}
	else {
		// cout << "PASS!" << endl;
	}
}

bool testCase01() {
	SkipList *s = new SkipList();

	s->insert(97, 10);
	s->insert(55, 20);
	s->insert(71, 3);
	s->insert(41, 15);
	s->insert(109, 5);

	int level;
	bool success = true;

	s->print(0);

	success &= (s->getSize() == 5);
	success &= (s->getHeight() == 3);

	if(success) {
		cout << "Test Case 01: PASS!" << endl;
	} else {
		cout << "Test Case 01: FAIL!" << endl;
	}

	return success;
}

bool testCase02() {

	RNG* rng = new RNG();

	int size, height, initHeight = 3;

	SkipList *skiplist = new SkipList(initHeight);
	checkList(skiplist, 0, 3);

	vector<int> powers = {0, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024};

	for(int i = 0; i < powers.size() -1; i++) {
		for(size = powers[i] + 1; size <= powers[i+1]; size++) {
			skiplist->insert(rng->getUniqueRandomInteger(), rng->getUniqueRandomInteger());
			checkList(skiplist, size, max(initHeight, i + 1));
		}
	}

	cout << "Test Case 02: PASS!" << endl;
	return true;
}

bool testCase03() {
	RNG *rng = new RNG();

	for(int i = 0; i < 10; i++) {
		rng->clear();
		int numElements = 1000;

		SkipList *skiplist = new SkipList();
		for(int j = 0; j < numElements; j++) {
			skiplist->insert(rng->getUniqueRandomInteger(), rng->getUniqueRandomInteger());
		}

		Node* temp = (skiplist->getHead())->getNext(1);

		int count = 0;

		while(temp != nullptr) {
			count++;
			temp = temp->getNext(1);
		}

		if(abs(numElements / 2 - count) <= 10) {
			cout << "Test Case 03: PASS!" << endl;
			return true;
		}
	}

	cout << "Test Case 03: FAIL!" << endl;
	return false;
}

bool testCase04() {
	RNG *rng = new RNG();

	int size, height;
	SkipList* skiplist = new SkipList(5);

	skiplist->insert(rng->getUniqueRandomInteger(), rng->getUniqueRandomInteger());
	checkList(skiplist, 1, 5);

	skiplist->insert(rng->getUniqueRandomInteger(), rng->getUniqueRandomInteger());
	checkList(skiplist, 2, 5);

	skiplist->insert(rng->getUniqueRandomInteger(), rng->getUniqueRandomInteger());
	checkList(skiplist, 3, 5);

	skiplist->delete_min();
	checkList(skiplist, 2, 1);

	cout << "Test Case 04: PASS!" << endl;

	return true;
}

int main() {

	testCase01();
	testCase02();
	testCase03();
	testCase04();

}