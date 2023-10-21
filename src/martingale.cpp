#include <fmt/format.h>
#include <fmt/ostream.h>
#include <spdlog/spdlog.h>
#include <catch2/catch.hpp>

#include <unordered_map>
#include <string>
#include <chrono>
#include <ctime>  

template<typename ResultType = double, template<typename> class DistributionType>
class MartingaleGenerator{
public:
    MartingaleGenerator(ResultType start_value) : m_state(start_value){}

    ResultType operator()
    {
        
    }

private:
    ResultType m_state;
}

TEST_CASE("MartingaleGenerator")
{

}