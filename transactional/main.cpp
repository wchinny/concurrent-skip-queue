#include "DLSM.h"
#include <cstdlib>
#include <cstring>

int main(int argc, char **argv) {

    srand(time(0));

    int num_threads = atoi(argv[1]);
    int num_inserts = atoi(argv[2]);

    DLSM *test = new DLSM(num_threads, num_inserts, 64);

    vector<thread> threads;

    for(int i = 0; i < num_threads; i++) {
        threads.emplace_back(&DLSM::ops, test, i, 0);
    }

    for(auto &t : threads) {
        t.join();
    }

    (test->shared)->print("before.txt");

    if(argc == 4 && strcmp(argv[3], "-rm") == 0) {
        vector<thread> threads2;

        for(int i = 1; i <= num_threads; i++) {
            threads2.emplace_back(&DLSM::ops, test, i, 1);
        }

        for(auto &t2 : threads2) {
            t2.join();
        }

    }

    (test->shared)->print("after.txt");
}