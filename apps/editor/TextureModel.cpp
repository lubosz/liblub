#include "TextureModel.h"
#include "Scene/Scene.h"

TextureModel::TextureModel(QObject *parent) : QAbstractListModel(parent) {}

int TextureModel::rowCount(const QModelIndex &) const {
    return Scene::Instance().textures.size();
}

int TextureModel::columnCount(const QModelIndex &) const {
    return 3;
}

Qt::ItemFlags TextureModel::flags(const QModelIndex &) const {
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant TextureModel::data(const QModelIndex &index, int role) const {

    if (role == Qt::DisplayRole) {
        int row = index.row();
        Texture * texture = Scene::Instance().textures.values().at(row);
        if (index.column() == 0) {
            return QString::number(texture->getHandle());
        } else if (index.column() == 1) {
            return QString::fromStdString(texture->name);
        } else if (index.column() == 2) {
            return QString::fromStdString(texture->path);
        }
    }

    return QVariant();
}
