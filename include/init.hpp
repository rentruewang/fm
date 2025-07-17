#pragma once
#include <vector>

#include "cells.hpp"
#include "fm.hpp"
#include "nets.hpp"

class floorplan;

class init_strategy {
   public:
    init_strategy(const floorplan& fp) : fp_(fp) {}
    virtual unsigned init(std::vector<std::shared_ptr<cell>>& cells) const = 0;
    virtual ~init_strategy() {}

    const floorplan& fp() const { return fp_; }

   protected:
    const floorplan& fp_;
};

class naive_init final : public init_strategy {
   public:
    naive_init(const floorplan& fp) : init_strategy(fp) {}
    unsigned init(std::vector<std::shared_ptr<cell>>& cells) const override;
    ~naive_init() {}
};

class sophisticated_init final : public init_strategy {
   public:
    sophisticated_init(const floorplan& fp) : init_strategy(fp) {}
    unsigned init(std::vector<std::shared_ptr<cell>>& cells) const override;
    ~sophisticated_init() {}

   private:
    const std::vector<std::shared_ptr<net>>& nets() const;
    unsigned tolerate() const;
};
