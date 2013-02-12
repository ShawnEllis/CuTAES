#ifndef LISTNODE_H
#define LISTNODE_H

template <class T>
class ListNode {
public:
    ListNode(T d) : data(d) {pNext = 0; pPrev = 0;};
    T data;
    ListNode<T> *pNext;
    ListNode<T> *pPrev;
};
#endif //LISTNODE_H
