#include <dlfcn.h>
#include <stdio.h>

void foo();

#define LIBNAME "libtest.so"

int main()
{
    puts(__PRETTY_FUNCTION__);

    auto *lib = dlopen(LIBNAME, RTLD_NOW | RTLD_LOCAL);
    if (!lib)
    {
        printf("Failed to load" LIBNAME ": %s!\n", dlerror());
        return 1;
    }

    auto *foo_f = reinterpret_cast<decltype(&foo)>(dlsym(lib, "test_lib_entrypoint"));
    if (!foo_f)
    {
        puts("Cannot find foo() in" LIBNAME "!");
        return 2;
    }

    foo_f();

    int ret = 0;
    int count = 0;
    do
    {
        ret = dlclose(lib);
        printf("dlclose(" LIBNAME ") %d: %d\n", count++, ret);
    } while (ret == 0);
    printf("dlerror: %s\n", dlerror());

    puts("END");

    return 0;
}
