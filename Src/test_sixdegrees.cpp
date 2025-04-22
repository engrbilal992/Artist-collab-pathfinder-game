/**
 ** test_sixdegrees.cpp
**
** Project Two: Six Degrees of Collaboration
**
** Purpose:
**   Unit tests for SixDegrees class command processing.
**
** ChangeLog:
**   April 2025: Student
**     
**/

#include <iostream>
#include <sstream>
#include <cassert>
#include <algorithm>

#include "SixDegrees.h"

// Helper function to normalize newlines for Windows compatibility
std::string normalize_newlines(const std::string &input) {
    std::string result = input;
    result.erase(std::remove(result.begin(), result.end(), '\r'), result.end());
    return result;
}

int main() {
    SixDegrees sd;

    // Populate graph with correct format
    std::istringstream data(
        "Ariana Grande\n"
        "Bang Bang\n"
        "*\n"
        "Nicki Minaj\n"
        "Bang Bang\n"
        "Girl On Fire (Inferno Version)\n"
        "*\n"
        "Alicia Keys\n"
        "Girl On Fire (Inferno Version)\n"
        "*\n"
    );
    sd.populate_graph(data);

    // Test 1: BFS valid path
    std::istringstream in1("bfs\nAriana Grande\nAlicia Keys\n");
    std::ostringstream out1;
    sd.run(in1, out1);
    std::string expected1 =
        "\"Alicia Keys\" collaborated with \"Nicki Minaj\" in \"Girl On Fire (Inferno Version)\".\n"
        "\"Nicki Minaj\" collaborated with \"Ariana Grande\" in \"Bang Bang\".\n";
    std::string actual1 = normalize_newlines(out1.str());
    if (actual1 != expected1) {
        std::cerr << "Test 1 failed. Expected:\n" << expected1
                << "Got:\n" << actual1 << std::endl;
    }
    assert(actual1 == expected1);

    // Test 2: DFS valid path
    std::istringstream in2("dfs\nAriana Grande\nAlicia Keys\n");
    std::ostringstream out2;
    sd.run(in2, out2);
    std::string actual2 = normalize_newlines(out2.str());
    if (actual2 != expected1) {
        std::cerr << "Test 2 failed. Expected:\n" << expected1
                << "Got:\n" << actual2 << std::endl;
    }
    assert(actual2 == expected1); // Same path expected

    // Test 3: Unknown artist
    std::istringstream in3("bfs\nUnknown\nAlicia Keys\n");
    std::ostringstream out3;
    sd.run(in3, out3);
    std::string expected3 = "\"Unknown\" was not found in the collaboration graph.\n";
    std::string actual3 = normalize_newlines(out3.str());
    assert(actual3 == expected3);

    // Test 4: Not command with exclude
    std::istringstream in4("not\nAriana Grande\nAlicia Keys\nNicki Minaj\n*\n");
    std::ostringstream out4;
    sd.run(in4, out4);
    std::string expected4 = "No path exists from \"Ariana Grande\" to \"Alicia Keys\".\n";
    std::string actual4 = normalize_newlines(out4.str());
    assert(actual4 == expected4);

    // Test 5: Same artist
    std::istringstream in5("bfs\nAriana Grande\nAriana Grande\n");
    std::ostringstream out5;
    sd.run(in5, out5);
    std::string expected5 = "No path exists.\n";
    std::string actual5 = normalize_newlines(out5.str());
    assert(actual5 == expected5);

    std::cout << "All SixDegrees tests passed!\n";
    return 0;
}