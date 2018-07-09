//
// Created by jiabei on 2018/7/6.
//

#ifndef POINT_VISION_PIPELINE_FRAMEWORK_EXECUTABLE_HPP
#define POINT_VISION_PIPELINE_FRAMEWORK_EXECUTABLE_HPP

#include <pvpf/pvpf.hpp>
#include <pvpf/utils/data_bucket.hpp>
#include <vector>

PVPF_NAMESPACE_BEGIN
    namespace task_execution {
        class executable {
            virtual void exec(data_bucket &data, data_bucket const &params) = 0;
        };

        class dynamic_library_func : public executable {
            void exec(data_bucket &data, data_bucket const &params) override;
        };

        class abstract_algorithm : public executable {
        protected:
            std::vector<std::unique_ptr<executable>> exes;
        };

        class normal_algorithm : public abstract_algorithm {
            void exec(data_bucket &data, data_bucket const &params) override;
        };

        class loop_algorithm : public abstract_algorithm {
            void exec(data_bucket &data, data_bucket const &params) override;
        };
    }

PVPF_NAMESPACE_END
#endif //POINT_VISION_PIPELINE_FRAMEWORK_EXECUTABLE_HPP

