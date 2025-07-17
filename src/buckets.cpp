#include "buckets.hpp"

#include "helper.hpp"

using namespace std;

bucket::bucket() : bucket_(map<int, unsigned_set>()) {}

bucket::bucket(const vector<shared_ptr<cell>>& cmap)
    : bucket_(map<int, unsigned_set>()) {
    fill(cmap);
}

const map<int, unsigned_set>& bucket::get() const {
    return bucket_;
}

void bucket::push(const unsigned name, const cell& cell) {
    int gain = cell.gain();
    bucket_[gain].insert(name);
}

unsigned bucket::pop() {
    auto& [gain, lst] = *bucket_.rbegin();

    auto begin = *lst.begin();
    const unsigned cell = lst.erase(begin);
    if (lst.size() == 0) {
        bucket_.erase(gain);
    }

    return cell;
}

unsigned bucket::size() const {
    unsigned count = 0;
    for (auto iter = bucket_.begin(); iter != bucket_.end(); ++iter) {
        const auto& [_, lst] = *iter;
        count += lst.size();
    }
    return count;
}

bool bucket::contains(unsigned name) {
    for (auto iter = bucket_.begin(); iter != bucket_.end(); ++iter) {
        const auto& [_, lst] = *iter;
        if (lst.contains(name)) {
            return true;
        }
    }
    return false;
}

void bucket::update(int old_gain, int new_gain, unsigned name) {
    auto& old_list = bucket_[old_gain];

    old_list.erase(name);
    if (old_list.size() == 0) {
        bucket_.erase(old_gain);
    }
    auto& new_list = bucket_[new_gain];

    new_list.insert(name);
}

void bucket::update(gain_delta gain, unsigned name) {
    return update(gain.original, gain.updated, name);
}

void bucket::fill(const vector<shared_ptr<cell>>& cmap) {
    for (unsigned idx = 0; idx < cmap.size(); ++idx) {
        push(idx, *cmap[idx]);
    }
}

bucket& bucket::operator=(bucket&& b) {
    bucket_ = std::move(b.bucket_);
    return *this;
}

void bucket::empty(bucket& other, unordered_set<unsigned>& seen) {
    for (auto iter = other.bucket_.begin(); iter != other.bucket_.end();
         ++iter) {
        auto& [gain, lst] = *iter;

        for (unsigned i = 0, size = lst.size(); i < size; ++i) {
            auto begin = *lst.begin();
            unsigned value = lst.erase(begin);
            bucket_[gain].insert(value);
            seen.insert(value);
        }
    }
}
