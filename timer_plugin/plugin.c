#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include "plugin.h"

#ifndef TASKTIMER_PLUGIN_ENVIRONMENT_VARIABLE_NAME
#define TASKTIMER_PLUGIN_ENVIRONMENT_VARIABLE_NAME "TASKTIMER_EXTERNAL_TOOL_LIBRARY"
#endif

void* get_library(void) {
    /* If, for some reason, we haven't preloaded the libXXX.so
     * library, Go looking for the library, and then the symbol.
     * This assumes that the LD_LIBRARY_PATH will include the
     * path to the library. */

    /* Check for the environment variable */
    const char * libname = getenv(TASKTIMER_PLUGIN_ENVIRONMENT_VARIABLE_NAME);
    /* If not set, return */
    if (libname == NULL) {
        return NULL;
    }
    /* Check to see if we've already loaded it */
    void* handle = dlopen(libname, RTLD_NOLOAD | RTLD_NOW | RTLD_LOCAL);
    if (handle == NULL) {
        /* Library hasn't been loaded, so load it */
        handle = dlopen(libname, RTLD_NOW | RTLD_LOCAL);
    }
    if (handle == NULL) {
        fprintf(stderr, "Unable to load library %s\n%s\n", libname, dlerror());
        return NULL;
    }
    return handle;
}

void* get_symbol(const char * symbol) {
    static void* library_handle = NULL;
    // Do we have the symbol already?
    void* func = dlsym(RTLD_DEFAULT, symbol);
    if (func != NULL) {
        return func;
    }
    // look up the library
    if(library_handle == NULL) {
        library_handle = get_library();
    }
    // Do we have a library to search?
    if (library_handle == NULL) {
        return NULL;
    }
    func = dlsym(library_handle, symbol);
    if (func == NULL) {
        fprintf(stderr, "Unable to load symbol %s\n%s\n", symbol, dlerror());
        return NULL;
    }
    return func;
}

