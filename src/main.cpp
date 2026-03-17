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

    std::vector<std::string> inputs;

    for(const auto& entry : std::filesystem::directory_iterator(".cp_test"))
    {
        std::string name = entry.path().filename();

        if (name.find("input_") != std::string::npos)
        {
            inputs.push_back(entry.path());
        }
    }

    std::cout << "Running...\n";
    sort(inputs.begin(), inputs.end());
    
    for (int i = 0; i < inputs.size(); ++i)
    {
        std::cout << "Running test " << i+1 << "...\n";

        auto start = std::chrono::high_resolution_clock::now();
        
        std::string input_file = inputs[i];
        std::string output_file = ".cp_test/user_output.txt";

        std::string run_cmd = "./solution < " + input_file + " > " + output_file;

        int run_status = std::system(run_cmd.c_str());

        if(run_status != 0)
        {
            std::cout << "Test " << i + 1 << ": RUNTIME ERROR\n";
            continue;
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        double runtime = std::chrono::duration<double, std::milli>(end - start).count();

        std::string expected_path = ".cp_test/output_" + std::to_string(i + 1) + ".txt";
        std::string expected;

        if (!std::filesystem::exists(expected_path))
        {
            std::cout << "Missing output file for test " << i+1 << '\n';
        }
        else
        {
            expected = readFile(expected_path);
        }
        
        std::string actual = readFile(output_file);
        
        int line = compareLines(expected, actual);

        if (line == -1)
        {
            std::cout << "\033[32mACCEPTED\033[0m\n";
            std::cout << "Execution time: " << runtime << "ms\n";
        }
        else
        {
            std::cout << "\033[31mWRONG ANSWER\033[0m\n";
            reportError(inputs.size(), i+1, line);
        }

        std::cout << "\n----------------\n";
        std::ofstream(output_file).close();
    }

    return 0;
}

