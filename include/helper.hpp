#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "buckets.hpp"
#include "cells.hpp"
#include "nets.hpp"

enum class modi { inc, dec };

struct gain_delta {
    int original;
    int updated;

    gain_delta() : gain_delta(0, 0) {}
    gain_delta(int o, int u) : original(o), updated(u) {}
};

template <modi mod>
void store_updates(std::vector<std::shared_ptr<cell>>& cmap,
                   std::unordered_map<unsigned, gain_delta>& records,
                   const unsigned name);

int flip_cell(std::vector<net*>& nmap,
              std::vector<cell*>& cmap,
              const unsigned cname,
              cell* cell,
              std::unordered_map<unsigned, gain_delta>& records);

int flip(std::vector<net*>& nmap,
         std::vector<cell*>& cmap,
         bucket& prev,
         bucket& next,
         const std::unordered_set<unsigned>& seen,
         const unsigned cname,
         cell* cell);
