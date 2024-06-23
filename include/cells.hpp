#pragma once

#include <vector>

class cell {
   public:
    cell();

    void flip();

    void side(bool s);
    bool side() const;

    int gain() const;
    void gain(int g);
    void inc_gain();
    void dec_gain();

    void push_net(unsigned net);
    const std::vector<unsigned>& nets() const;
    unsigned size() const;

   private:
    // using int to prevent padding issue
    int side_;
    int gain_;
    std::vector<unsigned> nets_;
};
