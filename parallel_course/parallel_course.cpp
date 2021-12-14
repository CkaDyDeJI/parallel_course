#include "Task16.h"
#include "Timer.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <mpi.h>

const int MAX_N = 50000;
char A[MAX_N + 10], B[MAX_N + 10];

void input(int& t)
{
    t = 4;
    // std::cout << "Enter number of threads: ";
    // std::cin >> t;
}

void read_file(const std::string& path)
{
    const std::string str = std::filesystem::current_path().string() + "\\..\\" + path;
    std::ifstream infile(str, std::ios_base::in);

    if (infile.is_open())
    {
        infile >> A;
        infile >> B;

        infile.close();
    }
}

void launch_method(int threads, Type type)
{
    read_file("input.txt");

    Task16 task1(threads);
    Task16 task2(threads);

    const auto len = strlen(A + 1);

    Timer t;
    t.start();
    
    task1.run(type, A, 1, len);
    task2.run(type, B, 1, len);

    std::string result;
    if (strcmp(A, B) == 0)
        result = "NO";
    else
        result = "YES";

    const auto time = t.elapsed();

    std::cout << "\nresult = " << result << "\ntime = " << time << std::endl;
}


int main(int argc, char** argv)
{
    int threads;
    input(threads);
    
    {
        launch_method(threads, Type::THREAD);
    }

    //////////

    {
        launch_method(threads, Type::OMP);
    }

    //////////

    {
        MPI_Init(&argc, &argv);
        launch_method(threads, Type::MPI);
        MPI_Finalize();
    }

    return 0;
}
