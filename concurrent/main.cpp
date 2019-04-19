#include "DLSM.h"
#include <cstdlib>

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

    // vector<thread> threads2;

    // thread t2(&DLSM::ops, test, 2, 1);
    // thread t3(&DLSM::ops, test, 3, 1);

    // t2.join();
    // t3.join();

    (test->shared)->print("after.txt");

    // (test->skip_array[0])->print(0, "output_test.txt");
    // (test->skip_array[1])->print(0, "output_test1.txt");
    // (test->skip_array[2])->print(0, "output_test2.txt");
    // (test->skip_array[3])->print(0, "output_test3.txt");

}