//
// Created by Icay on 23/06/2018.
//

#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE ConfigReaderTests
#endif


#include <boost/test/unit_test.hpp>
#include <pvpf/config/config_reader.h>
#include <rapidjson/document.h>

using namespace std;
using namespace rapidjson;
using namespace pvpf::config;

BOOST_AUTO_TEST_SUITE(config_reader_test)

    BOOST_AUTO_TEST_CASE(reader_read_json)
    {
        config_reader cr;
        string jsonfile = "../../unit_test/test_json/read_json.json";
        Document d = cr.load_json_conf(jsonfile);
        BOOST_CHECK(d.IsObject());
    }

    BOOST_AUTO_TEST_CASE(reader_read_other_file)
    {
        config_reader cr;
        string jsonfile = "";
        Document d = cr.load_json_conf(jsonfile);
        BOOST_CHECK(!d.IsObject());
    }


BOOST_AUTO_TEST_SUITE_END()