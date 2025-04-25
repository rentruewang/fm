#pragma once

#include <vector>

class Cell {
   public:
    Cell();

    void flip();

    void side(bool s);
    bool side() const;

    int gain() const;
    void gain(int g);

    void push_net(unsigned net);
    const std::vector<unsigned>& nets() const;
    unsigned size() const;

    Cell& operator++() {
        inc_gain();
        return *this;
    }

    Cell& operator--() {
        dec_gain();
        return *this;
    }

   private:
    // using int to prevent padding issue
    bool side_;
    int gain_;
    std::vector<unsigned> nets_;

    void inc_gain();
    void dec_gain();
};
