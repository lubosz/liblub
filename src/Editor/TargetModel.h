#ifndef TargetModel_H
#define TargetModel_H

#include "Renderer/RenderPasses.h"

#include <QAbstractTableModel>

class TargetModel : public QAbstractListModel {
    Q_OBJECT
public:
    TargetModel(QObject *parent);
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &) const;

    bool setData(const QModelIndex& index, const QVariant& value, int role);

signals:
  void draw();

};

#endif // TargetModel_H
