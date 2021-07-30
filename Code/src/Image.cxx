#include <sstream>
#include <stdexcept>      // std::out_of_range
#include <cfloat> //FLT_MAX

#include <algorithm> // For std::min
#include <cstring> // For toupper
#include <fstream> // For ofstream and ifstream
#include <cmath> // for sqrt

#ifdef HAS_OPENCV
#include <opencv2/opencv.hpp>
#endif


#include "Image.h"


//------------------------------------------------------
Image operator*(float aValue, const Image& anInputImage)
//------------------------------------------------------
{
    Image temp = anInputImage;
    
    float* p_data = temp.getPixelPointer();
    size_t number_of_pixels = temp.getWidth() * temp.getHeight();
    
    for (size_t i = 0; i < number_of_pixels; ++i)
    {
        *p_data++ *= aValue;
    }
    
    return temp;
}


//------------------------------------------------------
Image operator+(float aValue, const Image& anInputImage)
//------------------------------------------------------
{
    Image temp = anInputImage;
    
    float* p_data = temp.getPixelPointer();
    size_t number_of_pixels = temp.getWidth() * temp.getHeight();
    
    for (size_t i = 0; i < number_of_pixels; ++i)
    {
        *p_data++ += aValue;
    }
    
    return temp;
}


//--------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& anOutputStream, const Image& anImage)
//--------------------------------------------------------------------------
{
    // Output all the rows
    for (unsigned row = 0; row < anImage.getHeight(); ++row)
    {
        // Output all the columns
        for (unsigned col = 0; col < anImage.getWidth(); ++col)
        {
            // Output the corresponding pixel
            anOutputStream << anImage(col, row);

            // Add a space charater if needed
            if (col < anImage.getWidth() - 1)
            {
                anOutputStream << " ";
            }
        }

        // Add a end of line if needed
        if (row < anImage.getHeight() - 1)
        {
            anOutputStream << std::endl;
        }
    }

    // Return the stream
    return anOutputStream;
}


//---------------------------
Image::Image():
//---------------------------
    m_width(0),
    m_height(0),
    m_min_pixel_value(0),
    m_max_pixel_value(0),
    m_average_pixel_value(0),
    m_stddev_pixel_value(0),
    m_stats_up_to_date(true)
//---------------------------
{}


//-------------------------------------------------------
Image::Image(const Image& anImage):
//-------------------------------------------------------
    m_pixel_data(anImage.m_pixel_data),
    m_width(anImage.m_width),
    m_height(anImage.m_height),
    m_min_pixel_value(anImage.m_min_pixel_value),
    m_max_pixel_value(anImage.m_max_pixel_value),
    m_average_pixel_value(anImage.m_average_pixel_value),
    m_stddev_pixel_value(anImage.m_stddev_pixel_value),
    m_stats_up_to_date(anImage.m_stats_up_to_date)
//-------------------------------------------------------
{}


//----------------------------------------------------------------
Image::Image(const float* anImage, size_t aWidth, size_t aHeight):
//----------------------------------------------------------------
    m_pixel_data(anImage, anImage + aWidth * aHeight),
    m_width(aWidth),
    m_height(aHeight),
    m_min_pixel_value(0),
    m_max_pixel_value(0),
    m_average_pixel_value(0),
    m_stddev_pixel_value(0),
    m_stats_up_to_date(false)
//----------------------------------------------------------------
{}


//----------------------------------------------------------------------------------------------
Image::Image(const std::vector<float>& anImage, size_t aWidth, size_t aHeight):
//----------------------------------------------------------------------------------------------
    m_pixel_data(anImage),
    m_width(aWidth),
    m_height(aHeight),
    m_min_pixel_value(0),
    m_max_pixel_value(0),
    m_average_pixel_value(0),
    m_stddev_pixel_value(0),
    m_stats_up_to_date(false)
//----------------------------------------------------------------------------------------------
{}


//----------------------------------------------------------------------------------------------
Image::Image(float aConstant, size_t aWidth, size_t aHeight):
//----------------------------------------------------------------------------------------------
    m_pixel_data(aWidth * aHeight, aConstant),
    m_width(aWidth),
    m_height(aHeight),
    m_min_pixel_value(aConstant),
    m_max_pixel_value(aConstant),
    m_average_pixel_value(aConstant),
    m_stddev_pixel_value(0),
    m_stats_up_to_date(true)
//----------------------------------------------------------------------------------------------
{}


//----------------------------------------------------------------------------------------------
Image::Image(const char* aFilename):
//----------------------------------------------------------------------------------------------
    m_width(0),
    m_height(0),
    m_min_pixel_value(0),
    m_max_pixel_value(0),
    m_average_pixel_value(0),
    m_stddev_pixel_value(0),
    m_stats_up_to_date(true)
//----------------------------------------------------------------------------------------------
{
    load(aFilename);
}


//----------------------------------------------------------------------------------------------
Image::Image(const std::string& aFilename):
//----------------------------------------------------------------------------------------------
    m_width(0),
    m_height(0),
    m_min_pixel_value(0),
    m_max_pixel_value(0),
    m_average_pixel_value(0),
    m_stddev_pixel_value(0),
    m_stats_up_to_date(true)
//----------------------------------------------------------------------------------------------
{
    load(aFilename);
}


//----------------------------------------------------------------------------------------------
Image& Image::operator=(const Image& anInputImage)
//----------------------------------------------------------------------------------------------
{
    m_pixel_data = anInputImage.m_pixel_data;
    m_width = anInputImage.m_width;
    m_height = anInputImage.m_height;
    m_min_pixel_value = anInputImage.m_min_pixel_value;
    m_max_pixel_value = anInputImage.m_max_pixel_value;
    m_average_pixel_value = anInputImage.m_average_pixel_value;
    m_stddev_pixel_value = anInputImage.m_stddev_pixel_value;
    m_stats_up_to_date = anInputImage.m_stats_up_to_date;

    return *this;
}


//----------------------------------------------------------------------------------------------
Image& Image::operator=(const char* aFileName)
//----------------------------------------------------------------------------------------------
{
    load(aFileName);
    return *this;
}


//----------------------------------------------------------------------------------------------
Image& Image::operator=(const std::string& aFileName)
//----------------------------------------------------------------------------------------------
{
    load(aFileName);
    return *this;
}


//----------------------------------------------------------------------------------------------
void Image::load(const char* aFilename)
//----------------------------------------------------------------------------------------------
{
    std::string temp_filename = aFilename;
        std::string capital_filename;

        // Capitalise
        for (int i = 0; i < temp_filename.size(); ++i)
            capital_filename += std::toupper(temp_filename[i]);

        if (std::string(aFilename).size() > 4)
        {
            // Load a text file
            if(capital_filename.substr( capital_filename.length() - 4 ) == ".TXT")
            {
                // Open the file
                std::ifstream input_file (aFilename);

                // The file is not open
                if (!input_file.is_open())
                {
                    // Format a nice error message
                    std::stringstream error_message;
                    error_message << "ERROR:" << std::endl;
                    error_message << "\tin File:" << __FILE__ << std::endl;
                    error_message << "\tin Function:" << __FUNCTION__ << std::endl;
                    error_message << "\tat Line:" << __LINE__ << std::endl;
                    error_message << "\tMESSAGE: Can't open " << aFilename << std::endl;
                    throw std::runtime_error(error_message.str());
                }

                // Empty the image
                m_pixel_data.clear();
                m_width = 0;
                m_height = 0;

                // Load the data into a vector
                std::string line;
                int number_of_rows(0);
                int number_of_columns(0);

                // Read evely line
                while (std::getline(input_file, line))
                {
                    number_of_columns = 0;
                    float intensity;
                    std::stringstream line_parser;
                    line_parser << line;
                    while (line_parser >> intensity)
                    {
                        m_pixel_data.push_back(intensity);
                        ++number_of_columns;
                    }
                    ++number_of_rows;
                }

                // Wrong number of pixels
                if (number_of_rows * number_of_columns != m_pixel_data.size())
                {
                    // Format a nice error message
                    std::stringstream error_message;
                    error_message << "ERROR:" << std::endl;
                    error_message << "\tin File:" << __FILE__ << std::endl;
                    error_message << "\tin Function:" << __FUNCTION__ << std::endl;
                    error_message << "\tat Line:" << __LINE__ << std::endl;
                    error_message << "\tMESSAGE: The file " << aFilename << " is invalid" << std::endl;
                    throw std::runtime_error(error_message.str());
                }

                // Allocate memory for file content
                m_width = number_of_columns;
                m_height = number_of_rows;
                m_stats_up_to_date = false;
            }
            // Use OpenCV
            else
            {
    #ifdef HAS_OPENCV
                // Open the image in greyscale
                cv::Mat temp_image = cv::imread( aFilename, cv::IMREAD_GRAYSCALE);

                // The image did not load
                if ( !temp_image.data )
                {
                    // Format a nice error message
                    std::stringstream error_message;
                    error_message << "ERROR:" << std::endl;
                    error_message << "\tin File:" << __FILE__ << std::endl;
                    error_message << "\tin Function:" << __FUNCTION__ << std::endl;
                    error_message << "\tat Line:" << __LINE__ << std::endl;
                    error_message << "\tMESSAGE: Can't open " << aFilename << std::endl;
                    throw std::runtime_error(error_message.str());
                }

                // Save the size of the image
                m_width = temp_image.cols;
                m_height = temp_image.rows;
                m_pixel_data.resize(m_width * m_height);

                // Copy the pixel data
                cv::Mat img_float;
                temp_image.convertTo(img_float, CV_32F);
                for (int i = 0; i < m_width * m_height; ++i)
                {
                    int x = i / m_width;
                    int y = i % m_width;

                    m_pixel_data[i] = img_float.at<float>(x, y);
                }


                // The statistics is not up-to-date
                m_stats_up_to_date = false;
    #else
                // Format a nice error message
                std::stringstream error_message;
                error_message << "ERROR:" << std::endl;
                error_message << "\tin File:" << __FILE__ << std::endl;
                error_message << "\tin Function:" << __FUNCTION__ << std::endl;
                error_message << "\tat Line:" << __LINE__ << std::endl;
                error_message << "\tMESSAGE: OpenCV not supported" << std::endl;
                throw std::runtime_error(error_message.str());
    #endif
            }
        }
        // Don't know the file type
        else
        {
            // Format a nice error message
            std::stringstream error_message;
            error_message << "ERROR:" << std::endl;
            error_message << "\tin File:" << __FILE__ << std::endl;
            error_message << "\tin Function:" << __FUNCTION__ << std::endl;
            error_message << "\tat Line:" << __LINE__ << std::endl;
            error_message << "\tMESSAGE: Can't open " << aFilename << ", I don't understand the file type." << std::endl;
            throw std::runtime_error(error_message.str());
        }
}


//----------------------------------------------------------------------------------------------
void Image::load(const std::string& aFilename)
//----------------------------------------------------------------------------------------------
{
    load(aFilename.c_str());
}


//----------------------------------------------------------------------------------------------
void Image::save(const char* aFilename)
//----------------------------------------------------------------------------------------------
{
    std::string temp_filename = aFilename;
        std::string capital_filename;

        // Capitalise
        for (int i = 0; i < temp_filename.size(); ++i)
            capital_filename += std::toupper(temp_filename[i]);

        if (std::string(aFilename).size() > 4)
        {
            // Load a text file
            if(capital_filename.substr( capital_filename.length() - 4 ) == ".TXT")
            {
                // Open the file
                std::ofstream output_file (aFilename);

                // The file is not open
                if (!output_file.is_open())
                {
                    // Format a nice error message
                    std::stringstream error_message;
                    error_message << "ERROR:" << std::endl;
                    error_message << "\tin File:" << __FILE__ << std::endl;
                    error_message << "\tin Function:" << __FUNCTION__ << std::endl;
                    error_message << "\tat Line:" << __LINE__ << std::endl;
                    error_message << "\tMESSAGE: Can't open " << aFilename << std::endl;
                    throw std::runtime_error(error_message.str());
                }

                // Write content to file
                float* p_data(getPixelPointer());
                for (unsigned int j(0); j < m_height; ++j)
                {
                    for (unsigned int i(0); i < m_width; ++i)
                    {
                        output_file << *p_data++;

                        // This is not the last pixel of the line
                        if (i < m_width - 1)
                        {
                            output_file << " ";
                        }
                    }

                    // This is not the last line
                    if (j < m_height - 1)
                    {
                        output_file << std::endl;
                    }
                }
            }
            // Use OpenCV
            else
            {
    #ifdef HAS_OPENCV
                // Convert the data into an OpenCV Mat instance.
                cv::Mat temp_image(m_height, m_width, CV_32FC1, (float*)getPixelPointer());

                // Write the data
                cv::imwrite(aFilename, temp_image);

    #else
                // Format a nice error message
                std::stringstream error_message;
                error_message << "ERROR:" << std::endl;
                error_message << "\tin File:" << __FILE__ << std::endl;
                error_message << "\tin Function:" << __FUNCTION__ << std::endl;
                error_message << "\tat Line:" << __LINE__ << std::endl;
                error_message << "\tMESSAGE: OpenCV not supported" << std::endl;
                throw std::runtime_error(error_message.str());
    #endif
            }
        }
        // Don't know the file type
        else
        {
            // Format a nice error message
            std::stringstream error_message;
            error_message << "ERROR:" << std::endl;
            error_message << "\tin File:" << __FILE__ << std::endl;
            error_message << "\tin Function:" << __FUNCTION__ << std::endl;
            error_message << "\tat Line:" << __LINE__ << std::endl;
            error_message << "\tMESSAGE: Can't save " << aFilename << ", I don't understand the file type." << std::endl;
            throw std::runtime_error(error_message.str());
        }
}


//----------------------------------------------------------------------------------------------
void Image::save(const std::string& aFilename)
//----------------------------------------------------------------------------------------------
{
    save(aFilename.c_str());
}


//----------------------------------------------------------------------------------------------
const float& Image::operator()(size_t col, size_t row) const
//----------------------------------------------------------------------------------------------
{
    // Check if the coordinates are valid, if not throw an error
    if (col < 0 || col >= m_width || row < 0 || row >= m_height)
    {
        // Format a nice error message
        std::stringstream error_message;
        error_message << "ERROR:" << std::endl;
        error_message << "\tin File:" << __FILE__ << std::endl;
        error_message << "\tin Function:" << __FUNCTION__ << std::endl;
        error_message << "\tat Line:" << __LINE__ << std::endl;
        error_message << "\tMESSAGE: Pixel(" << col << ", " << row << ") does not exist. The image size is: " << m_width << "x" << m_height << std::endl;

        // Throw an exception
        throw std::out_of_range(error_message.str());
    }

    return m_pixel_data[row * m_width + col];
}


//----------------------------------------------------------------------------------------------
float& Image::operator()(size_t col, size_t row)
//----------------------------------------------------------------------------------------------
{
    // Check if the coordinates are valid, if not throw an error
    if (col < 0 || col >= m_width || row < 0 || row >= m_height)
    {
        // Format a nice error message
        std::stringstream error_message;
        error_message << "ERROR:" << std::endl;
        error_message << "\tin File:" << __FILE__ << std::endl;
        error_message << "\tin Function:" << __FUNCTION__ << std::endl;
        error_message << "\tat Line:" << __LINE__ << std::endl;
        error_message << "\tMESSAGE: Pixel(" << col << ", " << row << ") does not exist. The image size is: " << m_width << "x" << m_height << std::endl;

        // Throw an exception
        throw std::out_of_range(error_message.str());
    }

    // To be on the safe side, turn the flag off
    m_stats_up_to_date = false;

    return m_pixel_data[row * m_width + col];
}


//----------------------------------------------------------------------------------------------
size_t Image::getWidth() const
//----------------------------------------------------------------------------------------------
{
    return m_width;
}


//----------------------------------------------------------------------------------------------
size_t Image::getHeight() const
//----------------------------------------------------------------------------------------------
{
    return m_height;
}


//----------------------------------------------------------------------------------------------
const float* Image::getPixelPointer() const
//----------------------------------------------------------------------------------------------
{
    // There are pixels
    if (m_pixel_data.size() && m_width && m_height)
        return &m_pixel_data[0];
    // There is no pixel
    else
        return 0;
}

//----------------------------------------------------------------------------------------------
float* Image::getPixelPointer()
//----------------------------------------------------------------------------------------------
{
    // To be on the safe side, turn the flag off
    m_stats_up_to_date = false;

    // There are pixels
    if (m_pixel_data.size() && m_width && m_height)
        return &m_pixel_data[0];
    // There is no pixel
    else
        return 0;
}


//----------------------------------------------------------------------------------------------
Image Image::operator+(float aValue) const
//----------------------------------------------------------------------------------------------
{
    Image temp = *this;
    
    for (size_t i = 0; i < m_width * m_height; ++i)
    {
        temp.m_pixel_data[i] += aValue;
    }
    
    return temp;
}


//----------------------------------------------------------------------------------------------
Image Image::operator-(float aValue) const
//----------------------------------------------------------------------------------------------
{
    Image temp = *this;
    
    for (size_t i = 0; i < m_width * m_height; ++i)
    {
        temp.m_pixel_data[i] -= aValue;
    }
    
    return temp;
}


//----------------------------------------------------------------------------------------------
Image Image::operator*(float aValue) const
//----------------------------------------------------------------------------------------------
{
    Image temp = *this;
    
    for (size_t i = 0; i < m_width * m_height; ++i)
    {
        temp.m_pixel_data[i] *= aValue;
    }
    
    return temp;
}


//----------------------------------------------------------------------------------------------
Image Image::operator/(float aValue) const
//----------------------------------------------------------------------------------------------
{
    Image temp = *this;
    
    for (size_t i = 0; i < m_width * m_height; ++i)
    {
        temp.m_pixel_data[i] /= aValue;
    }
    
    return temp;
}


//----------------------------------------------------------------------------------------------
Image& Image::operator+=(float aValue)
//----------------------------------------------------------------------------------------------
{
    *this = *this + aValue;
    return *this;
}


//----------------------------------------------------------------------------------------------
Image& Image::operator-=(float aValue)
//----------------------------------------------------------------------------------------------
{
    *this = *this - aValue;
    return *this;
}


//----------------------------------------------------------------------------------------------
Image& Image::operator*=(float aValue)
//----------------------------------------------------------------------------------------------
{
    *this = *this * aValue;
    return *this;
}


//----------------------------------------------------------------------------------------------
Image& Image::operator/=(float aValue)
//----------------------------------------------------------------------------------------------
{
    *this = *this / aValue;
    return *this;
}


//----------------------------------------------------------------------------------------------
Image Image::normalise()
//----------------------------------------------------------------------------------------------
{
    return (*this - getMinValue()) / (getMaxValue() - getMinValue());
}

//Image Image::operator!() const;


//----------------------------------------------------------------------------------------------
float Image::getMinValue()
//----------------------------------------------------------------------------------------------
{
    if (!m_stats_up_to_date) updateStats();
    
    return m_min_pixel_value;
}


//----------------------------------------------------------------------------------------------
float Image::getMaxValue()
//----------------------------------------------------------------------------------------------
{
    if (!m_stats_up_to_date) updateStats();
    
    return m_max_pixel_value;
}


//----------------------------------------------------------------------------------------------
void Image::updateStats()
//----------------------------------------------------------------------------------------------
{
    // Need to udate the stats
    if (!m_stats_up_to_date && m_width * m_height)
    {
        m_min_pixel_value = m_pixel_data[0];
        m_max_pixel_value = m_pixel_data[0];
        m_average_pixel_value = m_pixel_data[0];
        
        for (size_t i = 1; i < m_width * m_height; ++i)
        {
            if (m_min_pixel_value > m_pixel_data[i]) m_min_pixel_value = m_pixel_data[i];
            if (m_max_pixel_value < m_pixel_data[i]) m_max_pixel_value = m_pixel_data[i];
            
            m_average_pixel_value += m_pixel_data[i];
        }
        m_average_pixel_value /= m_width * m_height;
        
        m_stddev_pixel_value = 0;
        for (size_t i = 0; i < m_width * m_height; ++i)
        {
            m_stddev_pixel_value += (m_pixel_data[i] - m_average_pixel_value) * (m_pixel_data[i] - m_average_pixel_value);
        }
        m_stddev_pixel_value /= m_width * m_height;
        m_stddev_pixel_value = sqrt(m_stddev_pixel_value);
        
        m_stats_up_to_date = true;
    }
}

//----------------------------------------------------------------------------------------------
float Image::getMean()
//----------------------------------------------------------------------------------------------
{
    if(!m_stats_up_to_date){
        updateStats();
    }
    return m_average_pixel_value;
}
//----------------------------------------------------------------------------------------------
float Image::getStdDev()
//----------------------------------------------------------------------------------------------
{
    if(!m_stats_up_to_date){
        updateStats();
    }
    return m_stddev_pixel_value;
}

//----------------------------------------------------------------------------------------------
Image Image::operator-(const Image& img) const
//----------------------------------------------------------------------------------------------
{
    Image temp(0.0, std::min(m_width, img.m_width), std::min(m_height, img.m_height));

    for (size_t j = 0; j < temp.m_height; ++j)
    {
        for (size_t i = 0; i < temp.m_width; ++i)
        {
            temp(i,j) = (*this)(i, j) - img(i, j);
        }
    }
    return temp;
}
//----------------------------------------------------------------------------------------------
Image Image::operator+(const Image& img) const
//----------------------------------------------------------------------------------------------
{
    Image temp(0.0, m_width, m_height);

    for (size_t j = 0; j < temp.m_height; ++j)
    {
        for (size_t i = 0; i < temp.m_width; ++i)
        {
            temp(i,j) = (*this)(i, j) + img(i, j);
        }
    }
    return temp;
}

//----------------------------------------------------------------------------------------------
Image Image::operator*(const Image& img) const
//----------------------------------------------------------------------------------------------
{
    Image temp(0.0, m_width, m_height);

    for (size_t j = 0; j < temp.m_height; ++j)
    {
        for (size_t i = 0; i < temp.m_width; ++i)
        {
            temp(i,j) = (*this)(i, j) * img(i, j);
        }
    }
    return temp;
}

//----------------------------------------------------------------------------------------------
Image Image::operator/(const Image& img) const
//----------------------------------------------------------------------------------------------
{
    Image temp(0.0, m_width, m_height);

    for (size_t j = 0; j < temp.m_height; ++j)
    {
        for (size_t i = 0; i < temp.m_width; ++i)
        {
            temp(i,j) = (*this)(i, j) / img(i, j);
        }
    }
    return temp;
}


//----------------------------------------------------------------------------------------------
Image operator-(float aValue, const Image& anInputImage)
//----------------------------------------------------------------------------------------------
{
    Image temp(aValue, anInputImage.getWidth(), anInputImage.getHeight());

    return temp - anInputImage;
}


//----------------------------------------------------------------------------------------------
Image Image::operator-=(const Image& img)
//----------------------------------------------------------------------------------------------
{
    
    for (size_t j = 0; j < img.m_height; ++j)
    {
        for (size_t i = 0; i < img.m_width; ++i)
        {
            (*this)(i,j) = (*this)(i, j) - img(i, j);
        }
    }
    return *this;
}


//----------------------------------------------------------------------------------------------
Image Image::operator+=(const Image& img)
//----------------------------------------------------------------------------------------------

{
    
    for (size_t j = 0; j < img.m_height; ++j)
    {
        for (size_t i = 0; i < img.m_width; ++i)
        {
            (*this)(i,j) = (*this)(i, j) + img(i, j);
        }
    }
    return *this;
}

//----------------------------------------------------------------------------------------------
Image Image::operator*=(const Image& img)
//----------------------------------------------------------------------------------------------
{
    
    for (size_t j = 0; j < img.m_height; ++j)
    {
        for (size_t i = 0; i < img.m_width; ++i)
        {
            (*this)(i,j) = (*this)(i, j) * img(i, j);
        }
    }
    return *this;
}


//----------------------------------------------------------------------------------------------
Image Image::operator/=(const Image& img)
//----------------------------------------------------------------------------------------------
{
    
    for (size_t j = 0; j < img.m_height; ++j)
    {
        for (size_t i = 0; i < img.m_width; ++i)
        {
            (*this)(i,j) = (*this)(i, j) / img(i, j);
        }
    }
    return *this;
}

//----------------------------------------------------------------------------------------------
Image Image::operator!()
//----------------------------------------------------------------------------------------------
{
    return getMinValue() + getMaxValue()-(*this);
}
//----------------------------------------------------------------------------------------------
Image blending(float alpha, const Image& img1, const Image& img2)
//----------------------------------------------------------------------------------------------
{
    return ((1.0f - alpha) * img1) + (alpha * img2);
}

//----------------------------------------------------------------------------------------------
void Image::display(bool aNormaliseFlag) const
//----------------------------------------------------------------------------------------------
{
  #ifdef HAS_OPENCV
    Image display_image = *this;

    // Normalise the image if needed
    if (aNormaliseFlag)
        display_image = display_image.normalise();

    // Convert the data into an OpenCV Mat instance.
    cv::Mat cv_image(m_height, m_width, CV_32FC1, (float*) display_image.getPixelPointer());

    // Display the image
    cv::imshow("Display window", cv_image);

    // Wait for a keystroke in the window
    cv::waitKey(0);
#endif // HAS_OPENCV
}

//----------------------------------------------------------------------------------------------
double Image::getRMSE(const Image& anImage) const
//----------------------------------------------------------------------------------------------
{
    //Compare height and width of both images
    if ((*this).getHeight() == anImage.getHeight() & (*this).getWidth() == anImage.getWidth())
    {
        Image temp(0.0, (*this).getWidth(), (*this).getHeight());
        
        //Calculate the difference between the 2 images
        temp = *this-anImage;
        //square the difference value
        temp *= temp;
        
        //accumulator
        float sum = 0;
        
        //add value of width and height into sum
        for(int i = 0; i < (*this).getHeight(); i++){
            for (int j = 0; j < (*this).getWidth(); j++){
                sum += temp(j,i);
            }
        }
        
        //divide sum by height times width
        sum /= (*this).getHeight() * (*this).getWidth();
        
        //return square root of sum
        return sqrt(sum);
        
    }
    else {
        return FLT_MAX;        
    }
}


//----------------------------------------------------------------------------------------------
double Image::getZNCC(const Image& anImage) const
//----------------------------------------------------------------------------------------------
{
    if (getHeight() == anImage.getHeight() & getWidth() == anImage.getWidth())
    {
        Image input_image = anImage;
        // standard dev of both images
        float stdDev = m_stddev_pixel_value * input_image.getStdDev();
     
        // subtract avg pixel from each pixel value
        Image i_1 = (*this)-m_average_pixel_value;
        Image i_2 = input_image-input_image.getMean();
        
        // store the sum/product
        Image product = i_1*i_2;
      
        product /= stdDev;
        float sum = 0;
        for (int i = 0; i < product.getHeight() ; i++){
            for(int j = 0; j < product.getWidth() ; j++) {
                sum += product(j,i);
            }
        }
        
        sum /= product.getHeight() * product.getWidth();
        return sum;
    } else{
        return 0.0;
    }
   
}

//----------------------------------------------------------------------------------------------
Image Image::conv2d(const Image& aKernel) const
//----------------------------------------------------------------------------------------------
{
    // calculate the padding required
    int width = aKernel.getWidth();
    int padding = (width - 1)/2;
    // Create black image
    Image output_image(0.0, m_width, m_height);

    // Create padded image
    Image padded_image(0.0, m_width + (2*padding), m_height + (2*padding));
    for(int i = padding; i < m_height + padding; i++){
        for(int j = padding; j < m_width + padding; j++){
            padded_image(j, i) = (*this)(j - padding, i - padding);
        }
    }
    // for each pixel image
    for(int i = 0; i < m_height; i++){
        for(int j = 0; j < m_width; j++){

            // initialise sum
            float sum = 0.0;

            // for each pixel in the kernal
            for (int k = 0; k < aKernel.getHeight(); k++){
                for (int l = 0; l < aKernel.getWidth(); l++){

                    // sum up the elementwise multiplication
                    sum += padded_image(j + l, i + k) * aKernel(l,k);
                }
            }
            output_image(j, i) = sum;
        }
    }
    // Return the output
    return output_image;
}


//----------------------------------------------------------------------------------------------
Image Image::gaussianFilter() const
//----------------------------------------------------------------------------------------------
{
  // Create the kernel
  Image kernel(
    {
      1., 2., 1.,
      2., 4., 2.,
      1., 2., 1.
    }, 3, 3);

  // Normalise the kernel so that the sum of its coefficients is 1.
  kernel /= 16.0;

  // Filter the image
  return conv2d(kernel);
}


//----------------------------------------------------------------------------------------------
Image Image::meanFilter() const
//----------------------------------------------------------------------------------------------
{
  // Create the kernel
  Image kernel(
    {
      1., 1., 1.,
      1., 1., 1.,
      1., 1., 1.
    }, 3, 3);

  // Normalise the kernel so that the sum of its coefficients is 1.
  kernel /= 9.0;

  // Filter the image
  return conv2d(kernel);
}


//----------------------------------------------------------------------------------------------
Image Image::averageFilter() const
//----------------------------------------------------------------------------------------------
{
  return meanFilter();
}


//----------------------------------------------------------------------------------------------
Image Image::boxFilter() const
//----------------------------------------------------------------------------------------------
{
  return meanFilter();
}


//----------------------------------------------------------------------------------------------
Image Image::laplacianFilter() const
//----------------------------------------------------------------------------------------------
{
  // Create the kernel
  Image kernel(
    {
      1.,  1., 1.,
      1., -8., 1.,
      1.,  1., 1.
    }, 3, 3);

  // Filter the image
  return conv2d(kernel);
}
//----------------------------------------------------------------------------------------------
Image Image::absoluteValue() const
//----------------------------------------------------------------------------------------------
{
    Image temp = (*this);
    
    for (int i = 0; i < m_height; i++){
        for (int j =0; j < m_width; j++){
            temp(j, i) = abs((*this)(j,i));
            
        }
        
    }
    return temp;
}

//----------------------------------------------------------------------------------------------
Image Image::gradientMagnitude() const
//----------------------------------------------------------------------------------------------
{
    //create temp image
    Image temp = (*this);
    
    // Create the kernel
    Image kernel_gx(
      {
        1.,  0., -1.,
        2., 0., -2.,
        1.,  0., -1.
      }, 3, 3);
    
    // Create the kernel
    Image kernel_gy(
      {
        1.,  2., 1.,
        0., 0., 0.,
        -1.,  -2., -1.
      }, 3, 3);
    //get the derivative of gx and gy
    Image vertical_derivative_gx = conv2d(kernel_gx);
    Image horizontal_derivative_gy = conv2d(kernel_gy);
    
    //Create image with the corresponding absolute values
    Image G_gx = vertical_derivative_gx.absoluteValue();
    Image G_gy = horizontal_derivative_gy.absoluteValue();
    //Multiple Img gx with gy
    Image G = G_gx + G_gy;
    
    return G;
}

//----------------------------------------------------------------------------------------------
Image Image::sharpen(double alpha)
//----------------------------------------------------------------------------------------------
{
    //setup 5by5 kernel
    Image gaussian_5x5_kernel(
            {
                1.,  4.,  7.,  4., 1.,
                4., 16., 26., 16., 4.,
                7., 26., 41., 26., 7.,
                4., 16., 26., 16., 4.,
                1.,  4.,  7.,  4., 1.
            }, 5, 5);
        //normalise 
        gaussian_5x5_kernel /= 273.;
    
    Image blur = conv2d(gaussian_5x5_kernel);
    Image details = *this - blur;
    
    Image output = *this + alpha * details;
    
    return output.clamp(getMinValue(), getMaxValue());
}

//----------------------------------------------------------------------------------------------
Image Image::clamp(float aLowerThreshold, float anUpperThreshold) const
//----------------------------------------------------------------------------------------------
{
 
    Image temp = *this;
    
    for (int i = 0; i < m_height; i++){
        for (int j = 0; j < m_width; j++){
            if(temp(j,i) < aLowerThreshold)
                temp(j,i) = aLowerThreshold;
            else if (temp(j,i)>anUpperThreshold)
                temp(j,i) = anUpperThreshold;
        }
    }
    
    return temp;
}
