#include <catch2/catch.hpp>
#include "shared_ptr.hpp"
#include <thread>
#include <functional>

TEST_CASE("shared_ptr_test")
{
    agb::shared_ptr<int> p(new int(5));
    spdlog::info("Count value 1 = {}",p.count());
    spdlog::info("Pointer value = {}",*p);

    std::thread([p](){
        spdlog::info("Count value in thread = {}",p.count());
    }).detach();

    spdlog::info("Count value 2 = {}",p.count());

    agb::shared_ptr<int> p2 = p;

    spdlog::info("Count value 3 = {}",p.count());

    spdlog::info("Pointer value = {}",*p2);

}