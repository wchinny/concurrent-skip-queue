#ifndef _SHAREDSKIPLIST_H
#define _SHAREDSKIPLIST_H

#include "SkipList.h"

class SharedNode {
public:

    int key;
    int topLayer;
    int threadID;
    SharedNode **nexts;
    bool marked;
    bool fullyLinked;

    [[transaction_safe]] SharedNode(int _key, int _height) {
        fullyLinked = false;
        marked = false;
        threadID = -1;
        key = _key;
        topLayer = _height;
        nexts = (SharedNode **)malloc(sizeof(SharedNode *) * _height + 1);
    }

    [[transaction_safe]] SharedNode(int _key, int _height, int _threadID) {
        fullyLinked = false;
        marked = false;
        threadID = _threadID;
        key = _key;
        topLayer = _height;
        nexts = (SharedNode **)malloc(sizeof(SharedNode *) * _height + 1);
    }
};

class SharedSkipList {
public:

    SharedNode *LSentinel;
    SharedNode *RSentinel;

    int pRelaxation;

    int MaxHeight;

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

        while(true) {

            int lFound;

            __transaction_atomic{
                lFound = findNode(v, preds, succs);
            }

            if(lFound != -1) {
                SharedNode *nodeFound = succs[lFound];

                if(!nodeFound->marked) {
                    while(!nodeFound->fullyLinked) {
                        ;
                    }

                    return false;
                }

                continue;
            }

            SharedNode *pred, *succ, *prevPred = NULL;
            bool valid = true;

            __transaction_atomic {
                for(int layer = 0; valid && (layer <= topLayer); layer++) {
                    pred = preds[layer];
                    succ = succs[layer];

                    if(pred != prevPred) {
                        prevPred = pred;
                    }

                    valid = !pred->marked && !succ->marked && pred->nexts[layer] == succ;
                }

                if(!valid) {

                    continue;
                }

                SharedNode *newNode = new SharedNode(v, topLayer, threadID);

                for(int layer = 0; layer <= topLayer; layer++) {
                    newNode->nexts[layer] = succs[layer];
                    preds[layer]->nexts[layer] = newNode;
                }

                newNode->fullyLinked = true;

                return true;
            }
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

                SharedNode *pred, *succ, *prevPred = NULL;
                bool valid = true;

                __transaction_atomic {

                    for(int layer = 0; valid && (layer <= topLayer); layer++) {
                        pred = preds[layer];
                        succ = succs[layer];

                        if(pred != prevPred) {
                            prevPred = pred;
                        }
                        valid = !pred->marked && pred->nexts[layer] == succ;
                    }

                    if(!valid) {

                        continue;
                    }

                    for(int layer = topLayer; layer >= 0; layer--)
                        preds[layer]->nexts[layer] = nodeToDelete->nexts[layer];

                    return true;
                }

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
                    myfile << "+∞" << "->"; //Updated with actual infinity symbol :)

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

#endif /* _SHAREDSKIPLIST */