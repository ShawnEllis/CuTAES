/*
 *  Queue.h
 *
 *  A singly linked list with queue-style access & modification methods.
 *
 */

#ifndef QUEUE_H
#define QUEUE_H

/*
 *  Node: A node in the queue, containing a value and a pointer to next.
 */
template <class T>
class Node {
public:
    Node(T value) {
        this->value = value;
        m_pNext = 0;
    }
    T value;
    Node<T> *m_pNext;
};


template <class T>
class Queue {
public:
    Queue() {
        m_pFirst = 0;
        m_pLast = 0;
        m_size = 0;
    
    }
    Queue(const Queue& pQueue) {
        m_pFirst = pQueue->m_pFirst;
        m_pLast = pQueue->m_pLast;
        m_size = pQueue->getSize();
    }
    
    virtual ~Queue() {
        //TODO: Free memory
    }
    
    /*
     * Add a value to the back of the queue.
     */
    void pushBack(T val) {
        m_size++;
        Node<T> *pNode = new Node<T>(val);
        if (m_pLast == 0) { // first is always also 0
            m_pFirst = pNode;
            m_pLast = pNode;
            return;
        }
        m_pLast->m_pNext = pNode;
        m_pLast = pNode;        
    }
    
    /*
     * Removes the front of the queue.
     * Returns the value of the head. Caller must delete it.
     */
    T popFront() {
        if (m_pFirst == 0) {
            return 0;
        }
        m_size--;
        Node<T> *pNode = m_pFirst;
        if (m_pFirst == m_pLast) {
            m_pLast = 0;
        }
        m_pFirst = m_pFirst->m_pNext;
        T val = pNode->value;
        delete pNode;
        return val;
    }
    
    /*
     * Returns the value of the front.
     */
    T peek() {
        if (m_pFirst == 0) {
            return 0;
        }
        return m_pFirst->value;
    }
    
    /*
     * Returns the front node. Useful for iterating over the queue.
     */
    Node<T> *front() {
        return m_pFirst;
    }
    
    int getSize() {return m_size;}
    
    bool isEmpty() {return m_pFirst == 0;}
    
private:
    int m_size;
    
    Node<T> *m_pFirst;
    Node<T> *m_pLast;
};

#endif //QUEUE_H
