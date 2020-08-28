#include <iostream>

#ifdef DEBUG_BUILD
    #define PRINT_TEST() std::cout << "Hello Monarch Debug!" << std::endl;
#else
    #define PRINT_TEST() std::cout << "Hello Monarch Release!" << std::endl;
#endif // DEBUG

#define LOG(x) std::cout << x << std::endl;
