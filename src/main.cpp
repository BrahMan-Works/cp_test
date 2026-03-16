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

std::string trim(const std::string& s)
{
    size_t start = s.find_first_not_of("\n\r\t");
    size_t end = s.find_last_not_of("\n\r\t");

    if (start == std::string::npos) return "";
    return s.substr(start, end - start + 1);
}

void reportError(int total_tests, int test, int line)
{
    if(total_tests > 1)
    {
        std::cout << "Wrong answer at test case " << test << '\n';
    }

    std::cout << "Mismatch at line " << line << '\n';
}

int compare(const std::string& expected_path, const std::string& user_path)
{
    std::ifstream expected(expected_path);
    std::ifstream user(user_path);

    std::string e, u;
    int line = 1;

    while (true)
    {
        bool e_end = !getline(expected, e);
        bool u_end = !getline(user, u);

        if(e_end && u_end) return 0;

        if(e != u) return line;
        line++;
    }
}

bool compareOutputs(const std::string& expected_path, const std::string& user_path) //useless for now
{
    std::ifstream expected(expected_path);
    std::ifstream user(user_path);

    std::string a, b;

    while(true)
    {
        bool e = !(expected >> a);
        bool u = !(user >> b);

        if (e && u) return true;
        if (e || u) return false;
        
        if(a != b) return false;
    }
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
        expected = trim(expected);

        actual = normalize(actual);
        actual = trim(actual);

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
            std::cout << "\033[32mACCEPTED\033[0m\n";
            std::cout << "Execution time: " << runtime << "ms\n";
        }
        else
        {
            std::cout << "\033[31mWRONG ANSWER\033[0m\n";
            int line = compare(expected, actual);
            reportError(inputs.size(), i+1, line);
        }
    }

    return 0;
}

