//
// Created by Icay on 21/06/2018.
//

#ifndef PVPF_H
#define PVPF_H

#ifndef PVPF_NAMESPACE
#define PVPF_NAMESPACE pvpf
#endif
#ifndef PVPF_NAMESPACE_BEGIN
#define PVPF_NAMESPACE_BEGIN namespace PVPF_NAMESPACE {
#endif
#ifndef PVPF_NAMESPACE_END
#define PVPF_NAMESPACE_END }
#endif

// development mode
#define DEV_MODE

#ifdef DEV_MODE
#define private public
#endif

#define MAX_SUCCESSOR 8

#endif //PVPF_H
