#ifndef _DLSM_H
#define _DLSM_H

#include "LockFreeSkiplist.h"

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

        //SHAVIT LIST

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

#endif /* _DLSM_ */