#pragma once

#include <iostream>
#include <functional>

#define DYNAMIC_ALLOC

template <class _elem, typename compare_by = std::less<int>>
class heap {  /// expects functor
/// compare is expected to return bool value
/// by-default min_heap, change compare function accordingly
public:
    heap(size_t _size = 1024) : max_size(_size),
        current_bound(1) {
        storage = new _elem[max_size];
    }

    heap(const heap<_elem, compare_by>& target) { *this = target; }

    heap& operator = (const heap<_elem, compare_by>& target) {
        delete[] storage;
        max_size = target.max_size;
        storage = new _elem[max_size];
        for (unsigned i = 0; i < max_size; ++i)
            storage[i] = target.storage[i];
        current_bound = target.current_bound;
    }

    [[gnu::pure]] inline bool empty() { return (current_bound == 1); }

    [[gnu::pure]] inline _elem top() { return storage[1]; }

    [[gnu::pure]] inline _elem last_item() { return storage[0]; }

    [[gnu::hot]] inline void add(_elem target) {
        storage[current_bound] = target;
        sift_up(current_bound);
#ifdef DYNAMIC_ALLOC
        if (++current_bound == max_size) {
            max_size <<= 1;
            realloc(storage, max_size);
        }
#else
        ++current_bound;
#endif
    }

    [[gnu::hot]] inline _elem pop() {
        storage[0] = storage[1];
        storage[1] = storage[--current_bound];

        sift_down(1);

        return storage[0];
    }

    ~heap() { delete[] storage; }

private:
    [[gnu::hot]] inline void sift_up(unsigned crawler) {
        _elem aux = storage[crawler];

        while (parent_pos(crawler) && comparator(aux, parent(crawler))) {
            storage[crawler] = parent(crawler);
            crawler = parent_pos(crawler);
        }

        storage[crawler] = aux;
    }

    [[gnu::hot]] inline void sift_down(unsigned crawler) {
        _elem aux = storage[crawler];

        while (!is_leaf(crawler)) {
            if (!comparator(aux, left_son(crawler)) &&
                !comparator(right_son(crawler), left_son(crawler)) ||
                right_son_pos == current_bound) {
                storage[crawler] = left_son(crawler);
                crawler = left_son_pos(crawler);
            }
            else if (!comparator(aux, right_son(crawler))) {
                storage[crawler] = right_son(crawler);
                crawler = right_son_pos(crawler);
            }
            else {
                break;
            }
        }

        storage[crawler] = aux;
    }

    [[gnu::hot, gnu::pure]] inline unsigned right_son_pos(unsigned pos) {
        return 1 + (pos << 1);
    }

    [[gnu::hot, gnu::pure]] inline _elem right_son(unsigned pos) {
        return storage[right_son_pos(pos)];
    }

    [[gnu::hot, gnu::pure]] inline unsigned left_son_pos(unsigned pos) {
        return (pos << 1);
    }

    [[gnu::hot, gnu::pure]] inline _elem left_son(unsigned pos) {
        return storage[left_son_pos(pos)];
    }

    [[gnu::hot, gnu::pure]] inline unsigned parent_pos(unsigned pos) {
        return (pos >> 1);
    }

    [[gnu::hot, gnu::pure]] inline _elem parent(unsigned pos) {
        return storage[parent_pos(pos)];
    }

    [[gnu::hot]] inline bool is_leaf(unsigned pos) {
        return !(right_son_pos(pos) < current_bound && left_son_pos(pos) < current_bound);
    }

    compare_by comparator;
    size_t max_size, current_bound;
    _elem* storage;
};