//
// Created by Icay on 20/06/2018.
//

#include "pvpf/config/config_reader.hpp"
#include <iostream>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem.hpp>
#include <sstream>

using namespace rapidjson;
using namespace std;

PVPF_NAMESPACE_BEGIN

namespace config
{
    Document config_reader::load_json_conf(const string &file) {
        boost::filesystem::path filePath(file);
        Document d;

        if (!boost::filesystem::exists(filePath)) {
            cerr << "Configuration file not found"<< endl;
            return d;
        }
        else if (boost::filesystem::is_regular_file(filePath)) {
            string extension = filePath.extension().string();
            if(extension != ".json") {
                cerr << "Path shoud be a json file" << endl;
                return d;
            }
        }
        else {
            cerr << "Path should be a json file" << endl;
            return d;
        }

        boost::filesystem::ifstream inFile;
        inFile.open(filePath);//open the input file

        stringstream strStream;
        strStream << inFile.rdbuf();//read the file
        string str = strStream.str();//str holds the content of the file

        char char_array[str.size()+1];
        strcpy(char_array, str.c_str());

        d.Parse(char_array);

        return d;
    }

    std::unordered_map load_algorithm(rapidjson::Document const &d){

    }

}

PVPF_NAMESPACE_END







