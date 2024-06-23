target("fm") add_rules("mode.debug", "mode.release") set_languages("c++20")
    set_optimize("fastest") set_kind("binary") add_includedirs("include")
        add_files("src/*.cpp")
