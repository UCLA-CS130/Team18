#include "gtest/gtest.h"
#include "config_parser.h"
#include <sstream>
#include <string>

using namespace std;

class NginxConfigStringParserTest : public ::testing::Test {
  protected:
  
    //Tests Parse with config in stringstream form
    bool ParseString(string config_string) {
      stringstream config_stream(config_string);
      return _parser.Parse(&config_stream, &_out_config);
    }
  
    //Tests Parse with path to file containing config
    bool ParseConfig(string config_file) {
      return _parser.Parse(config_file.c_str(), &_out_config);
    }

    NginxConfigParser _parser;
    NginxConfig _out_config;
};

TEST_F(NginxConfigStringParserTest, SimpleConfig) {
  EXPECT_TRUE(ParseConfig("example_config"));
}

TEST_F(NginxConfigStringParserTest, NginxExampleConfig) {
  EXPECT_TRUE(ParseConfig("testing_config"));
}

TEST_F(NginxConfigStringParserTest, NonExistingExampleConfig) {
  EXPECT_FALSE(ParseConfig("non_existing_config"));
}

TEST_F(NginxConfigStringParserTest, ZeroStatementConfig) {
  EXPECT_FALSE(ParseString(""));
}

TEST_F(NginxConfigStringParserTest, ZeroStatementCommentConfig) {
  EXPECT_FALSE(ParseString("#Comment"));
}

TEST_F(NginxConfigStringParserTest, OneStatementOneTokenConfig) {
  EXPECT_TRUE(ParseString("foo;"));
  EXPECT_EQ(1, _out_config.statements_.size()) << "Expected 1 statement";
  EXPECT_EQ(1, _out_config.statements_.at(0)->tokens_.size()) << "Expected 1 token";
  EXPECT_EQ("foo", _out_config.statements_.at(0)->tokens_.at(0));
}

TEST_F(NginxConfigStringParserTest, OneStatementOneTokenCommentConfig) {
  EXPECT_TRUE(ParseString("foo; #Comment"));
  EXPECT_EQ(1, _out_config.statements_.size()) << "Expected 1 statement";
  EXPECT_EQ(1, _out_config.statements_.at(0)->tokens_.size()) << "Expected 1 token";
  EXPECT_EQ("foo", _out_config.statements_.at(0)->tokens_.at(0));
}

TEST_F(NginxConfigStringParserTest, UnnecessaryWhitespaceConfig) {
  EXPECT_TRUE(ParseString("  foo           ;"));
  EXPECT_EQ(1, _out_config.statements_.size()) << "Expected 1 statement";
  EXPECT_EQ(1, _out_config.statements_.at(0)->tokens_.size()) << "Expected 1 token";
  EXPECT_EQ("foo", _out_config.statements_.at(0)->tokens_.at(0));
}

TEST_F(NginxConfigStringParserTest, UnnecessaryNewlineConfig) {
  EXPECT_TRUE(ParseString("\n  foo    \n       ;"));
  EXPECT_EQ(1, _out_config.statements_.size()) << "Expected 1 statement";
  EXPECT_EQ(1, _out_config.statements_.at(0)->tokens_.size()) << "Expected 1 token";
  EXPECT_EQ("foo", _out_config.statements_.at(0)->tokens_.at(0));
}

TEST_F(NginxConfigStringParserTest, UnnecessaryTabConfig) {
  EXPECT_TRUE(ParseString("\t  foo    \t       ;"));
  EXPECT_EQ(1, _out_config.statements_.size()) << "Expected 1 statement";
  EXPECT_EQ(1, _out_config.statements_.at(0)->tokens_.size()) << "Expected 1 token";
  EXPECT_EQ("foo", _out_config.statements_.at(0)->tokens_.at(0));
}

TEST_F(NginxConfigStringParserTest, UnnecessaryReturnConfig) {
  EXPECT_TRUE(ParseString("\r  foo    \r       ;"));
  EXPECT_EQ(1, _out_config.statements_.size()) << "Expected 1 statement";
  EXPECT_EQ(1, _out_config.statements_.at(0)->tokens_.size()) << "Expected 1 token";
  EXPECT_EQ("foo", _out_config.statements_.at(0)->tokens_.at(0));
}

TEST_F(NginxConfigStringParserTest, OneStatementMultipleTokenConfig) {
  EXPECT_TRUE(ParseString("foo bar cat;"));
  EXPECT_EQ(1, _out_config.statements_.size()) << "Expected 1 statement";
  EXPECT_EQ(3, _out_config.statements_.at(0)->tokens_.size()) << "Expected 3 tokens";
  EXPECT_EQ("foo", _out_config.statements_.at(0)->tokens_.at(0));
  EXPECT_EQ("bar", _out_config.statements_.at(0)->tokens_.at(1));
  EXPECT_EQ("cat", _out_config.statements_.at(0)->tokens_.at(2));
}

TEST_F(NginxConfigStringParserTest, MultipleStatementOneLineConfig) {
  EXPECT_TRUE(ParseString("foo bar; cat foo; bar cat;"));
  EXPECT_EQ(3, _out_config.statements_.size()) << "Expected 3 statements";
  EXPECT_EQ(2, _out_config.statements_.at(0)->tokens_.size()) << "Expected 2 tokens";
  EXPECT_EQ(2, _out_config.statements_.at(1)->tokens_.size()) << "Expected 2 tokens";
  EXPECT_EQ(2, _out_config.statements_.at(2)->tokens_.size()) << "Expected 2 tokens";
  EXPECT_EQ("foo", _out_config.statements_.at(0)->tokens_.at(0));
  EXPECT_EQ("bar", _out_config.statements_.at(0)->tokens_.at(1));
  EXPECT_EQ("cat", _out_config.statements_.at(1)->tokens_.at(0));
  EXPECT_EQ("foo", _out_config.statements_.at(1)->tokens_.at(1));
  EXPECT_EQ("bar", _out_config.statements_.at(2)->tokens_.at(0));
  EXPECT_EQ("cat", _out_config.statements_.at(2)->tokens_.at(1));
}

TEST_F(NginxConfigStringParserTest, MultipleStatementSeparateLinesConfig) {
  EXPECT_TRUE(ParseString("foo bar;\n cat foo;\nbar cat;"));
  EXPECT_EQ(3, _out_config.statements_.size()) << "Expected 3 statements";
  EXPECT_EQ(2, _out_config.statements_.at(0)->tokens_.size()) << "Expected 2 tokens";
  EXPECT_EQ(2, _out_config.statements_.at(1)->tokens_.size()) << "Expected 2 tokens";
  EXPECT_EQ(2, _out_config.statements_.at(2)->tokens_.size()) << "Expected 2 tokens";
  EXPECT_EQ("foo", _out_config.statements_.at(0)->tokens_.at(0));
  EXPECT_EQ("bar", _out_config.statements_.at(0)->tokens_.at(1));
  EXPECT_EQ("cat", _out_config.statements_.at(1)->tokens_.at(0));
  EXPECT_EQ("foo", _out_config.statements_.at(1)->tokens_.at(1));
  EXPECT_EQ("bar", _out_config.statements_.at(2)->tokens_.at(0));
  EXPECT_EQ("cat", _out_config.statements_.at(2)->tokens_.at(1));
}

TEST_F(NginxConfigStringParserTest, MissingSemiColonConfig) {
  EXPECT_FALSE(ParseString("foo"));
}

TEST_F(NginxConfigStringParserTest, NoTokenConfig) {
  EXPECT_FALSE(ParseString(";"));
}

TEST_F(NginxConfigStringParserTest, OneTokenBracesConfig) {
  EXPECT_TRUE(ParseString("foo { bar; }"));
  EXPECT_EQ(1, _out_config.statements_.size()) << "Expected 1 statements";
}

TEST_F(NginxConfigStringParserTest, NoTokenBeforeBracesConfig) {
  EXPECT_FALSE(ParseString("{ bar }"));
}

TEST_F(NginxConfigStringParserTest, NoSemiColonWithinBracesConfig) {
  EXPECT_FALSE(ParseString("foo { bar }"));
}

TEST_F(NginxConfigStringParserTest, OneTokenNewLineBracesConfig) {
  EXPECT_TRUE(ParseString("foo\n { bar; }"));
  EXPECT_EQ(1, _out_config.statements_.size()) << "Expected 1 statements";
}

TEST_F(NginxConfigStringParserTest, SemiColonBeforeBracesConfig) {
  EXPECT_FALSE(ParseString("foo; { bar; }"));
}

TEST_F(NginxConfigStringParserTest, SemiColonAfterBracesConfig) {
  EXPECT_FALSE(ParseString("foo { bar; } ;"));
}

TEST_F(NginxConfigStringParserTest, EmptyBracesConfig) {
  EXPECT_TRUE(ParseString("foo { }"));
}

TEST_F(NginxConfigStringParserTest, DoubleBracesConfig) {
  EXPECT_TRUE(ParseString("foo { cat { bar; } }"));
}

TEST_F(NginxConfigStringParserTest, UnbalancedMissingRightBraceConfig) {
  EXPECT_FALSE(ParseString("foo { bar;"));
}

TEST_F(NginxConfigStringParserTest, UnbalancedMissingLeftBraceConfig) {
  EXPECT_FALSE(ParseString("foo bar; }"));
}

TEST_F(NginxConfigStringParserTest, UnbalancedRightBracesConfig) {
  EXPECT_FALSE(ParseString("foo { bar; } }"));
}

TEST_F(NginxConfigStringParserTest, UnbalancedLeftBracesConfig) {
  EXPECT_FALSE(ParseString("foo { { bar; }"));
}

TEST_F(NginxConfigStringParserTest, MultipleConsecutiveBracesConfig) {
  EXPECT_TRUE(ParseString("foo { bar; } bar { cat; }"));
}

TEST_F(NginxConfigStringParserTest, OneSingleQuoteConfig) {
  EXPECT_FALSE(ParseString("\'"));
}

TEST_F(NginxConfigStringParserTest, OneDoubleQuoteConfig) {
  EXPECT_FALSE(ParseString("\""));
}

TEST_F(NginxConfigStringParserTest, ConfigToString) {
  EXPECT_TRUE(ParseString("foo { bar; }"));
  EXPECT_EQ("foo {\n  bar;\n}\n", _out_config.ToString()) << "Expected ToString to return different string";
}
