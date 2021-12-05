#include "Method.h"
#include "Task23.h"
#include "Random.h"
#include "Timer.h"

#include <iostream>
#include <fstream>
#include <filesystem>


void input(int& t)
{
    std::cout << "Enter number of threads: ";
    std::cin >> t;
}


std::vector<int> read_file(const std::string& path)
{
    std::string str = std::filesystem::current_path().string() + "\\..\\" + path;
    std::ifstream infile(str, std::ios_base::in);

    int temp;
    std::vector<int> result;

    if (infile.is_open())
    {
        while (infile >> temp) 
            result.push_back(temp);

        infile.close();
    }

    return result;
}


int main(int argc, char** argv)
{
    int threads;
    
    input(threads);

    std::vector<int> set = read_file("input.txt");

    Task23* task = new Task23(set, threads);

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
