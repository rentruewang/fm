#pragma once

#include <vector>

class Net {
   public:
    Net();

    void set_true_count(unsigned u);

    template <bool side>
    unsigned count() const;

    unsigned count(bool side) const;

    void inc_count(bool side);
    void dec_count(bool side);

    void push_cell(unsigned cell);
    const std::vector<unsigned>& cells() const;
    unsigned size() const;

   private:
    unsigned true_count_;
    std::vector<unsigned> cells_;

    unsigned true_count() const;
    unsigned false_count() const;
};
