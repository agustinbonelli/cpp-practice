#include <boost/current_function.hpp>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include <exception>

struct A
{
    A() {
        spdlog::info(BOOST_CURRENT_FUNCTION);
    }
    ~A()
    {
        // Without this try catch block. even with only 1 exception in flight
        // std::terminate will be called. Gcc will warn if -Wterminate
        try{
            throw std::runtime_error("Pepito");
        }
        catch (std::exception& e)
        {
            spdlog::info(fmt::format("Caught Exception in {}. Everything is fine. Error = {}",BOOST_CURRENT_FUNCTION,e.what()));
        }
    }

    void foo()
    {
        spdlog::info(BOOST_CURRENT_FUNCTION);
    }
};

void test_exceptions_in_destructors()
{
    spdlog::info(fmt::format("Current function: {}",BOOST_CURRENT_FUNCTION));
    try
    {
        A mya;
        mya.foo();
    }
    catch (std::exception& e)
    {
        spdlog::info(fmt::format("Caught Exception. Everything is fine. Error = {}",e.what()));
    }
    spdlog::info("It all went well");
}