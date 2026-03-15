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

std::string normalize(const std::string& text)
{
    std::string result;
    for (char c : text)
    {
        if (c != '\r') result += c;
    }
    return result;
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

        std::string expected = readFile(".cp_test/output_" + std::to_string(i + 1) + ".txt");

        std::string actual = readFile(output_file);

        expected = normalize(expected);
        actual = normalize(actual);

        auto end = std::chrono::high_resolution_clock::now();
        double runtime = std::chrono::duration<double, std::milli>(end - start).count();
        
        if (expected.empty())
        {
            std::cout << actual << '\n';
            std::cout << "Execution time: " << runtime << "ms\n";
            return 0;
        }

        if (expected == actual)
        {
            std::cout << "ACCEPTED\n";
            std::cout << "Execution time: " << runtime << "ms\n";
        }
        else
        {
            std::cout << "WRONG ANSWER\n\n";
        }
    }

    return 0;
}

