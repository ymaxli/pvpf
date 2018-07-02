#include <iostream>
//#include <tbb/tbb.h>
//#include <opencv/cv.h>
//#include <rapidjson/rapidjson.h>
//#include <boost/any.hpp>
//#include <CUDA/cuda.h>
<<<<<<< HEAD
#include <pvpf/data_io/buffer.h>
#include "pvpf/config/config_reader.h"
#include "pvpf/config/config_validator.h"
#include <rapidjson/document.h>
#include "pvpf/config/validation_rule.h"

using namespace std;
using namespace pvpf;
using namespace rapidjson;


int main() {
    const char* json = "{\n"
                       "    \"hello\": \"\",\n"
                       "    \"t\": true ,\n"
                       "    \"f\": false,\n"
                       "    \"n\": null,\n"
                       "    \"i\": 123,\n"
                       "    \"pi\": 3.1416,\n"
                       "    \"a\": [1, 2, 3, 4]\n"
                       "}";
    Document d;
    d.Parse(json);

    string i = d["hello"].GetString();
    cout << i << endl;

//    config::validation_result res = crs.validate(d);

}
