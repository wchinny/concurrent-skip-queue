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

	// void sldelete(int data, int priority) {
	// 	Node* curr = this->head;
	// 	Node* want = NULL;
	// 	vector<Node*> path;

	// 	for(int i = 0; i < this->height; i++) {
	// 		path.push_back(nullptr);
	// 	}

	// 	for(int i = this->height - 1; i >= 0; i--) {
	// 		if(curr->getNext(i) == NULL) {
	// 			path[i] = curr;
	// 		}
	// 		else if(data < (curr->getNext(i))->getValue()) {
	// 			path[i] = curr;
	// 		}
	// 		else if(data == curr->getNext(i)->getValue()) {
	// 			path[i] = curr;
	// 			want = curr->getNext(i);
	// 		}
	// 		else {
	// 			curr = curr->getNext(i);
	// 			i++;
	// 		}
	// 	}

	// 	if(want == NULL) {
	// 		return;
	// 	}

	// 	for(int i = want->getHeight() - 1; i >= 0; i--) {
	// 		path[i]->setNext(i, want->getNext(i));
	// 	}

	// 	this->size--;

	// 	int newsize = this->size;
	// 	int log = (int)ceil((log2 (double(newsize))));

	// 	if(newsize == 1 || newsize == 0) {
	// 		log = 1;
	// 	}

	// 	while(log < this->height) {
	// 		trimSkipList();
	// 	}

	// 	return;
	// }

	int delete_min() {

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

		int temp = want->value;

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

		return temp;
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

void BTreeNode::traverse() 
{ 
    // There are n keys and n+1 children, travers through n keys 
    // and first n children 
    int i; 
    for (i = 0; i < n; i++) 
    { 
        // If this is not leaf, then before printing key[i], 
        // traverse the subtree rooted with child C[i]. 
        if (leaf == false) 
            C[i]->traverse(); 
        cout << " " << keys[i]; 
    } 
  
    // Print the subtree rooted with last child 
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

	srand(time(NULL));

	SkipList *s = new SkipList();
	BTree t(4096);

	clock_t begin, end;

	begin = clock();

	int numInserts = 1000000;

	for(int i = 1; i <= numInserts; i++) {
		s->insert(i+1, i);
		if(s->getHeight() >= (int)ceil((log2 (double(numInserts)))) - 3) {
			// cout << "height: " << s->getHeight() << endl;
			while(true) {
				// cout << "asdasd" << endl;
				t.insert(s->delete_min()); // throw it into b-tree
				if(s->height == (int)ceil((log2 (double(numInserts)))) - 5) {
					break;
				}
			}
		}
	}

	s->print(0);


	end = clock();

	double elapsed_secs = (double(end - begin) / CLOCKS_PER_SEC);

	t.traverse(); 

	// int height1 = s->getHeight();

	// s->print(0);

	// for(int i = 0; i <= 800000; i++) {
	// 	s->delete_min();
	// }

	// int height2 = s->getHeight();

	cout << "delete: " << elapsed_secs * 1000 << " ms" << endl;

	// cout << "prev height: " << height1 << " new height: " << height2 << endl;

	// s->print(1);

}