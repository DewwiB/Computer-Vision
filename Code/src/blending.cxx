#include <iostream>
#include <exception>
#include <string>
#include <sstream>
#include <iomanip>

#include "Image.h"

using namespace std;

int main(int argc, char** argv)
{
    try
    {
        if (argc == 5)
        {
            Image image1(argv[1]);
            Image image2(argv[2]);

            string number_of_frames_str = argv[3];

            int number_of_frames = stoi(number_of_frames_str);

            for (int i = 0; i < number_of_frames; ++i)
            {
                // Compute alpha
                float alpha = float(i) / (number_of_frames - 1);

                // Blend the images
                Image blend = blending(alpha, image1, image2);

                // Create a filename with leading 0s
                std::ostringstream filename;
                filename << argv[4] << "_" << std::setw(number_of_frames_str.size() - 1) << std::setfill('0') << i << ".png";

                // Save the image
                blend.save(filename.str());
            }
        }
        else
        {
            string error_message = "Usage: ";
            error_message += argv[0];
            error_message += " input1 input2 NUM output";
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

