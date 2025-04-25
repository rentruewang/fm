#include "nets.hpp"

using namespace std;

Net::Net() : true_count_(0), cells_(vector<unsigned>()) {}

void Net::set_true_count(unsigned u) {
    true_count_ = u;
}

unsigned Net::true_count() const {
    return true_count_;
}

unsigned Net::false_count() const {
    return cells_.size() - true_count_;
}

template <bool side>
unsigned Net::count() const {
    return count(side);
}

// Both template instantiation or full specification make linking possible.
template unsigned Net::count<true>() const;
template unsigned Net::count<false>() const;

unsigned Net::count(bool side) const {
    if (side) {
        return true_count();
    } else {
        return false_count();
    }
}

void Net::inc_count(bool side) {
    if (side) {
        ++true_count_;
    } else {
        --true_count_;
    }
}

void Net::dec_count(bool side) {
    inc_count(!side);
}

void Net::push_cell(unsigned cell) {
    cells_.push_back(cell);
}

const vector<unsigned>& Net::cells() const {
    return cells_;
}

unsigned Net::size() const {
    return cells_.size();
}
