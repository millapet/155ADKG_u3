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
    std::vector<QPoint3D> coordinates;
    //bool edges_bool=false;
    //bool triangulations_bool=false;
    //bool coordinates_bool=false;
    //bool delaunaydraw_bool=false;
    bool slopeflag=false;
    bool orientflag=false;
    bool contflag = false;
    double minZ;
    double maxZ;
    int maxhd;
    int minhd;
    int mdistd;
    int distd;
    double minX;
    double maxX;
    double minY;
    double maxY;
    void loaddata(std::vector<double> &data);
    void delaunaydraw(QSize s);
    void clear();
private:
    std::vector<Edge> edges;
    std::vector<Triangle> triangulations;
    std::vector<QPolygonF> drawpolygons;
    std::vector<Edge> cont_edge;
    std::vector<Edge> mcont_edge;
    void slopedraw();
    void orientdraw();
    void contoursdrawing();

    int w;
    int h;

signals:

public slots:
};

#endif // DRAW_H
