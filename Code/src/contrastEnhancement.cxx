#include <iostream>
#include <exception>
#include <string>

#include "Image.h"

using namespace std;

int main(int argc, char** argv)
{
    try
    {
        if (argc == 3)
        {
            Image input(argv[1]);

            input -= input.getMinValue();
            input /= input.getMaxValue();
            input *= 255.0;

            // Same as
            // input = 255.0 * (input - input.getMinValue()) /
            //     (input.getMaxValue() - input.getMinValue());


            // Same as
            // input = 255.0 * input.normalise();

            input.save(argv[2]);
        }
        else
        {
            string error_message = string("Usage: ") + argv[0] + string(" input_image output_image");
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
