#include "fm.hpp"
#include "init.hpp"

using namespace std;

// Since net_map_ and cell_map_ are shared, can't use `unique_ptr` here.
// https://stackoverflow.com/questions/73714773/no-matching-function-for-call-to-construct-at
FloorPlan::FloorPlan() : net_map_({}), cell_map_({}) {}

FloorPlan::FloorPlan(vector<shared_ptr<Net>>& n, vector<shared_ptr<Cell>>& c)
    : net_map_(n),
      cell_map_(c),
      bucket_(),
      balance_(0.),
      total_count_((unsigned)-1),
      tolerate_((unsigned)-1) {}

FloorPlan::FloorPlan(vector<shared_ptr<Net>>&& n,
                       vector<shared_ptr<Cell>>&& c)
    : net_map_(std::move(n)),
      cell_map_(std::move(c)),
      balance_(0.),
      total_count_((unsigned)-1),
      tolerate_((unsigned)-1) {
    bucket_ = Bucket(cell_map_);
}

void FloorPlan::nmap(vector<shared_ptr<Net>>&& net_map) {
    this->net_map_ = std::move(net_map);
}

vector<shared_ptr<Net>>& FloorPlan::nmap() {
    return net_map_;
}

const vector<shared_ptr<Net>>& FloorPlan::nmap() const {
    return net_map_;
}

void FloorPlan::cmap(vector<shared_ptr<Cell>>&& cmap) {
    this->cell_map_ = cmap;
}

vector<shared_ptr<Cell>>& FloorPlan::cmap() {
    return cell_map_;
}

const vector<shared_ptr<Cell>>& FloorPlan::cmap() const {
    return cell_map_;
}

double FloorPlan::balance() const {
    return balance_;
}

unsigned FloorPlan::tolerate() const {
    return tolerate_;
}
