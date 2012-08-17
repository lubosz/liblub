#include "TargetModel.h"
#include <QTime>
#include "Renderer/DeferredRenderer.h"

TargetModel::TargetModel(QObject *parent) : QAbstractListModel(parent) {}

int TargetModel::rowCount(const QModelIndex &) const {
    SinkPass * sinkPass = DeferredRenderer::Instance().sinkPass;
    return sinkPass->debugPlanes.size();
}

Qt::ItemFlags TargetModel::flags(const QModelIndex &) const {
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
}

QVariant TargetModel::data(const QModelIndex &index, int role) const {
    int row = index.row();

    DebugPlane * plane = DeferredRenderer::Instance().sinkPass->debugPlanes[row];

    switch(role) {
        case Qt::DisplayRole:
            return QString::fromStdString(plane->targetName);
            break;
        case Qt::CheckStateRole:
            if (plane->visible) {
                return Qt::Checked;
            } else {
                return Qt::Unchecked;
            }
            break;
        default:
            break;
    }

    return QVariant();
}

bool TargetModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    int row = index.row();

    DebugPlane * plane = DeferredRenderer::Instance().sinkPass->debugPlanes[row];

    if (role == Qt::CheckStateRole) {
        if (value == Qt::Checked) {
            plane->visible = true;
        } else {
            plane->visible = false;
        }
        emit draw();
    }
    return true;
}
