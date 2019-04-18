#ifndef __BTREE_H
#define __BTREE_H
#pragma once

class BTreeNode { 
	int *keys;
	int t;	
	BTreeNode **C; 
	int n;	
	bool leaf; 
public: 
	BTreeNode(int _t, bool _leaf); 

	void insertNonFull(int k); 
	void splitChild(int i, BTreeNode *y); 
	void traverse(); 

	BTreeNode *search(int k);  

	friend class BTree; 
}; 

class BTree { 
	BTreeNode *root; 
	int t;
public: 
	
	BTree(int _t) { 
		root = NULL; t = _t; 
	} 

	void traverse() { 
		if (root != NULL) root->traverse(); 
	} 

	BTreeNode* search(int k) { 
		return (root == NULL)? NULL : root->search(k); 
	} 

	void insert(int k); 

}; 


#endif /*  __BTREE_H*/