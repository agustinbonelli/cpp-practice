#include <boost/current_function.hpp>
#include <nlohmann/json.hpp>
#include <fmt/format.h>
#include <fmt/ostream.h>
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

TEST_CASE("ConcatMap")
{
    fmt::print("Main function On thread id {}\n",std::this_thread::get_id());
    auto numbers = rxcpp::observable<>::range(0,10).concat_map(
        [](int v){
            std::vector<int> vec;
            vec.reserve(v);
            while(v > 0)
            {
                vec.push_back(v--);
            };
            fmt::print("Make Observable vector on thread id {}\n",std::this_thread::get_id());
            return rxcpp::observable<>::iterate(vec);
        },
        [](int a, int b){
            fmt::print("Make tuple ({},{}) on thread id {}\n",a,b,std::this_thread::get_id());
            return std::make_tuple(a,b);
        },
        rxcpp::observe_on_new_thread()
    );

    numbers.as_blocking().subscribe([](const auto& t){
        auto [a,b] = t;
        fmt::print("On id {} --> {} : {}\n",std::this_thread::get_id(),a,b);
    },
    errorHandler,
    completedHandler);
}