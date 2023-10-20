#include <fmt/format.h>
#include <fmt/ostream.h>
#include <spdlog/spdlog.h>
#include <rxcpp/rx.hpp>
#include <taskflow/taskflow.hpp>

#include <catch2/catch.hpp>

TEST_CASE("TaskFlowWithRX")
{
    tf::Executor executor;
    tf::Taskflow taskflow;

    auto [A, B, C, D] = taskflow.emplace(  // create four tasks
        [] () { std::cout << "TaskA\n"; },
        [] () { std::cout << "TaskB\n"; },
        [] () { std::cout << "TaskC\n"; },
        [] () { std::cout << "TaskD\n"; } 
    );                                  
                                    
    A.precede(B, C);  // A runs before B and C
    D.succeed(B, C);  // D runs after  B and C
                                    
    executor.run(taskflow).wait(); 

}