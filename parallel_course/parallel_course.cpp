#include "Task16.h"
#include "Random.h"
#include "Timer.h"

#include <iostream>
#include <fstream>
#include <filesystem>

#include <thread>

void input(int& t)
{
    std::cout << "Enter number of threads: ";
    std::cin >> t;

    /*std::cout << "Enter first string: ";
    std::cin >> first;

    std::cout << "Enter second string: ";
    std::cin >> second;*/
}


void read_file(const std::string& path, std::string& s1, std::string& s2)
{
    std::string str = std::filesystem::current_path().string() + "\\..\\" + path;
    std::ifstream infile(str, std::ios_base::in);

    if (infile.is_open())
    {
        infile >> s1;
        infile >> s2;

        infile.close();
    }
}


int main(int argc, char** argv)
{
    int threads;
    std::string s1;
    std::string s2;
    
    input(threads);

    read_file("input.txt", s1, s2);

    Task16 task1(threads);
    Task16 task2(threads);

    std::string res1;
    std::string res2;

    //Timer t2;
    //t2.start();

    ///*task1.altThread(s1, 0, res1);
    //task2.altThread(s2, 0, res2);*/

    //res1 = task1.runOmp(s1, 0);
    //res2 = task2.runOmp(s2, 0);

    //std::cout << (res1 == res2) << "\ntime: " << t2.elapsed();

    Timer t1;
    t1.start();

    res1 = task1.altOmp(s1, 1);
    res2 = task2.altOmp(s2, 1);

    std::cout << (res1 == res2) << "\ntime: " << t1.elapsed();
    
    

    return 0;
}
