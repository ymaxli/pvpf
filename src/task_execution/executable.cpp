//
// Created by Max on 2018/7/8.
//

#ifdef DEV_MODE
#include <iostream>
#endif

#include <pvpf/pvpf.hpp>
#include <pvpf/task_execution/executable.hpp>
#include <pvpf/utils/data_bucket.hpp>

using namespace std;

PVPF_NAMESPACE_BEGIN
    namespace task_execution {
        executable::~executable() = default;

        void dynamic_library_func::exec(data_bucket &data, data_bucket const &params) {
            func_ptr(data, params);
        }

        void normal_algorithm::exec(data_bucket &data, data_bucket const &params) {
            for (const auto &exe: exes) {
                exe->exec(data, params);
            }
        }

        void loop_algorithm::exec(data_bucket &data, data_bucket const &params) {
            // check existence and sizes
            int size = -1;
            for (const auto &key: _loop_key_map) {
                if (size == -1) {
                    size = static_cast<int>(data.get_collection_size(key.first));
                } else if (size != static_cast<int>(data.get_collection_size(key.first))) {
                    throw utils::pvpf_exception(string("sizes of data objects in loop algorithm do not equal").c_str());
                }
            }

            // move out collections
            unordered_map<string, vector<core::any>> input_collections;
            unordered_map<string, vector<core::any>> result_collections;
            for (const auto &key: _loop_key_map) {
                input_collections[key.first] = std::move(*data.get_ptr<vector<core::any>>(key.first));
                result_collections[key.second] = vector<core::any>();
                data.remove(key.first);
            }

            for (int i = 0; i < size; i++) {
                // set item
                for (const auto &key: _loop_key_map) {
                    data.put(key.first, std::move(input_collections[key.first][i]));
                }

                // execution
                for (const auto &exe: exes) {
                    exe->exec(data, params);
                }

                // retrieve result
                for (const auto &key: _loop_key_map) {
                    result_collections[key.second].push_back(std::move(*data.get_core_any_ptr(key.second)));
                }
            }

            // move in results
            for (const auto &key: _loop_key_map) {
                data.put(key.second, std::move(result_collections[key.second]));
            }
        }

        void abstract_algorithm::add_executable(unique_ptr<executable> exe) {
            exes.push_back(std::move(exe));
        }
    }
PVPF_NAMESPACE_END


