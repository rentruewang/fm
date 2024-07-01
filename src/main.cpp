#include <cassert>
#include <string>

#include "fm.hpp"
#include "init.hpp"

int main(int argc, char const* argv[]) {
    assert(argc == 3);

    floor_plan fp;

    fp << argv[1];

    init_strategy* strat;

    auto soph_init{sophisticated_init{fp}};
    auto naiv_init{naive_init{fp}};

    strat = &soph_init;
    strat = &naiv_init;

    fp.init_side(*strat);
    fp.fm();

    fp >> argv[2];

    return 0;
}
