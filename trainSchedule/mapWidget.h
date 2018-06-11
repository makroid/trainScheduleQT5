#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWebEngineView>
#include <QSize>
#include <QPoint>

#include "mapPage.h"

class MapRoute;
class QAction;

class MapWidget : public QWebEngineView
{
    Q_OBJECT
public:
    MapWidget(QWidget* parent = 0);
    ~MapWidget();

    QSize minimumSizeHint() const;
    MapPage* getMapPage() const { return mapPage; }
    double getDistanceInMeter() const;
    void fillMapRoute(MapRoute&) const;
    int loadMapRoute(const MapRoute&) const;
    bool extendMapRoute(const MapRoute&) const;
    bool isCurRouteNull() const { return mapPage->evalIsCurRouteNull(); }

protected:
   void mousePressEvent(QMouseEvent *);
   void wheelEvent(QWheelEvent* event);

   bool eventFilter(QObject *object, QEvent *event);

private:
   QPoint clickPos;
    MapPage* mapPage;
    QAction* addMarkerAction;

    void createActions();

private slots:
    void addMarker();
    void showContextMenu(const QPoint& pos);
};

#endif // MAPWIDGET_H
