#ifdef STAND_ALONE
#define BOOST_TEST_MODULE FrameworkTest
#endif

#include <boost/test/unit_test.hpp>
#include <iostream>
#include <pvpf/config/config_reader.hpp>
#include <rapidjson/document.h>
#include <pvpf/config/config_validator.hpp>

using namespace pvpf;
using namespace std;
using namespace rapidjson;
using namespace pvpf::config;

string const &TEST_ALGORITHM_DIR("./test_algorithms");

BOOST_AUTO_TEST_SUITE(system_testing_framework_test_suite)
BOOST_AUTO_TEST_CASE(weight_tv)
{
    config_reader cr;
    Document d = cr.load_json_conf(TEST_ALGORITHM_DIR + string("/weight_tv.json"));
    config_validator cv;
    vector<validation_result> res = cv.validate(d);
    for (auto aa : res)
    {
        cout << aa.get_message() << endl;
    }

    auto algo_map = cr.load_algorithm(d, TEST_ALGORITHM_DIR);
}
BOOST_AUTO_TEST_SUITE_END()
