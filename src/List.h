#ifndef LIST_H
#define LIST_H

#include "ListNode.h"

template <class T>
class List {
public:
    List() {pFirst = 0; pLast = 0; m_size = 0;};
    virtual ~List() {};

    void addFront(T data) {
        ListNode<T> *node = new ListNode<T>(data);
        if (pFirst) {
            node->pNext = pFirst;
            pFirst->pPrev = node;
        }
        pFirst = node;
        if (pLast == 0) {
            pLast = node;
        }
        m_size++;
    }

    void addBack(T data) {
        ListNode<T> *node = new ListNode<T>(data);
        if (pLast) {
            node->pPrev = pLast;
            pLast->pNext = node;
        }
        pLast = node;
        if (pFirst == 0) {
            pFirst = node;
        }
        m_size++;
    }

    ListNode<T>* first() {
        return pFirst;
    }

    ListNode<T>* last() {
        return pLast;
    }

    int getSize() { return m_size; }
    bool isEmpty() { return m_size == 0; }

private:
    ListNode<T> *pFirst;
    ListNode<T> *pLast;

    int m_size;
};

#endif //LIST_H

