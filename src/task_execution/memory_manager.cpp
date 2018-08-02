//
// Created by Icay on 13/07/2018.
//

#include "pvpf/pvpf.hpp"
#include "pvpf/utils/data_bucket.hpp"
#include "pvpf/task_execution/memory_manager.hpp"
#include "pvpf/task_execution/memory_utils.hpp"
#include <vector>

using namespace std;

PVPF_NAMESPACE_BEGIN
    namespace task_execution {
        void memory_manager::pre_process(vector<data_bucket> & data, context const & context) {
            //check if read only
            for(int i = 0; i < data.size(); ++i) {
                // if false, copy data
                if(!context.read_only[i] || (context.pre_is_cpu[i] ^ context.is_cpu)) { //
                    //cpu to cpu
                    if(context.pre_is_cpu[i] && context.is_cpu) {
                        copy_cpu(data[i]);
                    }

                    //cpu to gpu
                    if(context.pre_is_cpu[i] && !context.is_cpu) {
                        copy_cpu_to_gpu(data[i]);
                    }

                    //gpu to cpu
                    if(!context.pre_is_cpu[i] && context.is_cpu) {
                        copy_gpu_to_cpu(data[i]);
                    }

                    //gpu to gpu
                    if(!context.pre_is_cpu[i] && !context.is_cpu) {
//                        copy_gpu(data[i]);
                    }

                }
            }

        }

        //generate output data_bucket
        void memory_manager::post_process(data_bucket & data, context const & context) {

            for(auto itr = data.begin(); itr != data.end(); ++itr) {
                auto context_itr = context.output_mapping.find(itr->first);
                if(context_itr == context.output_mapping.end()) {
                    data.remove(itr->first);
                }
                else {
                    data.rename(context_itr->second, context_itr->first);
                }
            }


        }

        vector<data_bucket> memory_manager::generate_output_buckets(data_bucket & original) {

        }
    }
PVPF_NAMESPACE_END

