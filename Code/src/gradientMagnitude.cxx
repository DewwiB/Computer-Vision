#include <iostream>
#include <exception>
#include <string>

#include "Image.h"

using namespace std;

int main(int argc, char** argv)
{
    try
    {
        if (argc == 3 || argc == 4)
        {
            Image input(argv[1]);
            Image output(input.gradientMagnitude());

            // Save the image
            std::string output_filename = argv[2];
            std::string capital_filename;

            // Capitalise
            for (int i = 0; i < output_filename.size(); ++i)
                capital_filename += std::toupper(output_filename[i]);

            // There are enough characters for a file extension
            if (std::string(output_filename).size() > 4)
            {
                // Save an ASCII image file: Do not normalise
                if(capital_filename.substr( capital_filename.length() - 4 ) == ".TXT")
                {
                    output.save(output_filename);
                }
                // Save the data using an image file format: Normalise
                else
                {
                    (output.normalise() * 255).save(output_filename);
                }
            }

            // Display the image
            if (argc == 4)
            {
                output.display();
            }
        }
        else
        {
            string error_message = string("Usage: ") + argv[0] + " input_image output_image [-display]";
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
