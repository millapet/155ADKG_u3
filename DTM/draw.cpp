#include "draw.h"
#include "qpoint3d.h"
#include <QDebug>
#include <QtGui>
#include "algorithms.h"
#include "triangle.h"
#include "edge.h"
#include "sortbyxasc.h"


draw::draw(QWidget *parent) : QWidget(parent)
{

}

void draw::loaddata(std::vector<double> &data){

    double numb;
    double x;
    double y;
    double z;

    // data parse
    for( int i = 0; i < data.size(); i=i+3) //i+4
    {
      /*  numb=data[i];
        x = data[i+1];
        y = data[i + 2];
        z = data[i + 3];*/

         x = data[i];
         y = data[i + 1];
         z = data[i + 2];

         QPoint3D p (x, y, z);
         coordinates.push_back(p);

     }


    std::vector<QPoint3D> points_sort;
    double x1;
    double z1;

    for(int i = 0; i < coordinates.size(); i++)
    {
         x1 = coordinates[i].getX();
         z1 = coordinates[i].getZ();

         QPoint3D h_point (z1, x1, 0);
         points_sort.push_back(h_point);
    }

    std::sort(points_sort.begin(), points_sort.end(), sortByXAsc());

    min = points_sort[0].getX();
    max = points_sort[points_sort.size()-1].getX();

}

void draw::delaunaydraw(bool delaunay_click){
   edges=Algorithms::dt(coordinates);
   triangulations = Algorithms::convertDTM(edges);
   edges_bool=true;
   triangulations_bool=true;
   delaunaydraw_bool=true;


}

void draw::slopedraw(bool slopecklick){
    slopedraw_bool=true;
    orientdraw_bool=false;

    // convert triangles to polygons
    QPolygonF pol;

    for(int i = 0; i < triangulations.size(); i++)
    {
        pol.push_back(triangulations[i].getP1());
        pol.push_back(triangulations[i].getP2());
        pol.push_back(triangulations[i].getP3());

        drawpolygons.push_back(pol);

        pol.clear();
    }
}

void draw::orientdraw(bool orientcklick){
    slopedraw_bool=false;
    orientdraw_bool=true;

    // convert triangles to polygons
    QPolygonF pol;

    for(int i = 0; i < triangulations.size(); i++)
    {
        pol.push_back(triangulations[i].getP1());
        pol.push_back(triangulations[i].getP2());
        pol.push_back(triangulations[i].getP3());

        drawpolygons.push_back(pol);

        pol.clear();
    }
}

void draw::contoursdrawing(int maxh, int minh, int mdist1, int dist1){
    maxhd=maxh;
    minhd=minh;
    mdistd=mdist1;
    distd=dist1;
    cont_edge=Algorithms::createContours(edges, minhd, maxhd, mdistd*distd);
    mcont_edge=Algorithms::createContours(edges, minhd, maxhd, distd);
}

void draw::paintEvent(QPaintEvent *e)
{

    QPainter painter(this);
    painter.begin(this);

    // draw points
    for(unsigned int i = 0; i < coordinates.size(); i++)
    {
        painter.drawPoint(coordinates[i]);
        painter.drawEllipse(coordinates[i].x()-1, coordinates[i].y()-1, 2, 2);
    }

    coordinates_bool=true;


    // draw triangles
    for(unsigned int i = 0; i < edges.size(); i++)
    {
        painter.drawLine(edges[i].getStart(),edges[i].getEnd());
    }

// slope
if(slopedraw_bool==true){
        for(unsigned int i = 0; i < drawpolygons.size(); i++)
        {   //try to change shadow of colour
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
}

    // draw orientation

       if(orientdraw_bool==true){
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
    }

       // contours
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
}
