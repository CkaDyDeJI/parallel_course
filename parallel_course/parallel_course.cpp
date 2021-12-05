#include "Method.h"
#include "Task18.h"
#include "Random.h"
#include "Timer.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <string>


void input(int& t)
{
    std::cout << "Enter number of threads: ";
    std::cin >> t;
}


std::vector<std::pair<int, int>> read_file(const std::string& path)
{
    std::string str = std::filesystem::current_path().string() + "\\..\\" + path;
    std::ifstream infile(str, std::ios_base::in);

    std::vector<std::pair<int, int>> result;

    if (infile.is_open())
    {
        std::string line;

        while (std::getline(infile, line)) 
        {
            std::istringstream iss(line);

            int temp1, temp2;
            while (iss >> temp1 && iss >> temp2)
                result.push_back({ temp1, temp2 });
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

    Task18* task = new Task18(set, threads);

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
