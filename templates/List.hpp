#pragma once

#include <cstddef>
#include <initializer_list>
#include <utility>
#include <stdexcept>

template<typename T>
class List {
private:
    struct Node {
        T value;
        Node* next = nullptr;
        Node* prev = nullptr;

        explicit Node(const T& v);
        explicit Node(T&& v);
    };

    Node* head = nullptr;
    Node* tail = nullptr;
    size_t size_ = 0;

    void copyFrom(const List& other);
    void moveFrom(List&& other) noexcept;

public:
    class Iterator {
        Node* current = nullptr;
        Node* tail = nullptr;

    public:
        explicit Iterator(Node* node = nullptr, Node* tailNode = nullptr);

        T& operator*() const;

        Iterator& operator++();
        Iterator operator++(int);

        Iterator& operator--();
        Iterator operator--(int);

        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;

        friend class List;
    };

    List();
    List(std::initializer_list<T> init);

    List(const List& other);
    List(List&& other) noexcept;

    List& operator=(const List& other);
    List& operator=(List&& other) noexcept;

    ~List();

    void push_back(const T& value);
    void push_back(T&& value);
    void pop_back();

    T& front();
    T& back();

    size_t size() const;
    bool empty() const;

    void clear();

    Iterator begin();
    Iterator end();
};

#include "List.tpp"
