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

        void copy_cpu_to_gpu(data_bucket& data){
            //TODO
        }

        void copy_gpu_to_cpu(data_bucket& data){
            //TODO
        }


        void copy_cpu(data_bucket& data){

            data = data.clone();

        }


        void copy_gpu(data_bucket& data){
            //TODO
        }



    }
PVPF_NAMESPACE_END