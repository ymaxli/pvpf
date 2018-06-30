//
// Created by Icay on 20/06/2018.
//

#include "pvpf/config/config_reader.h"
#include <iostream>
#include <rapidjson/istreamwrapper.h>
#include <fstream>

using namespace rapidjson;
using namespace std;

PVPF_NAMESPACE_BEGIN

namespace config
{
    Document config_reader::load_json_conf(const string &file) {
        ifstream f;

        //prepare f to throw if failbit gets set
        std::ios_base::iostate exceptionMask = f.exceptions() | std::ios::failbit;
        f.exceptions(exceptionMask);

        try {
            f.open(file);
        }
        catch (std::ios_base::failure& e) {
            if ( e.code() == std::make_error_condition(std::io_errc::stream) )
                std::cerr << "Stream error!\n";
            else
                std::cerr << "Unknown failure opening file.\n";

            //if cannot open file, return NULL
            Document d = NULL;
            return d;
        }
        IStreamWrapper isw(f);
        Document d;
        d.ParseStream(isw);

        return d;
    }

}

PVPF_NAMESPACE_END







