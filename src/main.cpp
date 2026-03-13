#include <iostream>
#include <fstream>
#include <cstdlib>
#include <chrono>
#include <sstream>

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

    std::system("mkdir -p .cp_test");

    std::string compile_cmd = "g++ " + source + " -O2 -std=c++17 -o solution";
    int compile_status = std::system(compile_cmd.c_str());

    if (compile_status != 0)
    {
        std::cout << "COMPILATION ERROR\n";
        return 1;
    }

    std::cout << "Running...\n";

    auto start = std::chrono::high_resolution_clock::now();

    int run_status = std::system("./solution < .cp_test/input.txt > .cp_test/user_output.txt");

    if (run_status != 0)
    {
        std::cout << "RUNTIME ERROR\n";
        return 1;
    }

    auto end = std::chrono::high_resolution_clock::now();

    double runtime = std::chrono::duration<double, std::milli>(end - start).count();

    std::string expected = readFile(".cp_test/output.txt");
    std::string actual = readFile(".cp_test/user_output.txt");

    expected = normalize(expected);
    actual = normalize(actual);

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
        std::cout << "Expected:\n" << expected << '\n';
        std::cout << "Got:\n" << actual << '\n';
    }

    return 0;
}

