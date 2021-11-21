#include "Method.h"
#include "Task13.h"
#include "Random.h"
#include "Timer.h"

#include <iostream>
#include <fstream>
#include <filesystem>


void input(int& t, int& n)
{
    //std::cout << "Enter size of set: ";
    //std::cin >> s;

    std::cout << "Enter number of threads: ";
    std::cin >> t;

    std::cout << "Enter number (q): ";
    std::cin >> n;
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
    int number;
    
    input(threads, number);

    std::vector<int> set = read_file("input.txt");
    /*Random rnd;

    for (auto i = 0; i < set.size(); ++i)
        set[i] = rnd.returnRandom(100);*/

    Task13* task = new Task13(set, number, threads);

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
