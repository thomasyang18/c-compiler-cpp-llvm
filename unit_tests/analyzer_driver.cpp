#include <gtest/gtest.h>
#include <string>
#include "analyzer.hpp"
#include "parser.hpp"

/** 
 * This fixture and series of tests make sure that the parser always returns true for these inputs
*/

class AnalyzeCorrectFixture : public ::testing::TestWithParam<std::string> {
 protected:
  std::unique_ptr<peg::parser> parser = nullptr;
  AnalyzeCorrectFixture() {
    parser = std::move(new_parser());
    parser->enable_ast();
    reset_driver();
  }
};

TEST_P(AnalyzeCorrectFixture, FileAnalyzesCorrectly) {
  std::shared_ptr<peg::Ast> ast;
  ASSERT_TRUE(parser->parse(GetParam(), ast));
  ast = parser->optimize_ast(ast);
  EXPECT_NO_THROW(definition_ordering(ast));
}

// Note that this only guarantees that it PARSES correctly, not that it's semantically correct.

INSTANTIATE_TEST_SUITE_P(
  CorrectBasicSubsetAnalyze,
  AnalyzeCorrectFixture,
  ::testing::Values(
    "int main(){int x, y;}",
    "int main(){int x; int y; int z;}",
    "int main(){if (1) {int x; x = 3;} else {int x; x = 4;}}"
  ));

/** 
 * This fixture and series of tests make sure that the parser always returns false for these inputs
*/

class AnalyzeFailFixture : public ::testing::TestWithParam<std::string> {
 protected:
  std::unique_ptr<peg::parser> parser = nullptr;
  AnalyzeFailFixture() {
    parser = std::move(new_parser());
    parser->enable_ast();
    reset_driver();
  }
};

TEST_P(AnalyzeFailFixture, FileAnalyzesIncorrectly) {
  std::shared_ptr<peg::Ast> ast;
  ASSERT_TRUE(parser->parse(GetParam(), ast));
  ast = parser->optimize_ast(ast);
  EXPECT_ANY_THROW(definition_ordering(ast));
}

// Note that this only guarantees that it PARSES correctly, not that it's semantically correct.

INSTANTIATE_TEST_SUITE_P(
  FailBasicSubsetAnalyze,
  AnalyzeFailFixture,
  ::testing::Values(
    "int main(){int y, y;}",
    "int main(){int x; x = 3; int x;}",
    "int main(){x = 3; int x;}",
    "int main(){x = 2;}"
  ));
