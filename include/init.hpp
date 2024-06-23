#pragma once
#include <vector>
#include "cells.hpp"
#include "nets.hpp"

class init_strategy {
   public:
    virtual unsigned init(std::vector<cell*>& cells) const = 0;
    virtual ~init_strategy() {}
};

class naive_init final : public init_strategy {
   public:
    naive_init() {}
    unsigned init(std::vector<cell*>& cells) const override;
    ~naive_init() {}
};

class sophisticated_init final : public init_strategy {
   public:
    sophisticated_init(const std::vector<net*>& nets, unsigned tolerate)
        : nets_(nets), tolerate_(tolerate) {}
    unsigned init(std::vector<cell*>& cells) const override;
    ~sophisticated_init() {}

   private:
    const std::vector<net*>& nets_;
    unsigned tolerate_;
};
