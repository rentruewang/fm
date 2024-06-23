#include "helper.hpp"
#include <cassert>
#include <unordered_map>
#include <vector>
#include "cells.hpp"
#include "fm.hpp"
#include "nets.hpp"

using namespace std;

template <modi mod>
void store_updates(vector<cell*>& cmap,
                   unordered_map<unsigned, gain_delta>& records,
                   unsigned name) {
    cell* cell = cmap[name];
    int old_gain = cell->gain();
    int new_gain;
    switch (mod) {
        case modi::inc:
            new_gain = old_gain + 1;
            cell->inc_gain();
            break;
        case modi::dec:
            new_gain = old_gain - 1;
            cell->dec_gain();
            break;
    }
    assert(new_gain == cell->gain());
    if (records.contains(name)) {
        auto& record = records[name];
        assert(old_gain == record.updated);
        record.updated = new_gain;
    } else {
        records[name] = gain_delta(old_gain, new_gain);
    }
}

int flip_cell(vector<net*>& nmap,
              vector<cell*>& cmap,
              unsigned cname,
              unordered_map<unsigned, gain_delta>& records) {
    assert(records.size() == 0);

    cell* named_cell = cmap[cname];

    int cutsize_reduction = 0;

    const vector<unsigned>& nets = named_cell->nets();
    const bool fromSide = named_cell->side();

    for (unsigned idx = 0; idx < nets.size(); ++idx) {
        const unsigned nname = nets[idx];
        net* net = nmap[nname];
        const vector<unsigned>& cells = net->cells();

        const unsigned toCount = net->count(!fromSide);

        unsigned jdx, cnt;
        switch (toCount) {
            case 0:
                --cutsize_reduction;
                for (jdx = 0; jdx < cells.size(); ++jdx) {
                    const unsigned other_name = cells[jdx];
                    store_updates<modi::inc>(cmap, records, other_name);
                }
                store_updates<modi::inc>(cmap, records, cname);
                assert(records[cname].updated == cell->gain());
                break;
            case 1:
                for (jdx = cnt = 0; jdx < cells.size(); ++jdx) {
                    const unsigned other_name = cells[jdx];
                    cell* other_end = cmap[cells[jdx]];
                    if (other_end->side() != fromSide) {
                        store_updates<modi::dec>(cmap, records, other_name);
                        ++cnt;
                    }
                }
                assert(cnt == 1);
                break;
        }

        // cell is moved here
        named_cell->flip();
        assert(cell->side() != fromSide && "Cell should be flipped here");

        net->dec_count(fromSide);
        const unsigned fromCount = net->count(fromSide);

        switch (fromCount) {
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
                    cell* other_end = cmap[cells[jdx]];
                    if (other_end->side() == fromSide) {
                        store_updates<modi::inc>(cmap, records, other_name);
                        ++cnt;
                    }
                }
                assert(cnt == 1);
                break;
            default:
                // Do nothing.
                break;
        }

        named_cell->flip();
        assert(cell->side() == fromSide && "Cell shouldn't be flipped here");

        assert(fromCount + toCount + 1 == cells.size() && "Counting error!");
    }
    named_cell->flip();
    assert(cell->side() != fromSide && "Cell should be flipped here");

    return cutsize_reduction;
}

int floor_plan::flip(bucket& nbucket,
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
        const unsigned name = iter->first;
        const gain_delta& record = iter->second;
        if (seen.contains(name)) {
            assert(!_bucket.contains(name));
            nbucket.update(record.original, record.updated, name);
        } else {
            assert(!nbucket.contains(name));
            bucket_.update(record.original, record.updated, name);
        }
    }

    return gain;
}
