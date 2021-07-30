#include "Image.h"
#include "gtest/gtest.h"

// Test the load method
TEST(testrmse, LoadPNG)
{

    
    Image I1({1, 1, 1, 1, 1, 1}, 2, 3);
    // I1.getRMSE(I1); <-- must be close to 0
    ASSERT_NEAR(I1.getRMSE(I1), 0.0, 1e-6);

    // I1.getRMSE(I1 * 3); must be close to sqrt(1/6 * 6 * (1 - 3)^2), i.e. 2
    ASSERT_NEAR(I1.getRMSE(I1 * 3), 2.0, 1e-5);

    // I1.getRMSE(I1 * 9); must be close to sqrt(1/6 * 6 * (1 - 9)^2), i.e. 8
    ASSERT_NEAR(I1.getRMSE(I1 * 9), 8.0, 1e-5);
    
    return 0;
}



