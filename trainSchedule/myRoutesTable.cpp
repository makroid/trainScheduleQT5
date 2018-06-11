#include "myRoutesTable.h"
#include "helpers.h"

#include <QHeaderView>
#include <QStringList>
#include <QAbstractItemView>
#include <QModelIndex>
#include <QtDebug>

#include "distSortFilterProxyModel.h"
#include "myRoutesModel.h"
#include "mapRoute.h"
#include "mapWidget.h"

#include <QtWebChannel>

MyRoutesTable::MyRoutesTable(QSharedPointer<TrainingData> td, MapWidget* mw, QWidget *parent)
    : QTableView(parent),
      mapWidget(mw),
      model(td->getMyRoutesModel().data())
{
    proxyModel = new DistSortFilterProxyModel(this);
    qDebug() << "model number of lines=" << model->getMyRouteCount();
    proxyModel->setSourceModel(model);
    proxyModel->setFilterKeyColumn(0);
    // javascript is now able to communicate with proxy model, see setCurPos in javascript-file
    //mapWidget->getMapPage()->mainFrame()->addToJavaScriptWindowObject("QTMyRoutesProxyModel", proxyModel);

    //QWebChannel * channel1 = new QWebChannel(this);
    //channel1->registerObject(QString("QTMyRoutesProxyModel"), proxyModel);
    //mapWidget->page()->setWebChannel(channel1);

    QWebChannel * channel;
    if (mapWidget->page()->webChannel() != nullptr) {
        channel = mapWidget->page()->webChannel();
    } else {
        channel = new QWebChannel(mapWidget->page());
    }
    channel->registerObject(QString("QTMyRoutesTable"), this);
    mapWidget->page()->setWebChannel(channel);

    //mapWidget->getMapPage()->mainFrame()->addToJavaScriptWindowObject("QTMyRoutesTable", this);

    this->setModel(proxyModel);    
    this->setSortingEnabled(true);
    //this->sortByColumn(0, Qt::AscendingOrder);

    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->horizontalHeader()->setStretchLastSection(true);
    this->verticalHeader()->hide();
    this->setSelectionMode(QAbstractItemView::SingleSelection);    
}

void MyRoutesTable::addEntry(const MapRoute &amapRoute) {
    // inserts empty MapRoute into model
    qDebug() << "before insertRows";
    model->insertRows(0, 1, QModelIndex());
    qDebug() << "after inserRows";
    QModelIndex index = model->index(0, 0, QModelIndex());
    qDebug() << "after index";
    model->setData(index, amapRoute.getName(), Qt::DisplayRole);
    qDebug() << "after setData";
    model->setMyRoute(index, amapRoute);
    qDebug() << "after setMyRoute";
}

void MyRoutesTable::editEntry() {

}

void MyRoutesTable::removeEntry() {
    QItemSelectionModel* selectionModel = this->selectionModel();
    QModelIndexList indices = selectionModel->selectedRows();
    QModelIndex index;

    foreach (index, indices) {
        int row = proxyModel->mapToSource(index).row();
        model->removeRows(row, 1, QModelIndex());
    }
}

int MyRoutesTable::insertMyRouteIntoMap(QModelIndex index) {
    int row = proxyModel->mapToSource(index).row();
    return mapWidget->extendMapRoute(model->getMyRoute(row));
}

void MyRoutesTable::setPosAndSort(double lat, double lng) {
    //if (this->isVisible() == false) return;
    qDebug() << "setPosAndSort" << lat << "," << lng;
    proxyModel->setCurPos(lat, lng);
    this->sortByColumn(0, Qt::AscendingOrder);
}
