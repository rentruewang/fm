#include "buckets.hpp"
#include <cassert>

using namespace std;

list::list() : cells_(unordered_set<unsigned>()) {}

const unordered_set<unsigned>& list::get() const {
    return cells_;
}

unsigned list::size() const {
    return cells_.size();
}

void list::push(unsigned name) {
    assert(!cells.contains(name));
    cells_.insert(name);
}

unsigned list::pop() {
    assert(cells.size() != 0);

    auto last = cells_.begin();
    const unsigned cell = *last;
    cells_.erase(last);

    return cell;
}

bool list::contains(const unsigned name) const {
    return cells_.contains(name);
}

void list::erase(const unsigned name) {
    cells_.erase(name);
}

bucket::bucket() : bucket_(map<int, list>()) {}

bucket::bucket(const vector<cell*>& cmap) : bucket_(map<int, list>()) {
    fill(cmap);
}

const map<int, list>& bucket::get() const {
    return bucket_;
}

void bucket::push(const unsigned name, const cell* cell) {
    int gain = cell->gain();
    bucket_[gain].push(name);
}

unsigned bucket::pop() {
    auto max_iter = bucket_.rbegin();
    list& list = max_iter->second;

    assert(list.size() != 0);

    const unsigned cell = list.pop();
    if (list.size() == 0) {
        bucket_.erase(max_iter->first);
    }

    return cell;
}

unsigned bucket::size() const {
    unsigned count = 0;
    for (auto iter = bucket_.begin(); iter != bucket_.end(); ++iter) {
        count += iter->second.size();
    }
    return count;
}

bool bucket::contains(unsigned name) {
    for (auto iter = bucket_.begin(); iter != bucket_.end(); ++iter) {
        if (iter->second.contains(name)) {
            return true;
        }
    }
    return false;
}

void bucket::update(int old_gain, int new_gain, unsigned name) {
    auto& old_list = bucket_[old_gain];

    assert(old_list.contains(name));

    old_list.erase(name);
    if (old_list.size() == 0) {
        bucket_.erase(old_gain);
    }
    auto& new_list = bucket_[new_gain];

    assert(!new_list.contains(name));

    new_list.push(name);
}

void bucket::fill(const vector<cell*>& cmap) {
    for (unsigned idx = 0; idx < cmap.size(); ++idx) {
        push(idx, cmap[idx]);
    }
    assert(size() == cmap.size());
}

bucket& bucket::operator=(bucket&& b) {
    bucket_ = move(b.bucket_);
    return *this;
}

void bucket::empty(bucket& other, unordered_set<unsigned>& seen) {
    for (auto iter = other.bucket_.begin(); iter != other.bucket_.end();
         ++iter) {
        const unsigned gain = iter->first;
        list& list = iter->second;

        assert(list.size() != 0);

        for (unsigned idx = 0, SIZE = list.size(); idx < SIZE; ++idx) {
            unsigned value = list.pop();
            bucket_[gain].push(value);
            seen.insert(value);
        }
    }
}
