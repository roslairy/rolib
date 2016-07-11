#include <cpptest/cpptest.h>
#include "test.h"

#pragma comment(lib, "cpptest.lib")
//#pragma comment(lib, "rolib.lib")

int main(int argc, char* argv[])
{
    RolibTestSuite test;
    Test::TextOutput output(Test::TextOutput::Verbose);
    test.run(output);
    system("pause");
}