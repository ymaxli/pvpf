//
// Created by Max on 2018/6/23.
//

#include "pvpf/data_io/buffer.h"

PVPF_NAMESPACE_BEGIN
namespace data_io
{

    void Buffer::write(int a) {
        this->a = a;
    }

    int Buffer::read() {
        return a;
    }
}
PVPF_NAMESPACE_END