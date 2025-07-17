#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "buckets.hpp"
#include "cells.hpp"
#include "nets.hpp"

class init_strategy;

class floorplan {
   public:
    floorplan();
    floorplan(std::vector<std::shared_ptr<net>>& nmap,
              std::vector<std::shared_ptr<cell>>& cmap);
    floorplan(std::vector<std::shared_ptr<net>>&& nmap,
              std::vector<std::shared_ptr<cell>>&& cmap);

    void fm();

    void nmap(std::vector<std::shared_ptr<net>>&& nmap);
    std::vector<std::shared_ptr<net>>& nmap();
    const std::vector<std::shared_ptr<net>>& nmap() const;

    void cmap(std::vector<std::shared_ptr<cell>>&& cmap);
    std::vector<std::shared_ptr<cell>>& cmap();
    const std::vector<std::shared_ptr<cell>>& cmap() const;

    double balance() const;

    void init_side(const init_strategy& init);
    void sort_net_list();
    unsigned tolerate() const;

    floorplan& operator<<(std::string fname);
    floorplan& operator<<(unsigned tolerate);
    floorplan& operator>>(std::string fname);

   private:
    std::vector<std::shared_ptr<net>> net_map_;
    std::vector<std::shared_ptr<cell>> cell_map_;
    std::vector<std::string> net_names_;
    std::vector<std::string> cell_names_;
    bucket bucket_;

    double balance_;
    unsigned total_count_;
    unsigned tolerate_;

    void input(std::string fname);
    void output(std::string name);

    void init_gains();

    void init_bucket();

    int fm_once(std::function<bool(const unsigned)> condition);

    int flip(bucket& nbucket,
             const std::unordered_set<unsigned>& seen,
             unsigned cname);
};
