#include <cassert>
#include "fm.hpp"

using namespace std;

unsigned floor_plan::soph_init_side() {
    const unsigned too_much = (cell_map_.size() >> 1) + tolerate_;

    const unsigned net_size = net_map_.size();
    unordered_set<unsigned> cell_inited;

    auto net_list = net_map_;
    auto net_cmp = [](const net* n1, const net* n2) {
        return n1->size() < n2->size();
    };
    sort(net_list.begin(), net_list.end(), net_cmp);

    unsigned net_idx, count_true, count_false;
    for (net_idx = count_true = count_false = 0; net_idx < net_size;
         ++net_idx) {
        const vector<unsigned>& associated = net_list[net_idx]->cells();

        unsigned cell_idx, confirmed_true, confirmed_false, unconfirmed;
        for (cell_idx = confirmed_true = confirmed_false = unconfirmed = 0;
             cell_idx < associated.size(); ++cell_idx) {
            const unsigned name = associated[cell_idx];
            if (cell_inited.contains(name)) {
                const cell* cell = cell_map_[name];
                if (cell->side()) {
                    ++confirmed_true;
                } else {
                    ++confirmed_false;
                }
            } else {
                ++unconfirmed;
            }
        }

        assert(confirmed_true + confirmed_false + unconfirmed ==
                   associated.size() &&
               "Arithmetic error!");

        bool push_to_true = confirmed_true > confirmed_false;
        if (push_to_true && count_true + unconfirmed < too_much) {
            for (unsigned name : associated) {
                if (!cell_inited.contains(name)) {
                    cell_inited.insert(name);
                    cell* cell = cell_map_[name];
                    cell->side(true);
                    ++count_true;
                }
            }
        } else if (!push_to_true && count_false + unconfirmed < too_much) {
            for (unsigned name : associated) {
                if (!cell_inited.contains(name)) {
                    cell_inited.insert(name);
                    cell* cell = cell_map_[name];
                    cell->side(false);
                    ++count_false;
                }
            }
        } else {
            // count + unconfirmed >= too_much
            if (push_to_true) {
                assert(count_false + unconfirmed < too_much &&
                       "Need to rethink");
                for (unsigned name : associated) {
                    if (!cell_inited.contains(name)) {
                        cell_inited.insert(name);
                        cell* cell = cell_map_[name];
                        cell->side(false);
                        ++count_false;
                    }
                }
            } else {
                assert(count_true + unconfirmed < too_much &&
                       "Need to rethink");
                for (cell_idx = 0; cell_idx < associated.size(); ++cell_idx) {
                    const unsigned name = associated[cell_idx];
                    if (!cell_inited.contains(name)) {
                        cell_inited.insert(name);
                        cell* cell = cell_map_[name];
                        cell->side(true);
                        ++count_true;
                    }
                }
            }
        }
    }

    // Assertions
    assert(count_true + count_false == _cmap.size());
    assert(cell_inited.size() == _cmap.size());
    unsigned ctrue = 0, cfalse = 0;
    for (unsigned idx = 0; idx < cell_map_.size(); ++idx) {
        assert(cell_inited.contains(idx));
        if (cell_map_[idx]->side()) {
            ++ctrue;
        } else {
            ++cfalse;
        }
    }
    assert(ctrue == count_true && cfalse == count_false);
    assert(ctrue < too_much && cfalse < too_much && "Logical error");
    return count_true;
}

template <bool checking>
void floor_plan::cal_gains() {
    if (checking) {
#ifdef NDEBUG
        return;
#endif
        printf("Checking Nets and Cells\n");
    }

    vector<int> simulation = vector<int>(cell_map_.size(), 0);

    for (net* net : net_map_) {
        const vector<unsigned>& cells = net->cells();
        unsigned cnt = 0;
        for (unsigned cname : cells) {
            if (cell_map_[cname]->side()) {
                ++cnt;
            }
        }

        if (checking) {
            assert(net->true_count() == cnt && "Net count error");
        } else {
            net->setCount(cnt);
        }

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
                assert(count == 1 && "Update too much");
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
                assert(count == 1 && "Update too much");
            }
        }
    }

    assert(simulation.size() == _cmap.size());

    for (unsigned idx = 0; idx < simulation.size(); ++idx) {
        if (checking) {
            assert(_cmap[idx]->gain() == simulation[idx] &&
                   "Gain miscalculated");
        } else {
            cell_map_[idx]->gain(simulation[idx]);
        }
    }
}

void floor_plan::init_gains() {
    cal_gains<false>();
}

void floor_plan::check_gains() {
    cal_gains<true>();
}

void floor_plan::init_bucket() {
    bucket_ = bucket(cell_map_);
}
