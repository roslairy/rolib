#include <gtest/gtest.h>

#pragma comment(lib, "gtestd.lib")

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
    system("pause");
}