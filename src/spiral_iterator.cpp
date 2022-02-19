#include <catch2/catch.hpp>
#include <spdlog/spdlog.h>
#include <boost/current_function.hpp>
#include "pretty_print.hpp"
#include <sstream>

using T = int;

/*

[
    [ 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 ]
    [ 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 ]
    [ 1 , 0 , 2 , 2 , 2 , 2 , 2 , 2 , 0 , 1 ]
    [ 1 , 0 , 2 , 3 , 3 , 3 , 3 , 2 , 0 , 1 ]
    [ 1 , 0 , 2 , 3 , 3 , 3 , 3 , 2 , 0 , 1 ]
    [ 1 , 0 , 2 , 2 , 2 , 2 , 2 , 2 , 0 , 1 ]
    [ 1 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 1 ]
    [ 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 ]
]

This rotine will print the sub matrix with corners at 
top_left    = (row          , col               )
top_right   = (row          , m[row].size()-col )   // non incluse. m[row].size()-col marks one past the end for the row iteration.
bottom_left = (m.size()-row , col               )   // non incluse. m.size()-row makrs one past the end for iteration over column.
bottom_right= (m.size()-row , m[row].size()-col )


*/


std::vector<T> output_border_of_submatrix(const std::vector<std::vector<T>>& m, int row, int col,int& counter)
{
    std::vector<T> res;
    int col_idx;
    int row_idx;
    // top 
    for( col_idx = col; col_idx < m[row].size()- col; ++col_idx )
    {
        res.push_back(m[row][col_idx]);
        counter--;
    }
    col_idx--;
    // row + 1  start because the top right orner was done by previous loop, last value
    for( row_idx = row + 1; row_idx < m.size() - row; ++row_idx)
    {
        res.push_back(m[row_idx][col_idx]);
        counter--;
    }
    row_idx--;
    // col_idx-- because bottom_right corner was done by previous loop.
    for(col_idx--; col_idx >= col; --col_idx)
    {
        res.push_back(m[row_idx][col_idx]);
        counter--;
    }
    col_idx++;

    // here condition to end going up is that it should be strictly greater because the top left value was printed by first loop.
    for(row_idx--; row_idx > row; --row_idx)
    {
        res.push_back(m[row_idx][col_idx]);
        counter--;
    }
    return res;
}

// template<typename T>
std::vector<T> spiral_output(const std::vector<std::vector<T>>& matrix)
{
    int total_size = matrix.size() * matrix[0].size();
    //assume all rows have the same columns
    int diag = 0;
    std::vector<T> res;
    while(total_size)
    {
        auto v = output_border_of_submatrix(matrix,diag,diag,total_size);
        res.insert(res.end(),v.begin(),v.end());
        diag++;
    }

    return res;
}

TEST_CASE("spiral_iterator")
{
    int dummy;
    std::vector<std::vector<T>> v = {
        { 0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 },
        { 31 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 10 },
        { 30 , 0 , 2 , 2 , 2 , 2 , 2 , 2 , 0 , 11 },
        { 29 , 0 , 2 , 3 , 3 , 3 , 3 , 2 , 0 , 12 },
        { 28 , 0 , 2 , 3 , 3 , 3 , 3 , 2 , 0 , 13 },
        { 27 , 0 , 2 , 2 , 2 , 2 , 2 , 2 , 0 , 14 },
        { 26 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 15 },
        { 25 , 24 , 23 , 22 , 21 , 20 , 19 , 18 , 17 , 16 }
    };
    std::ostringstream ss;
    ss << output_border_of_submatrix(v,0,0,dummy);
    spdlog::info(ss.str());

    ss.str("");
    ss.clear();
    ss << output_border_of_submatrix(v,1,1,dummy);
    spdlog::info(ss.str());

    ss.str("");
    ss.clear();
    ss << output_border_of_submatrix(v,2,2,dummy);
    spdlog::info(ss.str());

    ss.str("");
    ss.clear();
    ss << output_border_of_submatrix(v,3,3,dummy);
    spdlog::info(ss.str());

}

TEST_CASE("SimpleCase")
{
    int dummy;
    std::ostringstream ss;
    std::vector<std::vector<T>> v = {
        { 1 , 2 , 3},
        { 8 , 9 , 4},
        { 7 , 6 , 5}
    };

    auto result = output_border_of_submatrix(v,0,0,dummy);
    ss << result;
    spdlog::info(ss.str());

    REQUIRE(result == std::vector{1,2,3,4,5,6,7,8});

    auto result2 = output_border_of_submatrix(v,1,1,dummy);

    ss.str("");
    ss.clear();
    ss << result2;
    spdlog::info(ss.str());
    REQUIRE(result2 == std::vector{9});

}

TEST_CASE("SpiralOutput")
{
    std::vector<std::vector<T>> v = {
        { 1 , 2 , 3},
        { 8 , 9 , 4},
        { 7 , 6 , 5}
    };

    auto vec = spiral_output(v);

    REQUIRE(vec == std::vector{1,2,3,4,5,6,7,8,9});
}

TEST_CASE("SpiralOutputomplex")
{
    std::vector<std::vector<T>> v = {
        { 1     , 2     , 3     , 4},
        { 10    , 11    , 12    , 5},
        { 9     , 8     , 7     , 6}
    };

    auto vec = spiral_output(v);

    REQUIRE(vec == std::vector{1,2,3,4,5,6,7,8,9,10,11,12});
}