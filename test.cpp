#include <bits/stdc++.h>

#define NUM_THREADS 4
#define NUM_INSERTS 500

using namespace std;

int randomLevel(int _maxHeight) {
    int selected = (int)((rand() % _maxHeight)); //Not rand() % _maxHeight + 1 "Array index out of bounds!"
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

    void sldelete(int data, int priority) {
        Node *curr = this->head;
        Node *want = NULL;
        vector<Node *> path;

        for(int i = 0; i < this->height; i++) {
            path.push_back(nullptr);
        }

        for(int i = this->height - 1; i >= 0; i--) {
            if(curr->getNext(i) == NULL) {
                path[i] = curr;
            } else if(data < (curr->getNext(i))->getValue()) {
                path[i] = curr;
            } else if(data == curr->getNext(i)->getValue()) {
                path[i] = curr;
                want = curr->getNext(i);
            } else {
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

    int delete_k_min(Node *head, int k) {

        Node *curr = head->getNext(0);

        int rnd = rand() % (k + 1) + 1;

        for(int i = 0; i < rnd; i++) {
            curr = curr->getNext(0);
        }

        int val = curr->getValue();

        sldelete(val, curr->getPriority());

        return val;

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

class SharedNode {
public:

    int key;
    int topLayer;
    int threadID;
    SharedNode **nexts;
    bool marked;
    bool fullyLinked;

    [[transaction_safe]] SharedNode(int _key, int _height) {
        fullyLinked = false; // check this if we fuck up
        marked = false;
        threadID = -1;
        key = _key;
        topLayer = _height;
        // nexts = new SharedNode*[_height + 1];
        nexts = (SharedNode **)malloc(sizeof(SharedNode *) * _height + 1);

    }

    [[transaction_safe]] SharedNode(int _key, int _height, int _threadID) {
        fullyLinked = false; // check this if we fuck up
        marked = false;
        threadID = _threadID;
        key = _key;
        topLayer = _height;
        // nexts = new SharedNode*[_height + 1];
        nexts = (SharedNode **)malloc(sizeof(SharedNode *) * _height + 1);
    }
};

class SharedSkipList {
public:

    SharedNode *LSentinel;
    SharedNode *RSentinel;

    int pRelaxation;

    int MaxHeight; // current max height

    [[transaction_safe]] SharedSkipList(int _MaxHeight, int _pRelaxation) {
        pRelaxation = _pRelaxation;
        MaxHeight = _MaxHeight;
        LSentinel = new SharedNode(INT_MIN + 1, _MaxHeight);
        RSentinel = new SharedNode(INT_MAX - 1, _MaxHeight);

        for(int i = 0; i < _MaxHeight; i++) {
            LSentinel->nexts[i] = RSentinel;
            RSentinel->nexts[i] = NULL;
        }
    }


    [[transaction_safe]] int findNode(int v, SharedNode *preds [], SharedNode *succs []) {

        int lFound = -1;
        SharedNode *pred = LSentinel;

        for(int layer = MaxHeight - 1; layer >= 0; layer--) {
            SharedNode *curr = pred->nexts[layer];
            while(v > curr->key) {
                pred = curr;
                curr = pred->nexts[layer];
            }
            if(lFound == -1 && v == curr->key) {
                lFound = layer;
            }
            preds[layer] = pred;
            succs[layer] = curr;
        }
        return lFound;
    }

    bool add(int v, int threadID) {

        int topLayer = randomLevel(MaxHeight);

        SharedNode **preds = new SharedNode*[MaxHeight];
        SharedNode **succs = new SharedNode*[MaxHeight];

        int highestLocked = -1;

        while(true) {

            int lFound;

            __transaction_atomic{
                lFound = findNode(v, preds, succs);
            }

            if(lFound != -1) {
                SharedNode *nodeFound = succs[lFound];

                //Wait until the node is fully linked. Should not be an issue for sequential!
                if(!nodeFound->marked) {
                    while(!nodeFound->fullyLinked) {
                        ;
                    }

                    return false;
                }

                continue;
            }

            highestLocked = -1;

            SharedNode *pred, *succ, *prevPred = NULL;
            bool valid = true;

            for(int layer = 0; valid && (layer <= topLayer); layer++) {
                pred = preds[layer];
                succ = succs[layer];

                if(pred != prevPred) {
                    highestLocked = layer;
                    prevPred = pred;
                }

                __transaction_atomic{
                    valid = !pred->marked && !succ->marked && pred->nexts[layer] == succ;
                }

            }

            if(!valid) {

                // for(int i = 0; i < MaxHeight; i++) {
                //     preds[i]->lock.unlock();
                // }

                continue;
            }

            SharedNode *newNode = new SharedNode(v, topLayer, threadID);

            for(int layer = 0; layer <= topLayer; layer++) {
                newNode->nexts[layer] = succs[layer];
                preds[layer]->nexts[layer] = newNode;
            }

            newNode->fullyLinked = true;

            for(int i = 0; i < MaxHeight; i++) {
                preds[i]->lock.unlock();
            }

            return true;
        }

    }

    bool remove(int v) {

        SharedNode *nodeToDelete = NULL;
        bool isMarked = false;
        int topLayer = -1;

        SharedNode **preds = new SharedNode*[MaxHeight];
        SharedNode **succs = new SharedNode*[MaxHeight];

        while(true) {
            int lFound = findNode(v, preds, succs);

            if(isMarked || (lFound != -1 && okToDelete(succs[lFound], lFound))) {
                if(!isMarked) {
                    nodeToDelete = succs[lFound];
                    topLayer = nodeToDelete->topLayer;

                    if(nodeToDelete->marked) {
                        return false;
                    }

                    nodeToDelete->marked = true;
                    isMarked = true;
                }

                int highestLocked = -1;

                SharedNode *pred, *succ, *prevPred = NULL;
                bool valid = true;

                for(int layer = 0; valid && (layer <= topLayer); layer++) {
                    pred = preds[layer];
                    succ = succs[layer];

                    if(pred != prevPred) {
                        pred->lock.lock();
                        highestLocked = layer;
                        prevPred = pred;
                    }
                    valid = !pred->marked && pred->nexts[layer] == succ;
                }

                if(!valid) {
                    // for(int i = MaxHeight-1; i >= highestLocked; i--)
                    // 	preds[i]->lock.unlock();
                    for(int i = 0; i < MaxHeight; i++) {
                        preds[i]->lock.unlock();
                    }

                    continue;
                }

                for(int layer = topLayer; layer >= 0; layer--)
                    preds[layer]->nexts[layer] = nodeToDelete->nexts[layer];

                nodeToDelete->lock.unlock();

                // for(int i = MaxHeight-1; i >= highestLocked; i--) //Check if this is needed here!
                // 	preds[i]->lock.unlock();
                for(int i = 0; i < MaxHeight; i++) {
                    preds[i]->lock.unlock();
                }

                return true;
            }

            else return false;
        }
    }

    bool okToDelete(SharedNode *candidate, int lFound) {
        return (candidate->fullyLinked && candidate->topLayer == lFound && !candidate->marked);
    }

    bool contains(int v) {
        SharedNode **preds = new SharedNode*[MaxHeight];
        SharedNode **succs = new SharedNode*[MaxHeight];
        int lFound = findNode(v, preds, succs);
        return(lFound != -1 && succs[lFound]->fullyLinked && !succs[lFound]->marked);
    }

    void print(string fileName) {

        ofstream myfile;

        myfile.open(fileName);

        int curr_level = LSentinel->topLayer;

        myfile << "Sentinel Node Height: " << curr_level << endl;

        for(int i = curr_level - 1; i >= 0; i--) {
            SharedNode *curr = LSentinel->nexts[i];

            while(curr != NULL) {
                if(curr->key != INT_MAX - 1) {
                    myfile << "[" << curr->key << "," << curr->threadID << "]" << "->";
                } else
                    myfile << "+∞" << "->";	//Updated with actual infinity symbol :)

                curr = curr->nexts[i];
            }
            myfile << endl;
        }
        myfile.close();

    }

    bool isEmpty() {

        if(LSentinel->nexts[0]->key == INT_MAX - 1) {
            return true;
        }

        return false;
    }

    int minTraverse(int k, int threadID) {

        SharedNode *temp = LSentinel->nexts[0];

        int retVal;

        for(int i = 0; i < k + 1; i++) {

            if(temp == NULL) {
                return retVal;
            }

            retVal = temp->key;

            if(temp->threadID == threadID) {
                return retVal;
            }

            temp = temp->nexts[0];
        }

        return retVal;
    }

};

class DLSM {
public:

    SkipList **skip_array;
    SharedSkipList *shared;

    int num_threads;

    DLSM(int numThreads, int pRelaxation) {
        num_threads = numThreads;
        skip_array = new SkipList*[numThreads];
        shared = new SharedSkipList(10, pRelaxation);

        for(int i = 0; i < numThreads; i++) {
            skip_array[i] = new SkipList(pRelaxation, numThreads, i);
        }
    }

    void test_remove() {
        for(int i = 0; i < 100000; i++) {
            shared->remove((int)(rand() % 10000 + 1));
        }
    }

    void ops(int idx, int flag) {

        int k = (ceil(shared->pRelaxation / NUM_THREADS));

        printf("K: %d\n", k);

        int min_val;

        if(flag == 0) {
            // add only
            for(int i = 0; i < NUM_INSERTS / NUM_THREADS; i++) {
                this->skip_array[idx]->insert((int)(rand() % 1000000 + 1), (int)(rand() % 1000000 + 1));
            }
        } else if(flag == 1) {
            for(int i = 0; i < 1; i++) {
                if(shared->isEmpty()) {
                    this->skip_array[idx]->delete_min();
                } else {
                    min_val = shared->minTraverse(k, idx);
                    shared->remove(min_val);
                }
            }
        }

        // else if(flag == 1) {
        // 	// delete only
        // 	for(int i = 0; i < 10000; i++) {
        // 		this->skip_array[idx]->delete_min();
        // 	}
        // }

        // else {
        // 	// both
        // 	for(int i = 0; i < 10000; i++) {
        // 		this->skip_array[idx]->insert((int)(rand() % 10000 + 1), (int)(rand() % 10000 + 1));
        // 	}
        // }



        //SHAVIT LIST

        // cout << this->skip_array[idx]->getHeight() << endl;

        Node *deletedNode;

        if(skip_array[idx]->getHeight() >= log2(NUM_INSERTS / NUM_THREADS)) {

            while(true) {

                deletedNode = skip_array[idx]->delete_min();

                shared->add(deletedNode->value, idx);

                if(skip_array[idx]->height <= log2(NUM_INSERTS / NUM_THREADS) - 3) {
                    break;
                }
            }
        }
    }
};

int main() {

	srand(time(0));

	DLSM *test = new DLSM(4, 64);

	vector<thread> threads;

	for(int i = 0; i < 4; i++) {
		threads.emplace_back(&DLSM::ops, test, i, 0);
	}

	for(auto &t : threads) {
		t.join();
	}

	(test->shared)->print("before.txt");

	vector<thread> threads2;

	thread t2(&DLSM::ops, test, 2, 1);
	thread t3(&DLSM::ops, test, 3, 1);

	t2.join();
	t3.join();

	(test->shared)->print("after.txt");

	(test->skip_array[0])->print(0, "output_test.txt");
	(test->skip_array[1])->print(0, "output_test1.txt");
	(test->skip_array[2])->print(0, "output_test2.txt");
	(test->skip_array[3])->print(0, "output_test3.txt");

    // __transaction_atomic {
    //     SharedSkipList *s = new SharedSkipList(20, 16);
    // }

}