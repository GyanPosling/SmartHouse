#pragma once

template <typename T>
Deque<T>& Deque<T>::operator=(const Deque<T>& otherDeque){
    if (this != &otherDeque){
        freeDeque();
        Node<T>* currentNode = otherDeque.head;
        while (currentNode != nullptr){
            pushBack(currentNode->item);
            currentNode = currentNode->nextNode;
        }
    }
    return *this;
}

template <typename T>
void Deque<T>::pushFront(T object){
    Node<T>* newNode = new Node<T>{object, nullptr, nullptr};
    if (head != nullptr){
        newNode->nextNode = head;
        head->previousNode = newNode;
        head = newNode;
    } else{
        head = tail = newNode;
    }
}

template <typename T>
void Deque<T>::pushBack(T object){
    Node<T>* newNode = new Node<T>{object, nullptr, nullptr};
    if (tail != nullptr){
        newNode->previousNode = tail;
        tail->nextNode = newNode;
        tail = newNode;
    } else{
        head = tail = newNode;
    }
}

template <typename T>
T Deque<T>::popFront(){
    if (head == nullptr){
        return T();
    }
    T object = head->item;
    Node<T>* nodeToRemove = head;
    head = head->nextNode;
    if (head != nullptr){
        head->previousNode = nullptr;
    } else{
        tail = nullptr;
    }

    delete nodeToRemove;
    return object;
}

template <typename T>
T Deque<T>::popBack(){
    if (tail == nullptr){
        return T();
    }
    T object = tail->item;
    Node<T>* nodeToRemove = tail;
    tail = tail->previousNode;
    if (tail != nullptr){
        tail->nextNode = nullptr;
    } else{
        head = nullptr;
    }

    delete nodeToRemove;
    return object;
}

template <typename T>
bool Deque<T>::isEmpty() const{
    return head == nullptr && tail == nullptr;
}

template <typename T>
int Deque<T>::size() const{
    int count = 0;
    Node<T>* currentNode = head;
    while (currentNode != nullptr){
        ++count;
        currentNode = currentNode->nextNode;
    }

    return count;
}

template <typename T>
template <typename Compare>
void Deque<T>::sortDeque(Compare compare){
    if (head == nullptr || head == tail){
        return;
    }

    bool swapped;
    do{
        swapped = false;
        Node<T>* currentNode = head;
        while (currentNode != nullptr && currentNode->nextNode != nullptr){
            if (compare(currentNode->nextNode->item, currentNode->item)){
                T temporaryItem = currentNode->item;
                currentNode->item = currentNode->nextNode->item;
                currentNode->nextNode->item = temporaryItem;
                swapped = true;
            }
            currentNode = currentNode->nextNode;
        }
    } while (swapped);
}

template <typename T>
void Deque<T>::freeDeque(){
    while (head != nullptr){
        Node<T>* nodeToRemove = head;
        head = head->nextNode;
        delete nodeToRemove;
    }
    tail = nullptr;
}

template <typename T>
typename Deque<T>::Iterator Deque<T>::removeAt(Iterator iterator){
    Node<T>* targetNode = iterator.getNode();
    if (targetNode == nullptr){
        return Iterator(nullptr);
    }

    Node<T>* nextNode = targetNode->nextNode;
    if (targetNode->previousNode){
        targetNode->previousNode->nextNode = targetNode->nextNode;
    } else{
        head = targetNode->nextNode;
    }

    if (targetNode->nextNode){
        targetNode->nextNode->previousNode = targetNode->previousNode;
    } else{
        tail = targetNode->previousNode;
    }

    delete targetNode;
    return Iterator(nextNode);
}

template <typename T>
T& Deque<T>::back(){
    return tail->item;
}

template <typename T>
const T& Deque<T>::back() const{
    return tail->item;
}
