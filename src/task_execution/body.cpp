//
// Created by Max on 2018/8/3.
//

#include <pvpf/task_execution/body.hpp>
#include <pvpf/task_execution/memory_manager.hpp>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <pvpf/utils/exception.hpp>

using namespace std;

PVPF_NAMESPACE_BEGIN
namespace task_execution
{
bool source_body::operator()(data_bucket &db)
{
    if (!pipe->is_empty())
    {
        db = pipe->read();
        return true;
    }

    return false;
}
} // namespace task_execution
PVPF_NAMESPACE_END
