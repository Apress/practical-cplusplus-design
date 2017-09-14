// Copyright 2016 Adam B. Singer
// Contact: PracticalDesignBook@gmail.com
//
// This file is part of pdCalc.
//
// pdCalc is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// pdCalc is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with pdCalc; if not, see <http://www.gnu.org/licenses/>.

#include "TokenizerTest.h"
#include "src/utilities/Tokenizer.h"
#include <iostream>
#include <sstream>

using std::endl;
using std::cout;
using std::vector;
using std::string;
using std::ostringstream;
using std::istringstream;

void TokenizerTest::assertTokenizerMatches(const std::vector<string>& tokens, const pdCalc::Tokenizer& tokenizer)
{
    QCOMPARE( tokens.size(), tokenizer.nTokens() );

    auto j = tokens.begin();
    for(auto i : tokenizer)
    {
        QCOMPARE(i, *j);
        ++j;
    }

    for(size_t i = 0; i < tokenizer.nTokens(); ++i)
    {
        QCOMPARE(tokens[i], tokenizer[i]);
    }
}

void TokenizerTest::testTokenizationFromString()
{
    vector<string> tokens = {"7.3454", "8.21", "+", "dup", "dup", "*", "-", "4.35", "tan" };

    string str;
    for(size_t i = 0; i < tokens.size(); ++i)
    {
        str += tokens[i] + (i == 4 ? "\n" : " ");
    }

    pdCalc::Tokenizer tokenizer(str);

    assertTokenizerMatches(tokens, tokenizer);

    return;
}

void TokenizerTest::testTokenizationFromStream()
{
    vector<string> tokens = {"7.3454", "8.21", "sin", "dup", "dup", "/", "pow", "4.35", "arctan" "-18.4" "neg" "root"};

    ostringstream oss;
    for(size_t i = 0; i < tokens.size(); ++i)
    {
        oss << tokens[i] + (i % 3 == 0 ? "\n" : " ");
    }

    istringstream iss( oss.str() );

    pdCalc::Tokenizer tokenizer(iss);

    assertTokenizerMatches(tokens, tokenizer);

    return;
}

