#include <boost/current_function.hpp>
#include <nlohmann/json.hpp>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include <NamedType/named_type.hpp>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
using namespace fluent;

using FirstName = NamedType<std::string, struct FirstNameTag,Comparable,Printable,BinaryAddable>;
using LastName = NamedType<std::string, struct LastNameTag,Comparable,Printable,BinaryAddable>;
using FullName = NamedType<std::string, struct FullNameTag,Comparable,Printable,BinaryAddable>;

FullName fullNameImpl(const FirstName& fn,const LastName& ln)
{
    return FullName(fn.get() + " " + ln.get());
}
static auto fullName = make_named_arg_function<FirstName,LastName>(fullNameImpl);

TEST_CASE("NamedTypeNormal")
{
    FirstName name("Bob");
    LastName surname("Hope");
    auto fn = fullNameImpl(name,surname);

    REQUIRE(fn == FullName("Bob Hope") );
    
}

TEST_CASE("NamedTypeAnyOrderCallable")
{
    FirstName name("Bob");
    LastName surname("Hope");
    auto fn = fullName(name,surname);

    REQUIRE(fn == FullName("Bob Hope") );   
}

TEST_CASE("NamedTypeAnyOrderCallableReversedArguments")
{
    FirstName name("Bob");
    LastName surname("Hope");
    auto fn = fullName(surname,name);

    REQUIRE(fn == FullName("Bob Hope") );   
}


// struct Foo
// {
//     Foo(){ spdlog::info(BOOST_CURRENT_FUNCTION);}
//     void* operator new(std::size_t s)
//     {
//         spdlog::info(BOOST_CURRENT_FUNCTION);
//         return ::operator new(s);
//     }
// };

// TEST_CASE("OperatorNew")
// {
//     Foo * f = new Foo();
//     delete f;
// }