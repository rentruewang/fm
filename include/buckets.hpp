#pragma once

#include <map>
#include <unordered_set>
#include "cells.hpp"

class list {
   public:
    list();
    const std::unordered_set<unsigned>& get() const;
    unsigned size() const;
    void push(unsigned name);
    unsigned pop();
    bool contains(const unsigned name) const;
    void erase(const unsigned name);

   private:
    std::unordered_set<unsigned> cells_;
};

class bucket {
   public:
    bucket();
    bucket(const std::vector<cell*>& cmap);

    bucket& operator=(bucket&& b);

    const std::map<int, list>& get() const;

    void push(const unsigned name, const cell* cell);
    unsigned pop();
    unsigned size() const;

    bool contains(unsigned name);
    void update(int old_gain, int new_gain, unsigned name);
    void fill(const std::vector<cell*>& cmap);
    void empty(bucket& other, std::unordered_set<unsigned>& seen);

   private:
    std::map<int, list> bucket_;
};
