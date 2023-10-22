#include <fmt/format.h>
#include <fmt/ostream.h>
#include <spdlog/spdlog.h>
#include <catch2/catch.hpp>

#include <unordered_map>
#include <string>
#include <chrono>
#include <ctime>  

#include <random>

template<typename ResultType,typename Generator>
class AnyDistribution
{
public:
    template<typename T>
    AnyDistribution(T dist) : impl(std::make_unique<AnyDistImpl<T>>(std::move(dist))){}

    ResultType operator()(Generator& g)
    {
        return impl->call(g);
    }
private:
    struct AnyDistModel
    {
        virtual ~AnyDistModel(){};
        virtual ResultType call(Generator& g) = 0;
    };

    template<typename DistType>
    struct AnyDistImpl : public AnyDistModel
    {
        AnyDistImpl(DistType d) : distribution(std::move(d)){}
        ResultType call(Generator& g) override
        {
            return distribution(g);
        }
        DistType distribution;
    };
    std::unique_ptr<AnyDistModel> impl;
};

template<typename ResultType = double,typename EngineType = std::mt19937>
class MartingaleGenerator{
public:
    template<typename D>
    MartingaleGenerator(ResultType start_value, D distribution) : 
        m_state(start_value),
        gen(std::make_unique<EngineType>(std::random_device()())),
        distrib(distribution)
        {}

    ResultType operator()()
    {
        auto result = m_state;
        m_state = m_state + distrib(*gen);
        return result;
    }

private:
    std::unique_ptr<EngineType> gen;
    AnyDistribution<ResultType,EngineType> distrib;
    ResultType m_state;
};

TEST_CASE("MartingaleGenerator")
{
    std::normal_distribution n(0.0,5.0);

    decltype(n)::param_type p(0.0,2.0);
    n.param(p);

    MartingaleGenerator normal(100.0,n);

    MartingaleGenerator uniform(100.0,std::uniform_real_distribution(-5.0,5.0));

    MartingaleGenerator incremental(1.0,[](auto& g) mutable{ return 1.0;});

    for(int i = 0; i < 10; ++i)
    {
        fmt::print("Normal Dist Value {}: {}\n",i,normal());
    }

    for(int i = 0; i < 10; ++i)
    {
        fmt::print("Uniform Dist Value {}: {}\n",i,uniform());
    }

    for(int i = 0; i < 10; ++i)
    {
        fmt::print("incremental Dist Value {}: {}\n",i,incremental());
    }
}