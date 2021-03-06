#ifndef EDGE_H
#define EDGE_H
#include "qpoint3d.h"

class Edge
{
    private:
        QPoint3D start, end;

    public:
        Edge() : start(0.0,0.0,0.0), end(0.0,0.0,0.0){}
        Edge(QPoint3D &a, QPoint3D &b) : start(a), end(b){}
        QPoint3D & getStart(){return start;}
        QPoint3D & getEnd(){return end;}
        void switchOrientation(){
            QPoint3D c = start;
            start = end;
            end = c;
        }

        bool operator==(const Edge &e) const
        {
            return(e.start == start) && (e.end == end);
        }
};

#endif // EDGE_H


