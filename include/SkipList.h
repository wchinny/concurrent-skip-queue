#ifndef __SKIPLIST_H
#define __SKIPLIST_H
#pragma once
#include <bits/stdc++.h>
using namespace std;

class Node {

public:
	int priority;
	int value;
	int height;
	bool marked;

	vector<Node*> nextPointers;

	Node(int height){
		this->height = height;
		for(int i = 0; i < height; i++) {
			(this->nextPointers).push_back(nullptr);
		}
	}
	
	Node(int data, int priority, int height){
		this->value = data;
		this->height = height;
		this->priority = priority;
		for(int i = 0; i < height; i++) {
			(this->nextPointers).push_back(nullptr);
		}
	}

	int getValue();
	int getPriority();
	int getHeight();
	Node* getNext(int level);
	void setNext(int level, Node* node);
	void grow();
	void maybeGrow();
	void trim(int height);

	friend class SkipList; 

};

class SkipList {
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
	int getSize();
	int getHeight();
	Node* getHead();
	int generateRandomHeight(int maxHeight);
	void growSkipList();
	void trimSkipList();
	void insert(int data, int priority, int height);
	void insert(int data, int priority);
	void sldelete(int data, int priority);
	void delete_min();
	bool contains(int data);
	Node* get(int data);
	void print(int mode);
    void printHeight();
};

#endif /*  __SKIPLIST_H*/