#include <boost/current_function.hpp>
#include <nlohmann/json.hpp>
#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <rxcpp/rx.hpp>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

const auto errorHandler = [](const std::exception_ptr& e) {
            try {
                std::rethrow_exception(e);
            } catch (const std::exception& ex) {
                std::cerr << "Error: " << ex.what() << std::endl;
            }
        };
    
const auto completedHandler = []() {
            std::cout << "Completed" << std::endl;
        };

TEST_CASE("RxCPP")
{
    // Create an observable that emits values every 1 second
    auto values = rxcpp::observable<>::interval(std::chrono::seconds(1))
                      .map([](auto) { return "Tick"; });

    // Subscribe to the observable
    auto subscription = values.subscribe(
        [](const std::string& value) {
            std::cout << value << std::endl;
        },
        errorHandler,
        completedHandler);
}

TEST_CASE("EmitedbyLambda")
{

    auto numbers = rxcpp::observable<>::range(1,10);

    auto timer = rxcpp::observable<>::interval(std::chrono::seconds(1));

    auto values = timer.zip( numbers);

    auto subscription = values.subscribe(
        [](const auto& value) {
            const auto& [idx,val] = value;
            fmt::print("Idx {} : value {}\n",idx,val);
        },
        completedHandler);
}

