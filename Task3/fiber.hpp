#pragma once
#include "context/context.hpp"
#include <cstdint>

// Fiber class: Represents a lightweight, user-space thread with its own stack and execution context
class Fiber {
public:
    // Constructor: Initializes a fiber with a function pointer and optional shared data
    Fiber(void (*func)(), int* data = nullptr)
        : func_(func), data_(data), stack_(new char[4096]), done_(false) {  // Allocate 4096 bytes for the stack
        sp_ = stack_ + 4096;  // Point to the top of the stack
        sp_ = reinterpret_cast<char*>(reinterpret_cast<uintptr_t>(sp_) & -16L) - 128;  // Align stack to 16 bytes and adjust for Red Zone
        context_.rip = reinterpret_cast<void*>(func_);  // Set the instruction pointer to the fiber's function
        context_.rsp = sp_;  // Set the stack pointer to the top of the aligned stack
    }

    // Destructor: Frees the dynamically allocated stack memory
    ~Fiber() { delete[] stack_; }

    // Returns the execution context of the fiber
    Context& get_context() { 
        return context_; 
    }

    // Returns a pointer to the shared data, if any
    int* get_data() { 
        return data_; 
    }

    // Mark the fiber as completed
    void mark_done() { 
        done_ = true; 
    }

    // Check if the fiber has completed execution
    bool is_done() const { 
        return done_; 
    }

private:
    void (*func_)();       // Pointer to the function executed by the fiber
    Context context_;      // Stores the fiber's CPU context (instruction and stack pointers)
    char* sp_;             // Pointer to the top of the fiber's aligned stack
    char* stack_;          // Dynamically allocated memory for the fiber's stack
    int* data_;            // Optional pointer to shared data for communication between fibers
    bool done_;            // Flag to track if the fiber has completed execution
};
