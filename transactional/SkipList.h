#ifndef _SKIPLIST_H
#define _SKIPLIST_H

#include <iostream>
#include <fstream>
#include <vector>
#include <climits>
#include <thread>
#include <cmath>

#define NUM_THREADS 4
#define NUM_INSERTS 100000

using namespace std;

int randomLevel(int _maxHeight) {
    int selected = (int)((rand() % _maxHeight)); 
    return selected;
}

class Node {

public:
    int priority;
    int value;
    int height;
    int threadID;
    bool marked;

    vector<Node *> nextPointers;

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

    bool operator<(Node *other) {
        return priority > other->priority;
    }

    bool operator>(Node *other) {
        return priority < other->priority;
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

    Node *getNext(int level) {
        if(level < 0 || level > this->height - 1) {
            return nullptr;
        }
        return this->nextPointers[level];
    }

    void setNext(int level, Node *node) {
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

class SkipList {
public:

    Node *head;
    int height;
    int size;
    vector<Node *> k_priorities;
    int k;
    int threadID;

    SkipList(void) {
        this->head = new Node(1);
        this->height = 1;
        this->size = 0;
        this->k = 2; //This is just the default!
        this->threadID = 0;
    }

    SkipList(int p, int t, int _threadID) {
        k = (int)ceil(double(p) / t);
        this->head = new Node(1);
        this->height = 1;
        this->size = 0;
        this->threadID = _threadID;
    }

    int getSize() {
        return this->size;
    }

    int getHeight() {
        return this->height;
    }

    Node *getHead() {
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
        Node *curr = (this->getHead())->getNext(this->height);

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
        Node *temp;

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
        Node *curr = this->head;
        Node *temp = new Node(data, priority, height);
        vector<Node *> path;

        this->size++;

        for(int i = 0; i < this->height; i++) {
            path.push_back(nullptr);
        }

        for(int i = this->height - 1; i >= 0; i--) {
            if(curr->getNext(i) == nullptr) {
                path[i] = curr;
            } else if(priority < (curr->getNext(i))->getPriority()) {
                path[i] = curr;
            } else if(priority == (curr->getNext(i)->getPriority())) {
                path[i] = curr;
            } else {
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

    Node *delete_min() {

        Node *curr = this->head;
        Node *want = NULL;
        vector<Node *> path;

        for(int i = 0; i < this->height; i++) {
            path.push_back(nullptr);
        }

        for(int i = this->height - 1; i >= 0; i--) {
            path[i] = curr;
        }

        want = this->head->getNext(0);

        if(want == NULL) {
            return NULL;
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

        return want;
    }


    bool contains(int data) {
        Node *curr = this->head;

        for(int i = this->getHeight() - 1; i >= 0; i--) {
            if(curr->getNext(i) == NULL) {

            } else if(data < (curr->getNext(i))->getValue()) {
            } else if(data == (curr->getNext(i))->getValue()) {
                return true;
            } else {
                curr = curr->getNext(i);
                i++;
            }
        }
        return false;
    }

    Node *get(int data) {
        Node *curr = this->getHead();
        Node *want = NULL;

        for(int i = this->getHeight() - 1; i >= 0; i--) {
            if(curr->getNext(i) == NULL) {
            } else if(data < (curr->getNext(i))->getValue()) {
            } else if(data == (curr->getNext(i))->getValue()) {
                want = curr->getNext(i);
                return want;
            } else {
                curr = curr->getNext(i);
                i++;
            }
        }

        return want;
    }

    void print(int mode, string fileName) {

        ofstream myfile;

        if(mode == 0) {
            myfile.open(fileName);
        } else if(mode == 1) {
            myfile.open(fileName, ios_base::app);
        }

        int curr_level = head->height;

        for(int i = curr_level - 1; i >= 0; i--) {
            Node *curr = head->getNext(i);

            while(curr != NULL) {
                ((curr == NULL) ? myfile << "NULL" : myfile << "[" << curr->getPriority() << ", " << curr->getValue()) << "]" << " -> ";
                ((curr == NULL) ? cout << "NULL" : cout << "[" << curr->getPriority() << ", " << curr->getValue()) << "]" << " -> ";
                curr = curr->getNext(i);
            }
            cout << endl;
            myfile << endl;
        }

        myfile << "----" << endl;
        myfile << "height: " << this->getHeight() << endl;

        cout << "----" << endl;
        cout << "height: " << this->getHeight() << endl;

        cout << "----" << endl;
        cout << "thread ID: " << this->threadID << endl;

        myfile.close();
    }

    void printHeight() {
        cout << "height: " << this->getHeight() << endl;
    }
};

#endif /* _SKIPLIST */