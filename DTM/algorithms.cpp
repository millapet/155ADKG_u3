#include <list>
#include "algorithms.h"
#include "sortbyxasc.h"

Algorithms::Algorithms()
{

}

int Algorithms::getPosition(QPoint3D &q,QPoint3D &a, QPoint3D &b)
{
    double eps = 1.0e-6;
    double ux = b.getX() - a.getX();
    double uy = b.getY() - a.getY();
    double vx = q.getX() - a.getX();
    double vy = q.getY() - a.getY();

    double det = (ux * vy - uy * vx);
    if(det>eps)
        return 1;

    if(det<-eps)
        return 0;

    return -1;
}

double Algorithms::getCircleRadius(QPoint3D &p1, QPoint3D &p2,QPoint3D &p3)
{
    //Return circle radius
    double x1 = p1.getX();
    double y1 = p1.getY();
    double x2 = p2.getX();
    double y2 = p2.getY();
    double x3 = p3.getX();
    double y3 = p3.getY();

    //Compute k1 - k12
    double k1 = x1 * x1 + y1 * y1;
    double k2 = x2 * x2 + y2 * y2;
    double k3 = x3 * x3 + y3 * y3;
    double k4 = y1 - y2;
    double k5 = y1 - y3;
    double k6 = y2 - y3;
    double k7 = x1 - x2;
    double k8 = x1 - x3;
    double k9 = x2 - x3;
    double k10 = x1 * x1;
    double k11 = x2 * x2;
    double k12 = x3 * x3;

    //Mid-point of the circle
    double m = 0.5 * (k12 * (-k4) + k11 * k5 - (k10 + k4 * k5) * k6)/
                     (x3 * (-k4) + x2 * k5 + x1 * (-k6));
    double n = 0.5 * ((k1 * (-k9) + k2 * k8 + k3 * (-k7))/
                      (y1 * (-k9) + y2 * k8 + y3 * (-k7)));

    //Radius
    return sqrt((x1 - m) * (x1 - m) + (y1 - n) * (y1 - n));

}

int Algorithms::getDelaunayPoint(Edge &e, std::vector <QPoint3D> &points)
{
    QPoint3D p1 = e.getStart();
    QPoint3D p2 = e.getEnd();
    int i_min = -1;
    double r_min = 1e9;

    for( int i = 0 ; i < points.size() ; i++)
    {
        int test = getPosition(points[i], p1, p2);
        if( test > 0){
            double radius = getCircleRadius(points[i], p1, p2);
            if (radius < r_min)
            {
                r_min = radius;
                i_min = i;
            }
        }
    }
return i_min;
}

int Algorithms::getNearestPoint(QPoint3D &p, std::vector <QPoint3D> &points)
{
    int i_min = -1;
    double d_min = 1e9;

    for(int i = 0; i < points.size(); i++)
    {
        double dy =points[i].getY() - p.getY();
        double dx =points[i].getX() - p.getX();

        if(fabs(dy) > 0  || fabs(dx) > 0)
        {
            double d=sqrt(dy*dy+dx*dx);

            if (d<d_min)
            {
                i_min=i;
                d_min=d;
            }
        }
    }
    return i_min;
}

std::vector<Edge> Algorithms::dt(std::vector <QPoint3D> &points)
{
    std::list<Edge> ael;
    std::vector<Edge> dt;

    //Sort points according to x coordinate
    std::sort(points.begin(),points.end(),sortByXAsc());

    //Find the nearest point
    QPoint3D p1 = points[0];
    int i_nearest = getNearestPoint(p1,points);

    //Nearest point has been found?
    if(i_nearest != -1)
    {
        QPoint3D p2 = points[i_nearest];
        Edge e(p1, p2);

        //Get delaunay point
        int i_dt = getDelaunayPoint(e, points);

        //Delaunay point has been found
        if( i_dt != -1)
        {
            //Create triangle edges
            QPoint3D pi = points[i_dt];
            Edge e2(e.getEnd(),pi);
            Edge e3(pi,e.getStart());

            //Addsedges to ael
            ael.push_back(e);
            ael.push_back(e2);
            ael.push_back(e3);

            //Add e3dges to dt
            dt.push_back(e);
            dt.push_back(e2);
            dt.push_back(e3);
        }

        //Delaunay point has not been found, switch orientation
        else{

            e.switchOrientation();
            i_dt = getDelaunayPoint(e, points);
            if( i_dt != -1)
            {
                //Create triangle edges
                QPoint3D pi = points[i_dt];
                Edge e2(e.getEnd(),pi);
                Edge e3(pi,e.getStart());

                //Addsedges to ael
                ael.push_back(e);
                ael.push_back(e2);
                ael.push_back(e3);

                //Add e3dges to dt
                dt.push_back(e);
                dt.push_back(e2);
                dt.push_back(e3);

            }

            //Probably colinear points
            else
                return dt;

        }
    }

    //Process list of active edges until the list is empty
    while(!ael.empty())
    {
        //Get first edge
       Edge e = ael.back();
       ael.pop_back();
       e.switchOrientation();

       int i_dt = getDelaunayPoint(e,points);

       //Delaunay point has been found
       if (i_dt > -1)
       {
           //Create remaining edges
           Edge e2(e.getEnd(),points[i_dt]);
           Edge e3(points[i_dt],e.getStart());

           //Add triangle to DT
           dt.push_back(e);
           dt.push_back(e2);
           dt.push_back(e3);

           //Swich orientation
           e2.switchOrientation();
           e3.switchOrientation();

           //Find edge with the opposite orientation
           std::list<Edge>::iterator i_e2 = std::find(ael.begin(),ael.end(),e2);
           std::list<Edge>::iterator i_e3 = std::find(ael.begin(),ael.end(),e3);

           //Edge e2 has not been found
           if(i_e2 == ael.end())
           {
               e2.switchOrientation();
               ael.push_back(e2);
           }
           //Edge e2 has been found
           else {
               ael.erase(i_e2);
           }


           //Edge e3 has not been found
           if(i_e3 == ael.end())
           {
               e3.switchOrientation();
               ael.push_back(e3);
           }
           //Edge e3 has been found
           else {
               ael.erase(i_e3);
           }

       }
    }

    return dt;
}

QPoint3D Algorithms::getConPoint(QPoint3D &p1, QPoint3D &p2, double z)
{
    double x1 = p1.getX();
    double x2 = p2.getX();
    double y1 = p1.getY();
    double y2 = p2.getY();
    double z1 = p1.getZ();
    double z2 = p2.getZ();

    double xb = ((x2 - x1)/(z2 - z1))*(z-z1) + x1;
    double yb = ((y2 - y1)/(z2 - z1))*(z-z1) + y1;

    QPoint3D c(xb,yb,z);

    return c;

}

std::vector<Edge> Algorithms::createContours(std::vector<Edge> &dt, double zmin, double zmax, double h)
{
    std::vector<Edge> contours;
    double eps = 5e-4;

    for(int i=0; i<dt.size()-2; i=i+3){

        //Get triangle vertices
        QPoint3D p1=dt[i].getStart();
        QPoint3D p2=dt[i].getEnd();
        QPoint3D p3=dt[i+2].getStart();

        //Create all contour lines for given triangle
        for(double z = zmin; z <= zmax; z+=h)
        {
            //Get height differences between points and plane
            double dz1 = p1.getZ() - z;
            double dz2 = p2.getZ() - z;
            double dz3 = p3.getZ() - z;

            //Points in the plane?
            bool b1 = (fabs(dz1)<eps);
            bool b2 = (fabs(dz2)<eps);
            bool b3 = (fabs(dz3)<eps);

            //Edges in the plane
            bool b12 = b1 && b2;
            bool b23 = b2 && b3;
            bool b31 = b3 && b1;

            //Edges intersected by the plane
            bool bi12 = dz1 * dz2 <0;
            bool bi23 = dz2 * dz3 <0;
            bool bi31 = dz3 * dz1 <0;

            // case 1 - triangle is coplanar
            if (b12 && b23 )
                continue;

            // case 2 -  triangle edge colinear
            else if(b12 || b23 || b31){
                //First triangle edge is colinear
                if(b12)
                {
                    contours.push_back(dt[i]);
                }

                //Second triangle edge is colinear
                else if(b23)
                {
                    contours.push_back(dt[i+1]);
                }

                //Third triangle edge is colinear
                else
                {
                    contours.push_back(dt[i+2]);
                }

            }

            //case 3 - onlz one point lies on plane
            else if(b1||b2||b3)
                continue;

            // case 4 - contour line passing through a point and intersecting edge
            else if(b1 && bi23 || b2 && bi31  || b3 && bi12){
                //p1 x (p2,p3)
                if(b1 && bi23)
                {
                    QPoint3D i23 = getConPoint(p2,p3,z);
                    contours.push_back(Edge(p1,i23));
                }
                //p2 x (p3,p1)
                else if(b2 && bi31)
                {
                    QPoint3D i31 = getConPoint(p3,p1,z);
                    contours.push_back(Edge(p2,i31));
                }
                //p3 x (p1,p2)
                else if(b3 && bi12)
                {
                    QPoint3D i12 = getConPoint(p1,p2,z);
                    contours.push_back(Edge(p3,i12));
                }
            }

            // case 5 - contour line intersects both edges
            else if(bi12 && bi31 || bi23 && bi12 || bi23 && bi31)
            {
                    //(p1,p2) x (p3,p1)
                    if(bi12 && bi31)
                    {
                        QPoint3D i12 = getConPoint(p1,p2,z);
                        QPoint3D i31 = getConPoint(p3,p1,z);

                        contours.push_back(Edge(i12,i31));
                    }

                    //(p2,p3) x (p1,p2)
                    else if(bi23 && bi12)
                    {
                        QPoint3D i23 = getConPoint(p2,p3,z);
                        QPoint3D i12 = getConPoint(p1,p2,z);

                        contours.push_back(Edge(i23,i12));
                    }

                    //(p2,p3) x (p3,p1)
                    else if(bi23 && bi31)
                    {
                        QPoint3D i23 = getConPoint(p2,p3,z);
                        QPoint3D i31 = getConPoint(p3,p1,z);

                        contours.push_back(Edge(i23,i31));
                    }
                }
            }
        }


    return contours;

}

std::vector<Triangle> Algorithms::convertDTM(std::vector<Edge>&dt){
    std::vector<Triangle> dtt;

    for(int i=0; i<dt.size()-2; i += 3 ){
        QPoint3D p1 = dt[i].getStart();
        QPoint3D p2 = dt[i].getEnd();
        QPoint3D p3 = dt[i+1].getEnd();

        Triangle t(p1,p2,p3,0,0);
        dtt.push_back(t);
    }

    return dtt;
}

double Algorithms::getSlope(Triangle &tr)
{
    QPoint3D p1 = tr.getP1();
    QPoint3D p2 = tr.getP2();
    QPoint3D p3 = tr.getP3();

    double ux = p1.getX() - p2.getX();
    double uy = p1.getY() - p2.getY();
    double uz = p1.getZ() - p2.getZ();
    double vx = p3.getX() - p2.getX();
    double vy = p3.getY() - p2.getY();
    double vz = p3.getZ() - p2.getZ();

    double nx = uy*vz-vy*uz;
    double ny = -(ux*vz-vx*uz);
    double nz = ux*vy-vx*uy;

    return acos(fabs(nz)/ sqrt(nx*nx + ny*ny + nz*nz)) * 180/ atan (1);

}

double Algorithms::getExposition(Triangle &tr){
    QPoint3D p1 = tr.getP1();
    QPoint3D p2 = tr.getP2();
    QPoint3D p3 = tr.getP3();

    double ux = p1.getX() - p2.getX();
    double uy = p1.getY() - p2.getY();
    double uz = p1.getZ() - p2.getZ();
    double vx = p3.getX() - p2.getX();
    double vy = p3.getY() - p2.getY();
    double vz = p3.getZ() - p2.getZ();

    double nx = uy*vz-vy*uz;
    double ny = -(ux*vz-vx*uz);

    return atan2(nx,ny)* 180/ atan (1);
}

void Algorithms::getSlopes(std::vector<Triangle> &dtt)
{
    for(int i = 0; i<dtt.size(); i++){
        dtt[i].setSlope(getSlope(dtt[i]));
    }
}
