#pragma once
#include <deque>
#include "fiber.hpp"
#include "context/context.hpp"

// The Scheduler class manages the execution of fibers in a cooperative multitasking system
class Scheduler {
public:
    // Add a new fiber to the scheduler's queue
    void spawn(Fiber* fiber) {
        fibers_.push_back(fiber);  // Add the fiber to the end of the queue
    }

    // Execute all fibers in the queue sequentially
    void do_it() {
        get_context(&main_context_);  // Save the current context (main) to allow returning here later
        while (!fibers_.empty()) {    // Continue until all fibers are executed
            Fiber* fiber = fibers_.front();  // Get the next fiber in the queue
            fibers_.pop_front();      // Remove the fiber from the queue
            current_fiber_ = fiber;   // Set the current fiber being executed
            set_context(&fiber->get_context());  // Switch to the fiber's context and start execution
        }
    }

    // Called when a fiber finishes its task to return to the main context
    void fiber_exit() {
        set_context(&main_context_);  // Restore the main context, resuming execution in the scheduler
    }

    // Retrieve the shared data pointer of the currently running fiber
    int* get_data() {
        return current_fiber_ ? current_fiber_->get_data() : nullptr;  // Return data if a fiber is active
    }

private:
    std::deque<Fiber*> fibers_;       // Queue of fibers to execute
    Context main_context_;            // Stores the execution context of the main function
    Fiber* current_fiber_ = nullptr;  // Pointer to the fiber currently being executed
};

// Global instance of the Scheduler class, enabling fibers to access shared functions
extern Scheduler scheduler;

// Inline helper function for fibers to signal their completion
inline void fiber_exit() { scheduler.fiber_exit(); }

// Inline helper function to access shared data from the currently running fiber
inline int* get_data() { return scheduler.get_data(); }
