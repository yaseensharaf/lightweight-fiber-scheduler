//main.cpp
#include <iostream>
#include "fiber.hpp"
#include "scheduler.hpp"

// Define the global scheduler
Scheduler scheduler;

// Fiber function 1
void func1() {
    std::cout << "fiber 1 started" << std::endl;
    int* shared_data = get_data();
    if (shared_data) {
        std::cout << "fiber 1: shared data = " << *shared_data << std::endl;
        (*shared_data)++;  // Increment the shared data
        std::cout << "fiber 1: incremented shared data to " << *shared_data << std::endl;
    }
    fiber_exit();
}

// Fiber function 2
void func2() {
    std::cout << "fiber 2 started" << std::endl;
    int* shared_data = get_data();
    if (shared_data) {
        std::cout << "fiber 2: shared data = " << *shared_data << std::endl;
    }
    fiber_exit();
    
}

int main() {
    std::cout << "Main function started" << std::endl;

    int shared_value = 10;  // Shared data between fibers
    std::cout << "Main: Initial shared value = " << shared_value << std::endl;

    // Create fiber instances with shared data
    Fiber f1(func1, &shared_value);
    Fiber f2(func2, &shared_value);

    // Spawn the fibers in the scheduler
    std::cout << "Main: Spawning fiber 1" << std::endl;
    scheduler.spawn(&f1);

    std::cout << "Main: Spawning fiber 2" << std::endl;
    scheduler.spawn(&f2);

    // Start the scheduler to run the fibers
    std::cout << "Main: Starting scheduler" << std::endl;
    scheduler.do_it();

    std::cout << "Main function finished" << std::endl;
    return 0;
}
