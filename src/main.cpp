#include <cassert>
#include <iostream>
#include <string>
#include <unordered_map>
#include "cells.hpp"
#include "fm.hpp"
#include "nets.hpp"

int main(int argc, char const* argv[]) {
    using namespace std;

    assert(argc == 3);

    floor_plan fp;

    fp.input(string(argv[1]));

    unsigned nsize = fp.nmap().size();
    unsigned csize = fp.cmap().size();
    double bal = fp.balance();

    unsigned tolerate = static_cast<unsigned>(bal * csize);
    fp.tolerate(tolerate);
    fp.init_side();
    fp.fm();
    fp.output(string(argv[2]));

    return 0;
}
