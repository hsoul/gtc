add_executable(consitent_hash_example example.cc)

target_link_libraries(consitent_hash_example
    fmt
)

add_executable(consitent_hash_test test.cc)

target_link_libraries(consitent_hash_test
    fmt
)
