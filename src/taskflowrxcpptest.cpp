#include <fmt/format.h>
#include <fmt/ostream.h>
#include <spdlog/spdlog.h>
#include <rxcpp/rx.hpp>
#include <taskflow/taskflow.hpp>
#include <date/date.h>
#include <catch2/catch.hpp>

#include <unordered_map>
#include <string>
#include <chrono>
#include <ctime>  

TEST_CASE("TaskFlowWithRX")
{
    tf::Executor executor;
    tf::Taskflow taskflow;

    std::unordered_map<std::string,std::tuple<size_t,std::vector<std::string>>> graph{
        {"Five",{5,{"Eleven"}}},
        {"Seven",{4,{"Eleven","Eight"}}},
        {"Three",{3,{"Eight","Ten"}}},
        {"Eleven",{11,{"Two","Ten"}}},
        {"Eight",{8,{"Nine"}}},
        {"Two",{2,{}}},
        {"Nine",{9,{}}},
        {"Ten",{10,{}}},
    };

    std::unordered_map<std::string, tf::Task> executionGraph;

    fmt::print("Setting Up Tasks on Thread {}\n",std::this_thread::get_id());

    for(const auto&[node, tp]: graph)
    {
        const auto&[value,deps] = tp;
        executionGraph[node] = taskflow.emplace([name = node,waittime = value](){ 
            auto currentTime = std::chrono::system_clock::now();
            auto secondsSinceEpoch = std::chrono::time_point_cast<std::chrono::seconds>(currentTime);
            auto currentEpochTime = secondsSinceEpoch.time_since_epoch().count();
            auto t = date::format("%F %T", date::sys_time<std::chrono::seconds>{std::chrono::seconds{currentEpochTime}});

            fmt::print("Running Task {} on Thread {}. Current time: {}\n",name,std::this_thread::get_id(),t);
            std::this_thread::sleep_for(std::chrono::seconds(waittime));
            }
        );
    }

    for(const auto&[node, tp]: graph)
    {
        const auto&[value,dependencies] = tp;
        auto& tsk = executionGraph[node];
        for(const auto& dependentNode: dependencies)
        {
            tsk.precede(executionGraph[dependentNode]);
        }
    }

    // auto [A, B, C, D] = taskflow.emplace(  // create four tasks
    //     [] () { std::cout << "TaskA\n"; },
    //     [] () { std::cout << "TaskB\n"; },
    //     [] () { std::cout << "TaskC\n"; },
    //     [] () { std::cout << "TaskD\n"; } 
    // );                                  
                                    
    // A.precede(B, C);  // A runs before B and C
    // D.succeed(B, C);  // D runs after  B and C
                                    
    executor.run(taskflow).wait(); 

    fmt::print("all work done on Thread {}\n",std::this_thread::get_id());

}

TEST_CASE("AddHocRxGraph")
{
    std::unordered_map<std::string,std::tuple<size_t,std::vector<std::string>>> graph{
        {"Five",{5,{"Eleven"}}},
        {"Seven",{4,{"Eleven","Eight"}}},
        {"Three",{3,{"Eight","Ten"}}},
        {"Eleven",{11,{"Two","Ten"}}},
        {"Eight",{8,{"Nine"}}},
        {"Two",{2,{}}},
        {"Nine",{9,{}}},
        {"Ten",{10,{}}},
    };

    std::unordered_map<std::string,rxcpp::observable<int>> executionGraph;

    //First create all the observables

    for(const auto&[node, tp]: graph)
    {
        const auto&[waittime,deps] = tp;
        auto observable = rxcpp::observable<>::create<int>(
            [waittime,node](const auto& s){
                fmt::print("Executin Observable {} on thread {}. speeling for {}\n",
                    node,std::this_thread::get_id(),waittime);
                std::this_thread::sleep_for(std::chrono::seconds(waittime));
                s.on_next(1);
                s.on_completed();
            }
        );
        executionGraph.emplace(node,std::move(observable));
    }

    auto base = rxcpp::observable<int>();
    for(const auto&[node, tp]: graph)
    {
        const auto&[waittime,deps] = tp;
        auto& obs = executionGraph[node];
        for(const auto& dependentNode: dependencies)
        {
            obs.
        }
    }

}

TEST_CASE("LaconchaDeTuHermana")
{
using namespace rxcpp;
using namespace rxcpp::sources;
using namespace rxcpp::util;
using namespace rxcpp::subjects;

    std::vector<observable<int>> observables = {
        observable<>::create<int>([](const subscriber<int>& s) {
            std::cout << "obs2 emitting values..." << std::endl;
            s.on_next(1);
            s.on_next(2);
            s.on_completed();
        }),
        observable<>::create<int>([](const subscriber<int>& s) {
            std::cout << "obs3 emitting values..." << std::endl;
            s.on_next(3);
            s.on_next(4);
            s.on_completed();
        }),
        observable<>::create<int>([](const subscriber<int>& s) {
            std::cout << "obs4 emitting values..." << std::endl;
            s.on_next(5);
            s.on_next(6);
            s.on_completed();
        })
    };

    // Create a vector of subscriptions to observables
    std::vector<subscription> subscriptions;
    for (auto& obs : observables) {
        subscriptions.push_back(obs.subscribe());
    }

    // Wait for all observables to complete
    auto waitForAll = rxcpp::observable<>::from(subscriptions).merge().last();

    // Observable obs1 emitting values
    auto obs1 = observable<>::create<int>([](const subscriber<int>& s) {
        std::cout << "obs1 emitting values..." << std::endl;
        s.on_next(7);
        s.on_next(8);
        s.on_completed();
    });

    // Merge the results of obs1 and the waitForAll observable
    auto finalObservable = waitForAll.merge(obs1);

    // Subscribe to the final observable
    finalObservable.subscribe(
        [](int value) {
            std::cout << "Received value: " << value << std::endl;
        },
        [](const std::exception_ptr& e) {
            try {
                std::rethrow_exception(e);
            } catch (const std::exception& ex) {
                std::cerr << "Error: " << ex.what() << std::endl;
            }
        },
        []() {
            std::cout << "Completed" << std::endl;
        });
}