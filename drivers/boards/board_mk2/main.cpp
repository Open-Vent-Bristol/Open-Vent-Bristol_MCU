#include <thread>
#include <cstdio>
#include <cstring>

#include "drivers/system.hpp"

extern void application_main(void);

int main(void)
{
  System::initialize();
  application_main();
}