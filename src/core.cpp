#include <cassert>
#include <unordered_set>
#include <utility>
#include "buckets.hpp"
#include "fm.hpp"
#include "utils.hpp"

using namespace std;

int floor_plan::fm_once(function<bool(const unsigned)> condition) {
    unordered_set<unsigned> seen;
    bucket nbucket;
    vector<unsigned> history;

    const unsigned SIZE = cell_map_.size();

    assert(_bucket.size() == SIZE);
    assert(nbucket.size() == 0);

    int max_once = 1;
    unsigned max_idx, count;
    int max_gain, acc, once;
    for (count = max_idx = 0, max_gain = acc = 0; count < SIZE; ++count) {
        const unsigned curr_cname = bucket_.pop();
        cell* cell = cell_map_[curr_cname];

        assert(!seen.contains(curr_cname));
        seen.insert(curr_cname);
        nbucket.push(curr_cname, cell);

        const unsigned new_tcount =
            cell->side() ? total_count_ - 1 : total_count_ + 1;

        if (!condition(new_tcount)) {
            continue;
        }

        once = flip(nbucket, seen, curr_cname);
        acc += once;

        if (max_once < once) {
            max_once = once;
        }

        assert(seen.contains(curr_cname));
        history.push_back(curr_cname);
        if (acc >= max_gain) {
            max_gain = acc;
            max_idx = history.size();
        }
        if (max_once == 1 && once < 0) {
            nbucket.empty(bucket_, seen);
            break;
        }
    }

    int total = acc;

    for (count = max_idx; count < history.size(); ++count) {
        const unsigned curr_cname = history[count];

        assert(seen.contains(curr_cname));
        once = flip(nbucket, seen, curr_cname);

        total += once;
    }

    assert(_bucket.size() == 0);
    assert(nbucket.size() == SIZE);

    bucket_ = move(nbucket);

    assert(max_gain == total && "Rewind is incorrect");

    for (auto iter = bucket_.get().begin(); iter != bucket_.get().end();
         ++iter) {
        assert(iter->second.size() != 0);
    }

    debug_printf("Made %u moves\n", max_idx);
    return max_gain;
}

void floor_plan::fm() {
    init_gains();
    check_gains();

    init_bucket();

    const unsigned SIZE = cell_map_.size();
    const unsigned MIDDLE = SIZE >> 1;
    const unsigned LOWER = MIDDLE - tolerate_;
    const unsigned UPPER = MIDDLE + tolerate_;

    debug_printf("between (%d, %d)\n", LOWER, UPPER);

    auto runner = [&](unsigned iter) -> bool {
        auto is_balanced = [=](const unsigned size) -> bool {
            return size > LOWER && size < UPPER;
        };
        int result = fm_once(is_balanced);
        debug_printf("On iteration = %03d, gain = %06d\n", iter, result);
        return result > 0;
    };

    for (unsigned iter = 0; runner(iter); ++iter)
        ;

    unsigned count_true = 0;
    for (cell* cell : cell_map_) {
        count_true += cell->side();
    }

    assert(count_true == _tcount);

    check_gains();
}

unsigned floor_plan::naiv_init_side() {
    const unsigned SIZE = cell_map_.size();
    const unsigned HALF = SIZE >> 1;
    for (unsigned idx = 0, cnt = 0; idx < cell_map_.size(); ++idx, ++cnt) {
        cell_map_[idx]->side(cnt < HALF);
    }
    return HALF;
}
