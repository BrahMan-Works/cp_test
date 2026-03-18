#include <iostream>
#include <algorithm>
#include <fstream>
#include <cstdlib>
#include <chrono>
#include <sstream>
#include <filesystem>
#include <vector>

std::string readFile(const std::string& path)
{
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::vector<std::string> normalizeLines(const std::string& text)
{
    std::stringstream ss(text);
    std::string line;
    std::vector<std::string> lines;

    while (getline(ss, line))
    {
        while (!line.empty() && isspace(line.back()))
        {
            line.pop_back();
        }

        lines.push_back(line);
    }

    while (!lines.empty() && lines.front().empty())
    {
        lines.erase(lines.begin());
    }

    while (!lines.empty() && lines.back().empty())
    {
        lines.pop_back();
    }

    return lines;
}

void reportError(int total_tests, int test, int line)
{
    if(total_tests > 1)
    {
        std::cout << "Wrong answer at test case " << test << '\n';
    }

    std::cout << "Mismatch at line " << line << '\n';
}

int compareLines(const std::string& expected, const std::string& actual)
{
    auto e_lines = normalizeLines(expected);
    auto a_lines = normalizeLines(actual);

    int n = std::max(e_lines.size(), a_lines.size());

    for (int i = 0; i < n; ++i)
    {
        if ((i >= e_lines.size()) || (i >= a_lines.size()))
        {
            return i+1;
        }
        if (e_lines[i] != a_lines[i])
        {
            return i+1;
        }
    }

    return -1;
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: cp_test <file.cpp>\n";
        return 1;
    }

    std::string source = argv[1];

    std::cout << "Compiling...\n";

    {
        int sdjkfhsdjfh = std::system("mkdir -p .cp_test");
        // just to eliminate the warning
    }

    std::string compile_cmd = "g++ " + source + " -O2 -std=c++17 -o solution";

    if (std::system(compile_cmd.c_str()) != 0)
    {
        std::cout << "COMPILATION ERROR\n";
        return 1;
    }

    std::vector<int> tests;

    for (int i = 1; ; ++i)
    {
        std::string in = ".cp_test/" + std::to_string(i) + ".in";
        
        if (!std::filesystem::exists(in)) break;
        tests.push_back(i);
    }

    std::cout << "Running...\n";
    
    for (int t : tests)
    {
        std::cout << "Running test " << t << "...\n";

        std::string input_path = ".cp_test/" + std::to_string(t) + ".in";
        std::string output_path = ".cp_test/" + std::to_string(t) + ".out";
        
        std::string output_file = ".cp_test/user_output.txt";

        auto start = std::chrono::high_resolution_clock::now();
        
        std::string run_cmd = "./solution < " + input_path + " > " + output_file;

        int run_status = std::system(run_cmd.c_str());

        if(run_status != 0)
        {
            std::cout << "Test " << t << ": RUNTIME ERROR\n";
            continue;
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        double runtime = std::chrono::duration<double, std::milli>(end - start).count();

        std::string expected_path = ".cp_test/" + std::to_string(t) + ".out";
        std::string expected;
        std::string actual = readFile(output_file);
        
        if (!std::filesystem::exists(expected_path))
        {
            std::cout << "Test " << t << " (no expected output)\n";
            std::cout << actual << '\n';
        }
        else
        {
            expected = readFile(expected_path);
        }
        
        int line = compareLines(expected, actual);

        if (line == -1)
        {
            std::cout << "\033[32mACCEPTED\033[0m\n";
            std::cout << "Execution time: " << runtime << "ms\n";
        }
        else
        {
            std::cout << "\033[31mWRONG ANSWER\033[0m\n";
            reportError(tests.size(), t, line);
        }

        std::cout << "\n----------------\n";
        std::ofstream(output_file).close();
    }

    return 0;
}

