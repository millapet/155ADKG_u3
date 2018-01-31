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
    //ui->Canvas->coordinates_bool=false;

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
  double Min=ui->Canvas->minZ;
  double Max=ui->Canvas->maxZ;
   ui->topheight->setNum(Max);
   ui->lowheight->setNum(Min);
   ui->report->setText("Now apply the Delaunay triangulation.");
   ui->delaunayButton->setEnabled(1);
   QSize s = ui->Canvas->size();
   ui->Canvas->repaint();
}

void Widget::on_delaunayButton_clicked()
{
     QSize s = ui->Canvas->size();
     ui->Canvas->delaunaydraw(s);
     ui->Canvas->repaint();

     ui->Canvas->repaint();
     ui->report->setText("Select another action.");
     ui->slopeButton->setEnabled(1);
     ui->orientationButton->setEnabled(1);
     ui->contourButton->setEnabled(1);
     ui->cdist->setEnabled(1);
     ui->userlowheight->setEnabled(1);
     ui->usertopheight->setEnabled(1);

}

void Widget::on_slopeButton_clicked()
{
    QSize s = ui->Canvas->size();
    ui->Canvas->delaunaydraw(s);
    ui->Canvas->repaint();

    ui->report->setText("Select another action.");
    ui->Canvas->orientflag = false;
    ui->Canvas->slopeflag = true;
    ui->Canvas->repaint();
}

void Widget::on_orientationButton_clicked()
{
    QSize s = ui->Canvas->size();
    ui->Canvas->delaunaydraw(s);
    ui->Canvas->repaint();

    ui->report->setText("Select another action.");
    ui->Canvas->orientflag = true;
    ui->Canvas->slopeflag = false;
    ui->Canvas->repaint();
}

void Widget::on_contourButton_clicked()
{
QString heit = ui->usertopheight->text();
QString heil = ui->userlowheight->text();
QString dist_=ui->cdist->text();
int maxh=heit.toInt();
int minh=heil.toInt();
int dist=dist_.toInt();
double pt_maxh = ui->topheight->text().toDouble();
double pt_minh = ui->lowheight->text().toDouble();
if(maxh<pt_maxh&&minh>pt_minh&&dist<(pt_maxh-pt_minh)){
    ui->Canvas->maxhd = maxh;
    ui->Canvas->minhd = minh;
    ui->Canvas->distd = dist;
    ui->Canvas->contflag = true;
    ui->Canvas->repaint();
}
else{
    ui->report->setText("<font color='Red'>Error, check if contour parameters <br> are within limits.</font>");
}
}

void Widget::on_clearButton_clicked()
{
ui->Canvas->clear();
ui->report->setText("Now apply the Delaunay triangulation again.");
ui->contourButton->setDisabled(1);
ui->slopeButton->setDisabled(1);
ui->orientationButton->setDisabled(1);
ui->usertopheight->setDisabled(1);
ui->usertopheight->clear();
ui->userlowheight->setDisabled(1);
ui->userlowheight->clear();
//ui->lowheight->clear();
//ui->topheight->clear();
//ui->cdist->setDisabled(1);
ui->Canvas->repaint();
}
