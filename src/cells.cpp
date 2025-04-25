#include "cells.hpp"

using namespace std;

Cell::Cell() : side_(false), gain_(0), nets_(vector<unsigned>()) {}

void Cell::flip() {
    side_ = !side_;
}

void Cell::side(bool s) {
    side_ = s;
}

bool Cell::side() const {
    return side_;
}

int Cell::gain() const {
    return gain_;
}

void Cell::gain(int g) {
    gain_ = g;
}

void Cell::inc_gain() {
    ++gain_;
}

void Cell::dec_gain() {
    --gain_;
}

void Cell::push_net(unsigned net) {
    nets_.push_back(net);
}

const vector<unsigned>& Cell::nets() const {
    return nets_;
}

unsigned Cell::size() const {
    return nets_.size();
}
