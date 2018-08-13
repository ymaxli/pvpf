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
#include <memory>

using namespace rapidjson;
using namespace std;
using namespace boost::filesystem;

PVPF_NAMESPACE_BEGIN

namespace config
{
Document config_reader::load_json_conf(const string &file)
{
    boost::filesystem::path filePath(file);
    Document d;

    if (!boost::filesystem::exists(filePath))
    {
        cerr << "Configuration file not found" << endl;
        return d;
    }
    else if (boost::filesystem::is_regular_file(filePath))
    {
        string extension = filePath.extension().string();
        if (extension != ".json")
        {
            cerr << "Path shoud be a json file" << endl;
            return d;
        }
    }
    else
    {
        cerr << "Path should be a json file" << endl;
        return d;
    }

    boost::filesystem::ifstream inFile;
    inFile.open(filePath); //open the input file

    stringstream strStream;
    strStream << inFile.rdbuf();  //read the file
    string str = strStream.str(); //str holds the content of the file

    char char_array[str.size() + 1];
    strcpy(char_array, str.c_str());

    d.Parse(char_array);

    parse_library_location(d, filePath.parent_path().string());

    return d;
}

std::shared_ptr<std::unordered_map<std::string, rapidjson::Document>> config_reader::load_algorithm(rapidjson::Document const &d, string const &parent_path)
{
    const Value &graph_json = d["graph"];
    auto map = make_shared<unordered_map<string, Document>>();
    for (auto node = graph_json.Begin(); node != graph_json.End(); node++)
    {
        if ((*node)["task"].HasMember("algorithm"))
        {
            config_algorithm(*(map), (*node)["task"]["algorithm"].GetString(), parent_path);
        }
    }
    return map;
}

void config_reader::config_algorithm(std::unordered_map<std::string, rapidjson::Document> &map,
                                     string const &algorithm_name,
                                     string const &parent_path)
{
    if (map.count(algorithm_name) > 0)
        return;
    path algo_config_file(parent_path + string("/pvpf_algorithms/") + algorithm_name + string("/config.json"));

    Document d = load_json_conf(algo_config_file.string());
    parse_algorithm_library_location(d, parent_path + string("/pvpf_algorithms/") + algorithm_name);

    map[algorithm_name] = std::move(d);
    for (Value::ConstValueIterator algo_config = map[algorithm_name]["body"].Begin();
         algo_config != map[algorithm_name]["body"].End(); algo_config++)
    {
        string str = (*algo_config)["type"].GetString();
        if (str == "algorithm")
        {
            config_algorithm(map, (*algo_config)["algorithm"].GetString(), parent_path);
        }
    }
};

void config_reader::parse_library_location(Document &d, std::string const &parent_path)
{
    // source lib
    if (d.HasMember("source"))
    {
        auto &source_json = d["source"];
        for (auto node = source_json.Begin(); node != source_json.End(); node++)
        {
            auto &dylib = (*node)["task"]["dylib"]["location"];
            path source_lib(parent_path + string("/") + dylib.GetString());
            dylib.SetString(source_lib.string().c_str(), source_lib.string().length(), d.GetAllocator());
        }
    }

    // source lib
    if (d.HasMember("sink"))
    {
        auto &sink_json = d["sink"];
        for (auto node = sink_json.Begin(); node != sink_json.End(); node++)
        {
            auto &dylib = (*node)["task"]["dylib"]["location"];
            path sink_lib(parent_path + string("/") + dylib.GetString());
            dylib.SetString(sink_lib.string().c_str(), sink_lib.string().length(), d.GetAllocator());
        }
    }
}

void config_reader::parse_algorithm_library_location(rapidjson::Document &d, std::string const &parent_path)
{
    if (d.HasMember("body"))
    {
        auto &body_json = d["body"];
        for (auto node = body_json.Begin(); node != body_json.End(); node++)
        {
            if ((*node)["type"] == "dylib")
            {
                auto &location = (*node)["location"];
                path lib_location(parent_path + string("/") + location.GetString());
                location.SetString(lib_location.string().c_str(), lib_location.string().length(), d.GetAllocator());
            }
        }
    }
}

} // namespace config

PVPF_NAMESPACE_END
