add_executable(test_logging test_logging.cpp)
target_link_libraries(test_logging common)

install(
    TARGETS test_logging
    RUNTIME DESTINATION bin
)
