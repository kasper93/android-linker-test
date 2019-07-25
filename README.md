## Description

Reference counting of the libraries is broken due to invalid group handling starting from Android O. Let's consider following case:

**linker-test** -[dlopen]-> **TestLib** -[DT_NEEDED]-> **BlackBoxLib** -[10 x dlopen]-> **liblog**

**TestLib** -[DT_NEEDED]-> **liblog**

Looking at the linker code I see that whenever library is "needed" by other library it will be added to the same local_group_root which will count refs for that group.

Now let's consider our example. TestLib has dependency on **liblog** and after it is loaded any and all references to liblog will also increese reference count of TestLib which will prevent it from being unloaded if any of the library in dependency graph doesn't close liblog or have it as DT_NEEEDED.

And even if BlackBoxLib handles its dependencies properly, let say by dlclosing everything in global dtor it will not be unloaded since that destructor will not fire, because we already created a dependency cycle.

I prepared simple testcase, please refer to the code for the details. Basically BlackBoxLib doesn't release liblog which prevents TestLib from being unloaded since their reference count is incorrectly counted. **linker-test** dlopens **TestLib** which depends on **BlackBoxLib** and **liblog**. Then **BlackBoxLib** dlopens **liblog** 10 times and at this point it is impossible to unload **TestLib** as its reference count has been increesed while in fact there should be only one reference to it.

The issue in on Android P+. It seems to work ok on Oreo.

### Steps to reproduce
1. Clone test project
1. ndk-build
1. ./run.sh

#### Actual output (Android P+)

```
int main()
BlackBoxLib::BlackBoxLib()
TestLib::TestLib()
void test_lib_entrypoint()
void blackbox_entrypoint()
dlclose(libtest.so) 0: 0
dlclose(libtest.so) 1: 0
dlclose(libtest.so) 2: 0
dlclose(libtest.so) 3: 0
dlclose(libtest.so) 4: 0
dlclose(libtest.so) 5: 0
dlclose(libtest.so) 6: 0
dlclose(libtest.so) 7: 0
dlclose(libtest.so) 8: 0
dlclose(libtest.so) 9: 0
TestLib::~TestLib()
BlackBoxLib::~BlackBoxLib()
dlclose(libtest.so) 10: 0
dlclose(libtest.so) 11: -1
dlerror: dlclose failed: invalid handle: 0xb0887107
END
```


#### Expected output

```
int main()
BlackBoxLib::BlackBoxLib()
TestLib::TestLib()
void test_lib_entrypoint()
void blackbox_entrypoint()
TestLib::~TestLib()
BlackBoxLib::~BlackBoxLib()
dlclose(libtest.so) 0: 0
dlclose(libtest.so) 1: -1
dlerror: dlclose failed: invalid handle: 0x72ccd117
END
```
