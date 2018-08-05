//
// Created by Max on 2018/8/3.
//

#include <pvpf/task_execution/body.hpp>
#include <pvpf/task_execution/memory_manager.hpp>

using namespace std;

PVPF_NAMESPACE_BEGIN
namespace task_execution
{
template <size_t input_size>
data_bucket node_body<input_size>::operator()(array<data_bucket, input_size> const &data)
{

    // pre process
    vector<data_bucket> preprocessedVec(data.begin(), data.end());
    memory_manager::pre_process(preprocessedVec, *cont);

    // combine data
    data_bucket input_data;
    for (auto it = cont->input_mapping.begin(); it != cont->input_mapping.end(); it++)
    {
        if (it->second.size() == 1)
        {
            auto pair = it->second[0];
            preprocessedVec[get<0>(pair)].transfer(input_data, it->first, get<1>(pair));
        }
        else
        {
            input_data.start_constructing_vector(it->first);

            auto mappings = it->second;
            for (auto mapping_it = mappings.begin(); mapping_it != mappings.end(); mapping_it++)
            {
                preprocessedVec[get<0>(*mapping_it)].transferByConstructingVector(input_data, it->first,
                                                                                  get<1>(*mapping_it));
            }

            input_data.complete_constructing_vector(it->first);
        }
    }

    // invoke executable
    exec->exec(input_data, *params);

    // post process
    memory_manager::post_process(input_data, *cont);

    return input_data;
}

template <size_t input_size>
data_bucket sink_body<input_size>::operator()(array<data_bucket, input_size> const &data)
{

    // pre process
    vector<data_bucket> preprocessedVec(data.begin(), data.end());
    memory_manager::pre_process(preprocessedVec, *cont);

    // combine data
    data_bucket input_data;
    for (auto it = cont->input_mapping.begin(); it != cont->input_mapping.end(); it++)
    {
        if (it->second.size() == 1)
        {
            auto pair = it->second[0];
            preprocessedVec[get<0>(pair)].transfer(input_data, it->first, get<1>(pair));
        }
        else
        {
            input_data.start_constructing_vector(it->first);

            auto mappings = it->second;
            for (auto mapping_it = mappings.begin(); mapping_it != mappings.end(); mapping_it++)
            {
                preprocessedVec[get<0>(*mapping_it)].transferByConstructingVector(input_data, it->first,
                                                                                  get<1>(*mapping_it));
            }

            input_data.complete_constructing_vector(it->first);
        }
    }

    // write to output library
    pipe->write(std::move(input_data));

    return data_bucket();
}
} // namespace task_execution
PVPF_NAMESPACE_END

// // no need to invoke this method
// void avoid_body_link_error()
// {
//     pvpf::task_execution::node_body<1> node1;
//     pvpf::task_execution::node_body<2> node2;
//     pvpf::task_execution::node_body<3> node3;
//     pvpf::task_execution::node_body<4> node4;
//     pvpf::task_execution::node_body<5> node5;
//     pvpf::task_execution::node_body<6> node6;
//     pvpf::task_execution::node_body<7> node7;
//     pvpf::task_execution::node_body<8> node8;
//     pvpf::task_execution::node_body<9> node9;
//     pvpf::task_execution::node_body<10> node10;
//     pvpf::task_execution::sink_body<1> node11;
//     pvpf::task_execution::sink_body<2> node12;
//     pvpf::task_execution::sink_body<3> node13;
//     pvpf::task_execution::sink_body<4> node14;
//     pvpf::task_execution::sink_body<5> node15;
//     pvpf::task_execution::sink_body<6> node16;
//     pvpf::task_execution::sink_body<7> node17;
//     pvpf::task_execution::sink_body<8> node18;
//     pvpf::task_execution::sink_body<9> node19;
//     pvpf::task_execution::sink_body<10> node20;
// }