#include "PassModel.h"
#include <QTime>
#include "Renderer/DeferredRenderer.h"

PassModel::PassModel(QObject *parent) : QAbstractListModel(parent) {}

int PassModel::rowCount(const QModelIndex &) const {
    return DeferredRenderer::Instance().drawPasses.size();
}

Qt::ItemFlags PassModel::flags(const QModelIndex &) const {
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
}

QVariant PassModel::data(const QModelIndex &index, int role) const {
    int row = index.row();

    DrawThing * pass = DeferredRenderer::Instance().drawPasses[row];

    QString passType = "Pass";

    SourcePass * sourceCheck = dynamic_cast<SourcePass*>(pass);
    if (sourceCheck != nullptr) {
        passType = "SourcePass";
    }

    InOutPass * inOutCheck = dynamic_cast<InOutPass*>(pass);
    if (inOutCheck != nullptr)
        passType = "InOutPass";

    SinkPass * sinkCheck = dynamic_cast<SinkPass*>(pass);
    if (sinkCheck != nullptr)
        passType = "SinkPass";

    switch(role) {
        case Qt::DisplayRole:
        return passType + QString::number(row);
            break;
        case Qt::CheckStateRole:
            if (pass->enabled) {
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

bool PassModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    int row = index.row();

    DrawThing * pass = DeferredRenderer::Instance().drawPasses[row];

    if (role == Qt::CheckStateRole) {
        if (value == Qt::Checked) {
            pass->enabled = true;
        } else {
            pass->enabled = false;
        }
        emit draw();
    }
    return true;
}
