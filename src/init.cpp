#include "init.hpp"
#include <algorithm>
#include "fm.hpp"

using namespace std;

void floor_plan::sort_net_list() {
    auto net_cmp = [](const net* n1, const net* n2) {
        return n1->size() < n2->size();
    };
    sort(net_map_.begin(), net_map_.end(), net_cmp);
}

unsigned naive_init::init(std::vector<cell*>& cells) const {
    unsigned half = cells.size() / 2;
    for (unsigned idx = 0, cnt = 0; idx < cells.size(); ++idx, ++cnt) {
        cells[idx]->side(cnt < half);
    }
    return half;
}

unsigned sophisticated_init::init(std::vector<cell*>& cells) const {
    unsigned too_much = (cells.size() >> 1) + tolerate_;

    unsigned net_size = nets_.size();
    unordered_set<unsigned> cell_inited;

    unsigned net_idx, count_true, count_false;
    for (net_idx = count_true = count_false = 0; net_idx < net_size;
         ++net_idx) {
        const vector<unsigned>& associated = nets_[net_idx]->cells();

        unsigned cell_idx, confirmed_true, confirmed_false, unconfirmed;
        for (cell_idx = confirmed_true = confirmed_false = unconfirmed = 0;
             cell_idx < associated.size(); ++cell_idx) {
            const unsigned name = associated[cell_idx];
            if (cell_inited.contains(name)) {
                const cell* cell = cells[name];
                if (cell->side()) {
                    ++confirmed_true;
                } else {
                    ++confirmed_false;
                }
            } else {
                ++unconfirmed;
            }
        }

        bool push_to_true = confirmed_true > confirmed_false;
        if (push_to_true && count_true + unconfirmed < too_much) {
            for (unsigned name : associated) {
                if (!cell_inited.contains(name)) {
                    cell_inited.insert(name);
                    cell* cell = cells[name];
                    cell->side(true);
                    ++count_true;
                }
            }
        } else if (!push_to_true && count_false + unconfirmed < too_much) {
            for (unsigned name : associated) {
                if (!cell_inited.contains(name)) {
                    cell_inited.insert(name);
                    cell* cell = cells[name];
                    cell->side(false);
                    ++count_false;
                }
            }
        } else {
            // count + unconfirmed >= too_much
            if (push_to_true) {
                for (unsigned name : associated) {
                    if (!cell_inited.contains(name)) {
                        cell_inited.insert(name);
                        cell* cell = cells[name];
                        cell->side(false);
                        ++count_false;
                    }
                }
            } else {
                for (cell_idx = 0; cell_idx < associated.size(); ++cell_idx) {
                    const unsigned name = associated[cell_idx];
                    if (!cell_inited.contains(name)) {
                        cell_inited.insert(name);
                        cell* cell = cells[name];
                        cell->side(true);
                        ++count_true;
                    }
                }
            }
        }
    }

    return count_true;
}

void floor_plan::init_side() {
    total_count_ = init_->init(cell_map_);
}

void floor_plan::init_gains() {
    vector<int> simulation = vector<int>(cell_map_.size(), 0);

    for (net* net : net_map_) {
        const vector<unsigned>& cells = net->cells();
        unsigned cnt = 0;
        for (unsigned cname : cells) {
            if (cell_map_[cname]->side()) {
                ++cnt;
            }
        }

        net->setCount(cnt);

        if (cnt == 0 || cnt == cells.size()) {
            for (unsigned cname : cells) {
                --(simulation[cname]);
            }
        } else {
            unsigned count;
            if (cnt == 1) {
                count = 0;
                for (unsigned cname : cells) {
                    cell* cell = cell_map_[cname];
                    if (cell->side()) {
                        ++(simulation[cname]);
                        ++count;
                    }
                }
            }
            if (cnt + 1 == cells.size()) {
                count = 0;
                for (unsigned cname : cells) {
                    cell* cell = cell_map_[cname];
                    if (!(cell->side())) {
                        ++(simulation[cname]);
                        ++count;
                    }
                }
            }
        }
    }

    for (unsigned idx = 0; idx < simulation.size(); ++idx) {
        cell_map_[idx]->gain(simulation[idx]);
    }
}

void floor_plan::init_bucket() {
    bucket_ = bucket(cell_map_);
}
