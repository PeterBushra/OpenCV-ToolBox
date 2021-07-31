#ifndef MAINWINDOW_H
#define MAINWINDOW_H


//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
// Req. Libraries
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------


#include <QMainWindow>
#include<QFileDialog>
#include<QPixmap>
#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/imgcodecs/imgcodecs.hpp>
#include<list>
#include <QDebug>
#include <math.h>
#include <QMouseEvent>
#include<QRadioButton>
#include <QMessageBox>
#include <QValidator>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QEvent>
#include <QGraphicsItem>

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
// Declaration of GUI Element
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------

class QGraphicsScene;
class QEvent;

using namespace cv;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow :   public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    bool eventFilter(QObject *watched , QEvent *event);
    ~MainWindow();

private slots:

    void showImage(Mat image); // from me
    void on_actionExit_triggered();
    void on_actionOpen_Image_triggered();
    void on_Button_GammaTrans_clicked();
    void on_Horz_gamma_valueChanged(int value);
    void on_actionRevert_triggered();
    void on_actionBack_triggered();
    void on_Button_EqualizeHist_clicked();
    void on_Button_Negative_clicked();
    void on_actionSave_triggered();
    void on_Horz_gamma_const_valueChanged(int value);
    void on_Button_LogTrans_clicked();
    void on_Horz_log_const_valueChanged(int value);
    void on_PushBustton_Flipping_clicked();
    void on_pushButton_bitSlice_clicked();
    void on_pushButton_getTransPoint_clicked();
    void on_pushButton_Translate_clicked();
    void on_pushButton_Rotate_clicked();
    void on_pushButton_getSrcPt1_clicked();
    void on_pushButton_getSrcPt2_clicked();
    void on_pushButton_getSrcPt3_clicked();
    void on_pushButton_getDstPt1_clicked();
    void on_pushButton_getDstPt1_2_clicked();
    void on_pushButton_getDstPt3_clicked();
    void on_pushButton_Skew_clicked();
    void on_tabWidget_currentChanged(int index);
    void set_vaildators();
    void Zoom(int);
    void on_actionZoom_0_triggered();
    void on_actionZoom_out_triggered();
    void on_actionZoom_IN_triggered();
    void on_dial_valueChanged(int value);
    void on_listWidget_currentRowChanged(int currentRow);
    void on_pushButton_blur_clicked();
    void setGroupBoxHidden(bool);

    void on_checkBox_selectTool_stateChanged(int arg1);

    void on_pushButton_cropResize_clicked();

    void on_pushButton_resize_clicked();

    void on_pushButton_gaussianBlur_clicked();

    void on_actioninfo_triggered();

    void on_pushButton_median_clicked();

    void on_pushButton_brightnessUp_clicked();

    void on_pushButton_brightnessDown_clicked();

    void on_pushButton_clicked();

    void on_PushBustton_Sobel_clicked();


    void on_pushButton_Blind_clicked();

    void on_horizontalSlider_sliderMoved(int position);


    void on_horizontalSlider_sliderReleased();

private:
    Ui::MainWindow *ui;
    cv::Mat image;
    QGraphicsScene *scene;
    QGraphicsRectItem *rect;
    QPointF movingPosition = QPointF(0,0);
    QPointF pressedPosition = QPointF(0,0);
    QPointF releasedPosition = QPointF(0,0);
    QPointF dclickedPosition = QPointF(0,0);
};
#endif // MAINWINDOW_H
