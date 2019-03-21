// C++ program for B-Tree insertion 
#include<iostream> 
#include <ctime>
using namespace std; 

// A BTree node 
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
	BTree(int _t) 
	{ root = NULL; t = _t; } 

	void traverse() 
	{ if (root != NULL) root->traverse(); } 

	BTreeNode* search(int k) 
	{ return (root == NULL)? NULL : root->search(k); } 

	void insert(int k); 
}; 

BTreeNode::BTreeNode(int t1, bool leaf1) { 
	t = t1; 
	leaf = leaf1; 

	keys = new int[2*t-1]; 
	C = new BTreeNode *[2*t]; 

	n = 0; 
} 

void BTreeNode::traverse() { 

	int i; 
	for (i = 0; i < n; i++) { 
		if (leaf == false) 
			C[i]->traverse(); 
	} 

	if (leaf == false) 
		C[i]->traverse(); 
} 

BTreeNode *BTreeNode::search(int k) { 
	int i = 0; 
	while (i < n && k > keys[i]) 
		i++; 

	if (keys[i] == k) 
		return this; 

	if (leaf == true) 
		return NULL; 

	return C[i]->search(k); 
} 

void BTree::insert(int k) { 

	if (root == NULL) { 
		root = new BTreeNode(t, true); 
		root->keys[0] = k; 
		root->n = 1; 
	} 
	else { 
		if (root->n == 2 * t - 1) { 
			BTreeNode *s = new BTreeNode(t, false); 

			s->C[0] = root; 

			s->splitChild(0, root); 

			int i = 0; 
			if (s->keys[0] < k) 
				i++; 
			s->C[i]->insertNonFull(k); 

			root = s; 
		} 
		else 
			root->insertNonFull(k); 
	} 
} 

void BTreeNode::insertNonFull(int k) { 

	int i = n - 1; 

	if (leaf == true) { 

		while (i >= 0 && keys[i] > k) { 
			keys[i + 1] = keys[i]; 
			i--; 
		} 

		keys[i + 1] = k; 
		n = n + 1; 
	} 
	else { 
		while (i >= 0 && keys[i] > k) 
			i--; 

		if (C[i+1]->n == 2 * t - 1) { 
			splitChild(i + 1, C[i + 1]); 

			if (keys[i + 1] < k) 
				i++; 
		} 
		C[i + 1]->insertNonFull(k); 
	} 
} 

void BTreeNode::splitChild(int i, BTreeNode *y) { 

	BTreeNode *z = new BTreeNode(y->t, y->leaf); 
	z->n = t - 1; 

	for (int j = 0; j < t-1; j++) 
		z->keys[j] = y->keys[j+t]; 

	if (y->leaf == false) { 
		for (int j = 0; j < t; j++) 
			z->C[j] = y->C[j + t]; 
	} 

	y->n = t - 1; 

	for (int j = n; j >= i + 1; j--) 
		C[j + 1] = C[j]; 

	C[i + 1] = z; 

	for (int j = n - 1; j >= i; j--) 
		keys[j + 1] = keys[j]; 
 
	keys[i] = y->keys[t - 1]; 

	n = n + 1; 
} 

int main() { 

	BTree t(4096); // A B-Tree with minimum degree 3 

	for(int i = 0; i < 1000000; i++) {
		t.insert(i);
	}

	// cout << "Traversal of the constucted tree is "; 

	clock_t begin = clock();

	t.traverse(); 

	clock_t end = clock();

	double elapsed_secs = (double(end - begin) / CLOCKS_PER_SEC);

	cout << "traverse: " << elapsed_secs * 1000 << " ms" << endl;

	begin = clock();

	t.search(918);

	end = clock();

	elapsed_secs = (double(end - begin) / CLOCKS_PER_SEC);

	cout << "search: " << elapsed_secs * 1000 << " ms" << endl;


	// int k = 6; 
	// (t.search(k) != NULL)? cout << "\nPresent" : cout << "\nNot Present\n"; 

	// k = 15; 
	// (t.search(k) != NULL)? cout << "\nPresent" : cout << "\nNot Present\n"; 

	return 0; 
} 

