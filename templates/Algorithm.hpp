#pragma once

#include "Deque.hpp"
#include <type_traits>
using namespace std;

template <class T>
class Algorithm {
public:
    template <class Iterator, class Value>
    static Deque<T> find(Iterator begin, Iterator end, const Value& value);

    template <class Iterator, class Compare>
    static void sort(Iterator begin, Iterator end, Compare comp);

    template <class Iterator>
    static void swap(Iterator first, Iterator second);
};

#include "Algorithm.tpp"
