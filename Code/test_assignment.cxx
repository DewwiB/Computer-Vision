#include "Image.h"
#include "gtest/gtest.h"
#include <iostream>

using namespace std;
//-------------------------------------------------------------------------------------------------------------------------
// Test the RMSE method
TEST(testrmse_ta, LoadPNG)
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
//-------------------------------------------------------------------------------------------------------------------------
// Test the load method
TEST(testzncc_ta, LoadPNG)
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
//-------------------------------------------------------------------------------------------------------------------------
TEST(test_assignment_outputs, LoadPNG)
{
    Image i1("tulips.png");
    Image i2("negative.png");
    
    //assert i1 is identical
    ASSERT_NEAR(i1.getZNCC(i1), 1.0,1e-5);
    ASSERT_NEAR(i1.getZNCC(i2), -1.0,1e-5);
}
//-------------------------------------------------------------------------------------------------------------------------
TEST(conv2d, conv2d)
{
    Image img_kernel({1.,0.,1.,
                    0.,0.,0.,
                    1.,0.,1.
    },3,3);
    
    Image test_img({1.,2.,3.,
                    4.,5.,6.,
                    7.,8.,9.
    },3,3);
    
    Image img = test_img.conv2d(img_kernel);
    
    ASSERT_EQ(img(0,0),5);
    ASSERT_EQ(img(1,0),10);
    ASSERT_EQ(img(2,0),5);
    
    ASSERT_EQ(img(0,1),10);
    ASSERT_EQ(img(1,1),20);
    ASSERT_EQ(img(2,1),10);
    
    ASSERT_EQ(img(0,2),5);
    ASSERT_EQ(img(1,2),10);
    ASSERT_EQ(img(2,2),5);
}
//-------------------------------------------------------------------------------------------------------------------------
// Test the default constructor
TEST(Test_Constructor, DefaultConstructor)
{
    Image test_default_constructor;
    
    ASSERT_EQ(test_default_constructor.getWidth(), 0);
    ASSERT_EQ(test_default_constructor.getHeight(), 0);
    EXPECT_TRUE(test_default_constructor.getPixelPointer() == NULL);
}
//-------------------------------------------------------------------------------------------------------------------------
// Test the constructor from a C array
TEST(Test_Constructor, CArrayConstructor)
{
    float p_c_array[] = {1, 2, 3, 4, 5, 6, 7, 8};
    Image test_c_array_constructor(p_c_array, 4, 2);

    ASSERT_EQ(test_c_array_constructor.getWidth(), 4);
    ASSERT_EQ(test_c_array_constructor.getHeight(), 2);
    
    size_t k = 0;
    for (size_t j = 0; j < test_c_array_constructor.getHeight(); ++j)
    {
        for (size_t i = 0; i < test_c_array_constructor.getWidth(); ++i, ++k)
        {
            ASSERT_EQ(test_c_array_constructor(i, j), p_c_array[k]);
        }
    }
}
//-------------------------------------------------------------------------------------------------------------------------
// Test the constructor from a C++ STL vector
TEST(Test_Constructor, CXXArrayConstructor)
{
    vector<float> p_cxx_array = {1, 2, 3, 4, 5, 6, 7, 8};
    Image test_cxx_array_constructor(p_cxx_array, 2, 4);

    ASSERT_EQ(test_cxx_array_constructor.getWidth(), 2);
    ASSERT_EQ(test_cxx_array_constructor.getHeight(), 4);
    
    size_t k = 0;
    for (size_t j = 0; j < test_cxx_array_constructor.getHeight(); ++j)
    {
        for (size_t i = 0; i < test_cxx_array_constructor.getWidth(); ++i, ++k)
        {
            ASSERT_EQ(test_cxx_array_constructor(i, j), p_cxx_array[k]);
        }
    }
}
//-------------------------------------------------------------------------------------------------------------------------
// Test the constructor from a constant
TEST(Test_Constructor, ConstantConstructor)
{
    Image test_constant_constructor(0.0, 6, 3);
    
    ASSERT_EQ(test_constant_constructor.getWidth(), 6);
    ASSERT_EQ(test_constant_constructor.getHeight(), 3);
    
    for (size_t j = 0; j < test_constant_constructor.getHeight(); ++j)
    {
        for (size_t i = 0; i < test_constant_constructor.getWidth(); ++i)
        {
            ASSERT_EQ(test_constant_constructor(i, j), 0.0);
        }
    }
}
//-------------------------------------------------------------------------------------------------------------------------
// Test the copy constructor
TEST(Test_Constructor, CopyConstructor)
{
    Image test_constant_constructor(0.0, 6, 3);
    Image test_copy_constructor = test_constant_constructor;
    
    ASSERT_EQ(test_constant_constructor.getWidth(), test_copy_constructor.getWidth());
    ASSERT_EQ(test_constant_constructor.getHeight(), test_copy_constructor.getHeight());
    
    for (size_t j = 0; j < test_constant_constructor.getHeight(); ++j)
    {
        for (size_t i = 0; i < test_constant_constructor.getWidth(); ++i)
        {
            ASSERT_EQ(test_constant_constructor(i, j), test_copy_constructor(i, j));
        }
    }
}
//-------------------------------------------------------------------------------------------------------------------------
// Test the load method
TEST(Load_Save, LoadPNG)
{
    Image input_image("tulips.png");

    ASSERT_EQ(input_image.getWidth(), 768);
    ASSERT_EQ(input_image.getHeight(), 512);
    ASSERT_EQ(input_image.getWidth() * input_image.getHeight(), 393216);

    ASSERT_NEAR(input_image.getMean(), 101.540, 1);
    ASSERT_NEAR(input_image.getStdDev(), 59.634, 1);
    ASSERT_NEAR(input_image.getMinValue(), 3, 1);
    ASSERT_NEAR(input_image.getMaxValue(), 242, 1);
}
//-------------------------------------------------------------------------------------------------------------------------

TEST(Load_Save, SavePNG)
{
    Image input_image("tulips.png");
    input_image.save("output.jpg");

    Image output_image("output.jpg");

    ASSERT_EQ(output_image.getWidth(), 768);
    ASSERT_EQ(output_image.getHeight(), 512);
    ASSERT_EQ(output_image.getWidth() * output_image.getHeight(), 393216);

    ASSERT_NEAR(output_image.getMean(), 101.540, 2);
    ASSERT_NEAR(output_image.getStdDev(), 59.634, 2);
    ASSERT_NEAR(output_image.getMinValue(), 3, 2);
    ASSERT_NEAR(output_image.getMaxValue(), 242, 2);
}
//-------------------------------------------------------------------------------------------------------------------------
// Test ASCII files
TEST(Load_Save, LoadSaveASCII)
{
    // Create a 3x2 image
    Image input_image({1, 2, 3, 4, 5, 6}, 3, 2);

    // Save the image in a text file
    input_image.save("output.txt");

    // Load the text file
    Image output_image("output.txt");

    // Check the image size
    ASSERT_EQ(output_image.getWidth(), 3);
    ASSERT_EQ(output_image.getHeight(), 2);
    ASSERT_EQ(output_image.getWidth() * output_image.getHeight(), 6);

    // Check the image stats
    ASSERT_NEAR(output_image.getMean(), (1 + 2 + 3 + 4 + 5 + 6) / 6.0, 1.0 / 6.0);
    ASSERT_NEAR(output_image.getMinValue(), 1, 1.0 / 6.0);
    ASSERT_NEAR(output_image.getMaxValue(), 6, 1.0 / 6.0);

    // Check all the pixel values
    ASSERT_NEAR(output_image(0, 0), 1, 1.0 / 6.0);
    ASSERT_NEAR(output_image(1, 0), 2, 1.0 / 6.0);
    ASSERT_NEAR(output_image(2, 0), 3, 1.0 / 6.0);
    ASSERT_NEAR(output_image(0, 1), 4, 1.0 / 6.0);
    ASSERT_NEAR(output_image(1, 1), 5, 1.0 / 6.0);
    ASSERT_NEAR(output_image(2, 1), 6, 1.0 / 6.0);

}
//-------------------------------------------------------------------------------------------------------------------------
// Test the default constructor
TEST(Operators_ta, FloatImageOperators)
{
    Image input_image({0, 1, 2, 3, 4, 5, 6 , 7}, 4, 2);

    Image image_product = 2 * input_image;
    ASSERT_EQ(image_product.getWidth(), input_image.getWidth());
    ASSERT_EQ(image_product.getHeight(), input_image.getHeight());
    
    Image image_sum = 2 + input_image;
    ASSERT_EQ(image_sum.getWidth(), input_image.getWidth());
    ASSERT_EQ(image_sum.getHeight(), input_image.getHeight());
    
    cout << image_sum << endl;
    size_t k = 0;
    for (size_t j = 0; j < image_product.getHeight(); ++j)
    {
        for (size_t i = 0; i < image_product.getWidth(); ++i, ++k)
        {
            ASSERT_NEAR(image_product(i, j), k * 2, 1e-6);
            ASSERT_NEAR(image_sum(i, j), k + 2, 1e-6);
        }
    }
}
//-------------------------------------------------------------------------------------------------------------------------
