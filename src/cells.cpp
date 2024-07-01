#include "cells.hpp"

using namespace std;

cell::cell() : side_(false), gain_(0), nets_(vector<unsigned>()) {}

void cell::flip() {
    side_ = !side_;
}

void cell::side(bool s) {
    side_ = s;
}

bool cell::side() const {
    return side_;
}

int cell::gain() const {
    return gain_;
}

void cell::gain(int g) {
    gain_ = g;
}

void cell::inc_gain() {
    ++gain_;
}

void cell::dec_gain() {
    --gain_;
}

void cell::push_net(unsigned net) {
    nets_.push_back(net);
}

const vector<unsigned>& cell::nets() const {
    return nets_;
}

unsigned cell::size() const {
    return nets_.size();
}
