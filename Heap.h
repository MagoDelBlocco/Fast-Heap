#pragma once

template <class _elem, typename is_less_than>
class heap {  /// expects functor
/// compare is expected to return bool value
/// by-default min_heap, change compare function accordingly
public:
    heap(size_t _size = 1000) {
        max_size = _size;
        storage = new _elem[max_size];
        current_bound = 1;
    }

    heap(const heap<_elem, is_less_than>& target) {
        *this = target;
    }

    heap& operator = (const heap<_elem, is_less_than>& target) {
        delete[] storage;
        max_size = target.max_size;
        storage = new _elem[max_size];
        for (unsigned i = 0; i < max_size; ++i)
            storage[i] = target.storage[i];
        current_bound = target.current_bound;
    }

    [[gnu::hot]] inline void add(_elem target) {
        storage[current_bound] = target;
        sift_up(current_bound);
        ++current_bound;
    }

    [[gnu::pure]] inline bool empty() { return (current_bound == 1); }

    [[gnu::pure]] inline _elem top() { return storage[1]; }

    [[gnu::hot]] inline _elem pop() {
        unsigned iterator = 1;

        _elem retval = storage[iterator];

        while (!is_leaf(iterator)) {
            if (comparator(left_son(iterator), right_son(iterator)))
                storage[iterator] = left_son(iterator),
                iterator = left_son_pos(iterator);
            else
                storage[iterator] = right_son(iterator),
                iterator = right_son_pos(iterator);
        }

        storage[iterator] = storage[--current_bound];

        return retval;
    }

    void run_diagnostic() {
        for (unsigned i = 1; i < current_bound; ++i)
            std::cout << storage[i] << " ";
        std::cout << std::endl;
    }

    ~heap() { delete[] storage; }

private:
    void sift_up(unsigned pos) {
        unsigned target = parent_pos(pos);
        _elem aux = storage[pos];

        while (target && comparator(aux, storage[target])) {
            storage[pos] = storage[target];
            pos = target;
            target = parent_pos(pos);
        }

        storage[pos] = aux;
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

    [[gnu::hot]] inline unsigned parent_pos(unsigned pos) {
        return (pos >> 1);
    }

    [[gnu::hot]] inline _elem parent(unsigned pos) {
        return storage[parent_pos(pos)];
    }

    [[gnu::hot]] inline bool is_leaf(unsigned pos) {
        return right_son_pos(pos) < current_bound && left_son_pos(pos) < current_bound;
    }

    is_less_than comparator;
    size_t max_size, current_bound;
    _elem* storage;
};