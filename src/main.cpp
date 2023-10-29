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

namespace
{
    template <typename T>
    rxcpp::observable<std::shared_ptr<std::vector<T>>> zip_v(const std::vector<rxcpp::observable<T>>& observables) {
        // map the first observable to convert values to a single-element vector
        auto it = observables.cbegin();
        rxcpp::observable<std::shared_ptr<std::vector<T>>> acc = it->map([](T t) {
            return std::make_shared<std::vector<T>>(std::initializer_list<T>{ t });
        });

        // fold each observable into accumulator by zipping, and pushing back value
        while (++it != observables.cend()) {
            acc = acc.zip([](std::shared_ptr<std::vector<T>> acc, T next) { 
                acc->push_back(next);
                return acc;
            }, *it);
        }
        return acc;
    }

    template<typename ValueType, template <typename,typename> class MapT,template<typename> class ContainerT>
    using GraphImpl = MapT<ValueType,ContainerT<ValueType>;

    using NodeValuetype = std::string;

    using Graph = GraphImpl<NodeValuetype,std::unordered_map,std::vector>;

    template<template<typename> class ContainerT>
    using GraphLevelsImpl = ContainerT<ContainerT<NodeValuetype>>;

    using GraphLevels = GraphLevelsImpl<std::vector>;

    using ExecutionGraph=std::unordered_map<NodeValuetype,rxcpp::observable<NodeValuetype>>;

    auto makeDagExecutionObservable(
        const Graph& g,
        const GraphLevels& levels,
        std::function<void(NodeValuetype,std::vector<NodeValuetype>)> f)
    {
        ExecutionGraph execGraph;

        for(const auto& level: levels)
        {
            for(const auto& node: level)
            {
                // Construct source obserbable in the map.
                execGraph.emplace(
                    node,
                    rxcpp::obserbable<>::create<NodeValuetype>(
                        [f,node,deps = g[node]](const subscriber<NodeValuetype>&){
                            f(node,deps);
                            s.on_next(node);
                            s.on_completed();
                        }
                    )
                );
                // Zip it to aal its neighbours 
            }   
        }

    }

}

TEST_CASE("DAGExecutionWithRx")
{
    /*
    https://dreampuf.github.io/GraphvizOnline
    digraph G {
    a -> c;
    b -> c;
    a -> e;
    d -> e;
    c -> e;
    c -> f;
    f -> g;
    e -> g;
    }
    */
}
