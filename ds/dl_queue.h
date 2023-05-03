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
class dlqueue
{
public:
    Node<T> *head;
    char padding0[192];
    Node<T> *tail;
    char padding1[192];
    mutex head_lock;
    char padding2[192];
    mutex tail_lock;
    char padding3[192];
    T _sentinel_value;

    bool push(const unsigned int tid, T data);
    bool pop(const unsigned int tid, T* ref);
    T get_value();
    T peek(const unsigned int tid);
    long long getSumOfKeys();
    long long getDSSize();
    void printDebuggingDetails();

    dlqueue(T sentinel_value, const unsigned int num_threads = 1, const unsigned int min_key = 1,
        const unsigned int max_key = 1): head(nullptr), tail(nullptr), head_lock(), tail_lock(), _sentinel_value(sentinel_value) {
        Node<T>* node = new Node<T>(sentinel_value);
        node->next = nullptr;
        this->head = this->tail = node;
    }

};

template<class T>
bool dlqueue<T>::push(const unsigned int tid,T data) {
    Node<T>* node = new Node<T>(data);
    node->next = nullptr;
    tail_lock.lock();
        int j = 0;
        tail->next = node;
        tail = node;
        __asm__ __volatile__ ("repne; mov %1, %0"
                                    :"=m" (tail)
                                    : "r" (j)
                                    );
    tail_lock.unlock();
    return true;
}

template<class T>
bool dlqueue<T>::pop(const unsigned int tid, T* ref) {
    head_lock.lock();
        Node<T>* node = head;
        Node<T>* new_head = node->next;
        if(new_head == nullptr) {
            head_lock.unlock();
            return false;
        }
        *ref = head->next->_data;
        head = new_head;
    head_lock.unlock();
    delete node;
    return true;
}

template<class T>
T dlqueue<T>::get_value() {
    if(head == nullptr) return _sentinel_value;

    T temp = _sentinel_value;
    //iteration starts from head->next as this linked list assumes
    //head is logically deleted already
    Node<T>* iter = head->next;

    while(iter != nullptr) {
        temp += iter->_data;
        iter = iter->next;
    }

    return temp;
}

template<class T>
T dlqueue<T>::peek(const unsigned int tid) {
    return head->_data;
}

template<class T>
long long dlqueue<T>::getSumOfKeys()
    {
      long long sum = 0;
      Node<T>* next_node = head;

      while( next_node != NULL){
         sum = sum + (long long)next_node->_data;
         next_node = next_node->next;
      }

      return sum;
    }

template<class T>
   long long dlqueue<T>::getDSSize()
   {
      long long size = 0;
      Node<T>* next_node = head;

      while( next_node != NULL){
         size ++;
         next_node = next_node->next;
      }
//      printDebuggingDetails();
//      assert(size != 0);
//      printf("size = %lld", size);

      return size -1; //removing sentinel node
   }

    template<class T>
    void dlqueue<T>::printDebuggingDetails()
    {
        Node<T>* next_node = head;

        while( next_node != NULL)
        {
            printf("-->%u", next_node->_data);
            next_node = next_node->next;
        }
        printf("\n");
    }

#endif // #ifndef CONQUEUE_H