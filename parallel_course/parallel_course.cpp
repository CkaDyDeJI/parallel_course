#include "Task2.h"
#include "mpi.h"
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
    MPI_Init(&argc, &argv);

    int size;
    int sections;
    int threads = 0;

    size = 35000; sections = 100; //mpi
    //input(size, sections, threads); //не mpi

    read_file("input.txt");

    Task2 task (a, size, sections, threads);

    Timer t1;
    t1.start();

    task.run(Type::MPI);
    std::cout << "\nresult is: " << task.getResult() << "\ntime: " << t1.elapsed();

    MPI_Finalize();
    
    /*Timer t2;
    t2.start();

    Task2 task2
    std::cout << "\nresult is: " << task->getResult() << "\ntime: " << t2.elapsed();*/

    return 0;
}
