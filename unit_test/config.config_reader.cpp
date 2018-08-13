//
// Created by Icay on 23/06/2018.
//

#ifdef STAND_ALONE
#define BOOST_TEST_MODULE ConfigReaderTests
#endif

#include <boost/test/unit_test.hpp>
#include <pvpf/config/config_reader.hpp>
#include <rapidjson/document.h>
#include <iostream>

using namespace std;
using namespace rapidjson;
using namespace pvpf::config;

BOOST_AUTO_TEST_SUITE(config_reader_test)

BOOST_AUTO_TEST_CASE(reader_read_json)
{
    config_reader cr;
    string jsonfile = "./test_json/read_json.json";
    Document d = cr.load_json_conf(jsonfile);
    BOOST_CHECK(d.IsObject());
}

BOOST_AUTO_TEST_CASE(reader_read_invalid_path)
{
    std::cerr.setstate(std::ios_base::failbit);

    config_reader cr;
    string jsonfile = "";
    Document d = cr.load_json_conf(jsonfile);
    BOOST_CHECK(!d.IsObject());

    std::cerr.clear();
}

BOOST_AUTO_TEST_CASE(reader_read_directory)
{
    std::cerr.setstate(std::ios_base::failbit);

    config_reader cr;
    string jsonfile = "./test_json";
    Document d = cr.load_json_conf(jsonfile);
    BOOST_CHECK(!d.IsObject());

    std::cerr.clear();
}

BOOST_AUTO_TEST_CASE(reader_read_not_json)
{
    std::cerr.setstate(std::ios_base::failbit);

    config_reader cr;
    string jsonfile = "./not_json.txt";
    Document d = cr.load_json_conf(jsonfile);
    BOOST_CHECK(!d.IsObject());

    std::cerr.clear();
}

// BOOST_AUTO_TEST_CASE(reader_read_algorithm)
// {
//     config_reader cr;
//     string jsonfile = "./test_json/context.json";
//     Document d = cr.load_json_conf(jsonfile);
//     auto map = cr.load_algorithm(d, "./test_json");
//     BOOST_CHECK((*map.get()).count("gpu_false") > 0);
//     BOOST_CHECK((*map.get()).count("lack_input") > 0);
//     BOOST_CHECK((*map.get()).count("happy_case") > 0);
//     BOOST_CHECK((*map.get()).count("no_gpu") > 0);
// }

BOOST_AUTO_TEST_SUITE_END()
