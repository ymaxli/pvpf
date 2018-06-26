#include <iostream>
//#include <tbb/tbb.h>
//#include <opencv/cv.h>
//#include <rapidjson/rapidjson.h>
#include <boost/any.hpp>
//#include <CUDA/cuda.h>
#include <pvpf/data_io/buffer.h>
#include "pvpf/config/config_reader.h"
#include "pvpf/config/config_validator.h"
#include <rapidjson/document.h>

using namespace std;
using namespace pvpf;

int test(rapidjson::Document &d){
    return 0;
}

int main() {
//    std::cout << __VERSION__ << std::endl;
//    data_io::Buffer b;
//    b.write(3);
//    std::cout << b.read() << std::endl;
//    return 0;
    config::config_reader cr;

    string json = "dadf";
    rapidjson::Document d = cr.load_json_conf(json);

    config::config_validator cv;

    vector<config::validation_result const> res = cv.validate(d);
    cout << res.size() << endl;
    for(auto a : res) {

        cout << a.message << endl;
    }
}


