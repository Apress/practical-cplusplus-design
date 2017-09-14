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

#include "CliTest.h"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstdio>

using std::ifstream;
using std::string;
using std::ostream;
using std::ostringstream;
using std::vector;

namespace {

void checkFile(ifstream& ifs)
{
    bool valid{ifs};
    QVERIFY(valid);
}

string fixupPath(const string& path)
{
#ifdef WIN32
    // inefficient, but in test code...who cares
    ostringstream oss;
    for(auto c : path)
    {
        if(c == '/') oss << R"(\\)";
        else oss << c;
    }

    return oss.str();
#else
    return path;
#endif
}

// Windows and linux don't print exponents the same way
// windows prints e-00x while linux prints e-0x. To simplify
// comparisons, as lines are read, this function replaces
// e-00x with e-0x
void rationalizeExponent(string& s)
{
    string::size_type pos = s.find("e-00");
    if(pos != string::npos)
    {
        s.erase(pos + 2, 1);
    }
}

}

std::string CliTest::path()
{
    string t = CLI_TEST_DIR;
    t += "/testCases/";
    return t;
}

void CliTest::runCliOnFile(const string& in, const string& out)
{
    ostringstream cmd;
    string exe(fixupPath("./pdCalc"));
    cmd << exe << " --batch " << in << " " << out;
    int result = system( cmd.str().c_str() );

    QCOMPARE(result, 0);

    return;
}

void CliTest::verifyOutput(const std::string& result, const std::string& baseline)
{
    vector<string> res{ vectorizeFile(result) };
    vector<string> base{ vectorizeFile(baseline) };

    QCOMPARE(res, base);

    return;
}

std::vector<std::string> CliTest::vectorizeFile(const std::string& fname)
{
    ifstream ifs{ fname.c_str() };
    checkFile(ifs);

    vector<string> tmp;
    string line;
    while( std::getline(ifs, line, '\n') )
    {
        rationalizeExponent(line);
        tmp.emplace_back( line );
    }

    return tmp;
}

void CliTest::removeFile(const std::string& f)
{
    int rem = remove( f.c_str() );
    QCOMPARE(rem, 0);

    return;
}

void CliTest::runTest(const std::string& name)
{

    string input = path() + "input" + name + ".txt";
    string output = path() + "output" + name + ".txt";
    string baseline = path() + "baseline" + name + ".txt";

    runCliOnFile(input, output);
    verifyOutput(output, baseline);
    removeFile(output);
}

void CliTest::testCli1()
{
    runTest("Cli1");

    return;
}

void CliTest::testCli2()
{
    runTest("Cli2");

    return;
}
