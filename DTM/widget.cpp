#include "widget.h"
#include "ui_widget.h"


#include "draw.h"
#include "algorithms.h"
#include "QDebug"
#include "QFileDialog"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_loadButton_clicked()
{
    ui->Canvas->coordinates.clear();
    ui->Canvas->coordinates_bool=false;

    QString path = QFileDialog::getOpenFileName(
                this, tr("coordinates file"), "C://", "Text File (*.txt)");



    std::vector<double> data;


    // read input.txt file from project folder
    QFile qfile(path);

    // checking if input file was succesfuly opened
    if(qfile.open(QIODevice::ReadOnly))
    {
        QString convertion;
      QTextStream stream(&qfile);

        // read each row separately
        while(!stream.atEnd())
        {
            stream >> convertion;
            if(stream.status() == QTextStream::Ok)
            {
                 data.push_back(convertion.toDouble());
            }
            else
                break;
        }
    }


   ui->Canvas->loaddata(data);
  double Min=ui->Canvas->min;
  double Max=ui->Canvas->max;
   ui->topheight->setNum(Max);
   ui->lowheight->setNum(Min);
    ui->Canvas->repaint();
}

void Widget::on_delaunayButton_clicked()
{
    bool load=ui->Canvas->coordinates_bool;
    if(load==true){
        bool delaunay_click=true;
        ui->Canvas->delaunaydraw(delaunay_click);
        ui->Canvas->repaint();
    } else if(load==false){
        ui->report->setText("Error");
    }

     ui->Canvas->repaint();

}

void Widget::on_slopeButton_clicked()
{   bool slopecklick=false;
    bool load_s=ui->Canvas->delaunaydraw_bool;
 if(load_s==true){
    slopecklick=true;
    ui->Canvas->slopedraw(slopecklick);

     ui->Canvas->repaint();
}
}

void Widget::on_orientationButton_clicked()
{   bool orientcklick=false;
    bool load_s=ui->Canvas->delaunaydraw_bool;
     if(load_s==true){
        orientcklick=true;
        ui->Canvas->orientdraw(orientcklick);
        ui->Canvas->repaint();
    }

}

void Widget::on_contourButton_clicked()
{
QString heit = ui->lineEdit->text();
QString heil = ui->lineEdit_2->text();
QString mdist_=ui->mcdist->text();
QString dist_=ui->cdist->text();
int maxh=heit.toInt();
int minh=heil.toInt();
int mdist1=mdist_.toInt();
int dist1=dist_.toInt();
ui->Canvas->contoursdrawing(maxh,minh,mdist1,dist1);
ui->Canvas->repaint();
}

void Widget::on_clearButton_clicked()
{

}
