#pragma once

#include <functional>
#include <string>
#include <unordered_set>
#include <vector>
#include "buckets.hpp"
#include "cells.hpp"
#include "nets.hpp"

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

    void input(const std::string fname);
    void output(const std::string name);

    double balance() const;

    template <bool soph>
    void init_side();

    void tolerate(unsigned amount);

   private:
    std::vector<net*> net_map_;
    std::vector<cell*> cell_map_;
    std::vector<std::string> net_names_;
    std::vector<std::string> cell_names_;
    bucket bucket_;
    double balance_;
    unsigned total_count_;
    unsigned tolerate_;

    unsigned naiv_init_side();
    unsigned soph_init_side();

    void init_gains();
    void check_gains();
    void init_bucket();

    template <bool check>
    void cal_gains();

    int fm_once(std::function<bool(const unsigned)> condition);

    int flip(bucket& nbucket,
             const std::unordered_set<unsigned>& seen,
             unsigned cname);
};

template <bool soph>
void floor_plan::init_side() {
    if (soph) {
        total_count_ = soph_init_side();
    } else {
        total_count_ = naiv_init_side();
    }
}
