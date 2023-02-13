#include "single_lock_concurrent_queue.hpp"

#include <iostream>
#include <thread>
#include <string>
#include <stdlib.h>

using namespace std;

class checker {
    public:
    int local_sum;

    int add() {
        int temp = rand()%10;
        local_sum += temp;
        return temp;
    }

    int subtract(int data) {
        local_sum -= data;
        return local_sum;
    }

    int get_checksum() {
        return local_sum;
    }

    checker(): local_sum(0) {
        srand((unsigned) time(nullptr));
    }
};

class Barrier {
private:
    char padding0[192];
    const int releaseValue;
    char padding1[192];
    volatile int v;
    char padding2[192];
public:
    Barrier(int _releaseValue) : releaseValue(_releaseValue), v(0) {}
    void wait() {
        __sync_fetch_and_add(&v, 1);
        while (v < releaseValue) {}
    }
};

//global concurrent queue with single lock
slqueue<int> slq(0);

thread_local checker local_checker;
int global_checksum;
mutex m_lock;

Barrier* b; //+1 for parent thread and +1 for consumer
int dummy;

void producer_thread(int tid, int num_enqueues) {
    b->wait();
    printf("producer busy wait\n");
    for(int i = 0;i < num_enqueues;i++) {
        int temp = local_checker.add();
        printf("enqueuing: %d\n", temp);
        fflush(stdout);
        slq.enqueue(temp);
    }

    // for(int i = 0;i < NUM_DEQUEUES;i++) {
    //     fflush(stdout);
    //     int retval = slq.dequeue();
    //     printf("dequeuing: %d\n", retval);

    //     local_checker.subtract(retval);
    // }

    m_lock.lock();
    global_checksum += local_checker.get_checksum();
    m_lock.unlock();
}

void consumer_thread(int num_dequeues, int tid) {
    b->wait();
    printf("consumer busy wait\n");
    for(int i = 0;i < num_dequeues;i++) {
        int retval = slq.dequeue();

        printf("dequeuing: %d\n", retval);
        fflush(stdout);

        local_checker.subtract(retval);
    }

    m_lock.lock();
    global_checksum += local_checker.get_checksum();
    m_lock.unlock();
}

void runExperiment(int num_producers, int num_consumers, int num_enqueues, int num_dequeues) {
    thread producer_threads[100];
    thread consumer_threads[100];

    for(int i = 0;i < num_producers;i++) {
        producer_threads[i] = thread(producer_thread, i, num_enqueues);
    }

    printf("spawned producer threads\n");

    for(int i = 0;i < num_consumers;i++) {
        consumer_threads[i] = thread(consumer_thread, i, num_dequeues);
    }

    printf("spawned consumer threads\n");

    for(int i = 0;i < num_producers;i++) {
        producer_threads[i].join();
    }

    for(int i = 0;i < num_consumers;i++) {
        consumer_threads[i].join();
    }

    printf("queue operations done... validating checksum\n");
    printf("global_checksum value: %d\n", global_checksum);
    printf("slq value: %d\n", slq.get_value());
    assert(global_checksum == slq.get_value());
    printf("queue is valid!\n");
}

int main(int argc, char** argv) {
    int num_producers = 1, num_consumers = 1, num_enqueues = 0, num_dequeues = 0;
    if (argc == 5)
    {
        num_producers = stoi(argv[1]);
        num_consumers = stoi(argv[2]);
        num_enqueues = stoi(argv[3]);
        num_dequeues = stoi(argv[4]);
    }
    else {
        printf("Invalid Arguments!\n");
        return -1;
    }

    b = new Barrier(num_producers+num_consumers); //+1 for parent thread and +1 for consumer)
    runExperiment(num_producers, num_consumers, num_enqueues, num_dequeues);
}