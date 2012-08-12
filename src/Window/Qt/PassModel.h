#ifndef PassModel_H
#define PassModel_H

#include "Renderer/RenderPasses.h"

#include <QAbstractTableModel>

class PassModel : public QAbstractListModel {
    Q_OBJECT
public:
    PassModel(QObject *parent);
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &) const;

    bool setData(const QModelIndex& index, const QVariant& value, int role);

signals:
  void draw();

};

#endif // PassModel_H
