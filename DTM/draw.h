#ifndef DRAW_H
#define DRAW_H

#include <QWidget>
#include "qpoint3d.h"
#include "edge.h"
#include "triangle.h"

class draw : public QWidget
{
    Q_OBJECT
public:
    explicit draw(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *e);

    bool edges_bool=false;
    bool triangulations_bool=false;
    bool coordinates_bool=false;
    bool delaunaydraw_bool=false;
    bool slopedraw_bool=false;
    bool orientdraw_bool=false;

    std::vector<QPoint3D> coordinates;
    std::vector<Edge> edges;
    std::vector<Triangle> triangulations;
    std::vector<QPolygonF> drawpolygons;
    std::vector<Edge> cont_edge;
    std::vector<Edge> mcont_edge;
    double min;
    double max;
    int maxhd;
    int minhd;
    int mdistd;
    int distd;

    void loaddata(std::vector<double> &data);
    void delaunaydraw(bool delaunay_click);
    void slopedraw(bool slopecklick);
    void orientdraw(bool orientcklick);
    void contoursdrawing(int maxh,int minh,int mdist1,int dist1);

signals:

public slots:
};

#endif // DRAW_H
