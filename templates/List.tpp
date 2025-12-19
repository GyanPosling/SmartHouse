#include "List.hpp"

#pragma once

template<typename T>
List<T>::Node::Node(const T& v) : value(v) {}

template<typename T>
List<T>::Node::Node(T&& v) : value(std::move(v)) {}

template<typename T>
void List<T>::copyFrom(const List& other) {
    for (auto it = other.begin(); it != other.end(); ++it) {
        push_back(*it);
    }
}

template<typename T>
void List<T>::moveFrom(List&& other) noexcept {
    this->head = other.head;
    this->tail = other.tail;
    this->size_ = other.size_;
    other.head = nullptr;
    other.tail = nullptr;
    other.size_ = 0;
}

template<typename T>
List<T>::Iterator::Iterator(Node* node, Node* tailNode)
    : current(node), tail(tailNode) {}

template<typename T>
T& List<T>::Iterator::operator*() const {
    if (!current) throw std::out_of_range("Iterator dereference");
    return current->value;
}

template<typename T>
typename List<T>::Iterator& List<T>::Iterator::operator++() {
    if (current) current = current->next;
    return *this;
}

template<typename T>
typename List<T>::Iterator List<T>::Iterator::operator++(int) {
    Iterator tmp(*this);
    ++(*this);
    return tmp;
}

template<typename T>
typename List<T>::Iterator& List<T>::Iterator::operator--() {
    if (!current) {
        current = tail;
        return *this;
    }
    current = current->prev;
    return *this;
}

template<typename T>
typename List<T>::Iterator List<T>::Iterator::operator--(int) {
    Iterator tmp(*this);
    --(*this);
    return tmp;
}

template<typename T>
bool List<T>::Iterator::operator==(const Iterator& other) const {
    return current == other.current;
}

template<typename T>
bool List<T>::Iterator::operator!=(const Iterator& other) const {
    return current != other.current;
}

template<typename T>
List<T>::List() = default;

template<typename T>
List<T>::List(std::initializer_list<T> init) {
    for (const T& v : init) push_back(v);
}

template<typename T>
List<T>::List(const List& other) {
    copyFrom(other);
}

template<typename T>
List<T>::List(List&& other) noexcept {
    moveFrom(std::move(other));
}

template<typename T>
List<T>& List<T>::operator=(const List& other) {
    if (this != &other) {
        clear();
        copyFrom(other);
    }
    return *this;
}

template<typename T>
List<T>& List<T>::operator=(List&& other) noexcept {
    if (this != &other) {
        clear();
        moveFrom(std::move(other));
    }
    return *this;
}

template<typename T>
List<T>::~List() {
    clear();
}

template<typename T>
void List<T>::push_back(const T& value) {
    Node* node = new Node(value);
    if (!head) head = tail = node;
    else {
        node->prev = tail;
        tail->next = node;
        tail = node;
    }
    ++size_;
}

template<typename T>
void List<T>::push_back(T&& value) {
    Node* node = new Node(std::move(value));
    if (!head) head = tail = node;
    else {
        node->prev = tail;
        tail->next = node;
        tail = node;
    }
    ++size_;
}

template<typename T>
void List<T>::pop_back() {
    if (!tail) throw std::out_of_range("List is empty");
    Node* node = tail;
    tail = tail->prev;
    if (tail) tail->next = nullptr;
    else head = nullptr;
    delete node;
    --size_;
}

template<typename T>
T& List<T>::front() {
    if (!head) throw std::out_of_range("List is empty");
    return head->value;
}

template<typename T>
T& List<T>::back() {
    if (!tail) throw std::out_of_range("List is empty");
    return tail->value;
}

template<typename T>
size_t List<T>::size() const {
    return size_;
}

template<typename T>
bool List<T>::empty() const {
    return size_ == 0;
}

template<typename T>
void List<T>::clear() {
    Node* cur = head;
    while (cur) {
        Node* next = cur->next;
        delete cur;
        cur = next;
    }
    head = tail = nullptr;
    size_ = 0;
}

template<typename T>
typename List<T>::Iterator List<T>::begin() {
    return Iterator(head, tail);
}

template<typename T>
typename List<T>::Iterator List<T>::end() {
    return Iterator(nullptr, tail);
}
