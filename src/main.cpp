#include <cassert>
#include <string>

#include "fm.hpp"
#include "init.hpp"

int main(int argc, char const* argv[]) {
    assert(argc == 3);

    FloorPlan fp;

    fp << argv[1];

    Init* strat;

    auto soph_init{SophInit{fp}};
    auto naiv_init{NaiveInit{fp}};

    strat = &soph_init;
    strat = &naiv_init;

    fp.init_side(*strat);
    fp.fm();

    fp >> argv[2];

    return 0;
}
