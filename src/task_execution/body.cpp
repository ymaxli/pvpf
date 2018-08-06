//
// Created by Max on 2018/8/3.
//

#include <pvpf/task_execution/body.hpp>
#include <pvpf/task_execution/memory_manager.hpp>

using namespace std;

PVPF_NAMESPACE_BEGIN
namespace task_execution
{
bool source_body::operator()(data_bucket &db)
{
    if (!pipe->is_empty())
    {
        db = std::move(pipe->read());
    }
    else
    {
        return false;
    }
}
} // namespace task_execution
PVPF_NAMESPACE_END
