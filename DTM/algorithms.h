#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include <vector>

#include "edge.h"
#include "qpoint3d.h"

class Algorithms
{
public:
    Algorithms();
    static int getPosition(QPoint3D &q,QPoint3D &a, QPoint3D &b);
    static double getCircleRadius(QPoint3D &p1, QPoint3D &p2,QPoint3D &p3);
    static int getDelaunayPoint(Edge &e, std::vector <QPoint3D> &points);
    static int getNearestPoint(QPoint3D &p, std::vector <QPoint3D> &points);

    static std::vector<Edge> dt(std::vector <QPoint3D> &points);
    static QPoint3D getConPoint(QPoint3D &p1,QPoint3D &p2, double z);

    static std::vector<Edge> createContours(std::vector<Edge>&dt, double zmin, double zmax, double h);
};

#endif // ALGORITHMS_H
