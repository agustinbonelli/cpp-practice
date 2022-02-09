#include <boost/current_function.hpp>
#include <nlohmann/json.hpp>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include "exceptions_in_destructors.hpp"

int main()
{
    test_exceptions_in_destructors();
    return 0;
}