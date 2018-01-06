#ifndef SORTBYYASC_H
#define SORTBYYASC_H
#include "qpoint3d.h"

class sortByYAsc
{
public:
    sortByYAsc();
    bool operator()(QPoint3D &p1, QPoint3D &p2){
        const double EPS = 1.0e-4;
        if(abs(p1.getY() - p2.getY())<EPS)
            return (p1.getX() < p2.getX());
        else
            return (p1.getY() < p2.getY());

    }
};

#endif // SORTBYYASC_H
