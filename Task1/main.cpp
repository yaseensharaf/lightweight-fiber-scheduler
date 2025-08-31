#include <iostream>
#include "context/context.hpp"  // Assuming context.hpp provides the correct context switching functions

// Global context for `goo`
Context gooContext;

// Implementation of `foo` function
void foo() {
    std::cout << "you called foo" << std::endl;
    set_context(&gooContext);  // Switch to `gooContext`
}

// Implementation of `goo` function
void goo() {
    std::cout << "you called goo" << std::endl;
    exit(0);  // End the program after goo finishes
}

int main() {
    // Allocate stacks for `foo` and `goo`, each with 4096 bytes
    char fooStack[4096];
    char gooStack[4096];

    // Align stack pointers to 16 bytes and account for the Red Zone
    char* fooSP = fooStack + sizeof(fooStack);  // Point to the top of `foo` stack
    fooSP = reinterpret_cast<char*>(reinterpret_cast<uintptr_t>(fooSP) & -16L);  // Align to 16 bytes
    fooSP -= 128;  // Account for 128-byte Red Zone

    char* gooSP = gooStack + sizeof(gooStack);  // Point to the top of `goo` stack
    gooSP = reinterpret_cast<char*>(reinterpret_cast<uintptr_t>(gooSP) & -16L);  // Align to 16 bytes
    gooSP -= 128;  // Account for 128-byte Red Zone

    // Set up contexts for `foo` and `goo`
    Context fooContext;
    fooContext.rip = reinterpret_cast<void*>(foo);  // Set instruction pointer to `foo`
    fooContext.rsp = fooSP;  // Set stack pointer for `foo`

    gooContext.rip = reinterpret_cast<void*>(goo);  // Set instruction pointer to `goo`
    gooContext.rsp = gooSP;  // Set stack pointer for `goo`

    // Start execution in `foo`
    set_context(&fooContext);

    // This line will not be reached because `goo` exits the program
    return 0;
}