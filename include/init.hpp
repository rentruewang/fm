#pragma once
#include <vector>

#include "cells.hpp"
#include "fm.hpp"
#include "nets.hpp"

class FloorPlan;

class Init {
   public:
    Init(const FloorPlan& fp) : fp_(fp) {}
    virtual unsigned init(std::vector<std::shared_ptr<Cell>>& cells) const = 0;
    virtual ~Init() {}

    const FloorPlan& fp() const { return fp_; }

   protected:
    const FloorPlan& fp_;
};

class NaiveInit final : public Init {
   public:
    NaiveInit(const FloorPlan& fp) : Init(fp) {}
    unsigned init(std::vector<std::shared_ptr<Cell>>& cells) const override;
    ~NaiveInit() {}
};

class SophInit final : public Init {
   public:
    SophInit(const FloorPlan& fp) : Init(fp) {}
    unsigned init(std::vector<std::shared_ptr<Cell>>& cells) const override;
    ~SophInit() {}

   private:
    const std::vector<std::shared_ptr<Net>>& nets() const;
    unsigned tolerate() const;
};
