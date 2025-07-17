#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "buckets.hpp"
#include "cells.hpp"
#include "nets.hpp"

enum class mod_kind { INC, DEC };

struct gain_delta {
    int original;
    int updated;

    gain_delta(int o = 0, int u = 0) : original(o), updated(u) {}
};

template <mod_kind mod>
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
