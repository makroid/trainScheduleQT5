#include "mapAndControlWidget.h"
#include "mapAndOptionWidget.h"
#include "mapControlWidget.h"
#include "mapWidget.h"
#include "mapOptionWidget.h"
#include "configData.h"

#include <QtDebug>
#include <QKeyEvent>
#include <QDir>

MapAndControlWidget::MapAndControlWidget(QWidget* parent) :
    QSplitter(parent)
{
    createWidgets();
    createLayout();
    createConnections();
}

void MapAndControlWidget::createWidgets() {
    mapWidget = new MapWidget(this);

    QString htmlPath = QDir(QDir::currentPath()).filePath("mapsTest.html");

    QUrl url = QUrl::fromLocalFile(htmlPath);
    qDebug() << url.toLocalFile();
    mapWidget->load(url);

    qDebug() << QDir::currentPath();
    mapOptionWidget = new MapOptionWidget(mapWidget);
    mapAndOptionWidget = new MapAndOptionWidget(mapWidget, mapOptionWidget, this);

    mapControlWidget = new MapControlWidget(mapWidget);
    mapControlWidget->setEnabled(false);
}

void MapAndControlWidget::createLayout() {
    addWidget(mapAndOptionWidget);
    addWidget(mapControlWidget);

    QSizePolicy policy = sizePolicy();
    policy.setVerticalPolicy(QSizePolicy::Expanding);
    setSizePolicy(policy);
}

void MapAndControlWidget::createConnections() {
    connect(mapWidget, SIGNAL(loadFinished(bool)), mapControlWidget, SLOT(setEnabled(bool)));
    connect(mapWidget, SIGNAL(loadFinished(bool)), this, SLOT(initializeMap(bool)));
}

void MapAndControlWidget::initializeMap(bool b) {
    mapWidget->getMapPage()->runJavaScript("initialize();");
    mapWidget->getMapPage()->evalMoveToCenter();
}

void MapAndControlWidget::keyPressEvent(QKeyEvent *event) {
    switch (event->key()) {
        case Qt::Key_Shift: mapControlWidget->forwardModeRadioButton(); break;
        case Qt::Key_L: mapAndOptionWidget->setOptionVisible(true); break;
        default: break;
    }
}
