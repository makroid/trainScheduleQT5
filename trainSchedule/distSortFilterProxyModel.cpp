#include "distSortFilterProxyModel.h"

#include <QtDebug>

#include "helpers.h"

DistSortFilterProxyModel::DistSortFilterProxyModel(QObject* parent)
    : QSortFilterProxyModel(parent),
      curPos(0.0, 0.0)
{
}

bool DistSortFilterProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const {
    // start position of paths to compare
    QPointF startPosLeft = sourceModel()->data(left, Qt::UserRole).toPointF();
    QPointF startPosRight = sourceModel()->data(right, Qt::UserRole).toPointF();

    // decide which start position is nearer to curPos
    double distCurPosToLeft = Helper::distanceMeterBetween(curPos, startPosLeft);
    double distCurPosToRight = Helper::distanceMeterBetween(curPos, startPosRight);

    qDebug() << "distCurPosToLeft=" << distCurPosToLeft;
    qDebug() << "distCurPosToRight=" << distCurPosToRight;

    return distCurPosToLeft < distCurPosToRight;
}


void DistSortFilterProxyModel::setCurPos(double lat, double lng) {
    QPointF pos(lat, lng);
    qDebug() << "setCurPos(" << lat << "," << lng << ")";
    curPos = pos;
}
