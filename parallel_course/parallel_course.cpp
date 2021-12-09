#include "Method.h"
#include "Task2.h"
#include "Random.h"
#include "Timer.h"

#include <iostream>
#include <fstream>
#include <filesystem>

int a[35010];

void input(int& n, int& t)
{
    std::cout << "Enter set size: ";
    std::cin >> n;

    std::cout << "Enter number of threads: ";
    std::cin >> t;

    for (int i = 1; i <= n; ++i)
        std::cin >> a[i];
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
    int size;
    int threads;
    
    input(size, threads);

    //std::vector<int> set = read_file("input.txt");

    Task2 task (a, size, threads);

    Timer t1;
    t1.start();

    task.run(Type::THREAD);
    std::cout << "\nresult is: " << task.getResult() << "\ntime: " << t1.elapsed();
    
    /*Timer t2;
    t2.start();
    method_exec->exec(task);

    std::cout << "\nresult is: " << task->getResult() << "\ntime: " << t2.elapsed();*/

    return 0;
}
