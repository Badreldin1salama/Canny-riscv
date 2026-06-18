#include <gtest/gtest.h>

// المايسترو الخاص باختبارات GoogleTest
int main(int argc, char **argv) {
    std::cout << "==========================================\n";
    std::cout << "🚀 Starting Professional Canny Test Suite\n";
    std::cout << "==========================================\n";
    
    ::testing::InitGoogleTest(&argc, argv);
    
    return RUN_ALL_TESTS();
}
