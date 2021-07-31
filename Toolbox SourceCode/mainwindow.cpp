#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "functions.h"

// Create an Object From ToolBox which all Backend Function Existed
ToolBox TL;

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
// Set some inital States in Constructors of MainWindow
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{

    ui->setupUi(this);
    ui->centralwidget->setEnabled(false);
    ui->buttonGroup_Axis->setId(ui->radioButton_Xaxis,0);
    ui->buttonGroup_Axis->setId(ui->radioButton_Yaxis,1);
    ui->buttonGroup_Axis->setId(ui->radioButton_Baxis,2);
    ui->tabWidget->setStyleSheet("QTabWidget::pane { border: 0; }");
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    set_vaildators();
    scene->installEventFilter(this);


}


// Destructor
MainWindow::~MainWindow()
{
    delete ui;
}

/*
    Filter all event got from QGraphicScene To Take Mouse Coordinates
    and initailize 4 modes for Mouse (Press, Release, Moving, Double Click)
*/

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{

    const QGraphicsSceneMouseEvent* const me = static_cast<const QGraphicsSceneMouseEvent*>(event); ;

    if(watched == scene && event->type() == QEvent::GraphicsSceneMouseMove)
    {
        movingPosition = me->scenePos();
        ui->statusbar->showMessage("Moving:: x: "+ QString::number(movingPosition.x()) +" y: " +QString::number(movingPosition.y()));

    }
    else if(watched == scene && event->type() == QEvent::GraphicsSceneMousePress)
    {
        pressedPosition = me->scenePos();
        ui->statusbar->showMessage("Pressed:: x: "+ QString::number(pressedPosition.x()) +" y: " +QString::number(pressedPosition.y()));
    }
    else if(watched == scene && event->type() == QEvent::GraphicsSceneMouseRelease)
    {
        releasedPosition = me->scenePos();
        ui->statusbar->showMessage("Released:: x: "+ QString::number(releasedPosition.x()) +" y: " +QString::number(releasedPosition.y()));
        if(ui->checkBox_selectTool->isChecked())
        {
            showImage(TL.getLastResult());
            scene->addRect(QRect(pressedPosition.toPoint(),releasedPosition.toPoint()),QPen(Qt::red));
        }

    }
    else if(watched == scene && event->type() == QEvent::GraphicsSceneMouseDoubleClick)
    {
        dclickedPosition = me->scenePos();
        ui->statusbar->showMessage("Selected:: x: "+ QString::number(dclickedPosition.x()) +" y: " +QString::number(dclickedPosition.y()));
    }

    return QMainWindow::eventFilter(watched, event);
}

//--------------------------------------------------------------
//--------------------------------------------------------------
//  Show Image > take an image argument to show in workspace
//  default arg: Last Processed Image
//--------------------------------------------------------------
//--------------------------------------------------------------

void MainWindow::showImage(Mat image = TL.getLastResult())
{

    //clear all previous scene
    scene->clear();
    //check if image loaded correctly or not
    //if loaded we resize the scene around to image and show image with green border
    if(image.data)
    {
        // Enable Workspace and Tool Widgets when image loaded
        ui->centralwidget->setEnabled(true);
        QImage img = QImage(image.data,image.cols,image.rows,image.step,QImage::Format_Grayscale8);
        QPixmap pix(QPixmap::fromImage(img));
        //scene->addPixmap(pix.scaled(800,800,Qt::KeepAspectRatio));
        scene->addPixmap(pix);
        scene->addRect(QRect(0,0,image.cols-2,image.rows-2),QPen(Qt::green));
        ui->graphicsView->resize(scene->width()+100,scene->height()+100);
    }
    // if not loaded correctly clear scenes
    // and show Message To User
    else
    {
        scene->clear();
        ui->centralwidget->setEnabled(false);
        ui->statusbar->showMessage("Empty!");
        ui->graphicsView->resize(0,0);

    }
}



//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
// All Triggers
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------

//--------------------------------------------------------------
//--------------------------------------------------------------
// Browsing Images to select input image
// and show message to use if successful or Failed loading
//--------------------------------------------------------------
//--------------------------------------------------------------
void MainWindow::on_actionOpen_Image_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    ("Open Image"),".",  //path
                                                    ("Image Files (*.png *.jpg *.jpeg *.bmp)")
                                                    );

    if(TL.setInputImage(filename))
    {
        showImage();
        ui->statusbar->showMessage("Image Loaded." , 5000);
    }
    else
    {
        ui->statusbar->showMessage("Failed !", 5000);
    }

}
//Exit Action
void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

// Revert Original Image
void MainWindow::on_actionRevert_triggered()
{
    TL.getOriginalImage();
    showImage();
}
//  Get Previous Edited Sate
void MainWindow::on_actionBack_triggered()
{
    TL.undo();
    showImage();
}
// To Save Processed Image
void MainWindow::on_actionSave_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this,"Save Image",".","Image Files (*.jpg *.png *.jpeg *.bmp)");
    if(filename!="")
        if(TL.getLastResult().data)
            imwrite(filename.toLatin1().data(),TL.getLastResult());
}

// Zoom Actions For Workspace not Image
void MainWindow::on_actionZoom_0_triggered()
{
    Zoom(0);
}
void MainWindow::on_actionZoom_out_triggered()
{
    Zoom(-1);
}
void MainWindow::on_actionZoom_IN_triggered()
{
    Zoom(1);
}
void MainWindow::on_actioninfo_triggered()
{
    QMessageBox::about(this,"Author Info","Peter Boshra Aziz");
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
// All Push Button Actions
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------

//--------------------------------------------------------------
//--------------------------------------------------------------
//Call Function Then Show Result
//--------------------------------------------------------------
//--------------------------------------------------------------

void MainWindow::on_Button_GammaTrans_clicked()
{
    float gamma = ui->Horz_gamma->value() * 0.1;
    float constant = ui->Horz_gamma_const->value() * 0.1;
    TL.gammaTrans(constant,gamma);
    showImage();
}
void MainWindow::on_Button_EqualizeHist_clicked()
{
    TL.equalizeHist2();
    showImage();
}
void MainWindow::on_Button_Negative_clicked()
{
    TL.negative();
    showImage();
}
void MainWindow::on_Button_LogTrans_clicked()
{
    float constant = ui->Horz_log_const->value();
    TL.logTrans(constant,ui->checkBox_inverseLog->isChecked());
    showImage();
}
void MainWindow::on_PushBustton_Flipping_clicked()
{
    int ax = ui->buttonGroup_Axis->checkedId();
    if(ax==2)
        ax=-1;
    TL.flipping(ax);
    showImage();
}
void MainWindow::on_pushButton_bitSlice_clicked()
{
    int sliceLevel = ui->lineEdit_bitSlice->text().toInt();
    TL.bitPlane(sliceLevel);
    showImage();
}
void MainWindow::on_pushButton_getTransPoint_clicked()
{
    ui->lineEdit_translation->setText(QString::number(dclickedPosition.x()) + "," +QString::number(dclickedPosition.y()));
}
void MainWindow::on_pushButton_Translate_clicked()
{
    QString point = ui->lineEdit_translation->text();
    if(point != "")
    {
    int tx = point.split(",")[0].toInt();
    int ty = point.split(",")[1].toInt();
    TL.translate(tx,ty);
    showImage();
    }


}
void MainWindow::on_pushButton_Rotate_clicked()
{

    if(ui->lineEdit_rotationangle->hasAcceptableInput() &&
        ui->lineEdit_rotationscale->hasAcceptableInput() )
    {
    int angle = ui->lineEdit_rotationangle->text().toInt();
    float scale = ui->lineEdit_rotationscale->text().toFloat();
    if(scale==0)
        scale=1;
    if(ui->checkBox_rotateFixedEdition->isChecked())
        TL.rotateFixedEdition(angle,scale);
    else
        TL.rotate(angle,scale);
    showImage();
    }
    else
        QMessageBox::warning(this,"Rotation Inputs Error","Check Inputs angle and scale");
}
// Skewing Action, Validate all Inputs then Process
void MainWindow::on_pushButton_Skew_clicked()
{
    Point2f srcPts[3],dstPts[3];
    QString point;
    if(ui->lineEdit_srcPt1->hasAcceptableInput() &&
        ui->lineEdit_srcPt2->hasAcceptableInput() &&
        ui->lineEdit_srcPt3->hasAcceptableInput() &&
        ui->lineEdit_DstPt1->hasAcceptableInput() &&
        ui->lineEdit_DstPt2->hasAcceptableInput() &&
        ui->lineEdit_DstPt3->hasAcceptableInput() ) {
            point = ui->lineEdit_srcPt1->text();
            srcPts[0]= Point2f(point.split(",")[0].toInt(),point.split(",")[1].toInt());
            point = ui->lineEdit_srcPt2->text();
            srcPts[1]= Point2f(point.split(",")[0].toInt(),point.split(",")[1].toInt());
            point = ui->lineEdit_srcPt3->text();
            srcPts[2]= Point2f(point.split(",")[0].toInt(),point.split(",")[1].toInt());

            point = ui->lineEdit_DstPt1->text();
            dstPts[0]= Point2f(point.split(",")[0].toInt(),point.split(",")[1].toInt());
            point = ui->lineEdit_DstPt2->text();
            dstPts[1]= Point2f(point.split(",")[0].toInt(),point.split(",")[1].toInt());
            point = ui->lineEdit_DstPt3->text();
            dstPts[2]= Point2f(point.split(",")[0].toInt(),point.split(",")[1].toInt());
            TL.skew(srcPts,dstPts);
            showImage();
    }
    else
        QMessageBox::warning(this,"Skew Inputs Error","Check Input Points");

}

// ---------------------------
// get all Skew Point By Mouse
// ---------------------------
void MainWindow::on_pushButton_getSrcPt1_clicked()
{
    ui->lineEdit_srcPt1->setText(QString::number(dclickedPosition.x()) + "," +QString::number(dclickedPosition.y()));
}
void MainWindow::on_pushButton_getSrcPt2_clicked()
{
    ui->lineEdit_srcPt2->setText(QString::number(dclickedPosition.x()) + "," +QString::number(dclickedPosition.y()));
}
void MainWindow::on_pushButton_getSrcPt3_clicked()
{
    ui->lineEdit_srcPt3->setText(QString::number(dclickedPosition.x()) + "," +QString::number(dclickedPosition.y()));

}
void MainWindow::on_pushButton_getDstPt1_clicked()
{
    ui->lineEdit_DstPt1->setText(QString::number(dclickedPosition.x()) + "," +QString::number(dclickedPosition.y()));

}
void MainWindow::on_pushButton_getDstPt1_2_clicked()
{
    ui->lineEdit_DstPt2->setText(QString::number(dclickedPosition.x()) + "," +QString::number(dclickedPosition.y()));
}
void MainWindow::on_pushButton_getDstPt3_clicked()
{
    ui->lineEdit_DstPt3->setText(QString::number(dclickedPosition.x()) + "," +QString::number(dclickedPosition.y()));
}


//--------------------------------------------------------------
//--------------------------------------------------------------
// Blurring Action , Validate input to make sure all work fine
//--------------------------------------------------------------
//--------------------------------------------------------------

void MainWindow::on_pushButton_blur_clicked()
{
    if(ui->lineEdit_blurKsize->hasAcceptableInput() && ui->lineEdit_blurAnchorPoint->hasAcceptableInput())
    {
        QString point ;
        point = ui->lineEdit_blurKsize->text();
        Size kSize= Size(point.split(",")[0].toInt(),point.split(",")[1].toInt());
        point = ui->lineEdit_blurAnchorPoint->text();
        Point anchor= Point(point.split(",")[0].toInt(),point.split(",")[1].toInt());
        if((anchor.x<kSize.width) && (anchor.y<kSize.height))
        {
            TL.blurring(kSize,anchor);
            showImage();
        }
        else
             QMessageBox::warning(this,"Blur Inputs Error","Cannot anchor greater than Kernel Size");

    }
    else
        QMessageBox::warning(this,"Blur Inputs Error","Wrong input Format");

}

//--------------------------------------------------------------
//--------------------------------------------------------------
// get Coordinate from Selection Tool And Crop and Resize
//--------------------------------------------------------------
//--------------------------------------------------------------
void MainWindow::on_pushButton_cropResize_clicked()
{
    Point2i pt1 = Point2i(pressedPosition.x(),pressedPosition.y());
    Point2i pt2 = Point2i(releasedPosition.x(),releasedPosition.y());
    ui->checkBox_selectTool->setChecked(false);
    TL.zoom(pt1,pt2);
    showImage();
}
void MainWindow::on_pushButton_resize_clicked()
{
    QString point = ui->lineEdit_Resizing->text();
    if(ui->lineEdit_Resizing->hasAcceptableInput())
        TL.resizing(point.split(",")[0].toInt(),point.split(",")[1].toInt());
    showImage();
}

void MainWindow::on_pushButton_gaussianBlur_clicked()
{
    if(ui->lineEdit_gaussianBlurKsize->hasAcceptableInput() && ui->lineEdit_gaussianBlurSigma->hasAcceptableInput())
    {
        QString point ;
        point = ui->lineEdit_gaussianBlurKsize->text();
        Size kSize= Size(point.split(",")[0].toInt(),point.split(",")[1].toInt());
        point = ui->lineEdit_gaussianBlurSigma->text();
        Point sigma= Point(point.split(",")[0].toInt(),point.split(",")[1].toInt());
        TL.Gaussblurring(kSize,sigma);
        showImage();
    }
    else
        QMessageBox::warning(this,"GaussBlur Inputs Error","Wrong input Format");
}

void MainWindow::on_pushButton_median_clicked()
{
    if(ui->lineEdit_medianKsize->hasAcceptableInput()){
    int kSize = ui->lineEdit_medianKsize->text().toInt();
    TL.Medianblurring(kSize);
    showImage();
    }
    else
        QMessageBox::warning(this,"MedianBlur Inputs Error","Wrong input Format");
}

void MainWindow::on_pushButton_brightnessUp_clicked()
{
    TL.brightUp();
    showImage();
}

void MainWindow::on_pushButton_brightnessDown_clicked()
{
    TL.brightDown();
    showImage();
}

void MainWindow::on_pushButton_clicked()
{
    TL.blackWhite();
    showImage();
}

void MainWindow::on_PushBustton_Sobel_clicked()
{
    int ax = ui->buttonGroup_Sobel->checkedId();
    if(ui->lineEdit_sobelKsize->hasAcceptableInput())
    {
    int kSize = ui->lineEdit_sobelKsize->text().toInt();
    if(ax==-2)
        TL.sobel(1,0,kSize);
    else if(ax==-3)
        TL.sobel(0,1,kSize);
    else if(ax==-4)
        TL.sobel(1,1,kSize);
    }
    else
        QMessageBox::warning(this,"Sobel Inputs Error","Wrong input Format");
    showImage();

}

void MainWindow::on_pushButton_Blind_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    ("Open 2nd Image"),".",  //path
                                                    ("Image Files (*.png *.jpg *.jpeg *.bmp)")
                                                    );
    if(filename != "")
    {
    TL.blind(filename);
    showImage();
}
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
// Lines Edit , Sliders , CheckBox and RadioButton Actions
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------

//--------------------------------------------------------------
//--------------------------------------------------------------
// Remove All Box in image when unChecked Selection Tool
//--------------------------------------------------------------
//--------------------------------------------------------------
void MainWindow::on_checkBox_selectTool_stateChanged(int arg1)
{
    if(!arg1)
        showImage(TL.getLastResult());

}

//--------------------------------------------------------------
//--------------------------------------------------------------
// Get selected Function In Filters List
// to Choose Appropiate Function from CV
//--------------------------------------------------------------
//--------------------------------------------------------------

void MainWindow::on_listWidget_currentRowChanged(int currentRow)
{
    setGroupBoxHidden(true);
    switch (currentRow)
    {
        case 0:
        ui->groupBox_blur->setHidden(false);
        break;
    case 1:
        ui->groupBox_GaussianBlur->setHidden(false);
        break;
    case 2:
        ui->groupBox_median->setHidden(false);
        break;
    case 3:
        ui->groupBox_sobel->setHidden(false);
        break;
    case 4:
        TL.laplace();
        showImage();
        break;
    }
}
//--------------------------------------------------------------
//--------------------------------------------------------------
// invert value of Dial in Rotation To Get same rotate image
//--------------------------------------------------------------
//--------------------------------------------------------------
void MainWindow::on_dial_valueChanged(int value)
{
    ui->lineEdit_rotationangle->setText(QString::number(-value));
}
//--------------------------------------------------------------
//--------------------------------------------------------------
// set Gamma vlaue
//--------------------------------------------------------------
//--------------------------------------------------------------
void MainWindow::on_Horz_gamma_valueChanged(int value)
{
    ui->label_horzGamma->setText(QString::number(value * 0.1));

}

//--------------------------------------------------------------
//--------------------------------------------------------------
// set Log Constant
//--------------------------------------------------------------
//--------------------------------------------------------------
void MainWindow::on_Horz_log_const_valueChanged(int value)
{
    ui->label_horzLog->setText(QString::number(value));

}
//--------------------------------------------------------------
//--------------------------------------------------------------
// set power Constant
//--------------------------------------------------------------
//--------------------------------------------------------------

void MainWindow::on_Horz_gamma_const_valueChanged(int value)
{
    ui->label_horzGamma_2->setText(QString::number(value * 0.1));

}

//--------------------------------------------------------------
//--------------------------------------------------------------
// change info when changing tab widget
//--------------------------------------------------------------
//--------------------------------------------------------------
void MainWindow::on_tabWidget_currentChanged(int index)
{
    ui->lineEdit_infoChannels->setText(QString::number(TL.getChannelsNb()));
    ui->lineEdit_infoHeight->setText(QString::number(TL.getHeight()));
    ui->lineEdit_infoWidth->setText(QString::number(TL.getWidth()));
    ui->lineEdit_infoSize->setText(QString::number(TL.getSize()));
    ui->lineEdit_infoDepth->setText(QString::number(TL.getDepth()));
}
void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    Mat image=TL.segment(position,false);
    ui->label_threshold->setText(QString::number(position));
    showImage(image);
}



void MainWindow::on_horizontalSlider_sliderReleased()
{
    int th = ui->horizontalSlider->value();
    Mat image=TL.segment(th,true);
    showImage();

}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
// Validation Functions
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------

//--------------------------------------------------------------
//--------------------------------------------------------------
//Make all Functions in group Box invisable
//--------------------------------------------------------------
//--------------------------------------------------------------
void MainWindow::setGroupBoxHidden(bool status)
{
    ui->groupBox_blur->setHidden(status);
    ui->groupBox_GaussianBlur->setHidden(status);
    ui->groupBox_median->setHidden(status);
    ui->groupBox_sobel->setHidden(status);

}

//--------------------------------------------------------------
//--------------------------------------------------------------
// Setting of Validation Rules of Inputs
// To Make All Functionality Works well
//--------------------------------------------------------------
//--------------------------------------------------------------
void MainWindow::set_vaildators()
{
    QRegExpValidator *vaildpoint , *validint,*validfloat,*validuint,*vaildupoint;
    vaildpoint = new QRegExpValidator(QRegExp("(\\d+[,]\\d+){1,1}"),this);
    vaildupoint = new QRegExpValidator(QRegExp("((\\d+|\-?\\d+)[,](\\d+|\-?\\d+)){1,1}"),this);
    validuint = new QRegExpValidator(QRegExp("(\\d*|\-?\\d*)"),this);
    validfloat = new QRegExpValidator(QRegExp("(\\d+|\-?\\d+)[\.]?(\\d+)"),this);
    validint = new QRegExpValidator(QRegExp("(\\d+)"),this);
    ui->lineEdit_rotationangle->setValidator(validuint);
    ui->lineEdit_rotationscale->setValidator(validfloat);
    ui->lineEdit_bitSlice->setValidator(validuint);
    ui->lineEdit_srcPt1->setValidator(vaildupoint);
    ui->lineEdit_srcPt2->setValidator(vaildupoint);
    ui->lineEdit_srcPt3->setValidator(vaildupoint);
    ui->lineEdit_DstPt1->setValidator(vaildupoint);
    ui->lineEdit_DstPt2->setValidator(vaildupoint);
    ui->lineEdit_DstPt3->setValidator(vaildupoint);
    ui->lineEdit_translation->setValidator(vaildupoint);
    ui->lineEdit_blurAnchorPoint->setValidator(vaildupoint);
    ui->lineEdit_blurKsize->setValidator(vaildpoint);
    ui->lineEdit_Resizing->setValidator(vaildupoint);
    ui->lineEdit_gaussianBlurKsize->setValidator(vaildupoint);
    ui->lineEdit_gaussianBlurSigma->setValidator(vaildpoint);
    ui->lineEdit_medianKsize->setValidator(validint);
    ui->lineEdit_sobelKsize->setValidator(validint);

}
//--------------------------------------------------------------
//--------------------------------------------------------------
// Zoom in workspace
//--------------------------------------------------------------
//--------------------------------------------------------------

void MainWindow::Zoom(int mode)
{

    if(mode == 1)
    {
        ui->graphicsView->resize(ui->graphicsView->size() + QSize(10,10));
    }
    else if(mode == -1)
    {
        ui->graphicsView->resize(ui->graphicsView->size() - QSize(10,10));
    }
    else if(mode == 0)
        ui->graphicsView->resize(QSize(scene->width()+1,scene->height()+1));
}



