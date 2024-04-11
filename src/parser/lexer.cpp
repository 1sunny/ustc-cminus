#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<syntax_analyzer.hpp>
#include "driver.hpp"

///
int main(int argc, const char **argv) {
     if (argc != 2) {
          printf("usage: lexer input_file\n");
          return 0;
     }

     CminusFlexLexer lexer;
     std::string file = std::string(argv[1]);
     lexer.loc.initialize(&file);
     std::ifstream instream;
     lexer.set_debug(true);

     instream.open(file);
     if (!instream.is_open()) {
          std::cerr << "can't open file: " << file << std::endl;
          exit(1);
     }
     //
     lexer.switch_streams(&instream, nullptr);
     driver driver;

     while (true) {
          yy::parser::symbol_type tok = lexer.yylex(driver);
          std::cout << tok.location << ": " << tok.name() << std::endl;
          if (tok.kind_ == yy::parser::symbol_kind_type::S_END) {
               break;
          }
     }
     return 0;
}
