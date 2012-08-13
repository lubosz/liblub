#include "TextureModel.h"
#include "Scene/Scene.h"

TextureModel::TextureModel(QObject *parent) : QAbstractListModel(parent) {}

int TextureModel::rowCount(const QModelIndex &) const {
    return Scene::Instance().textures.size();
}

Qt::ItemFlags TextureModel::flags(const QModelIndex &) const {
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant TextureModel::data(const QModelIndex &index, int role) const {

    if (role == Qt::DisplayRole) {
        int row = index.row();
        Texture * texture = Scene::Instance().textures.values().at(row);
        return QString::fromStdString(texture->name);
    }

    return QVariant();
}

bool TextureModel::setData(const QModelIndex& index, const QVariant& value, int role) {
//    int row = index.row();

//    DebugPlane * plane = DeferredRenderer::Instance().sinkPass->debugPlanes[row];

//    if (role == Qt::CheckStateRole) {
//        if (value == Qt::Checked) {
//            plane->visible = true;
//        } else {
//            plane->visible = false;
//        }
//        emit draw();
//    }
    return true;
}
