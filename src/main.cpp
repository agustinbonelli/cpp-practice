#include <boost/current_function.hpp>
#include <nlohmann/json.hpp>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

int main()
{
    spdlog::info(fmt::format("Current function: {}",BOOST_CURRENT_FUNCTION));
    return 0;
}