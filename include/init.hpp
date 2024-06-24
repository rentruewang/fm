#pragma once
#include <vector>
#include "cells.hpp"
#include "fm.hpp"
#include "nets.hpp"

class floor_plan;

class init_strategy {
   public:
    init_strategy(const floor_plan& fp) : fp_(fp) {}
    virtual unsigned init(std::vector<cell*>& cells) const = 0;
    virtual ~init_strategy() {}

    const floor_plan& fp() const { return fp_; }

   protected:
    const floor_plan& fp_;
};

class naive_init final : public init_strategy {
   public:
    naive_init(const floor_plan& fp) : init_strategy(fp) {}
    unsigned init(std::vector<cell*>& cells) const override;
    ~naive_init() {}
};

class sophisticated_init final : public init_strategy {
   public:
    sophisticated_init(const floor_plan& fp) : init_strategy(fp) {}
    unsigned init(std::vector<cell*>& cells) const override;
    ~sophisticated_init() {}

   private:
    const std::vector<net*>& nets() const;
    unsigned tolerate() const;
};
