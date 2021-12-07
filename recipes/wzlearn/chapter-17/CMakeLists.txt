add_executable(SkipList SkipList.cc)

target_link_libraries(SkipList
    fmt
)

add_executable(skip_list_test skip_list_test.cc)

target_link_libraries(skip_list_test
    fmt
)
