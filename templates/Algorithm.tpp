#pragma once

template <class T>
template <class Iterator, class Value>
Deque<T> Algorithm<T>::find(Iterator begin, Iterator end, const Value& value) {
    Deque<T> results;

    for (auto it = begin; it != end; ++it) {
        auto current = *it;
        if constexpr (is_pointer_v<T>) {
            if (current && *current == value) {
                results.pushBack(current);
            }
        } else {
            if (current == value) {
                results.pushBack(current);
            }
        }
    }

    return results;
}

template <class T>
template <class Iterator, class Compare>
void Algorithm<T>::sort(Iterator begin, Iterator end, Compare comp) {
    if (begin == end) {
        return;
    }

    bool swapped;
    do {
        swapped = false;
        for (auto it = begin; it != end; ++it) {
            auto next = it;
            ++next;
            if (next == end) {
                break;
            }

            if (comp(*next, *it)) {
                swap(it, next);
                swapped = true;
            }
        }
    } while (swapped);
}

template <class T>
template <class Iterator>
void Algorithm<T>::swap(Iterator first, Iterator second) {
    auto temp = *first;
    *first = *second;
    *second = temp;
}
