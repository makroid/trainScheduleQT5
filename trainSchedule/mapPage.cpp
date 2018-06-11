#include "mapPage.h"
#include "configData.h"
#include "helpers.h"
#include "kmlParameters.h"


#include <QtDebug>
#include <QEventLoop>
#include <QTimer>

MapPage::MapPage(QWidget *parent) : QWebEnginePage(parent) {
}


bool MapPage::evalIsCurRouteNull() {
    QVariant result = runJavaScriptSync("isCurRouteNull();");
    //this->runJavaScript("isCurRouteNull();", [&result](const QVariant &aresult){ result = aresult; qDebug() << "isCurRouteNull result=" << result.toString();});
    qDebug() << "evalIsCurRouteNull.toString()=" << result.toString();
    return result.toBool();
}

void MapPage::evalUndo() {
    this->runJavaScriptSync("curRoute.undoStep();");
}

int MapPage::evalStartNewRoute()  {
    QVariant id = runJavaScriptSync("startNewSingleRoute();");
    //this->runJavaScript("startNewSingleRoute();", [&id](const QVariant &aresult){ id = aresult; });
    return id.toInt();
}

bool MapPage::evalSwitchToRoute(int aid) {
    QVariant result = runJavaScriptSync("switchToRoute(" + QString::number(aid) + ");");
    //this->runJavaScript("switchToRoute(" + QString::number(aid) + ");", [&result](const QVariant &aresult){ result = aresult; });
    return result.toBool();
}

bool MapPage::evalDeleteCurRoute() {
    QVariant result = runJavaScriptSync("deleteCurRoute();");
    //this->runJavaScript("deleteCurRoute();", [&result](const QVariant &aresult){ result = aresult; });
    return result.toBool();
}

void MapPage::evalSetCenter() {
    QString cmd = "setCenter(" + QString::number(ConfigData::instance().getStartLocation().x(), 'f', 7) + "," + QString::number(ConfigData::instance().getStartLocation().y(), 'f', 7) + ");";
    qDebug() << cmd;
    this->runJavaScript(cmd);
}

void MapPage::evalMoveToCenter() {
    QString cmd = "setCenter(" + QString::number(ConfigData::instance().getStartLocation().x(), 'f', 7) + "," + QString::number(ConfigData::instance().getStartLocation().y(), 'f', 7) + ");";
    cmd += "moveToCenter();";
    qDebug() << "evalMoveToCenter():" << cmd;
    this->runJavaScript(cmd);
}

// TODO: implement this
QString MapPage::evalExportAsKml(const KmlParameters& kml) {
    QString cmd = QString("generateKmlCode(%1,%2,%3);").arg(kml.durationInSec).arg(kml.altitudeInMeter).arg(kml.rangeInMeter);
    qDebug() << "evalExportAsKml()=" << cmd;
    QVariant kmlCode = runJavaScriptSync(cmd);
    //this->runJavaScript(cmd, [&kmlCode](const QVariant &aresult){ kmlCode = aresult; });

    return kmlCode.toString();
}

void MapPage::evalGeocodeAddress(const QString &address) {
    QString cmd = QString("geocodeLocationForAddress('%1');").arg(address);
    qDebug() << "evalGeocodeAddress=" << cmd;
    this->runJavaScriptSync(cmd);
}

void MapPage::evalCreateAndAddRouteMarker(int posX, int posY, const QString& markerText) {
    QString cmd = QString("curRoute.createAndAddRouteMarker(%1,%2,'%3',true);").arg(posX).arg(posY).arg(markerText);
    qDebug() << cmd;
    this->runJavaScriptSync(cmd);
}

void MapPage::evalJoinWithCurRoute(const MapRoute &amapRoute) {
    QString cmd = "var poly=[];";
    for (int i=0; i<amapRoute.getPolyline().length(); i++) {
        cmd += "poly.push(" + LatLng(amapRoute.getPolyline().at(i)) + ");";
    }
    cmd += QString("var len=%1;").arg(amapRoute.getDistanceInMeter());
    cmd += "curRoute.joinWithCurRoute(poly,len);";
   // qDebug() << "evalJoinWithCurRoute=" << cmd;
    this->runJavaScriptSync(cmd);
}

/////////////////////////////////////////////////////////////////////////////////////
// setters

void MapPage::evalSetCurPos(QPointF acurPos) {
    QString cmd = "curRoute.setCurPos(" + LatLng(acurPos) + ");";
    qDebug() << "evalSetCurPos()" << cmd;
    this->runJavaScriptSync(cmd);
}

void MapPage::evalSetPrevPos(QPointF aprevPos) {
    QString cmd = "curRoute.setPrevPos(" + LatLng(aprevPos) + ");";
    qDebug() << "evalSetPrevPos()" << cmd;
    this->runJavaScriptSync(cmd);
}

void MapPage::evalSetDistanceInMeter(double adistanceInMeter)  {
    QString cmd = QString("curRoute.setDistance(%1);").arg(adistanceInMeter);
    qDebug() << "evalSetDistance()" << cmd;
    this->runJavaScriptSync(cmd);
}

void MapPage::evalSetName(QString aname) {
    QString cmd = "curRoute.setName(" + aname + ");";
    qDebug() << "evalSetName()" << cmd;
    this->runJavaScriptSync(cmd);
}

void MapPage::evalSetPolylineAndFitBounds(const QList<QPointF> &apoly) {
    QString cmd = "var poly=[];";
    for (int i=0; i<apoly.length(); i++) {
        cmd += "poly.push(" + LatLng(apoly[i]) + ");";
    }
    cmd += "curRoute.setPolylineAndFitBounds(poly);";
    // qDebug() << "evalSetPolylineAndFitBounds()" << cmd;
    this->runJavaScriptSync(cmd);
}

void MapPage::evalSetHistorySteps(const QList<int> &asteps) {
    QString cmd = "var poly=[];";
    for (int i=0; i<asteps.length(); i++) {
        cmd += QString("poly.push(%1);").arg(asteps[i]);
    }
    cmd += "curRoute.setHistorySteps(poly);";
    // qDebug() << "evalSetHistorySteps()" << cmd;
    this->runJavaScriptSync(cmd);
}

void MapPage::evalSetHistoryDists(const QList<double> &adists) {
    QString cmd = "var dists=[];";
    for (int i=0; i<adists.length(); i++) {
        cmd += QString("dists.push(%1);").arg(adists[i]);
    }
    cmd += "curRoute.setHistoryDists(dists);";
    // qDebug() << "evalSetHistoryDists()" << cmd;
    this->runJavaScriptSync(cmd);
}

void MapPage::evalSetId(int aid) {
    QString cmd = QString("curRoute.setId(%1);").arg(aid);
    qDebug() << "evalSetId()" << cmd;
    this->runJavaScriptSync(cmd);
}

void MapPage::evalSetModus(MapRoute::clickmodus amodus) {
    QString cmd = "curRoute.setModus(" + QString::number(amodus) + ");";
    qDebug() << "evalSetModus()" << cmd;
    this->runJavaScriptSync(cmd);
}

void MapPage::evalSetDistanceMarkers() {
    QString cmd = "curRoute.updateDistMarkers(curRoute.poly.getPath().getArray(), 0);";
    this->runJavaScriptSync(cmd);
}

void MapPage::evalSetRouteMarkers(const QList<RouteMarker>& amarkers) {
    for (int i=0; i<amarkers.length(); i++) {
        QString cmd = QString("curRoute.createAndAddRouteMarker(%1,%2,'%3',false);").arg(amarkers[i].getLatLng().x(),0,'f',8).arg(amarkers[i].getLatLng().y(),0,'f',8).arg(amarkers[i].getMarkerText());
        qDebug() << "route marker cmd = " << cmd;
        this->runJavaScriptSync(cmd);
    }
}

void MapPage::evalSetShowRouteMarkers(bool isShow) {
    QString show = isShow ? "true" : "false";
    QString cmd = QString("curRoute.showAllRouteMarkers(%1);").arg(show);
    this->runJavaScriptSync(cmd);
}

int MapPage::loadMapRoute(const MapRoute& amapRoute) {
    qDebug() << "START loadMapRoute";
    int id = evalStartNewRoute();
    qDebug() << "evalStartNewRoute";
    evalSetName(amapRoute.getName());
    qDebug() << "getName";
    evalSetPolylineAndFitBounds(amapRoute.getPolyline());
    qDebug() << "getpolyline";
    evalSetHistorySteps(amapRoute.getHistorySteps());
    qDebug() << "historysteps";
    evalSetHistoryDists(amapRoute.getHistoryDists());
    qDebug() << "historydists";
    evalSetId(amapRoute.getMapId());
    qDebug() << "mapid";
    evalSetCurPos(amapRoute.getCurPos());
    qDebug() << "curPos";
    evalSetPrevPos(amapRoute.getPrevPos());
    qDebug() << "prevPos";
    evalSetModus(amapRoute.getModus());
    qDebug() << "modus";
    evalSetDistanceMarkers();
    evalSetDistanceInMeter(amapRoute.getDistanceInMeter());
    qDebug() << "routeMarkers";
    evalSetRouteMarkers(amapRoute.getRouteMarkers());
    qDebug() << "END loadMapRoute";
    return id;
}

// if there is already an existing route => extend it by amapRoute
// otherwise just load amapRoute (extend it to empty route)
bool MapPage::extendMapRoute(const MapRoute &amapRoute) {
    bool isCurRouteNull = evalIsCurRouteNull();
    bool startNewRoute = false;
    if (isCurRouteNull) {
        loadMapRoute(amapRoute);
        startNewRoute = true;
    } else {
        evalJoinWithCurRoute(amapRoute);
    }
    return startNewRoute;
}

/////////////////////////////////////////////////////////////////////////////////////
// getters

QString MapPage::evalGetName() {
    QVariant name = runJavaScriptSync("curRoute.getName();");
    //this->runJavaScript("curRoute.getName();", [&name](const QVariant &aresult){ name = aresult; });
    return name.toString();
}

int MapPage::evalGetId() {
    QVariant id = runJavaScriptSync("curRoute.getId();");
    //this->runJavaScript("curRoute.getId();", [&id](const QVariant &aresult){ id = aresult; });
    return id.toInt();
}

// return in meter
double MapPage::evalGetDistanceInMeter() {
    QVariant distance = runJavaScriptSync("curRoute.getDistance();");
    //this->runJavaScript("curRoute.getDistance();", [&distance](const QVariant &aresult){ distance = aresult; });
    return distance.toDouble();
}

QPointF MapPage::evalGetCurPos() {
    QVariant result = runJavaScriptSync("curRoute.getCurPos();");
    //this->runJavaScript("curRoute.getCurPos();", [&result](const QVariant &aresult){ result = aresult; });
    QPointF curPos;
    variantToPointF(result, curPos);
    return curPos;
}

QPointF MapPage::evalGetPrevPos() {
    QVariant result = runJavaScriptSync("curRoute.getPrevPos();");
    //this->runJavaScript("curRoute.getPrevPos();", [&result](const QVariant &aresult){ result = aresult; });
    QPointF prevPos;
    variantToPointF(result, prevPos);
    return prevPos;
}

QList<QPointF> MapPage::evalGetPolyline() {
    QVariant result = runJavaScriptSync("curRoute.getPolyline();");
    //this->runJavaScript("curRoute.getPolyline();", [&result](const QVariant &aresult){ result = aresult; });

    QList<QPointF> polyline;
    variantToPointFList(result, polyline);

    for (int i=0; i<polyline.length(); i++) {
         qDebug() << "polyline[" << i << "]=" << polyline[i].x() << "," << polyline[i].y();
    }
    return polyline;
}

QList<RouteMarker> MapPage::evalGetRouteMarkers() {
    QVariant result = runJavaScriptSync("curRoute.getRouteMarkersAsTxt();");
    //this->runJavaScript("curRoute.getRouteMarkersAsTxt();", [&result](const QVariant &aresult){ result = aresult; });

    QList<RouteMarker> myRouteMarkers;
    variantToRouteMarkerList(result, myRouteMarkers);

    return myRouteMarkers;
}

QList<int> MapPage::evalGetHistorySteps() {
    QVariant result = runJavaScriptSync("curRoute.getHistorySteps();");
    //this->runJavaScript("curRoute.getHistorySteps();", [&result](const QVariant &aresult){ result = aresult; });
    QVariantList vpath = result.toList();

    QList<int> historySteps;
    for (int i=0; i<vpath.length(); i++) {
        historySteps.push_back(vpath[i].toInt());
    }
    return historySteps;
}

QList<double> MapPage::evalGetHistoryDists() {
    QVariant result = runJavaScriptSync("curRoute.getHistoryDists();");
    //this->runJavaScript("curRoute.getHistoryDists();", [&result](const QVariant &aresult){ result = aresult; });
    QVariantList vpath = result.toList();

    QList<double> historyDists;
    for (int i=0; i<vpath.length(); i++) {
        historyDists.push_back(vpath[i].toDouble());
    }
    return historyDists;
}

void MapPage::fillMapRoute(MapRoute &amapRoute) {
    bool noRoute = evalIsCurRouteNull();
    if (noRoute) {
        return;
    }
    amapRoute.setMapId(evalGetId());
    qDebug() << "setMapId";
    amapRoute.setCurPos(evalGetCurPos());
    qDebug() << "setCurPos";
    amapRoute.setPrevPos(evalGetPrevPos());
    qDebug() << "setPrevPos";
    amapRoute.setModus(MapRoute::runnerAutomatic);
    qDebug() << "setModus";
    amapRoute.setPolyline(evalGetPolyline());
    qDebug() << "setPolyline";
    amapRoute.setRouteMarkers(evalGetRouteMarkers());
    amapRoute.setDistanceInMeter(evalGetDistanceInMeter());
    qDebug() << "setDistance";
    amapRoute.setHistorySteps(evalGetHistorySteps());
    qDebug() << "setHistorySteps";
    amapRoute.setHistoryDists(evalGetHistoryDists());
    qDebug() << "setHistoryDists";
    amapRoute.setName(evalGetName());
    qDebug() << "setName";   
}



QString MapPage::LatLng(const QPointF &apoint) const {
    // increase precision for apoint, because default values are too low!
    return QString("new google.maps.LatLng(%1,%2)").arg(apoint.x(), 0, 'f', 8).arg(apoint.y(), 0, 'f', 8);
}

void MapPage::variantToPointF(const QVariant &aval, QPointF& p) const {
    QVariantList vlist = aval.toList();
    p.setX(0);
    p.setY(0);
    if (vlist.size() == 2) {
        p.setX(vlist[0].toDouble());
        p.setY(vlist[1].toDouble());
    }
}

void MapPage::variantToPointFList(const QVariant &aval, QList<QPointF> &pl) const {
    QVariantList vlist = aval.toList();
    for (int i=0; i<vlist.size(); i+=2) {
        pl.push_back(QPointF(vlist[i].toDouble(), vlist[i+1].toDouble()));
    }
}

void MapPage::variantToRouteMarkerList(const QVariant &aval, QList<RouteMarker>& sl) const {
    /* a RouteMarker consists of:
     * 1) rank
     * 2) lat
     * 3) lng
     * 4) markertext
     **/
    QVariantList slist = aval.toList();
    for (int i=0; i<slist.size(); i++) {
        QStringList splits = slist[i].toString().split("$$$");
        if (splits.size() < 4) continue;
        QString markerText = "";
        // join marker text
        for (int j=3; j<splits.size(); j++) { markerText += splits[j]; }
        sl.push_back(RouteMarker(splits[0].toInt(), splits[1].toDouble(), splits[2].toDouble(), markerText));
    }
}


QVariant MapPage::runJavaScriptSync(QString cmd) {
    m_javascript_cmd = cmd;
    QEventLoop loop;
    QObject::connect(this, SIGNAL(notifyRanJavaScript()), &loop, SLOT(quit()));

    QTimer::singleShot(0, this, SLOT(startScript()));

    loop.exec();

    return m_scriptResult;
}


void MapPage::onScriptEnded(const QVariant & data)
{
    qDebug() << QStringLiteral("Script ended: ") << data;
    m_scriptResult = data;
    emit notifyRanJavaScript();
}


struct Functor
{
    Functor(MapPage & page) : m_page(page) {}
    void operator()(const QVariant & data)
    {
        m_page.onScriptEnded(data);
    }
    MapPage & m_page;
};

void MapPage::startScript()
{
    qDebug() << QStringLiteral("Start script");
    this->runJavaScript(m_javascript_cmd, Functor(*this));
}
