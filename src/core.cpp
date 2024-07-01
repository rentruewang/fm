#include <algorithm>
#include <unordered_set>
#include <utility>

#include "buckets.hpp"
#include "fm.hpp"

using namespace std;

int floor_plan::fm_once(function<bool(const unsigned)> condition) {
    unordered_set<unsigned> seen;
    bucket next_bucket;
    vector<unsigned> history;

    const unsigned SIZE = cell_map_.size();

    int max_once = 1;
    unsigned max_idx, count;
    int max_gain, acc, once;
    for (count = max_idx = 0, max_gain = acc = 0; count < SIZE; ++count) {
        const unsigned curr_cname = bucket_.pop();
        auto cell = cell_map_[curr_cname];

        seen.insert(curr_cname);
        next_bucket.push(curr_cname, *cell);

        const unsigned new_tcount =
            cell->side() ? total_count_ - 1 : total_count_ + 1;

        if (!condition(new_tcount)) {
            continue;
        }

        once = flip(next_bucket, seen, curr_cname);
        acc += once;

        max_once = std::max(max_once, once);

        history.push_back(curr_cname);
        if (acc >= max_gain) {
            max_gain = acc;
            max_idx = history.size();
        }
        if (max_once == 1 && once < 0) {
            next_bucket.empty(bucket_, seen);
            break;
        }
    }

    for (count = max_idx; count < history.size(); ++count) {
        const auto curr_cname = history[count];
        flip(next_bucket, seen, curr_cname);
    }

    bucket_ = std::move(next_bucket);

    return max_gain;
}

void floor_plan::fm() {
    init_gains();

    init_bucket();

    unsigned size = cell_map_.size();
    unsigned middle = size >> 1;
    unsigned lower = middle - tolerate_;
    unsigned upper = middle + tolerate_;

    auto runner = [&]() -> bool {
        auto is_balanced = [=](unsigned size) -> bool {
            return lower < size && size < upper;
        };
        return fm_once(is_balanced) > 0;
    };

    while (runner())
        ;
}
