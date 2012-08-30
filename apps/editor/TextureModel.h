#ifndef TEXTUREMODEL_H
#define TEXTUREMODEL_H

#include <QAbstractTableModel>
class TextureModel : public QAbstractListModel {
    Q_OBJECT
 public:
    TextureModel(QObject *parent);
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    int columnCount(const QModelIndex &) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &) const;
};

#endif // TEXTUREMODEL_H
