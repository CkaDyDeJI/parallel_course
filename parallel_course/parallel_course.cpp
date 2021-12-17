#include "Task2.h"
#include "Random.h"
#include "Timer.h"

#include <iostream>
#include <fstream>
#include <filesystem>

int a[35010];

void input(int& n, int& k, int& t)
{
    std::cout << "Enter set size: ";
    std::cin >> n;

    std::cout << "Enter number of sections: ";
    std::cin >> k;

    std::cout << "Enter number of threads: ";
    std::cin >> t;

    /*for (int i = 1; i <= n; ++i)
        std::cin >> a[i];*/
}
void read_file(const std::string& path)
{
    std::string str = std::filesystem::current_path().string() + "\\..\\" + path;
    std::ifstream infile(str, std::ios_base::in);

   /* int temp;
    std::vector<int> result;*/

    if (infile.is_open())
    {
        int count = 1;
        while (infile >> a[count])
        {
            ++count;
        }

        infile.close();
    }
}


int main(int argc, char** argv)
{
    int size;
    int sections;
    int threads;
    
    input(size, sections, threads);

    read_file("input.txt");

    Task2 task (a, size, sections, threads);

    Timer t1;
    t1.start();

    task.run(Type::OMP);
    std::cout << "\nresult is: " << task.getResult() << "\ntime: " << t1.elapsed();
    
    /*Timer t2;
    t2.start();

    Task2 task2
    std::cout << "\nresult is: " << task->getResult() << "\ntime: " << t2.elapsed();*/

    return 0;
}
