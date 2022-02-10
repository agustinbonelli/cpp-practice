// interlace({1,2,3,4,5}, {10,20,30,40,50}) => {1,10,2,20,3,30,4,40,5,50}
// interlace({1,2,3,4,5}, {10,20,30,40,50}, 2) => {1,2,10,20,3,4,30,40,5,50}


/*

I was aksed to implement this as an exercise in an interview. I though about using
boost ranges or something, but interviewr suggested I do it manually.
This is my attempt to practice C++ and creating a bit of ranges code to achieve this elegantly
and more generically.

Write function
template<typename R1, typename R2>
auto partition(R1 rng1, std::size_t hop)

Which takes a range rng1, and returns a range of ranges
with each range having hop elements of the original rng1, except maybe the last which could have less.

Then

template<typename R1, typename R2>
auto interlace(R1 rng1, R2 rng2,std::size_t hop1, size_t hop2);

Which creates another range interlacing hop1 values of rng1 with hop2 values of rng2

Using the already provided partition.
*/

#include <catch2/catch.hpp>
#include <spdlog/spdlog.h>
#include <boost/range/adaptor/sliced.hpp>
using namespace boost::adaptors;

template<typename R1>
struct Partitioned
{
    Partitioned(R1& r,std::size_t step) rng(r),m_step(step) {}

    template<typename RANGE>
    struct iterator
    {
        RANGE& rng;
    }

    iterator& begin(){}

    iterator& end(){}

    R1& rng;
    std::size_t m_step
}


template<typename R1>
auto partition(R1 rng1, std::size_t hop)
{
    return Partitioned<R1>(rng1,hop);
}

TEST_CASE("ItWorks","PartitionTest")
{
    std::vector<int> v{1,2,3,4,5,6,7,8,9};
    int count=0;
    for(auto p = partition(v,3))
    {
        spdlog::info("Printing range {}",count++);
        for(auto item : p)
        {
            spdlog::info(item);
        }
    }
}