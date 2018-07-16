//
// Created by jiabei on 2018/7/6.
//

#ifndef POINT_VISION_PIPELINE_FRAMEWORK_EXECUTABLE_HPP
#define POINT_VISION_PIPELINE_FRAMEWORK_EXECUTABLE_HPP

#include <pvpf/pvpf.hpp>
#include <pvpf/utils/data_bucket.hpp>
#include <vector>
#include <pvpf/os_agnostic/dynamic_lib_func_manager.hpp>
#include <unordered_map>

using namespace pvpf;

PVPF_NAMESPACE_BEGIN
    namespace task_execution {
        class executable {
        public:
            virtual void exec(data_bucket &data, data_bucket const &params) = 0;

            virtual ~executable() = 0;
        };

        class dynamic_library_func : public executable {
        public:
            explicit dynamic_library_func(os_agnostic::dylib_func_ptr ptr) : func_ptr(ptr) {}

            ~dynamic_library_func() override = default;

            void exec(data_bucket &data, data_bucket const &params) override;

        private:
            os_agnostic::dylib_func_ptr func_ptr;
        };

        class abstract_algorithm : public executable {
        public:
            ~abstract_algorithm() override = default;

            void add_executable(std::unique_ptr<executable> exe);

        protected:
            std::vector<std::unique_ptr<executable>> exes;
        };

        class normal_algorithm : public abstract_algorithm {
        public:
            void exec(data_bucket &data, data_bucket const &params) override;
        };

        class loop_algorithm : public abstract_algorithm {
        public:
            explicit loop_algorithm(std::unordered_map<std::string, std::string> loop_key_map) :
                    _loop_key_map(std::move(loop_key_map)) {}

            void exec(data_bucket &data, data_bucket const &params) override;

        private:
            std::unordered_map<std::string, std::string> _loop_key_map;
        };
    }

PVPF_NAMESPACE_END
#endif //POINT_VISION_PIPELINE_FRAMEWORK_EXECUTABLE_HPP

