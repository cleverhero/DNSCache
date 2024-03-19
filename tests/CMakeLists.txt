include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)

set(HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../lib/)

add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

add_executable(onethread_test onethread_test.cpp)
target_include_directories(onethread_test PUBLIC ${HEADERS_DIR})
target_link_libraries(onethread_test PRIVATE GTest::GTest dnsc_lib)

add_executable(multithreads_test multithreads_test.cpp)
target_include_directories(multithreads_test PUBLIC ${HEADERS_DIR})
target_link_libraries(multithreads_test PRIVATE GTest::GTest dnsc_lib)

add_executable(singleton_test singleton_test.cpp)
target_include_directories(singleton_test PUBLIC ${HEADERS_DIR})
target_link_libraries(singleton_test PRIVATE GTest::GTest dnsc_lib)

add_test(onethread_test onethread_test)
add_test(multithreads_test multithreads_test)
add_test(singleton_test singleton_test)
