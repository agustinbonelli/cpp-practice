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
#include <range/v3/view.hpp>
#include "pretty_print.hpp"
#include <iostream>

// This was another possibility I explored before realizing that ranges had a chunk view
template<typename RNG>
auto split_in_chunks_of(RNG&& r,int k=1)
{
    return ranges::views::split_when(r,
        [k,counter=0](auto b,auto s) mutable -> std::pair<bool, decltype(b)>
        {
            return { (counter++ % k) == 0, b};
        }
    );
}

TEST_CASE("chunk_and_zip","InterlaceTest")
{
    using namespace ranges;

    std::vector<int> v1(10);
    std::vector<int> v2(10);
    std::generate(v1.begin(),v1.end(),[val=1]()mutable{return val++;});
    std::generate(v2.begin(),v2.end(),[val=0]()mutable{return val+=10,val;});
    int idx=0;
    std::vector<int> result;
    std::ostringstream ss;
    ss << v1;
    spdlog::info("v1 = {}",ss.str());
    ss.str("");
    ss.clear();
    ss << v2;
    spdlog::info("v2 = {}",ss.str());

    
    for(auto [rng1,rng2]: views::zip(v1 | views::chunk(2),v2 | views::chunk(2)))
    {
        result.insert(result.end(),rng1.begin(),rng1.end());
        result.insert(result.end(),rng2.begin(),rng2.end());
    }
    
    ss.str("");
    ss.clear();
    ss << result;
    spdlog::info("Interleaved result = {}",ss.str());
    
}