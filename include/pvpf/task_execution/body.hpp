//
// Created by jiabei on 2018/7/5.
//

#ifndef POINT_VISION_PIPELINE_FRAMEWORK_BODY_HPP
#define POINT_VISION_PIPELINE_FRAMEWORK_BODY_HPP

#include <vector>
#include <unordered_map>
#include <string>
#include <pvpf/data_io/io_pipe.hpp>
#include <pvpf/task_execution/executable.hpp>
#include <pvpf/utils/data_bucket.hpp>

PVPF_NAMESPACE_BEGIN
namespace task_execution
{
/**
 * structured context storing all the information one logical node needs
 */
struct context
{
  public:
    std::string const node_id;
    std::vector<std::string> pre;
    std::vector<std::string> succ; // not implemented
    std::unordered_map<std::string, std::vector<std::pair<int, std::string>>> input_mapping;
    std::unordered_map<std::string, std::string> output_mapping;
    std::unordered_map<std::string, std::string> output_data;

    std::vector<bool> read_only;
    std::vector<bool> pre_is_cpu;
    bool is_cpu;

    context(std::string id, std::vector<std::string> pre, std::vector<std::string> succ,
            std::unordered_map<std::string, std::vector<std::pair<int, std::string>>> input,
            std::unordered_map<std::string, std::string> output,
            std::unordered_map<std::string, std::string> data, std::vector<bool> read_only,
            std::vector<bool> pre_is_cpu, bool is_cpu)
        : node_id(id), pre(std::move(pre)), succ(std::move(succ)), input_mapping(std::move(input)),
          output_mapping(std::move(output)),
          output_data(std::move(data)), read_only(std::move(read_only)), pre_is_cpu(std::move(pre_is_cpu)),
          is_cpu(is_cpu) {}

    context(context const &cont) : node_id(cont.node_id), pre(cont.pre), succ(cont.succ),
                                   input_mapping(cont.input_mapping),
                                   output_mapping(cont.output_mapping),
                                   output_data(cont.output_data), read_only(cont.read_only),
                                   pre_is_cpu(cont.pre_is_cpu),
                                   is_cpu(cont.is_cpu) {}

    context(context &&cont) : node_id(std::move(cont.node_id)), pre(std::move(cont.pre)),
                              succ(std::move(cont.succ)),
                              input_mapping(std::move(cont.input_mapping)),
                              output_mapping(std::move(cont.output_mapping)),
                              output_data(std::move(cont.output_data)), read_only(std::move(cont.read_only)),
                              pre_is_cpu(std::move(cont.pre_is_cpu)),
                              is_cpu(cont.is_cpu) {}
};

/**
 * Body struct used in TBB for a source node
 */
struct source_body
{
    std::shared_ptr<context> cont;
    std::unique_ptr<data_io::io_pipe_for_source_node> pipe;

    source_body(std::shared_ptr<context> context, std::unique_ptr<data_io::io_pipe_for_source_node> pipe)
        : cont(
              std::move(
                  context)),
          pipe(std::move(
              pipe)){};

    source_body(const source_body &b) : cont(b.cont)
    {
        pipe = std::move(const_cast<source_body &>(b).pipe);
    }

    bool operator()(data_bucket &db);
};

/**
 * Body class used in TBB for a sink node
 */
template <size_t input_size>
struct sink_body
{
  public:
    sink_body() {}
    sink_body(std::shared_ptr<context> context, std::unique_ptr<data_io::io_pipe_for_sink_node> pipe) : cont(std::move(context)),
                                                                                                        pipe(std::move(pipe)){};

    sink_body(const sink_body &b) : cont(b.cont)
    {
        pipe = std::move(const_cast<sink_body &>(b).pipe);
    }

    data_bucket operator()(std::array<data_bucket, input_size> const &data);

  private:
    std::shared_ptr<context> cont;
    std::unique_ptr<data_io::io_pipe_for_sink_node> pipe;
};

/**
 * Body class used in TBB for a normal node
 */
template <size_t input_size>
struct node_body
{
  public:
    node_body() {}
    node_body(std::shared_ptr<context> context, std::shared_ptr<executable> exec) : cont(std::move(context)),
                                                                                    exec(std::move(exec)){};

    node_body(std::shared_ptr<context> context, std::shared_ptr<executable> exec,
              std::shared_ptr<data_bucket> params) : cont(std::move(context)),
                                                     exec(std::move(exec)), params(std::move(params)){};

    node_body(const node_body &b) : cont(b.cont), exec(b.exec), params(b.params){};

    data_bucket operator()(std::array<data_bucket, input_size> const &data);

  private:
    std::shared_ptr<context> cont;
    std::shared_ptr<executable> exec;
    std::shared_ptr<data_bucket> params;
};

} // namespace task_execution

PVPF_NAMESPACE_END

#endif //POINT_VISION_PIPELINE_FRAMEWORK_BODY_HPP
