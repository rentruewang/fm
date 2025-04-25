#include <fstream>
#include <iostream>
#include <istream>
#include <ostream>
#include <sstream>
#include <unordered_set>

#include "fm.hpp"

using namespace std;

FloorPlan& FloorPlan::operator<<(string fname) {
    input(fname);
    return *this;
}

FloorPlan& FloorPlan::operator<<(unsigned tolerate) {
    tolerate_ = tolerate;
    return *this;
}

FloorPlan& FloorPlan::operator>>(string fname) {
    output(fname);
    return *this;
}

void FloorPlan::input(string fname) {
    unordered_map<string, unordered_set<string>> nmap_set, cmap_set;

    auto file = ifstream{fname};

    file >> balance_;

    string buffer;
    while (file >> buffer) {
        if (buffer != "NET") {
            break;
        }

        string nname;
        file >> nname;

        nmap_set[nname] = unordered_set<string>();

        unordered_set<string>& current_net = nmap_set[nname];

        string cname;
        while (file >> cname) {
            if (cname == ";") {
                break;
            }

            if (!cmap_set.contains(cname)) {
                cmap_set[cname] = unordered_set<string>();
            }

            current_net.insert(cname);

            unordered_set<string>& current_cell = cmap_set[cname];
            current_cell.insert(nname);
        }
    }

    unordered_map<string, unsigned> rev_nnames, rev_cnames;

    unsigned nsize = nmap_set.size();
    unsigned csize = cmap_set.size();

    net_map_.reserve(nsize);
    cell_map_.reserve(csize);

    net_names_.reserve(nsize);
    cell_names_.reserve(csize);

    for (unsigned idx = 0; idx < nsize; ++idx) {
        net_map_.push_back(make_shared<Net>());
    }
    for (unsigned idx = 0; idx < csize; ++idx) {
        cell_map_.push_back(make_shared<Cell>());
    }

    for (auto iter : nmap_set) {
        const string name = iter.first;
        if (!rev_nnames.contains(name)) {
            rev_nnames[name] = net_names_.size();
            net_names_.push_back(name);
        }
    }

    for (auto iter : cmap_set) {
        const string name = iter.first;
        if (!rev_cnames.contains(name)) {
            rev_cnames[name] = cell_names_.size();
            cell_names_.push_back(name);
        }
    }

    for (auto iter : nmap_set) {
        const string nname = iter.first;
        const auto& cell_list = iter.second;
        const unsigned net_id = rev_nnames[nname];

        auto net = net_map_[net_id];

        for (string cname : cell_list) {
            const unsigned cell_id = rev_cnames[cname];

            net->push_cell(cell_id);

            auto cell = cell_map_[cell_id];
            cell->push_net(net_id);
        }
    }

    tolerate_ = static_cast<unsigned>(balance_ * csize);
}

void FloorPlan::output(string fname) {
    stringstream ss;
    auto file = ofstream(fname);

    unsigned cut_size = 0;
    for (unsigned idx = 0; idx < net_map_.size(); ++idx) {
        const auto n = net_map_[idx];
        cut_size += static_cast<int>(n->count<true>() && n->count<false>());
    }

    ss << "Cutsize = " << cut_size << "\n";
    file << ss.str();

    stringstream true_ss, false_ss;
    unsigned true_count = 0, false_count = 0;

    true_ss << "\n";
    false_ss << "\n";

    for (unsigned idx = 0; idx < cell_map_.size(); ++idx) {
        const string name = cell_names_[idx];
        const auto cell = cell_map_[idx];
        if (cell->side()) {
            ++true_count;
            true_ss << name << " ";
        } else {
            ++false_count;
            false_ss << name << " ";
        }
    }

    true_ss << ";\n";
    false_ss << ";\n";

    file << "G1 " << true_count << true_ss.str();
    file << "G2 " << false_count << false_ss.str();
}
