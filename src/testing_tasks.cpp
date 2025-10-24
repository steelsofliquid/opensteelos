#include <testing_tasks.h>

// using namespace osos;
using namespace osos::common;
void printf(char* str, ...);

namespace osos{
  void TestTask1()
  {
      while(true)
          printf("A");
  }

  void TestTask2()
  {
    while(true)
      printf("TT2B");
  }

  void TestTask3()
  {
    while(true)
    {
      printf("n3isa");
      taskManager.sleep(30);
    }
  }
}