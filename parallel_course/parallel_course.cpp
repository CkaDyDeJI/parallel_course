#include "Method.h"
#include "Random.h"
#include "Timer.h"

#include <iostream>
#include <fstream>
#include <filesystem>

#include "Task4.h"


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

    MethodHandler handler;

    Task4* task1 = new Task4(set, threads);
    Method* method_exec = handler.getMethod(METHOD::OMP);

    Timer t1;
    t1.start();
    method_exec->exec(task1);

    std::cout << "\nresult is: " << task1->getResult() << "\ntime: " << t1.elapsed();

    Task4* task2 = new Task4(set, threads);
    method_exec = handler.getMethod(METHOD::THREAD);
    
    Timer t2;
    t2.start();
    method_exec->exec(task2);

    std::cout << "\nresult is: " << task2->getResult() << "\ntime: " << t2.elapsed();

    return 0;
}
