#include "nets.hpp"

using namespace std;

net::net() : true_count_(0), cells_(vector<unsigned>()) {}

void net::set_true_count(unsigned u) {
    true_count_ = u;
}

unsigned net::true_count() const {
    return true_count_;
}

unsigned net::false_count() const {
    return cells_.size() - true_count_;
}

template <bool side>
unsigned net::count() const {
    return count(side);
}

// Both template instantiation or full specification make linking possible.
template unsigned net::count<true>() const;
template unsigned net::count<false>() const;

unsigned net::count(bool side) const {
    if (side) {
        return true_count();
    } else {
        return false_count();
    }
}

void net::inc_count(bool side) {
    if (side) {
        ++true_count_;
    } else {
        --true_count_;
    }
}

void net::dec_count(bool side) {
    inc_count(!side);
}

void net::push_cell(unsigned cell) {
    cells_.push_back(cell);
}

const vector<unsigned>& net::cells() const {
    return cells_;
}

unsigned net::size() const {
    return cells_.size();
}
