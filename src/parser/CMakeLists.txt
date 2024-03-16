# FLEX_TARGET(Name FlexInput FlexOutput
#            [COMPILE_FLAGS <string>]
#            [DEFINES_FILE <string>]
#            )
# https://cmake.org/cmake/help/latest/module/FindFLEX.html
flex_target(lex
            lexical_analyzer.l
            ${CMAKE_CURRENT_BINARY_DIR}/lexical_analyzer.cpp
            COMPILE_FLAGS "-v --c++"
        )
#target_include_directories(lex PRIVATE ${PROJECT_SOURCE_DIR}/include/front)

# BISON_TARGET(<Name> <YaccInput> <CodeOutput>
#              [COMPILE_FLAGS <flags>]
#              [DEFINES_FILE <file>]
#              [VERBOSE [<file>]]
#              [REPORT_FILE <file>]
#              )
bison_target(syntax
        syntax_analyzer.y
        ${CMAKE_CURRENT_BINARY_DIR}/syntax_analyzer.cpp
        COMPILE_FLAGS "--debug -Wcounterexamples"
        DEFINES_FILE ${PROJECT_SOURCE_DIR}/include/front/syntax_analyzer.hpp
        )

add_flex_bison_dependency(lex syntax)

add_library(syntax STATIC
  ${BISON_syntax_OUTPUTS}
  ${FLEX_lex_OUTPUTS}
  driver.cpp
)

include_directories(${PROJECT_BINARY_DIR})
add_executable(parser parser.cpp lexical_analyzer.cpp)
target_link_libraries(parser syntax common)
#add_executable(lexer lexer.cpp)
#target_link_libraries(lexer syntax common)

install(
    TARGETS parser
    RUNTIME DESTINATION bin
)

#install(
#    TARGETS lexer
#    RUNTIME DESTINATION bin
#)
