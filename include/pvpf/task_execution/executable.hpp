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
        public:
            virtual void exec(data_bucket &data, data_bucket const &params) = 0;

            virtual ~executable() = 0;
        };

        class dynamic_library_func : public executable {
            void exec(data_bucket &data, data_bucket const &params) override;

            ~dynamic_library_func() override {}
        };

        class abstract_algorithm : public executable {
        protected:
            std::vector<std::unique_ptr<executable>> exes;
        };

        class normal_algorithm : public abstract_algorithm {
            void exec(data_bucket &data, data_bucket const &params) override;

            ~normal_algorithm() override {}
        };

        class loop_algorithm : public abstract_algorithm {
            void exec(data_bucket &data, data_bucket const &params) override;

            ~loop_algorithm() override {}
        };
    }

PVPF_NAMESPACE_END
#endif //POINT_VISION_PIPELINE_FRAMEWORK_EXECUTABLE_HPP

