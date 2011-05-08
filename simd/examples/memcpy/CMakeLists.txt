add_executable(fast_memcpy fast_memcpy.cpp)
set_property(TARGET fast_memcpy PROPERTY COMPILE_FLAGS "${NT2_FLAGS} -fno-builtin-memcpy")
