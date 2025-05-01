#include "dr_api.h"
#include "drwrap.h"
#include <string.h>
#include <stdint.h>
#include "drmgr.h"

#define MAX_ALLOCS 4096

typedef struct {
    void *addr;
    size_t size;
    bool freed;
} alloc_t;

static alloc_t allocs[MAX_ALLOCS];
static int leak_count = 0;
static int double_free_count = 0;

static void event_exit(void);
static void wrap_malloc_pre(void *wrapcxt, void **user_data);
static void wrap_malloc_post(void *wrapcxt, void *user_data);
static void wrap_free_pre(void *wrapcxt, void **user_data);
static void track_allocation(void *addr, size_t size);
static bool mark_freed(void *addr);

DR_EXPORT void 
dr_client_main(client_id_t id, int argc, const char *argv[]) {
    dr_set_client_name("Memory Bug Detector", "");
    drmgr_init();
    drwrap_init();
    dr_register_exit_event(event_exit);

    if (!drwrap_wrap_ex((app_pc)malloc, wrap_malloc_pre, wrap_malloc_post, NULL, 0)) {
        dr_fprintf(STDERR, "Failed to wrap malloc\n");
    }

    if (!drwrap_wrap_ex((app_pc)free, wrap_free_pre, NULL, NULL, 0)) {
        dr_fprintf(STDERR, "Failed to wrap free\n");
    }
    if (!drwrap_wrap_ex((app_pc)calloc, wrap_malloc_pre, wrap_malloc_post, NULL, 0)) {
        dr_fprintf(STDERR, "Failed to wrap calloc\n");
    }
    if (!drwrap_wrap_ex((app_pc)realloc, wrap_malloc_pre, wrap_malloc_post, NULL, 0)) {
        dr_fprintf(STDERR, "Failed to wrap realloc\n");
    }
}

static void 
wrap_malloc_pre(void *wrapcxt, void **user_data) {
    *user_data = (void *)(uintptr_t)drwrap_get_arg(wrapcxt, 0);
}

static void 
wrap_malloc_post(void *wrapcxt, void *user_data) {
    void *ret = drwrap_get_retval(wrapcxt);
    size_t size = (size_t)(uintptr_t)user_data;
    if (ret != NULL) {
        track_allocation(ret, size);
    }
}

static void 
wrap_free_pre(void *wrapcxt, void **user_data) {
    void *ptr = drwrap_get_arg(wrapcxt, 0);
    if (!mark_freed(ptr)) {
        dr_fprintf(STDERR, "Double free detected at %p\n", ptr);
        double_free_count++;
    }
}

static void 
track_allocation(void *addr, size_t size) {
    for (int i = 0; i < MAX_ALLOCS; i++) {
        if (allocs[i].addr == NULL) {
            allocs[i].addr = addr;
            allocs[i].size = size;
            allocs[i].freed = false;
            return;
        }
    }
    dr_fprintf(STDERR, "Allocation list full!\n");
}

static bool 
mark_freed(void *addr) {
    for (int i = 0; i < MAX_ALLOCS; i++) {
        if (allocs[i].addr == addr) {
            if (allocs[i].freed)
                return false;
            allocs[i].freed = true;
            return true;
        }
    }
    return true; // assume not tracked, not double-freed by us
}

static void 
event_exit(void) {
    for (int i = 0; i < MAX_ALLOCS; i++) {
        if (allocs[i].addr != NULL && !allocs[i].freed) {
            dr_fprintf(STDERR, "Memory leak detected at %p (%lu bytes)\n",
                       allocs[i].addr, (unsigned long)allocs[i].size);
            leak_count++;
        }
    }

    dr_fprintf(STDERR, "Summary:\nMemory leaks: %d\nDouble frees: %d\n",
               leak_count, double_free_count);
}
