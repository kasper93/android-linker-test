#include <dlfcn.h>
#include <stdio.h>

struct BlackBoxLib
{
    BlackBoxLib()
    {
        puts(__PRETTY_FUNCTION__);
    }

    ~BlackBoxLib()
    {
        puts(__PRETTY_FUNCTION__);
    }
} a;


extern "C"
{

#define DLOPEN_COUNT 10

__attribute__((visibility("default")))
void blackbox_entrypoint()
{
    puts(__PRETTY_FUNCTION__);

    void* libs[DLOPEN_COUNT];
    for (int i = 0; i < DLOPEN_COUNT; ++i)
        libs[i] = dlopen("liblog.so", RTLD_NOW | RTLD_LOCAL);

#if 0
    for (int i = 0; i < DLOPEN_COUNT; ++i)
    {
        void* lib = libs[i];
        if (lib)
            dlclose(lib);
    }
#endif
}

}
