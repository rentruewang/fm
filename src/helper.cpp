#include "helper.hpp"

#include <unordered_map>
#include <vector>

#include "cells.hpp"
#include "fm.hpp"
#include "nets.hpp"

using namespace std;

// Template definition visible for all usage, so it's ok.
template <modi mod>
void store_updates(vector<shared_ptr<cell>>& cmap,
                   unordered_map<unsigned, gain_delta>& records,
                   unsigned name) {
    auto cell = cmap[name];
    int old_gain = cell->gain();
    int new_gain;
    switch (mod) {
        case modi::inc:
            new_gain = old_gain + 1;
            ++*cell;
            break;
        case modi::dec:
            new_gain = old_gain - 1;
            --*cell;
            break;
    }
    if (records.contains(name)) {
        auto& record = records[name];
        record.updated = new_gain;
    } else {
        records[name] = gain_delta(old_gain, new_gain);
    }
}

int flip_cell(vector<shared_ptr<net>>& nmap,
              vector<shared_ptr<cell>>& cmap,
              unsigned cname,
              unordered_map<unsigned, gain_delta>& records) {
    auto named_cell = cmap[cname];

    int cutsize_reduction = 0;

    const auto& nets = named_cell->nets();
    const bool from_side = named_cell->side();

    for (unsigned idx = 0; idx < nets.size(); ++idx) {
        const unsigned nname = nets[idx];
        auto net = nmap[nname];
        const auto& cells = net->cells();

        const unsigned to_count = net->count(!from_side);

        unsigned jdx, cnt;
        switch (to_count) {
            case 0:
                --cutsize_reduction;
                for (jdx = 0; jdx < cells.size(); ++jdx) {
                    const unsigned other_name = cells[jdx];
                    store_updates<modi::inc>(cmap, records, other_name);
                }
                store_updates<modi::inc>(cmap, records, cname);
                break;
            case 1:
                for (jdx = cnt = 0; jdx < cells.size(); ++jdx) {
                    const unsigned other_name = cells[jdx];
                    auto other_end = cmap[cells[jdx]];
                    if (other_end->side() != from_side) {
                        store_updates<modi::dec>(cmap, records, other_name);
                        ++cnt;
                    }
                }
                break;
        }

        // cell is moved here
        named_cell->flip();

        net->dec_count(from_side);
        const unsigned from_count = net->count(from_side);

        switch (from_count) {
            case 0:
                ++cutsize_reduction;
                for (jdx = 0; jdx < cells.size(); ++jdx) {
                    const unsigned other_name = cells[jdx];
                    store_updates<modi::dec>(cmap, records, other_name);
                }
                store_updates<modi::dec>(cmap, records, cname);
                break;
            case 1:
                for (jdx = cnt = 0; jdx < cells.size(); ++jdx) {
                    const unsigned other_name = cells[jdx];
                    auto other_end = cmap[cells[jdx]];
                    if (other_end->side() == from_side) {
                        store_updates<modi::inc>(cmap, records, other_name);
                        ++cnt;
                    }
                }
                break;
            default:
                // Do nothing.
                break;
        }

        named_cell->flip();
    }
    named_cell->flip();

    return cutsize_reduction;
}

int floor_plan::flip(bucket& next_bucket,
                     const unordered_set<unsigned>& seen,
                     unsigned cname) {
    unordered_map<unsigned, gain_delta> records;

    if (cell_map_[cname]->side()) {
        --total_count_;
    } else {
        ++total_count_;
    }
    int gain = flip_cell(net_map_, cell_map_, cname, records);

    for (auto iter = records.begin(); iter != records.end(); ++iter) {
        const auto& [name, record] = *iter;
        if (seen.contains(name)) {
            next_bucket.update(record, name);
        } else {
            bucket_.update(record, name);
        }
    }

    return gain;
}
