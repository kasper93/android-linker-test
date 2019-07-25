#include <stdio.h>

struct TestLib
{
    TestLib()
    {
        puts(__PRETTY_FUNCTION__);
    }

    ~TestLib()
    {
        puts(__PRETTY_FUNCTION__);
    }
} a;


extern "C"
{

void blackbox_entrypoint();

__attribute__((visibility("default")))
void test_lib_entrypoint()
{
    puts(__PRETTY_FUNCTION__);

    blackbox_entrypoint();
}

}
