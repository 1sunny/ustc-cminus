#include "driver.hpp"
#include "syntax_analyzer.hpp"

driver::driver() : trace_parsing(false), trace_scanning(false) {}

int driver::parse(const std::string &f) {
  file = f;
  lexer.loc.initialize(&file);
  scan_begin();
  yy::parser parse(*this);
  parse.set_debug_level(trace_parsing);
  int res = parse();
  scan_end();
  return res;
}

void driver::scan_begin() {
  lexer.set_debug(trace_scanning);

  instream.open(file);
  if (!instream.is_open()) {
    std::cerr << "can't open file: " << file << std::endl;
    exit(1);
  }
  //
  lexer.switch_streams(&instream, nullptr);
}

void driver::scan_end() {
  instream.close();
}

// .y发现错误后调用
void driver::error(const yy::location &l, const std::string &m) {
  std::cerr << l << ": " << m << std::endl;
  exit(1);
}