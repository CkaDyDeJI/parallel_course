#include "Method.h"
#include "Task26.h"
#include "Random.h"
#include "Timer.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <filesystem>


void input(int& t)
{
    //std::cout << "Enter size of set: ";
    //std::cin >> s;

    std::cout << "Enter number of threads: ";
    std::cin >> t;

    //std::cout << "Enter number (q): ";
    //std::cin >> n;
}


std::vector<int> read_file(const std::string& path)
{
    std::string str = std::filesystem::current_path().string() + "\\..\\Debug\\" + path;
    std::ifstream infile(str, std::ios_base::in);

    int temp;
    std::vector<int> result;

    if (infile.is_open())
    {
        while (infile >> temp)
            result.push_back(temp);

        infile.close();
    }

    std::sort(result.begin(), result.end());

    return result;
}


int main(int argc, char** argv)
{
    int threads;
    
    input(threads);

    std::vector<int> set = read_file("input.txt");
    /*Random rnd;

    for (auto i = 0; i < set.size(); ++i)
        set[i] = rnd.returnRandom(100);*/

    Task26* task = new Task26(set, threads);

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
