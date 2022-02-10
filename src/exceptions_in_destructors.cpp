#include <catch2/catch.hpp>

#include <boost/current_function.hpp>
#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <exception>

struct A
{
    A(bool c) : m_catch_ex(c){
        spdlog::info(BOOST_CURRENT_FUNCTION);
    }
    ~A()
    {
        // Without this try catch block. even with only 1 exception in flight
        // std::terminate will be called. Gcc will warn if -Wterminate
        if(m_catch_ex)
        {
            try{
                throw std::runtime_error("Pepito");
            }
            catch (std::exception& e)
            {
                spdlog::info(fmt::format("Caught Exception in {}. Everything is fine. Error = {}",BOOST_CURRENT_FUNCTION,e.what()));
            }
        }
        else
        {
            throw std::runtime_error("Pepito");
        }
    }

    void foo()
    {
        spdlog::info(BOOST_CURRENT_FUNCTION);
    }
    bool m_catch_ex{true};
};

void test_exceptions_in_destructors(bool m_catch_ex)
{
    spdlog::info(fmt::format("Current function: {}",BOOST_CURRENT_FUNCTION));
    try
    {
        A mya(m_catch_ex);
        mya.foo();
    }
    catch (std::exception& e)
    {
        spdlog::info(fmt::format("Caught Exception. Everything is fine. Error = {}",e.what()));
    }
    spdlog::info("It all went well");
}

TEST_CASE("Catching_in_destructor_does_not_terminate","Exceptions_in_destructors")
{
    REQUIRE_NOTHROW(test_exceptions_in_destructors(true));
}

TEST_CASE("Not_Catching_in_destructor_calls_terminate","Exceptions_in_destructors")
{
    REQUIRE_NOTHROW(test_exceptions_in_destructors(false));
}