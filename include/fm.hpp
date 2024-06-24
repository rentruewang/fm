#pragma once

#include <functional>
#include <string>
#include <unordered_set>
#include <vector>
#include "buckets.hpp"
#include "cells.hpp"
#include "nets.hpp"

class init_strategy;

class floor_plan {
   public:
    floor_plan();
    floor_plan(std::vector<net*>& nmap, std::vector<cell*>& cmap);
    floor_plan(std::vector<net*>&& nmap, std::vector<cell*>&& cmap);
    ~floor_plan();

    void fm();

    void nmap(std::vector<net*>&& nmap);
    std::vector<net*>& nmap();
    const std::vector<net*>& nmap() const;

    void cmap(std::vector<cell*>&& cmap);
    std::vector<cell*>& cmap();
    const std::vector<cell*>& cmap() const;

    double balance() const;

    void init_side(const init_strategy& init);
    void sort_net_list();
    unsigned tolerate() const;

    floor_plan& operator<<(std::string fname);
    floor_plan& operator<<(unsigned tolerate);
    floor_plan& operator>>(std::string fname);

   private:
    std::vector<net*> net_map_;
    std::vector<cell*> cell_map_;
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
