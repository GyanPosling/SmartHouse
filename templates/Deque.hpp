#pragma once

#include <iomanip>
#include <iostream>
#include <type_traits>
using namespace std;

template <typename T>
struct Node{
    T item;
    Node<T>* nextNode;
    Node<T>* previousNode;
};

template <typename T>
class Deque{
private:
    Node<T>* head;
    Node<T>* tail;

public:
    class Iterator{
    private:
        Node<T>* current;

    public:
        Iterator(Node<T>* node = nullptr): current(node){}

        bool operator==(const Iterator& other) const{
            return current == other.current;
        }

        bool operator!=(const Iterator& other) const{
            return !(*this == other);
        }

        Iterator& operator++(){
            if (current){
                current = current->nextNode;
            }
            return *this;
        }

        Iterator& operator--(){
            if (current){
                current = current->previousNode;
            }
            return *this;
        }

        T& operator*() const{
            return current->item;
        }

        T* operator->() const{
            return &(current->item);
        }

        Node<T>* getNode() const{
            return current;
        }
    };

    Deque(): head(nullptr), tail(nullptr){}
    Deque(const Deque& other): head(nullptr), tail(nullptr){
        Node<T>* currentNode = other.head;
        while (currentNode != nullptr){
            pushBack(currentNode->item);
            currentNode = currentNode->nextNode;
        }
    }
    ~Deque(){
        freeDeque();
    }

    Deque& operator=(const Deque& otherDeque);
    friend ostream& operator<<(ostream& out, const Deque& object){
        out << "Deque(size=" << object.size() << ")";

        if (object.isEmpty()){
            out << " [empty]";
            return out;
        }

        Node<T>* currentNode = object.head;
        int index = 0;

        while (currentNode != nullptr){
            out << "\n[" << index << "] ";

            if constexpr (is_pointer_v<T>){
                if (currentNode->item){
                    out << *(currentNode->item);
                } else{
                    out << "null";
                }
            } else{
                out << currentNode->item;
            }

            currentNode = currentNode->nextNode;
            ++index;
        }

        return out;
    }
    friend istream& operator>>(istream& in, Deque& object){
        if (object.head != nullptr){
            in >> object.head->item;
        }
        return in;
    }

    void pushFront(T object);
    void pushBack(T object);
    T popFront();
    T popBack();
    bool isEmpty() const;

    int size() const;
    template <typename Compare>
    void sortDeque(Compare compare);
    void freeDeque();
    Iterator removeAt(Iterator iterator);
    T& back();
    const T& back() const;

    Iterator begin(){ return Iterator(head); }
    Iterator end(){ return Iterator(nullptr); }
    Iterator begin() const{ return Iterator(head); }
    Iterator end() const{ return Iterator(nullptr); }
};

#include "Deque.tpp"
