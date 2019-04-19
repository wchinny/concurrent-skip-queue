#ifndef _DLSM_H
#define _DLSM_H

#include "SharedSkipList.h"

class DLSM {
public:

    SkipList **skip_array;
    SharedSkipList *shared;

    int num_threads;
    int num_inserts;

    DLSM(int numThreads, int numInserts, int pRelaxation) {
        num_threads = numThreads;
        num_inserts = numInserts;
        skip_array = new SkipList*[numThreads];
        shared = new SharedSkipList(25, pRelaxation);

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

        int k = (ceil(shared->pRelaxation / num_threads));

        int min_val;

        if(flag == 0) {
            for(int i = 0; i < num_inserts / num_threads; i++) {
                this->skip_array[idx]->insert(i, i + 1);
            }
        } else if(flag == 1) {
            for(int i = 0; i < num_inserts / num_threads; i++) {
                if(shared->isEmpty()) {
                    this->skip_array[idx]->delete_min();
                } else {
                    min_val = shared->minTraverse(k, idx);
                    shared->remove(min_val);
                }
            }
        }

        Node *deletedNode;

        if(skip_array[idx]->getHeight() >= log2(num_inserts / num_threads)) {

            while(true) {

                deletedNode = skip_array[idx]->delete_min();

                shared->add(deletedNode->value, idx);

                if(skip_array[idx]->height <= log2(num_inserts / num_threads) - 3) {
                    break;
                }
            }
        }
    }
};

#endif /* _DLSM_ */