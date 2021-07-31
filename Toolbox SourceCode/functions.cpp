#include"functions.h"



ToolBox::ToolBox()
{

}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
// Read an image and Validate data Then Save it in Linked List
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
bool ToolBox::setInputImage(QString filename)
{
    images.clear();
    images.push_back(cv::imread(filename.toLatin1().data(),0));

    if(!(images.back().data))
    {
        images.pop_back();
        return false;
    }
    else
    {
        cols = getLastResult().cols;
        rows = getLastResult().rows;
        size = getLastResult().total();
        n_channels = getLastResult().channels();
        n_depth = getLastResult().depth();
        return true;
    }
}



//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
// get First Element in Linked List > Original Image
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
void ToolBox::getOriginalImage()
{
    if((images.size()>0))
        images.push_back(images.front());
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
// return Image Properties Height, Width, Channels Numbers, Depth, Size
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
int ToolBox::getWidth()
{
    return getLastResult().cols;
}
int ToolBox::getHeight()
{
    return getLastResult().rows;
}
int ToolBox::getChannelsNb()
{
    return getLastResult().channels();
}
int ToolBox::getSize()
{
    return getLastResult().total();
}

int ToolBox::getDepth()
{
    return getLastResult().depth();
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
// Return Last Element In Linked List Which thats Last Edited image
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
const Mat ToolBox::getLastResult() const
{
    if((images.size()>0))
        return images.back();
    else
    {
        return Mat(0,0,0);
    }
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
// Remove Last Element Which Last Edited Element
// then Show Last Element Which it's Previous State
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
void ToolBox::undo()
{
    if((images.size()>0))
        images.pop_back();
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
// Implementation of OpenCV Function
// - Process Image
// - then Push image back in Linked List
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
void ToolBox::equalizeHist2()
{
    Mat temp;
    cv::equalizeHist(getLastResult(),temp);
    images.push_back(temp);
}
void ToolBox::negative()
    {
        int cols = getLastResult().cols, rows = getLastResult().rows;
        Mat Result(rows,cols,CV_8UC1);

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                int current = getLastResult().at<uchar>(i, j);
                Result.at<uchar>(i, j) = 255 - current;
            }
        }

        images.push_back(Result);
    }
void ToolBox::gammaTrans(float constant,float gamma)
{

   //int cols = getLastResult().cols, rows = getLastResult().rows;
    Mat src= getLastResult();
    src.convertTo(src,CV_32F);

    Mat Result = src ;
    for (int i = 0; i < src.rows; i++)
    {
        for (int j = 0; j < src.cols; j++)
        {
            float current = src.at<float>(i, j);
            Result.at<float>(i, j) =  constant*powf(current,gamma);
        }
    }
    normalize(Result,Result,0,255,NORM_MINMAX);
    convertScaleAbs(Result,Result);
    images.push_back(Result);

}
void ToolBox::logTrans(float constant, bool inv)
{
    int cols = getLastResult().cols, rows = getLastResult().rows;
    Mat Result = getLastResult();
    if(!inv)
    {
        getLastResult().convertTo(Result,CV_32F);
        Result +=1;
        log(Result,Result);
        Result *= constant;
        normalize(Result,Result,0,255,NORM_MINMAX);
        convertScaleAbs(Result,Result);
        images.push_back(Result);

    }
    else
    {
        /*for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {

                int current = getLastResult().at<uchar>(i, j);
                Result.at<uchar>(i, j) = constant*exp(current+1);
            }
        }*/

        Mat src= getLastResult();
        src.convertTo(src,CV_32F);

        Mat dst = src ;
        for (int i = 0; i < src.rows; i++)
        {
            for (int j = 0; j < src.cols; j++)
            {
                float current = src.at<float>(i, j);
                dst.at<float>(i, j) =  constant*exp(current);
            }
        }
        normalize(dst,dst,0,255,NORM_MINMAX);
        convertScaleAbs(dst,dst);
    }

    images.push_back(Result);
}
void ToolBox::flipping(int axis)
{
    Mat Result;
    cv::flip(getLastResult(), Result,axis);
    images.push_back(Result);
}
void ToolBox::bitPlane(int plane)
{
    int cols = getLastResult().cols, rows = getLastResult().rows;
    Mat Result(rows, cols, CV_8UC1);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            int curr = getLastResult().at<uchar>(i, j);
            if (curr & uchar(plane))
                Result.at<uchar>(i, j) = 255;
            else
                Result.at<uchar>(i, j) = 0;

        }
    }
    images.push_back(Result);
}
void ToolBox::translate(int tx, int ty)
{
    Mat M = (Mat_<float>(2, 3) << 1, 0, tx , 0, 1, ty);
    Mat Result;
    warpAffine(getLastResult(), Result, M,getLastResult().size());
    images.push_back(Result);
}
void ToolBox::rotate(int angle, float scale)
{
    int cols = getLastResult().cols, rows = getLastResult().rows;
    Point2i center = Point2i(rows / 2, cols / 2);
    Mat Result;
    Mat M = getRotationMatrix2D(center,angle,scale);
    warpAffine(getLastResult(), Result, M, getLastResult().size());
    images.push_back(Result);
}
void ToolBox::skew(Point2f *src_pts, Point2f *dst_pts)
{
    Mat Result;
    Mat M = getAffineTransform(src_pts, dst_pts);
    warpAffine(getLastResult(), Result, M, getLastResult().size());
    images.push_back(Result);
}
void ToolBox::blurring(Size Ksize,Point anchor)
{
    Mat Result;
    cv::blur(getLastResult(), Result,Ksize,anchor);
    images.push_back(Result);
}
void ToolBox::rotateFixedEdition(int angle, float scale)
{
    // get rotation matrix for rotating the image around its center in pixel coordinates
    cv::Point2f center((getLastResult().cols - 1) / 2.0, (getLastResult().rows - 1) / 2.0);
    cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
    // determine bounding rectangle, center not relevant
    cv::Rect2f bbox = cv::RotatedRect(cv::Point2f(), getLastResult().size(), angle).boundingRect2f();
    // adjust transformation matrix
    rot.at<double>(0, 2) += bbox.width / 2.0 - getLastResult().cols / 2.0;
    rot.at<double>(1, 2) += bbox.height / 2.0 - getLastResult().rows / 2.0;

    cv::Mat Result;
    cv::warpAffine(getLastResult(), Result, rot, bbox.size());
    images.push_back(Result);

}
void ToolBox::zoom(Point2i pt1, Point2i pt2)
{
        Mat image= getLastResult();
        int cols = image.cols, rows = image.rows;
        Mat imcrop = image(Rect(pt1,pt2));
        Mat Result;
        resize(imcrop, Result, image.size(), 2, 2, 0);
        images.push_back(Result);
}
void ToolBox::resizing(int w,int h)
{
    Mat image= getLastResult();
    Mat Result;
    resize(image, Result, Size(w,h), 2, 2, 0);
    images.push_back(Result);
}

void ToolBox::Gaussblurring(Size Ksize, Point sigma)
{
    Mat Result;
    cv::GaussianBlur(getLastResult(), Result,Ksize,sigma.x,sigma.y);
    images.push_back(Result);
}

void ToolBox::Medianblurring(int kSize)
{
    Mat Result;
    medianBlur(getLastResult(), Result, kSize);
    images.push_back(Result);

}

void ToolBox::brightUp()
{
    int cols = getLastResult().cols, rows = getLastResult().rows;
    Mat Result(rows, cols, CV_8UC1);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            int curr = getLastResult().at<uchar>(i, j);
             Result.at<uchar>(i, j) = curr+5>=255 ? curr = 255 : curr=curr+5;


        }
    }
    images.push_back(Result);
}

void ToolBox::brightDown()
{
    int cols = getLastResult().cols, rows = getLastResult().rows;
    Mat Result(rows, cols, CV_8UC1);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            int curr = getLastResult().at<uchar>(i, j);
            Result.at<uchar>(i, j) = curr-5 <=0 ? curr = 0 : curr=curr-5;


        }
    }
    images.push_back(Result);
}

void ToolBox::blackWhite()
{
    int cols = getLastResult().cols, rows = getLastResult().rows;
    Mat Result(rows, cols, CV_8UC1);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            int curr = getLastResult().at<uchar>(i, j);
            if(curr>127)
                Result.at<uchar>(i, j) = 255;
            else
                Result.at<uchar>(i, j) = 0;
        }
    }
    images.push_back(Result);
}

void ToolBox::sobel(int x, int y, int kSize)
{
    int cols = getLastResult().cols, rows = getLastResult().rows;
    Mat Result(rows, cols, CV_8UC1);
    Sobel(getLastResult(), Result, CV_8UC1, x, y, kSize);
    images.push_back(Result);

}

void ToolBox::blind(QString filename)
{
    Mat image2 = cv::imread(filename.toLatin1().data(),0);
    int cols = getLastResult().cols, rows = getLastResult().rows;
    resize(image2, image2, getLastResult().size(), 2, 2, 0);

    Mat Result(rows, cols, CV_8UC1);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            int curr1 = getLastResult().at<uchar>(i, j);
            int curr2 = image2.at<uchar>(i, j);
            Result.at<uchar>(i, j) = curr1 * .7 + curr2 * .3;
        }
    }

    images.push_back(Result);
}
// if status true ,save image , else show only
Mat ToolBox::segment(int th,bool status)
{
    Mat Result;
    threshold(getLastResult(),Result,th,255,THRESH_BINARY);
    if(status)
        images.push_back(Result);
    return Result;
}

void ToolBox::laplace()
{
    Mat Result;
    Laplacian(getLastResult(),Result,CV_8UC1);
    images.push_back(Result);

}






