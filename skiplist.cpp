#include <bits/stdc++.h>
#include <cmath>

using namespace std;

struct Node {

public:

	int value;
	int height;
	vector<Node*> nextPointers;

	Node(int height) {
		this->height = height;
		for(int i = 0; i < height; i++) {
			(this->nextPointers).push_back(nullptr);
		}
	}

	Node(int data, int height) {
		this->value = data;
		this->height = height;
		for(int i = 0; i < height; i++) {
			(this->nextPointers).push_back(nullptr);
		}
	}

	int getValue() {
		return this->value;
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

	void insert(int data, int height) {
		Node* curr = this->head;
		Node* temp = new Node(data, height);
		vector<Node*> path;

		this->size++;

		for(int i = 0; i < this->height; i++) {
			path.push_back(nullptr);
		}

		for(int i = this->height - 1; i >= 0; i--) {
			if(curr->getNext(i) == nullptr) {
				path[i] = curr;
			}
			else if(data < (curr->getNext(i))->getValue()) {
				path[i] = curr;
			}
			else if(data == (curr->getNext(i)->getValue())) {
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

	void insert(int data) {
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

		insert(data, randHeight);
		return;

	}

	void sldelete(int data) {
		Node* curr = this->head;
		Node* want = NULL;
		vector<Node*> path;
		Node* temp = new Node(data, this->height);

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
                ((curr == NULL) ? myfile << "NULL" : myfile << curr->getValue()) << " -> ";
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


int main() {

	srand(time(NULL));

	SkipList *s = new SkipList();

	for(int i = 0; i < 100000; i++) {
		s->insert(i);
	}

	s->print(0);

	for(int i = 100000; i >= 10000; i--) {
		s->sldelete(i);
	}

	s->print(1);

}