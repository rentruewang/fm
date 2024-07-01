#include "fm.hpp"
#include "init.hpp"

using namespace std;

// Since net_map_ and cell_map_ are shared, can't use `unique_ptr` here.
// https://stackoverflow.com/questions/73714773/no-matching-function-for-call-to-construct-at
floor_plan::floor_plan() : net_map_({}), cell_map_({}) {}

floor_plan::floor_plan(vector<shared_ptr<net>>& n, vector<shared_ptr<cell>>& c)
    : net_map_(n),
      cell_map_(c),
      bucket_(),
      balance_(0.),
      total_count_((unsigned)-1),
      tolerate_((unsigned)-1) {}

floor_plan::floor_plan(vector<shared_ptr<net>>&& n,
                       vector<shared_ptr<cell>>&& c)
    : net_map_(std::move(n)),
      cell_map_(std::move(c)),
      balance_(0.),
      total_count_((unsigned)-1),
      tolerate_((unsigned)-1) {
    bucket_ = bucket(cell_map_);
}

void floor_plan::nmap(vector<shared_ptr<net>>&& net_map) {
    this->net_map_ = std::move(net_map);
}

vector<shared_ptr<net>>& floor_plan::nmap() {
    return net_map_;
}

const vector<shared_ptr<net>>& floor_plan::nmap() const {
    return net_map_;
}

void floor_plan::cmap(vector<shared_ptr<cell>>&& cmap) {
    this->cell_map_ = cmap;
}

vector<shared_ptr<cell>>& floor_plan::cmap() {
    return cell_map_;
}

const vector<shared_ptr<cell>>& floor_plan::cmap() const {
    return cell_map_;
}

double floor_plan::balance() const {
    return balance_;
}

unsigned floor_plan::tolerate() const {
    return tolerate_;
}
