#include "Random.h"
#include "Timer.h"
#include "Task19.h"

#include <iostream>
#include <fstream>
#include <filesystem>


void input(int& t, long long& n)
{
    std::cout << "Enter number of threads: ";
    std::cin >> t;

    std::cout << "Enter number (q): ";
    std::cin >> n;
}


int main(int argc, char** argv)
{
    int threads;
    long long number;
    
    input(threads, number);

    Task19 task (threads);

    Timer t1;
    t1.start();

    std::cout << "\nresult is: " << task.ompTask(number, 16, 0) << "\ntime: " << t1.elapsed();
    
    Timer t2;
    t2.start();

    std::cout << "\nresult is: " << task.threadStart(number, 16, 0) << "\ntime: " << t2.elapsed();

    return 0;
}
