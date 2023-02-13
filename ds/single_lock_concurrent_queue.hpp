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
    char padding3[192];
    mutex q_lock;

    T enqueue(T &data);
    T dequeue();
    T get_value();

    slqueue(T sentinel_value) : head(nullptr), tail(nullptr), _sentinel_value(sentinel_value), q_lock() {}
};

template<class T>
T slqueue<T>::enqueue(T &data) {
    q_lock.lock();
    if(head == nullptr) {
        head = tail = new Node<T>(data);
        tail->next = nullptr;
        q_lock.unlock();
        return data;
    }

    Node<T>* temp = new Node<T>(data);
    tail->next = temp;
    tail = temp;
    q_lock.unlock();
    return data;
}

template<class T>
T slqueue<T>::dequeue() {
    q_lock.lock();
    if(head == nullptr) {
        printf("queue is empty!\n");
        q_lock.unlock();
        return _sentinel_value;
    }

    Node<T>* temp = head;

    if(head->next == nullptr) {
        head = tail = nullptr;
    }
    else head = head->next;

    T data = temp->_data;
    delete temp;
    q_lock.unlock();
    return data;
}

template<class T>
T slqueue<T>::get_value() {
    if(head == nullptr) return _sentinel_value;

    T temp = _sentinel_value;
    Node<T>* iter = head;

    while(iter != nullptr) {
        temp += iter->_data;
        iter = iter->next;
    }

    return temp;
}

#endif // #ifndef CONQUEUE_H