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
        m_pFirst = 0;
        m_pLast = 0;
        m_size = 0;
        Node<T> *currpQueueNode = pQueue.front();
        while (currpQueueNode != 0){
          pushBack(currpQueueNode->value);
          currpQueueNode = currpQueueNode->m_pNext;
        }
    }
    
    //Note that this does not free values.
    //Call clear<type*>() to free values and nodes.
    virtual ~Queue() {
        clear();
    }

    template <class C>
    void clear() {
        Node<C> *pCur = m_pFirst;
        Node<C> *pTmp;
        while (pCur != 0) {
            pTmp = pCur->m_pNext;
            delete pCur->value;
            delete pCur;
            pCur = pTmp;
        }
        m_size = 0;
        m_pFirst = 0;
        m_pLast = 0;
    }
    
    void clear() {
        Node<T> *pCur = m_pFirst;
        Node<T> *pTmp;
        while (pCur != 0) {
            pTmp = pCur->m_pNext;
            delete pCur;
            pCur = pTmp;
        }
        m_size = 0;
        m_pFirst = 0;
        m_pLast = 0;
    }
    
    T* toArray() {
        T* arr = new T[m_size];
        Node<T>* pCur = front();
        int i = 0;
        while (pCur != 0) {
            arr[i] = pCur->value;
            i++;
            pCur = pCur->m_pNext;
        }
        return arr;
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
    T peek() const {
        if (m_pFirst == 0) {
            return 0;
        }
        return m_pFirst->value;
    }
    
    /*
     * Returns the front node. Useful for iterating over the queue.
     */
    Node<T> *front() const {
        return m_pFirst;
    }
    
    int getSize() const {return m_size;}
    
    bool isEmpty() {return m_pFirst == 0;}
        
    /*
     *  Returns data at index
    */
    T *operator[](const int& index) const {
      Node<T> *currNode = 0;
      int i = 0;
      for (currNode = m_pFirst; currNode != 0 && i < index; currNode = currNode->m_pNext){
        ++i;
      }
      if (currNode == 0) return 0;
      else return &(currNode->value);
    }

    /*
     *  Adds data to queue
    */
    Queue<T>& operator+=(T *t){
      if (t != 0) pushBack(*t);
      return *this;
    }

    /*
     *  Iterates through second queue and adds all elements to first queue
    */
    Queue<T>& operator+=(const Queue<T>& queue){
      Node<T> *queueHead = queue.front();
      while (queueHead != 0){
        *this += &(queueHead->value);
        queueHead = queueHead->m_pNext;
      }
      return *this;
    }

    /*
     *  Adds data to queue
    */
    Queue<T>& operator+(T *t){
      Queue<T>* newQueue = new Queue<T>(*this);
      *newQueue += t;
      return *newQueue;
    }


    Queue<T>& operator+(const Queue<T>& queue){
      Queue<T>* newQueue = new Queue<T>(*this);
      *newQueue += queue;
      return *newQueue;
    }

    /*
     *  remove data from queue
    */
    Queue<T>& operator-=(T *t){
      if (t == 0) return *this; // if t is null
      if (isEmpty()) return *this; // if queue is empty
      Node<T> *currNode, *prevNode;
      prevNode = 0;
      currNode = front();
      while (currNode != 0){
        if (currNode->value == t) break;
        prevNode = currNode;
        currNode = currNode->m_pNext;
      }
      if (prevNode == 0) {  //if first element
        m_pFirst = currNode->m_pNext;
      }
      else {
        prevNode->m_pNext = currNode->m_pNext; // Otherwise, set prevNode'ss next to the currentNode's next
      }
      m_size--;
      delete currNode;
      return *this;
    }

    Queue<T>& operator-=(const Queue<T>& queue){
      if (queue->isEmpty() || isEmpty()) return *this; // if either queue is empty
      Node<T> *nodeToDel = queue.front();
      while (nodeToDel != 0){
        this -= nodeToDel->value;
        nodeToDel = nodeToDel->m_pNext;
      }
      return *this;
    }

    Queue<T>& operator-(T *t){
      Queue<T>* newQueue = new Queue<T>(*this);
      *newQueue -= t;
      return newQueue;
    }

    Queue<T>& operator-(const Queue<T>& queue){
      Queue<T>* newQueue = new Queue<T>(*this);
      *newQueue -= queue;
      return newQueue;
    }

    void operator!(){
      clear();      
    }

private:
    int m_size;
    
    Node<T> *m_pFirst;
    Node<T> *m_pLast;
};

#endif //QUEUE_H
