//
// Created by Icay on 23/06/2018.
//


#ifdef STAND_ALONE
#   define BOOST_TEST_MODULE ConfigValidatorTests
#endif


#include <boost/test/unit_test.hpp>
#include <pvpf/config/config_validator.hpp>
#include <rapidjson/document.h>

using namespace std;
using namespace rapidjson;
using namespace pvpf::config;

BOOST_AUTO_TEST_SUITE(config_validator_test)

    BOOST_AUTO_TEST_CASE(validator_valid_format)
    {
        config_validator cv;
        const char* json = "{\n"
                      "    \"meta\":[],\n"
                      "    \"source\": [],\n"
                      "    \"graph\": [],\n"
                      "    \"sink\": []\n"
                      "}";
        Document d;
        d.Parse(json);
        vector<validation_result const> res = cv.validate(d);

        BOOST_CHECK_EQUAL(res[0].type, 0);
    }

    BOOST_AUTO_TEST_CASE(validator_invalid_format)
    {
        config_validator cv;
        const char* json = "{\n"
                           "    \"meta\":[],\n"
                           "    \"source\": [,\n"
                           "    \"graph\": [],\n"
                           "    \"sink\": []\n"
                           "";
        Document d;
        d.Parse(json);
        vector<validation_result const> res = cv.validate(d);

        //for now we have 4 rules
        BOOST_CHECK_EQUAL(res.size(), 1);
        BOOST_CHECK_EQUAL(res[0].type, 2);
    }


BOOST_AUTO_TEST_SUITE_END()
