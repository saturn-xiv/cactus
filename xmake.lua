add_rules("mode.debug", "mode.release")
set_version("2026.3.25", {build = "%a %b %d %I:%M:%S %p %Z %Y"})

add_requires("argparse", "toml++", "spdlog", "cpp-httplib", "cppcodec", "stduuid", "cryptopp", "cpr", "nlohmann_json")

target("cactus")
    set_languages("c++23")
    set_kind("binary")
    add_ldflags("-static -static-libgcc -static-libstdc++")
    set_configdir("$(builddir)/$(plat)/$(arch)/$(mode)")
    add_configfiles("include/cactus/version.h.in", {filename = "include/cactus/version.hpp"})
    add_files("src/*.cpp")
    add_includedirs("include", "$(builddir)/$(plat)/$(arch)/$(mode)/include")    
    add_packages("argparse", "toml++", "spdlog", "cpp-httplib", "cppcodec", "stduuid", "cryptopp", "cpr", "nlohmann_json")
