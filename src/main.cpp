#include <boost/current_function.hpp>
#include <nlohmann/json.hpp>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

struct Foo
{
    Foo(){ spdlog::info(BOOST_CURRENT_FUNCTION);}
    void* operator new(std::size_t s)
    {
        spdlog::info(BOOST_CURRENT_FUNCTION);
        return ::operator new(s);
    }
};

TEST_CASE("OperatorNew")
{
    Foo * f = new Foo();
    delete f;
}