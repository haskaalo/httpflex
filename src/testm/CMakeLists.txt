project(testm)

file(GLOB_RECURSE testmsource *.cpp *.hpp)
include_directories(../main)

find_package(Threads)

add_executable(testm ${testmsource})
target_link_libraries(testm main ${CMAKE_THREAD_LIBS_INIT})