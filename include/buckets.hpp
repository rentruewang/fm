#pragma once

#include <map>
#include <memory>
#include <unordered_set>

#include "cells.hpp"

using unsigned_set = std::unordered_set<unsigned>;

struct gain_delta;

class bucket {
   public:
    bucket();
    bucket(const std::vector<std::shared_ptr<cell>>& cmap);

    bucket& operator=(bucket&& b);

    const std::map<int, unsigned_set>& get() const;

    void push(const unsigned name, const cell& cell);
    unsigned pop();
    unsigned size() const;

    bool contains(unsigned name);
    void update(int old_gain, int new_gain, unsigned name);
    void update(gain_delta delta, unsigned name);
    void fill(const std::vector<std::shared_ptr<cell>>& cmap);
    void empty(bucket& other, std::unordered_set<unsigned>& seen);

   private:
    std::map<int, unsigned_set> bucket_;
};
