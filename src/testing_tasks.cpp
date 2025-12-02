#include <testing_tasks.h>

// using namespace osos;
using namespace osos::common;
void printf(char* str, ...);

namespace osos
{
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
            taskManager.sleep(300000);
            printf(" TCC");
            taskManager.sleep(500);
        }
    }

    void TestTask4()
    {
        for (int i = 0; i == 20; i++)
            printf(" A ");
    }

    void TestTask5()
    {
        for (int i = 0; i == 20; i++)
            printf(" TE5(B) ");
    }

    void TestTask6()
    {
        for (int i = 0; i == 18; i++)
        {
            printf(" neisa...");
            taskManager.sleep(300000);
            printf(" T6C. ");
            taskManager.sleep(500);
        }
    }

    void TestTask7()
    {
    }
}