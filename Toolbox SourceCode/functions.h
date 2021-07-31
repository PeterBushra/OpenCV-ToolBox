#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "mainwindow.h"

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
// Declaration of ToolBox Functions
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------

class ToolBox
{
private:

    std::list<Mat> images;
    int cols=0 , rows=0 , size=0,n_channels=0 , n_depth= 0;

public:
    ToolBox();
    bool setInputImage(QString filename);
    void getOriginalImage();
    int getWidth();
    int getHeight();
    int getChannelsNb();
    int getSize();
    int getDepth();
    const Mat getLastResult() const;
    void equalizeHist2();
    void negative();
    void gammaTrans(float,float);
    void undo();
    void logTrans(float,bool);
    void flipping(int axis=0);
    void bitPlane(int plane=4);
    void translate(int tx=0,int ty=0);
    void rotate(int angle=0,float scale=1.0);
    void skew(Point2f*,Point2f*);
    void blurring(cv::Size , cv::Point);
    void rotateFixedEdition(int angle=0,float scale=1.0);
    void zoom(Point2i,Point2i);
    void resizing(int,int);
    void Gaussblurring(cv::Size , cv::Point);
    void Medianblurring(int kSize);
    void brightUp();
    void brightDown();
    void blackWhite();
    void sobel(int,int,int);
    void blind(QString filename);
    Mat segment(int,bool);
    void laplace();

};



#endif // FUNCTIONS_H
