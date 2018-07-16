//
// Created by Icay on 14/07/2018.
//

#include "pvpf/pvpf.hpp"
#include "pvpf/utils/data_bucket.hpp"
#include "pvpf/task_execution/memory_utils.hpp"
#include <vector>
#include <algorithm>

using namespace std;

PVPF_NAMESPACE_BEGIN
    namespace task_execution {

        shared_ptr<core::any> copy_cpu_to_gpu(const shared_ptr<core::any> &cpu_ptr) {
            //TODO
            return NULL;
        }

        void copy_cpu_to_gpu(data_bucket& data){
            //TODO
        }

        shared_ptr<core::any> copy_gpu_to_cpu(const shared_ptr<core::any> &gpu_ptr) {
            //TODO
            return NULL;
        }

        void copy_gpu_to_cpu(data_bucket& data){
            //TODO
        }

        shared_ptr<core::any> copy_cpu(const shared_ptr<core::any> &cpu_ptr){
            core::any copy_data = *cpu_ptr;
            shared_ptr<core::any> new_cpu_ptr = make_shared<core::any>(copy_data);
            return new_cpu_ptr;
        }

        void copy_cpu(data_bucket& data){
            for(auto &itr: data) {
                const shared_ptr<core::any> cpu_ptr = itr.second;
                itr.second = copy_cpu(cpu_ptr);
            }
        }

        shared_ptr<core::any> copy_gpu(const shared_ptr<core::any> &gpu_ptr){
            //TODO
            return NULL;
        }

        void copy_gpu(data_bucket& data){
            //TODO
        }

        void copy_gpu_to_put(data_bucket& data){
            //TODO
        }

        void free_cpu(std::shared_ptr<core::any> cpu_ptr){
            //TODO
        }

        void free_gpu(std::shared_ptr<core::any> gpu_ptr){
            //TODO
        }


    }
PVPF_NAMESPACE_END