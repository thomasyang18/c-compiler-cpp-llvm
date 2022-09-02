#include <gtest/gtest.h>
#include <string>
#include "parser.hpp"

/** 
 * This fixture and series of tests make sure that the parser always returns true for these inputs
*/

class ParseCorrectFixture : public ::testing::TestWithParam<std::string> {
 protected:
  std::unique_ptr<peg::parser> parser = nullptr;
  ParseCorrectFixture() {
    parser = std::move(new_parser());
  }
};

TEST_P(ParseCorrectFixture, FileParsesCorrectly) {
  ASSERT_TRUE(parser->parse(GetParam()));
}

// Note that this only guarantees that it PARSES correctly, not that it's semantically correct.

INSTANTIATE_TEST_SUITE_P(
  CorrectBasicSubset,
  ParseCorrectFixture,
  ::testing::Values(
    "int main () {}",
    "int main ( ) { }",
    R"(int main () { 
      int x, y, z;
    })",
    "int main() {x=2;}",
    "int main() {int x; x = 2;}",
    "int main() {int x; return x; return y; return z; }",
    "int main() {return x; return y; return z;}",
    "int main() {return x*4+z/3-3*4;}",
    "int main() {return 1+1-1/1*2/3+4;}",
    "int main() {return 1-2<3*4/32>2+3;}",
    "int main() {if (x > 3) return 4;}",
    "int main() {if (1) return 4; }",
    "int main() {if (1) return 1; else return 1;}",    
    "int main() {if (1) if (2) return 2; else return 2; else return 1;}",
    "int main() {while (1) {while (2) x = 3; }}",
    "int main() {int x, y, z; x = 3; y = 2; z = x+y; int a,b,c;}"
  ));

/** 
 * This fixture and series of tests make sure that the parser always returns false for these inputs
*/

class ParseFailFixture : public ::testing::TestWithParam<std::string> {
 protected:
  std::unique_ptr<peg::parser> parser = nullptr;
  ParseFailFixture() {
    parser = std::move(new_parser());
  }
};

TEST_P(ParseFailFixture, FileParsesIncorrectly) {
  ASSERT_FALSE(parser->parse(GetParam()));
}

// Note that this only guarantees that it PARSES correctly, not that it's semantically correct.

INSTANTIATE_TEST_SUITE_P(
  FailBasicSubset,
  ParseFailFixture,
  ::testing::Values(
    "intmain(){}",
    "int main () {int x}"
  ));
