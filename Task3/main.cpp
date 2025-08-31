#include <iostream>
#include "fiber.hpp"
#include "scheduler.hpp"

// Define the global scheduler
Scheduler scheduler;

// Fiber function 1
void func1() {
    std::cout << "fiber 1 before yield\n";
    yield();
    std::cout << "fiber 1 after yield\n";
    fiber_exit();
}

// Fiber function 2
void func2() {
    std::cout << "fiber 2 started\n";
    fiber_exit();
}

int main() {
    std::cout << "Main function started\n";

    // Create fiber instances
    Fiber f1(func1);
    Fiber f2(func2);

    // Spawn the fibers in the scheduler
    std::cout << "Main: Spawning fiber 1\n";
    scheduler.spawn(&f1);

    std::cout << "Main: Spawning fiber 2\n";
    scheduler.spawn(&f2);

    // Start the scheduler to run the fibers
    std::cout << "Main: Starting scheduler\n";
    scheduler.do_it();

    std::cout << "Main function finished\n";
    return 0;
}
