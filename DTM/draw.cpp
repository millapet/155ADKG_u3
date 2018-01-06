#include "draw.h"
#include "qpoint3d.h"
#include <QDebug>
#include <QtGui>
#include "algorithms.h"
#include "triangle.h"
#include "edge.h"
#include "sortbyxasc.h"
#include "sortbyyasc.h"


draw::draw(QWidget *parent) : QWidget(parent)
{

}

void draw::loaddata(std::vector<double> &data){

    double x;
    double y;
    double z;

    // data parse
    for( unsigned int i = 0; i < data.size(); i=i+3) //i+4
    {
         x = data[i];
         y = data[i + 1];
         z = data[i + 2];

         QPoint3D p (x, y, z);
         coordinates.push_back(p);

     }

    std::vector<QPoint3D> points_sortZ;
    double x1;
    double z1;
    for(unsigned int i = 0; i < coordinates.size(); i++)
    {
         x1 = coordinates[i].getX();
         z1 = coordinates[i].getZ();

         QPoint3D h_point (z1, x1, 0);
         points_sortZ.push_back(h_point);
    }

    std::sort(points_sortZ.begin(), points_sortZ.end(), sortByXAsc());
    minZ = points_sortZ[0].getX();
    maxZ = points_sortZ[points_sortZ.size()-1].getX();

    std::sort(coordinates.begin(),coordinates.end(),sortByXAsc());
    minX = coordinates[0].getX();
    maxX = coordinates[coordinates.size()-1].getX();

    std::sort(coordinates.begin(),coordinates.end(),sortByYAsc());
    minY = coordinates[0].getY();
    maxY = coordinates[coordinates.size()-1].getY();

}

void draw::delaunaydraw(QSize s){
    h = s.height();
    w = s.width();

   edges=Algorithms::dt(coordinates);
   triangulations = Algorithms::convertDTM(edges);
}

void draw::slopedraw(){

    // convert triangles to polygons
    QPainter painter(this);
    painter.begin(this);
    painter.scale(w/(maxX-minX),h/(maxY-minX));
    painter.translate(-minX,-minY);

    QPolygonF pol;

    for(unsigned int i = 0; i < triangulations.size(); i++)
    {
        pol.push_back(triangulations[i].getP1());
        pol.push_back(triangulations[i].getP2());
        pol.push_back(triangulations[i].getP3());

        drawpolygons.push_back(pol);

        pol.clear();
    }
    for(unsigned int i = 0; i < drawpolygons.size(); i++)
    {   //try to change shade of colour
        unsigned col = triangulations[i].getSlope()*200/180;
        col =col+ 5;

        // set filling color
        QBrush painpolslope;
        painpolslope.setColor(QColor(col,col,col));
        painpolslope.setStyle(Qt::SolidPattern);

        // initialize polygon painter path
        QPainterPath psway;

        psway.addPolygon(drawpolygons[i]);
        painter.fillPath(psway,painpolslope);
    }
    painter.end();

}

void draw::orientdraw(){
    QPainter painter(this);
    painter.begin(this);
    painter.scale(w/(maxX-minX),h/(maxY-minX));
    painter.translate(-minX,-minY);

    // convert triangles to polygons
    QPolygonF pol;

    for(unsigned int i = 0; i < triangulations.size(); i++)
    {
        pol.push_back(triangulations[i].getP1());
        pol.push_back(triangulations[i].getP2());
        pol.push_back(triangulations[i].getP3());

        drawpolygons.push_back(pol);

        pol.clear();
    }

    for(unsigned int i = 0; i < drawpolygons.size(); i++)
    {
        double exposit_angl = triangulations[i].getExposition();

        // set filling color
        QBrush painpolorient;

        if(exposit_angl < -157.5)
        painpolorient.setColor(Qt::yellow);

        if(exposit_angl >= -157.5 && exposit_angl < -112.5)
        painpolorient.setColor(Qt::blue);

        if(exposit_angl >= -112.5 && exposit_angl < -67.5)
        painpolorient.setColor(Qt::black);

        if(exposit_angl >= -67.5 && exposit_angl < 22.5)
        painpolorient.setColor(Qt::magenta);

        if(exposit_angl >= 22.5 && exposit_angl < 22.5)
        painpolorient.setColor(Qt::cyan);

        if(exposit_angl >= 22.5 && exposit_angl < 67.5)
        painpolorient.setColor(Qt::white);

        if(exposit_angl >= 67.5 && exposit_angl < 112.5)
        painpolorient.setColor(Qt::red);

        if(exposit_angl >= 112.5 && exposit_angl < 157.5)
        painpolorient.setColor(Qt::green);

        if(exposit_angl >= 157.5)
        painpolorient.setColor(Qt::yellow);

        painpolorient.setStyle(Qt::SolidPattern);

        // initialize polygon painter path
        QPainterPath path;

        path.addPolygon(drawpolygons[i]);
        painter.fillPath(path,painpolorient);
   }
    painter.end();
}

void draw::contoursdrawing(){
    QPainter painter(this);
    painter.begin(this);
    painter.scale(w/(maxX-minX),h/(maxY-minX));
    painter.translate(-minX,-minY);
    mdistd=distd*5;
    cont_edge=Algorithms::createContours(edges, minhd, maxhd, distd);
    mcont_edge=Algorithms::createContours(edges, minhd, maxhd, mdistd);

    for(unsigned int i = 0; i < cont_edge.size(); i++)
    {
        painter.drawLine(cont_edge[i].getStart(),cont_edge[i].getEnd());
    }
    // main contoures
    QPen line;
    line.setWidth(3);
    painter.setPen(line);

    for(unsigned int i = 0; i < mcont_edge.size(); i++)
    {
        painter.drawLine(mcont_edge[i].getStart(),mcont_edge[i].getEnd());
    }
    painter.end();

}

void draw::clear()
{
    edges.clear();
    triangulations.clear();
    drawpolygons.clear();
    cont_edge.clear();
    mcont_edge.clear();
    minZ = 0;
    maxZ=0;
    minX = 0;
    maxX = 0;
    minY = 0;
    maxY = 0;
    maxhd = 0;
    minhd = 0;
    mdistd = 0;
    distd = 0;
    orientflag = false;
    slopeflag = false;
    contflag = false;
    repaint();
}

void draw::paintEvent(QPaintEvent *e)
{

    QPainter painter(this);
    painter.begin(this);
    painter.scale(w/(maxX-minX),h/(maxY-minX));
    painter.translate(-minX,-minY);
    // draw points
    for(unsigned int i = 0; i < coordinates.size(); i++)
    {
        painter.drawPoint(coordinates[i]);
        painter.drawEllipse(coordinates[i].x()-1, coordinates[i].y()-1, 2, 2);
    }

    // draw triangles
    for(unsigned int i = 0; i < edges.size(); i++)
    {
        painter.drawLine(edges[i].getStart(),edges[i].getEnd());
    }
painter.end();
// slope
if(slopeflag==true){
    slopedraw();
}
if(orientflag==true){
    orientdraw();

}
if(contflag == true){
    contoursdrawing();
}


}
