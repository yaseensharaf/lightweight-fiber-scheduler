#pragma once
#include <deque>
#include "fiber.hpp"
#include "context/context.hpp"
#include <iostream>

// Scheduler class: Manages execution of fibers in a cooperative multitasking system
class Scheduler {
public:
    // Add a new fiber to the scheduler's queue
    void spawn(Fiber* fiber) {
        fibers_.push_back(fiber);  // Append the fiber to the end of the queue
    }

    // Execute all fibers in the queue sequentially
    void do_it() {
        get_context(&main_context_);  // Save the current (main) context for later restoration
        while (!fibers_.empty()) {    // Continue until all fibers have been executed
            Fiber* fiber = fibers_.front();  // Get the next fiber in the queue
            fibers_.pop_front();             // Remove the fiber from the front of the queue
            current_fiber_ = fiber;          // Set the current fiber being executed

            std::cout << "Scheduler: Running fiber\n";
            swap_context(&main_context_, &fiber->get_context());  // Switch to the fiber's context

            // If the fiber is not completed, re-add it to the queue for future execution
            if (!fiber->is_done()) {
                fibers_.push_back(fiber);
            }
        }
    }

    // Mark the current fiber as completed and return control to the main context
    void fiber_exit() {
        current_fiber_->mark_done();  // Mark the current fiber as done
        std::cout << "Scheduler: Fiber completed\n";
        set_context(&main_context_);  // Restore the main context
    }

    // Yield execution of the current fiber and return control to the scheduler
    void yield() {
        std::cout << "Scheduler: Fiber yielded\n";
        swap_context(&current_fiber_->get_context(), &main_context_);  // Save fiber state and restore main context
    }

    // Retrieve shared data from the currently running fiber
    int* get_data() {
        return current_fiber_ ? current_fiber_->get_data() : nullptr;  // Return the fiber's shared data pointer if available
    }

private:
    std::deque<Fiber*> fibers_;       // Queue of fibers to be executed
    Context main_context_;            // Stores the main program's execution context
    Fiber* current_fiber_ = nullptr;  // Pointer to the fiber currently being executed
};

// Global instance of the Scheduler class, allowing fibers to access shared functions
extern Scheduler scheduler;

// Helper function for fibers to signal completion and return control to the scheduler
inline void fiber_exit() { 
    scheduler.fiber_exit(); }

// Helper function to allow a fiber to yield control voluntarily
inline void yield() { scheduler.yield(); }

// Helper function to access shared data from the currently running fiber
inline int* get_data() { return scheduler.get_data(); }
