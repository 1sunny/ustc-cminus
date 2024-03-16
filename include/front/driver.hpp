#ifndef DRIVER_HH
#define DRIVER_HH

#include <string>
#include <map>
#include <fstream>
#include "syntax_analyzer.hpp"
#include "ast.hpp"
#include "CminusFlexLexer.hpp"

// Conducting the whole scanning and parsing of Calc++.
class driver {
public:
    driver();

    std::map<std::string, int> variables;

    int result{};

    CminusFlexLexer lexer;

    // Run the parser on file F.  Return 0 on success.
    int parse(const std::string &f);

    std::ifstream instream;
    // The name of the file being parsed.
    std::string file;
    // Whether to generate parser debug traces.
    bool trace_parsing;

    // Handling the scanner.
    void scan_begin();

    void scan_end();

    void error(const yy::location &l, const std::string &m);

    // Whether to generate scanner debug traces.
    bool trace_scanning;
    // The token's location used by the scanner.
    // yy::location location;
    std::shared_ptr<AstCompUnit> compUnitResult;
};

#endif // ! DRIVER_HH