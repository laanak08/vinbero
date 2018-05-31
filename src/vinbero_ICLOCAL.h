#ifndef _VINBERO_ICLOCAL_H
#define _VINBERO_ICLOCAL_H

#include <vinbero_common/vinbero_common_Module.h>
#include <vinbero_common/vinbero_common_ClData.h>

#define VINBERO_ICLOCAL_FUNCTIONS \
int vinbero_ICLOCAL_init(struct vinbero_common_Module* module, struct vinbero_common_ClData* clData, void* args[]); \
int vinbero_ICLOCAL_destroy(struct vinbero_common_Module* module, struct vinbero_common_ClData* clData)

#define VINBERO_ICLOCAL_FUNCTION_POINTERS \
int (*vinbero_ICLOCAL_init)(struct vinbero_common_Module*, struct vinbero_common_ClData*, void*[]); \
int (*vinbero_ICLOCAL_destroy)(struct vinbero_common_Module*, struct vinbero_common_ClData*)

struct vinbero_ICLOCAL_Interface {
    VINBERO_ICLOCAL_FUNCTION_POINTERS;
};

#define VINBERO_ICLOCAL_DLSYM(interface, dlHandle, ret) do { \
    VINBERO_COMMON_MODULE_DLSYM(interface, dlHandle, vinbero_ICLOCAL_init, ret); \
    if(*ret < 0) break; \
    VINBERO_COMMON_MODULE_DLSYM(interface, dlHandle, vinbero_ICLOCAL_destroy, ret); \
    if(*ret < 0) break; \
} while(0)

#endif
