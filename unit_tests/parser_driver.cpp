#include <gtest/gtest.h>
#include <peglib.h>
#include <memory>
#include <string>
#include <fstream>
#include <stdexcept>

std::string get_file_as_string(std::string file){
  std::ifstream t(file);
  if (!t.is_open()){
    throw new std::invalid_argument("File " + file + " was not found");
  }
  std::stringstream buffer;
  buffer << t.rdbuf();
  return buffer.str();
}

class ParserTestFixture : public ::testing::TestWithParam<std::string> {
 protected:
  std::unique_ptr<peg::parser> parser = nullptr;
  ParserTestFixture() {
    parser.reset(new peg::parser(get_file_as_string("../grammar.peg")));
  }
  void SetUp() override {
    // Can use GetParam() here.
  }
};

TEST_P(ParserTestFixture, FileParsesCorrectly) {
  ASSERT_TRUE(parser->parse(GetParam()));
}

INSTANTIATE_TEST_SUITE_P(
  ParserTests,
  ParserTestFixture,
  ::testing::Values(
    "1+2"
  ));