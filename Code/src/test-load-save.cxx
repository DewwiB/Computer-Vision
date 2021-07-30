#include "Image.h"
#include "gtest/gtest.h"

// Test the load method
TEST(LoadSave, LoadPNG)
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


TEST(LoadSave, SavePNG)
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

// Test ASCII files
TEST(LoadSave, LoadSaveASCII)
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
