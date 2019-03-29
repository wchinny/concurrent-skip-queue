#include <bits/stdc++.h>
#include <cmath>
#include "SkipList.h"

using namespace std;

int Node::getValue() {
	return this->value;
}

int Node::getPriority() {
	return this->priority;
}

int Node::getHeight() {
	return this->height;
}

Node* Node::getNext(int level) {
	if(level < 0 || level > this->height - 1) {
		return nullptr;
	}
	return this->nextPointers[level];
}

void Node::setNext(int level, Node* node) {
	this->nextPointers[level] = node;
}

void Node::grow() {
	(this->height)++;
	(this->nextPointers).push_back(nullptr);
}

void Node::maybeGrow() {

	double prob = 50;
	int chance = rand() % 100 + 1;
	if(chance < prob) {
		(this->height)++;
		(this->nextPointers).push_back(nullptr);
	}
}

void Node::trim(int height) {
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

int SkipList::getSize() {
	return this->size;
}

int SkipList::getHeight() {
	return this->height;
}

Node* SkipList::getHead() {
	return this->head;
}

int SkipList::generateRandomHeight(int maxHeight) {

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

void SkipList::growSkipList() {
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

void SkipList::trimSkipList() {
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

void SkipList::insert(int data, int priority, int height) {
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

void SkipList::insert(int data, int priority) {

	int newsize = this->size + 1;

	if(this->size == 0) {
		newsize++;
	}

	int log = (int)ceil((log2 (double(newsize))));
	int maxHeight = max(log, this->height);
	int randHeight = generateRandomHeight(maxHeight);

	if(log > this->height) {
		growSkipList();
	}

	insert(data, priority, randHeight);
	return;

}

void SkipList::sldelete(int data, int priority) {
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

Node* SkipList::delete_min() {

	Node* curr = this->head;
	Node* want = NULL;
	vector<Node*> path;

	for(int i = 0; i < this->height; i++) {
		path.push_back(nullptr);
	}

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

	return want;
}


bool SkipList::contains(int data) {
	Node* curr = this->head;
	
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

Node* SkipList::get(int data) {
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

int SkipList::delete_k_min(Node* head, int k) {

	Node *curr = head->getNext(0);

	int rnd = rand() % (k+1) + 1;

 	for(int i = 0; i < rnd; i++) {
   		curr = curr->getNext(0);
 	}

 	int val = curr->getValue();

 	sldelete(curr->getValue(), curr->getPriority());

 	return val;

}

void SkipList::print(int mode) {

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

void SkipList::printHeight() {
	cout << "height: " << this->getHeight() << endl;
}


// }