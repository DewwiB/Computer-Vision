
#include "Image.h"
#include "gtest/gtest.h"

// Test the load method
TEST(testzncc, LoadPNG)
{
    Image I1({1, 2, 3, 4, 5, 6}, 2, 3);
    Image I2(!I1); // Negative of I1
    Image I3({6, 6, 6, 0, 0, 0}, 2, 3); // A two-tone image

    // Same image
    // I1.getZNCC(I1); <-- must be close to 1
    ASSERT_NEAR(I1.getZNCC(I1), 1.0, 1e-5);

    // I1.getZNCC(10. + 4. * I1); <-- must be close to 1
    ASSERT_NEAR(I1.getZNCC(10. + 4. * I1), 1.0, 1e-5);

    // Negative image
    // I1.getZNCC(I2); <-- must be close to -1
    ASSERT_NEAR(I1.getZNCC(I2), -1.0, 1e-5);

    // I1.getZNCC(10. + 4. * I2); <-- must be close to -1
    ASSERT_NEAR(I1.getZNCC(10. + 4. * I2), -1.0, 1e-6);

    // Different image
    // I1.getZNCC(I3); <-- must be between -1 and 1
    double value1 = I1.getZNCC(I3);
    ASSERT_GT(value1, -1.0);
    ASSERT_LT(value1, 1.0);

    // I1.getZNCC(10. + 4. * I3); <-- must be between -1 and 1
    double value2 = I1.getZNCC(10. + 4. * I3);
    ASSERT_GT(value2, -1.0);
    ASSERT_LT(value2, 1.0);

}



