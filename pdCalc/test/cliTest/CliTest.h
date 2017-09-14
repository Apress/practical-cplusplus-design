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

#ifndef CLI_TEST_H
#define CLI_TEST_H

#include <QtTest/QtTest>
#include <iostream>
#include <string>
#include <vector>

class CliTest : public QObject
{
    Q_OBJECT
private slots:
    void testCli1();
    void testCli2();

private:
    void runCliOnFile(const std::string& in, const std::string& out);
    void verifyOutput(const std::string& result, const std::string& baseline);
    std::vector<std::string> vectorizeFile(const std::string& fname);
    void removeFile(const std::string& f);
    std::string path();
    void runTest(const std::string&);
};

#endif
