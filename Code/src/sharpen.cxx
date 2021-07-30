#include <iostream>
#include <exception>
#include <string>

#ifdef HAS_OPENCV
#include <opencv2/opencv.hpp>
#endif

#include "Image.h"

using namespace std;


void onTrackbar( int, void* );


Image g_input;
Image g_output;
double g_alpha = 1.0;

const int g_alpha_slider_int_max    = 100;
const double g_alpha_slider_double_max = 10;
int g_alpha_slider = 1;


//-----------------------------
int main(int argc, char** argv)
//-----------------------------
{
    try
    {

        if (argc == 3 || argc == 4|| argc == 5)
        {
            // Load the input image
            g_input = argv[1];

            // Retrieve alpha
            string argv_3 = argv[3];
            if (argc >= 4)
            {
                if (argv_3 != "-display")
                {
                    g_alpha = stof(argv_3);
                }
            }

            // Filter the image
            g_output = g_input.sharpen(g_alpha);

#ifdef HAS_OPENCV
            // Display the image
            if (argv_3 == "-display" || argc == 5)
            {
                cv::namedWindow("Sharpening", cv::WINDOW_AUTOSIZE); // Create Window
                cv::createTrackbar( "Alpha", "Sharpening", &g_alpha_slider, g_alpha_slider_int_max, onTrackbar );

                onTrackbar( g_alpha_slider, 0 );

                cv::waitKey(0);
            }
#endif // HAS_OPENCV


            // Save the output
            g_output.save(argv[2]);
        }
        else
        {
            string error_message = string("Usage: ") + argv[0] + " input_image output_image [alpha] [-display]";
            throw error_message;
        }
    }
    catch (const exception& e)
    {
        cerr << "An error occured, see the message below." << endl;
        cerr << e.what() << endl;
        return 1;
    }
    catch (const string& e)
    {
        cerr << "An error occured, see the message below." << endl;
        cerr << e << endl;
        return 2;
    }
    catch (const char* e)
    {
        cerr << "An error occured, see the message below." << endl;
        cerr << e << endl;
        return 3;
    }

    return 0;
}


//---------------------------
void onTrackbar( int, void* )
//---------------------------
{
    // Compute the value of alpha from the slider
    g_alpha = g_alpha_slider_double_max * g_alpha_slider / g_alpha_slider_int_max;

    // Sharpen
    g_output = g_input.sharpen(g_alpha);

    // Normalise for floating-point numbers
    Image display_image = g_output / 255.0;

    // Convert the data into an OpenCV Mat instance.
    cv::Mat cv_image(display_image.getHeight(), display_image.getWidth(), CV_32FC1, (float*) display_image.getPixelPointer());

    // Display the image
    cv::imshow("Sharpening", cv_image);
}

