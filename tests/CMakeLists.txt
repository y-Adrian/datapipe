# Tests CMakeLists.txt

# 添加测试程序
add_executable(test_buffer test_buffer.c)
target_link_libraries(test_buffer datapipe)

add_executable(test_codec test_codec.c)
target_link_libraries(test_codec datapipe)

add_executable(test_core test_core.c)
target_link_libraries(test_core datapipe)

add_executable(test_platform test_platform.c)
target_link_libraries(test_platform datapipe)

# 注册测试
add_test(NAME BufferTest COMMAND test_buffer)
add_test(NAME CodecTest COMMAND test_codec)
add_test(NAME CoreTest COMMAND test_core)
add_test(NAME PlatformTest COMMAND test_platform)
