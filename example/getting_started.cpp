// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifdef _MSC_VER
#  pragma warning(disable : 4127)  // conditional expression is constant.
#  pragma warning(disable : 4189)  // local variable is initialized but not referenced.
#  pragma warning(disable : 4100)  // unreferenced formal parameter.
#  pragma warning(disable : 4714)  // marked as __forceinline not inlined.
#endif

namespace { namespace local
{
#if defined(_MSC_VER)
    static bool const is_msc = true;
#else
    static bool const is_msc = false;
#endif
}}

#include <boost/bind.hpp>
#include <boost/detail/lightweight_test.hpp>

static
void
log(char const*)
{
  // Dummy function to demonstrate how problems might be logged.
}

//[getting_started_headers1
#include <boost/convert.hpp>
#include <boost/convert/lexical_cast.hpp>
//]

//[getting_started_using
using std::string;
using boost::lexical_cast;
using boost::convert;
//]
//[getting_started_default_converter
// Optional definition of the default converter
struct boost::cnv::by_default : public boost::cnv::lexical_cast {};
//]
static void getting_started_example1()
{
    //[getting_started_example1
    try
    {
        boost::cnv::lexical_cast cnv;

        int    i1 = lexical_cast<int>("123");
        int    i2 = convert<int>("123").value(); // With the default converter
        int    i3 = convert<int>("123", cnv).value(); // With explicitly supplied converter
        string s1 = lexical_cast<string>(123);
        string s2 = convert<string>(123).value(); // With the default converter
        string s3 = convert<string>(123, cnv).value(); // With explicitly supplied converter

        BOOST_TEST(i1 == 123);
        BOOST_TEST(i2 == 123);
        BOOST_TEST(i3 == 123);
        BOOST_TEST(s1 == "123");
        BOOST_TEST(s2 == "123");
        BOOST_TEST(s3 == "123");
    }
    catch (std::exception const& ex)
    {
        // Be aware that the conversion requests above can fail,
        // so always use try'n'catch blocks to handle any exceptions thrown.
        // Ignore this at your peril!
        std::cerr << "Exception " << ex.what() << std::endl;
    }
    //] [/getting_started_example1]
}

static void getting_started_example2()
{
    //[getting_started_example2
    // Does not throw. Returns fallback value (-1) when failed.
    int i = convert<int>("uhm", boost::cnv::lexical_cast()).value_or(-1);

    BOOST_TEST(i == -1); // Conversion failed. 'i' assigned the fallback value.
    //]
}

//[getting_started_headers3
#include <boost/convert/strtol.hpp>
#include <boost/convert/spirit.hpp>
//]
static void getting_started_example3()
{
    //[getting_started_example3
    boost::cnv::lexical_cast cnv1;
    boost::cnv::strtol       cnv2;
    boost::cnv::spirit       cnv3;

    int i1 = convert<int>("123", cnv1).value();
    int i2 = convert<int>("123", cnv2).value(); // Two times faster than lexical_cast.
    int i3 = convert<int>("123", cnv3).value(); // Four times faster than lexical_cast.
    //]
}

//[getting_started_headers4
#include <boost/convert/stream.hpp>
//]
static void getting_started_example4()
{
    //[getting_started_example4
    boost::cnv::cstream cnv;

    try
    {
        int i1 = lexical_cast<int>("   123"); // Does not work.
        BOOST_TEST(!"Never reached");
    }
    catch (...) {}

    int          i2 = convert<int>("   123", cnv(std::skipws)).value(); // Success
    string       s1 = lexical_cast<string>(12.34567);
    string       s2 = convert<string>(12.34567, cnv(std::fixed)(std::setprecision(3))).value();
    string       s3 = convert<string>(12.34567, cnv(std::scientific)(std::setprecision(3))).value();
    string expected = local::is_msc ? "1.235e+001" : "1.235e+01";

    BOOST_TEST(i2 == 123);
    BOOST_TEST(s1 == "12.34567"); // Precision is not configurable.
    BOOST_TEST(s2 == "12.346");   // Precision was set to 3. Fixed.
    BOOST_TEST(s3 == expected);   // Precision was set to 3. Scientific.
    //]
}

static
void
getting_started_example5()
{
    //[getting_started_example5
    boost::cnv::cstream cnv;

    int i1 = lexical_cast<int>("123"); // Throws when conversion fails.
    int i2 = convert<int>("123", cnv).value(); // Throws when conversion fails.
    int i3 = convert<int>("uhm", cnv).value_or(-1); // Returns -1 when conversion fails.

    BOOST_TEST(i1 == 123);
    BOOST_TEST(i2 == 123);
    BOOST_TEST(i3 == -1);
    //]
}

static void getting_started_example6()
{
    std::string const    s1 = "123";
    std::string const    s2 = "456";
    int const    default_i1 = 11;
    int const    default_i2 = 12;
    boost::cnv::cstream cnv;

    //[getting_started_example6

    int i1 = convert<int>(s1, cnv(std::hex)).value_or(-1); // Read as hex.
    int i2 = convert<int>(s2, cnv(std::dec)).value_or(-1); // Read as decimal.

    if (i1 == -1) log("bad i1"), i1 = default_i1;
    if (i2 == -1) log("bad i2"), i2 = default_i2;

    // ... proceed
    //]
    BOOST_TEST(i1 == 291);
    BOOST_TEST(i2 == 456);
}

static void getting_started_example7()
{
    std::string const    s1 = "123";
    std::string const    s2 = "456";
    int const    default_i1 = 11;
    int const    default_i2 = 12;
    boost::cnv::cstream cnv;

    //[getting_started_example7

    int i1 = convert<int>(s1, cnv(std::hex)).value_or(default_i1);
    int i2 = convert<int>(s2, cnv(std::dec)).value_or(default_i2);
    // ... proceed
    //]
    BOOST_TEST(i1 == 291);
    BOOST_TEST(i2 == 456);
}

static
//[getting_started_example9_func
int
fallback_fun(char const* msg, int fallback_value)
{
    log(msg); return fallback_value;
}
//]
static void getting_started_example9()
{
    std::string const s1 = "123";
    std::string const s2 = "456";
    int const default_i1 = 11;
    int const default_i2 = 12;

    //[getting_started_example9
    int i1 = convert<int>(s1).value_or_eval(boost::bind(fallback_fun, "bad i1", default_i1));
    int i2 = convert<int>(s2).value_or_eval(boost::bind(fallback_fun, "bad i2", default_i2));
    // ... proceed
    //]
    BOOST_TEST(i1 == 123);
    BOOST_TEST(i2 == 456);
}

static void getting_started_example8()
{
    std::string const str = "123";
    int const  default_i1 = 12;
    //[getting_started_example8
    int i1 = default_i1;

    try
    {
        i1 = lexical_cast<int>(str);
    }
    catch (...)
    {
        log("bad i1");
    }
    //]
    BOOST_TEST(i1 == 123);
}

int
main(int argc, char const* argv[])
{
    getting_started_example1();
    getting_started_example2();
    getting_started_example3();
    getting_started_example4();
    getting_started_example5();
    getting_started_example6();
    getting_started_example7();
    getting_started_example8();
    getting_started_example9();

    return boost::report_errors();
}