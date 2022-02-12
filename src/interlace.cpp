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

#include <boost/range/combine.hpp>

#include <iostream>

// Ranges_v3 split_when
// int counter{0};

// struct chunk_of
// {
//     chunk_of(int size = 1) : chunk_size(size){}

//     template<typename I, typename S>
//     std::pair<bool, I> operator()(I b, S) const
//     {
//         return { (counter++ % chunk_size) == 0, b};
//     }
//     int chunk_size;
//     int& counter;
// };

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

std::ostream& operator<<(std::ostream& os,std::vector<int> v)
{
    os << "{";
    for(auto item: v) { os << item << ',';}
    os << "}";
    return os;
}

TEST_CASE("ItWorks","PartitionTest")
{
    std::vector<int> v(20);
    std::vector<int> v2(20);
    std::iota(v.begin(),v.end(),0);
    std::iota(v2.begin(),v2.end(),50);
    int idx=0;

    std::vector<int> result;

    int k = 2;
    auto chunks_1 = split_in_chunks_of(v,k);
    auto chunks_2 = split_in_chunks_of(v2,k);
    auto itx = chunks_1.begin();
    auto ity = chunks_2.begin();
    for(; itx != chunks_1.end() && ity != chunks_2.end(); ++itx, ++ity )
    {
        std::copy((*itx).begin(),(*itx).end(),std::back_inserter(result));
        std::copy((*ity).begin(),(*ity).end(),std::back_inserter(result));
    }
    for(;itx != chunks_1.end(); ++itx)
    {
        std::copy((*itx).begin(),(*itx).end(),std::back_inserter(result));
    }
    for(;ity != chunks_2.end(); ++ity)
    {
        std::copy((*ity).begin(),(*ity).end(),std::back_inserter(result));
    }
    
    // for(auto p : split_in_chunks_of(v,2))
    // {
    //     spdlog::info("Printing range {}",idx++);
    //     for(auto item : p)
    //     {
    //         spdlog::info(item);
    //     }
    // }

    std::cout << result;
}