#include "init.hpp"

#include <algorithm>

#include "fm.hpp"

using namespace std;

void FloorPlan::sort_net_list() {
    auto net_cmp = [](const auto n1, const auto n2) {
        return n1->size() < n2->size();
    };
    sort(net_map_.begin(), net_map_.end(), net_cmp);
}

unsigned NaiveInit::init(vector<shared_ptr<Cell>>& cells) const {
    unsigned half = cells.size() / 2;
    for (unsigned idx = 0, cnt = 0; idx < cells.size(); ++idx, ++cnt) {
        cells[idx]->side(cnt < half);
    }
    return half;
}

unsigned SophInit::init(vector<shared_ptr<Cell>>& cells) const {
    unsigned too_much = (cells.size() >> 1) + tolerate();

    unsigned net_size = nets().size();
    unordered_set<unsigned> cell_inited;

    unsigned net_idx, count_true, count_false;
    for (net_idx = count_true = count_false = 0; net_idx < net_size;
         ++net_idx) {
        const auto& associated = nets()[net_idx]->cells();

        unsigned cell_idx, confirmed_true, confirmed_false, unconfirmed;
        for (cell_idx = confirmed_true = confirmed_false = unconfirmed = 0;
             cell_idx < associated.size(); ++cell_idx) {
            unsigned name = associated[cell_idx];
            if (cell_inited.contains(name)) {
                const auto cell = cells[name];
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
                    auto cell = cells[name];
                    cell->side(true);
                    ++count_true;
                }
            }
        } else if (!push_to_true && count_false + unconfirmed < too_much) {
            for (unsigned name : associated) {
                if (!cell_inited.contains(name)) {
                    cell_inited.insert(name);
                    auto cell = cells[name];
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
                        auto cell = cells[name];
                        cell->side(false);
                        ++count_false;
                    }
                }
            } else {
                for (cell_idx = 0; cell_idx < associated.size(); ++cell_idx) {
                    const unsigned name = associated[cell_idx];
                    if (!cell_inited.contains(name)) {
                        cell_inited.insert(name);
                        auto cell = cells[name];
                        cell->side(true);
                        ++count_true;
                    }
                }
            }
        }
    }

    return count_true;
}

const vector<shared_ptr<Net>>& SophInit::nets() const {
    return fp_.nmap();
}
unsigned SophInit::tolerate() const {
    return fp_.tolerate();
}

void FloorPlan::init_side(const Init& init) {
    sort_net_list();
    total_count_ = init.init(cell_map_);
}

void FloorPlan::init_gains() {
    vector<int> simulation = vector<int>(cell_map_.size(), 0);

    for (auto net : net_map_) {
        const auto& cells = net->cells();
        unsigned cnt = 0;
        for (auto cname : cells) {
            cnt += static_cast<int>(cell_map_[cname]->side());
        }

        net->set_true_count(cnt);

        if (cnt == 0 || cnt == cells.size()) {
            for (unsigned cname : cells) {
                --(simulation[cname]);
            }
        } else {
            if (cnt == 1) {
                for (unsigned cname : cells) {
                    auto cell = cell_map_[cname];
                    if (cell->side()) {
                        ++(simulation[cname]);
                    }
                }
            }
            if (cnt + 1 == cells.size()) {
                for (unsigned cname : cells) {
                    auto cell = cell_map_[cname];
                    if (!(cell->side())) {
                        ++(simulation[cname]);
                    }
                }
            }
        }
    }

    for (unsigned idx = 0; idx < simulation.size(); ++idx) {
        cell_map_[idx]->gain(simulation[idx]);
    }
}

void FloorPlan::init_bucket() {
    bucket_ = Bucket(cell_map_);
}
