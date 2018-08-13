//
// Created by Jiabei He on 2018/7/21.
//

#include <pvpf/task_execution/context_generator.hpp>
#include <pvpf/pvpf.hpp>
#include <rapidjson/document.h>
#include <pvpf/task_execution/scheduler.hpp>
#include <pvpf/config/config_reader.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <iostream>

using namespace rapidjson;
using namespace pvpf;
using namespace pvpf::task_execution;
using namespace std;

PVPF_NAMESPACE_BEGIN
namespace task_execution
{

shared_ptr<context>
create_context(rapidjson::Value const &obj, map<string, Value> const &json_object_map)
{

    string id = obj["id"].GetString();

    //            cout << "succesors" << endl;
    //add to successors
    vector<string> successors;
    // for (auto node = sch.json_object_map.begin(); node != sch.json_object_map.end(); node++)
    // {
    //     Value to_check = node->second;
    //     if (to_check.HasMember("input"))
    //     {
    //         const Value &pre_list = (*to_check)["input"]["pre"];
    //         for (Value::ConstValueIterator pre_it = pre_list.Begin(); pre_it != pre_list.End(); pre_it++)
    //         {
    //             if (pre_it->GetString() == id)
    //             {
    //                 successors.push_back(move((*to_check)["id"].GetString()));
    //                 break;
    //             }
    //         }
    //     }
    // }

    //            cout << "is_cpu" << endl;
    //add is_cpu to context;
    bool this_is_cpu = false;
    if (obj["task"].HasMember("algorithm"))
    {
        this_is_cpu = is_cpu(obj["task"]["algorithm"].GetString());
    }

    //            cout << "pre" << endl;
    //          add to pre{}
    //add pre list and configure for pre.

    vector<string> pre;
    vector<bool> pre_is_cpu;
    vector<bool> read_only;
    if (obj.HasMember("input"))
    {
        const Value &pre_list = obj["input"]["pre"];
        for (Value::ConstValueIterator pre_it = pre_list.Begin(); pre_it != pre_list.End(); pre_it++)
        {
            string id = pre_it->GetString();
            pre.push_back(id);
            if (json_object_map.count(id) > 0)
            {
                if (json_object_map.at(id)["task"].HasMember("algorithm"))
                {
                    pre_is_cpu.push_back(is_cpu(json_object_map.at(id)["task"]["algorithm"].GetString()));
                }
                else
                {
                    pre_is_cpu.push_back(true);
                }
            }
            else
            {
                cout << "there is no such predecessor" << endl;
                cout << id << endl;
            }
        }

        if (obj["input"].HasMember("pre_conf") && obj["input"]["pre_conf"].GetArray().Size() > 0)
        {
            for (Value::ConstValueIterator conf_it = obj["input"]["pre_conf"].Begin();
                 conf_it != obj["input"]["pre_conf"].End(); conf_it++)
            {
                read_only.push_back((*conf_it)["readonly"].GetBool());
            }
        }
        else
        {
            for (int i = 0; i < pre.size(); i++)
            {
                read_only.push_back(true);
            }
        }
    }

    //            cout << "input" << endl;
    //add to input mapping
    unordered_map<string, vector<pair<int, string>>> input;
    if (obj.HasMember("input"))
    {
        if (obj["input"].HasMember("mapping"))
        {
            const Value &input_list = obj["input"]["mapping"];
            for (Value::ConstMemberIterator input_it = input_list.MemberBegin();
                 input_it != input_list.MemberEnd(); input_it++)
            {
                vector<pair<int, string>> value_list;
                if (input_it->value.IsArray())
                {
                    const Value &key_list = input_it->value;
                    for (Value::ConstValueIterator key_it = key_list.Begin();
                         key_it != key_list.End(); key_it++)
                    {
                        vector<std::pair<int, std::string>> result;
                        result = analyze_mapping_value((*key_it).GetString(), pre.size());
                        value_list.insert(value_list.end(), result.begin(), result.end());
                    }
                }
                else
                {
                    value_list = analyze_mapping_value(input_it->value.GetString(), pre.size());
                }

                input[input_it->name.GetString()] = move(value_list);
            }
        }
    }

    //            cout << "output" << endl;
    //add to output
    unordered_map<string, string> output;
    unordered_map<string, string> data;
    if (obj.HasMember("output"))
    {
        if (obj["output"].HasMember("mapping"))
        {
            const Value &output_list = obj["output"]["mapping"];
            for (Value::ConstMemberIterator keys = output_list.MemberBegin();
                 keys != output_list.MemberEnd(); keys++)
            {
                output[keys->name.GetString()] = keys->value.GetString();
            }
        }

        if (obj["output"].HasMember("data"))
        {
            const Value &data_list = obj["output"]["data"];
            for (Value::ConstMemberIterator data_it = data_list.MemberBegin();
                 data_it != data_list.MemberEnd(); data_it++)
            {
                data[data_it->name.GetString()] = data_it->value.GetString();
            }
        }
    }
    //            cout << "gather all" << endl;

    auto cont = make_shared<context>(id, pre, successors, input, output, data, read_only, pre_is_cpu,
                                     this_is_cpu);

    //            cout << "finish" << endl;
    return cont;
}

bool is_cpu(std::string algorithm_name)
{
    //             config::config_reader cr;
    //             Document d = cr.load_json_conf("./pvpf_algorithm/" + algorithm_name + ".json");
    // //            cout << "get json" << endl;
    //             const Value &meta = d["meta"];
    //             if (meta.HasMember("gpu")) {
    //                 return !meta["gpu"].GetBool();
    //             }
    //            cout << "I will return" << endl;
    return true;
}

std::vector<std::pair<int, std::string>> analyze_mapping_value(std::string value, int size)
{
    vector<std::pair<int, std::string>> result;
    vector<string> split_result;
    string key = value;
    boost::algorithm::split(split_result, key, boost::is_any_of("."));
    if (split_result[1].compare("all") == 0)
    {
        for (int i = 0; i < size; i++)
        {
            pair<int, string> p(i, split_result[0]);
            result.push_back(move(p));
        }
    }
    else
    {
        pair<int, string> p(stoi(split_result[1]), split_result[0]);
        result.push_back(move(p));
    }
    return result;
}

} // namespace task_execution

PVPF_NAMESPACE_END