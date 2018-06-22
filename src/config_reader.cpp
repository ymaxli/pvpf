//
// Created by Icay on 20/06/2018.
//

#include "pvpf/config_reader.h"
#include <iostream>
#include <rapidjson/istreamwrapper.h>
#include <fstream>

using namespace rapidjson;
using namespace std;

PVPF_NAMESPACE_BEGIN

namespace config
{
    Document config_reader::load_json_conf(const string &file) {
        ifstream ifs(file);
        IStreamWrapper isw(ifs);
        Document d;
        d.ParseStream(isw);
        return d;
    }

}

PVPF_NAMESPACE_END







