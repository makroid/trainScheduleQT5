#ifndef MAPSPAGE_H
#define MAPSPAGE_H

#include <QWebEnginePage>
#include <QVariant>
#include <QMessageBox>
#include <QtDebug>
#include <QList>
#include <QPointF>
#include <QString>

class KmlParameters;

#include "mapRoute.h"
#include "routemarker.h"

class MapPage : public QWebEnginePage
{
    Q_OBJECT

public:
    MapPage(QWidget *parent = 0);
    virtual ~MapPage() {}

    void                fillMapRoute(MapRoute&);
    int                 loadMapRoute(const MapRoute&);
    bool                extendMapRoute(const MapRoute&);

    QString             LatLng(const QPointF&) const;

Q_SIGNALS:
    void notifyRanJavaScript();

public slots:
    // javascript function calls
    // getters
    bool                evalIsCurRouteNull();
    double              evalGetDistanceInMeter();
    QString             evalGetName();
    QList<QPointF>      evalGetPolyline();
    QList<int>          evalGetHistorySteps();
    QList<double>       evalGetHistoryDists();
    int                 evalGetId();
    QPointF             evalGetCurPos();
    QPointF             evalGetPrevPos();
    QList<RouteMarker>  evalGetRouteMarkers();

    // workers
    void                evalUndo();
    int                 evalStartNewRoute();
    bool                evalSwitchToRoute(int aid);
    bool                evalDeleteCurRoute();
    void                evalSetCenter();
    void                evalMoveToCenter();
    void                evalJoinWithCurRoute(const MapRoute&);
    QString             evalExportAsKml(const KmlParameters&);
    void                evalGeocodeAddress(const QString& address);
    void                evalCreateAndAddRouteMarker(int posX, int posY, const QString& markerText);

    // setters
    void                evalSetDistanceInMeter(double distanceInMeter);
    void                evalSetName(QString name);
    void                evalSetPolylineAndFitBounds(const QList<QPointF>&);
    void                evalSetHistorySteps(const QList<int>&);
    void                evalSetHistoryDists(const QList<double>&);
    void                evalSetId(int);
    void                evalSetCurPos(QPointF curPos);
    void                evalSetPrevPos(QPointF prevPos);
    void                evalSetModus(MapRoute::clickmodus);
    void                evalSetDistanceMarkers();
    void                evalSetRouteMarkers(const QList<RouteMarker>& amarkers);
    void                evalSetShowRouteMarkers(bool isShow);

    // handler
    void                startScript();
    void                onScriptEnded(const QVariant&);
    QVariant            runJavaScriptSync(QString);

protected:
    // this is needed to enable googlemaps for webkit
    //QString userAgentForUrl ( const QUrl & url ) const;

private:
    void variantToPointF(const QVariant&, QPointF&) const;
    void variantToPointFList(const QVariant&, QList<QPointF>&) const;
    void variantToRouteMarkerList(const QVariant &, QList<RouteMarker>&) const;

    QVariant m_scriptResult;
    QString m_javascript_cmd;

};

#endif // MAPSPAGE_H
