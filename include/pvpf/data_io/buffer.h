//
// Created by Max on 2018/6/23.
//

#ifndef DEV_ENV_BUFFER_H
#define DEV_ENV_BUFFER_H
#pragma once

#include "pvpf/pvpf.h"

PVPF_NAMESPACE_BEGIN

    namespace data_io
    {
        class Buffer {
        private: int a;
        public:
            void write(int a);
            int read();
        };
    }

PVPF_NAMESPACE_END

#endif //DEV_ENV_BUFFER_H

