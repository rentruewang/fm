#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

#include "buckets.hpp"
#include "cells.hpp"
#include "nets.hpp"

class Init;

class FloorPlan {
   public:
    FloorPlan();
    FloorPlan(std::vector<std::shared_ptr<Net>>& nmap,
               std::vector<std::shared_ptr<Cell>>& cmap);
    FloorPlan(std::vector<std::shared_ptr<Net>>&& nmap,
               std::vector<std::shared_ptr<Cell>>&& cmap);

    void fm();

    void nmap(std::vector<std::shared_ptr<Net>>&& nmap);
    std::vector<std::shared_ptr<Net>>& nmap();
    const std::vector<std::shared_ptr<Net>>& nmap() const;

    void cmap(std::vector<std::shared_ptr<Cell>>&& cmap);
    std::vector<std::shared_ptr<Cell>>& cmap();
    const std::vector<std::shared_ptr<Cell>>& cmap() const;

    double balance() const;

    void init_side(const Init& init);
    void sort_net_list();
    unsigned tolerate() const;

    FloorPlan& operator<<(std::string fname);
    FloorPlan& operator<<(unsigned tolerate);
    FloorPlan& operator>>(std::string fname);

   private:
    std::vector<std::shared_ptr<Net>> net_map_;
    std::vector<std::shared_ptr<Cell>> cell_map_;
    std::vector<std::string> net_names_;
    std::vector<std::string> cell_names_;
    Bucket bucket_;

    double balance_;
    unsigned total_count_;
    unsigned tolerate_;

    void input(std::string fname);
    void output(std::string name);

    void init_gains();

    void init_bucket();

    int fm_once(std::function<bool(const unsigned)> condition);

    int flip(Bucket& nbucket,
             const std::unordered_set<unsigned>& seen,
             unsigned cname);
};
