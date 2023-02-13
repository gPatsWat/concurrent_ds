/**
 * The concurrent queue producer consumer
 * test application.
 * Depending on how the class is inited
 * Supports multiple producers
 * Supports single consumers
 * @CopyRight Multicore Lab
 */

#ifndef CONQUEUE_H
#define CONQUEUE_H

#include <iostream>
#include <assert.h>
#include <mutex>

#include "gem5/m5ops.h"

using namespace std;

template<class T>
class Node {
    public:
    T sentinel;
    T _data;
    Node* next;

    Node(T data): _data(data), next(nullptr), sentinel() {}
};

template <class T>
class slqueue
{
public:
    Node<T> *head;
    char padding0[192];
    Node<T> *tail;
    char padding1[192];
    mutex slqueue_lock;
    char padding2[192];
    T _sentinel_value;

    T enqueue(T &data);
    T dequeue();

    slqueue(T sentinel_value) : head(nullptr), tail(nullptr), _sentinel_value(sentinel_value) {}
};

template<class T>
T slqueue<T>::enqueue(T &data) {
    if(head == nullptr) {
        head = tail = new Node<T>(data);
        head->next = nullptr;
        return data;
    }

    Node<T>* temp = new Node<T>(data);
    head->next = temp;
    head = temp;
    return data;
}

template<class T>
T slqueue<T>::dequeue() {
    if(tail == nullptr) {
        printf("queue is empty!\n");
        return _sentinel_value;
    }

    Node<T>* temp = tail;

    if(tail->next == nullptr) {
        tail = head = nullptr;
    }
    else tail = tail->next;

    T data = temp->_data;
    delete temp;
    return data;
}

#endif // #ifndef CONQUEUE_H