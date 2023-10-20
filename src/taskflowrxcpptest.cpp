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