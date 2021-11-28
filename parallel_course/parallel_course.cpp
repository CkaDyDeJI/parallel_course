#include "Method.h"
#include "Task29.h"
#include "Random.h"
#include "Timer.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <string>


void input(int& t)
{
    std::cout << "Enter number of threads: ";
    std::cin >> t;
}


std::vector<std::vector<int>> read_file(const std::string& path)
{
    std::string str = std::filesystem::current_path().string() + "\\..\\" + path;
    std::ifstream infile(str, std::ios_base::in);

    int count = 0;
    std::string line;
    std::vector<std::vector<int>> result(2);
    
    if (infile.is_open())
    {
        for (int i = 0; i < 2; ++i)
        {
            std::getline(infile, line);
            std::istringstream iss(line);

            int temp;
            while (iss >> temp)
                result[i].push_back(temp);
        }

        infile.close();
    }

    return result;
}


int main(int argc, char** argv)
{
    int threads;
    
    input(threads);

    auto set = read_file("input.txt");
    std::vector<int> a = set[0];
    std::vector<int> b = set[1];
    int sum = 0;

    for (int i = 0; i < b.size(); ++i)
        sum += a[i] * b[i];

    Task29* task = new Task29(a, b, threads, sum);

    MethodHandler handler;
    
    Method* method_exec = handler.getMethod(METHOD::OMP);

    Timer t1;
    t1.start();
    method_exec->exec(task);

    std::cout << "\nresult is: " << task->getResult() << "\ntime: " << t1.elapsed();

    method_exec = handler.getMethod(METHOD::THREAD);
    
    Timer t2;
    t2.start();
    method_exec->exec(task);

    std::cout << "\nresult is: " << task->getResult() << "\ntime: " << t2.elapsed();

    return 0;
}
