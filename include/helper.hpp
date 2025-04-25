#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "buckets.hpp"
#include "cells.hpp"
#include "nets.hpp"

enum class ModKind { INC, DEC };

struct gain_delta {
    int original;
    int updated;

    gain_delta() : gain_delta(0, 0) {}
    gain_delta(int o, int u) : original(o), updated(u) {}
};

template <ModKind mod>
void store_updates(std::vector<std::shared_ptr<Cell>>& cmap,
                   std::unordered_map<unsigned, gain_delta>& records,
                   const unsigned name);

int flip_cell(std::vector<Net*>& nmap,
              std::vector<Cell*>& cmap,
              const unsigned cname,
              Cell* cell,
              std::unordered_map<unsigned, gain_delta>& records);

int flip(std::vector<Net*>& nmap,
         std::vector<Cell*>& cmap,
         Bucket& prev,
         Bucket& next,
         const std::unordered_set<unsigned>& seen,
         const unsigned cname,
         Cell* cell);
