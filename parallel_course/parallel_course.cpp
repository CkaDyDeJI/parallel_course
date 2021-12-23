#include "Task16.h"
#include "Timer.h"

#include <iostream>
#include <fstream>
#include <filesystem>
#include <mpi.h>

const int MAX_N = 50000;
char A[MAX_N + 10], B[MAX_N + 10];
char A_copy[MAX_N + 10], B_copy[MAX_N + 10];

void input(int& t)
{
    std::cout << "Enter number of threads: ";
    std::cin >> t;
}

void read_file(const std::string& path)
{
    const std::string str = std::filesystem::current_path().string() + path;
    std::ifstream infile(str, std::ios_base::in);

    if (infile.is_open())
    {
        infile >> A;
        infile >> B;

        infile.close();
    }
}

void launch_method(Task16* tsk1, Task16* tsk2, int threads, Type type)
{
    const auto len = strlen(A + 1);

    Timer t;
    t.start();

    strcpy_s(A_copy, A);
    strcpy_s(B_copy, B);

    tsk1->run(type, A, 1, len);
    tsk2->run(type, B, 1, len);

    std::string result;
    if (strcmp(A, B) == 0)
        result = "NO";
    else
        result = "YES";

    const auto time = t.elapsed();

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0)
		std::cout << "\nresult = " << result << "\ntime = " << time << std::endl;

    strcpy_s(A, A_copy);
    strcpy_s(B, B_copy);
}


int main(int argc, char** argv)
{
    int size;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int threads;
    if (size == 1)
        input(threads);
    else
        threads = size;

    read_file("input.txt");

    Task16 task1(threads);
    Task16 task2(threads);

    if ( size == 1)
    {
        std::cout << "Thread";
        launch_method(&task1, &task2, threads, Type::THREAD);
    }

    //////////

    if (size == 1)
    {
        std::cout << "Omp";
        launch_method(&task1, &task2, threads, Type::OMP);
    }

    //////////
    if (size != 1)
    {
        std::cout << "Mpi";
        launch_method(&task1, &task2, threads, Type::MPI);
    }

    MPI_Finalize();

    return 0;
}
