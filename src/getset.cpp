#include <concepts>
#include "fm.hpp"
#include "init.hpp"
using namespace std;

floor_plan::floor_plan() : floor_plan(vector<net*>(), vector<cell*>()) {}

floor_plan::floor_plan(vector<net*>& n, vector<cell*>& c)
    : net_map_(n),
      cell_map_(c),
      bucket_(),
      balance_(0.),
      total_count_((unsigned)-1),
      tolerate_((unsigned)-1) {}

floor_plan::floor_plan(vector<net*>&& n, vector<cell*>&& c)
    : net_map_(std::move(n)),
      cell_map_(std::move(c)),
      balance_(0.),
      total_count_((unsigned)-1),
      tolerate_((unsigned)-1) {
    bucket_ = std::move(bucket(cell_map_));
}

floor_plan::~floor_plan() {
    for (net* nptr : net_map_) {
        delete nptr;
    }

    for (cell* cptr : cell_map_) {
        delete cptr;
    }
}

void floor_plan::nmap(vector<net*>&& nmap) {
    this->net_map_ = nmap;
}

vector<net*>& floor_plan::nmap() {
    return net_map_;
}

const vector<net*>& floor_plan::nmap() const {
    return net_map_;
}

void floor_plan::cmap(vector<cell*>&& cmap) {
    this->cell_map_ = cmap;
}

vector<cell*>& floor_plan::cmap() {
    return cell_map_;
}

const vector<cell*>& floor_plan::cmap() const {
    return cell_map_;
}

double floor_plan::balance() const {
    return balance_;
}

unsigned floor_plan::tolerate() const {
    return tolerate_;
}
