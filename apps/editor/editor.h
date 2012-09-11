/*
 Copyright © 2010 Lubosz Sarnecki

 This file is part of liblub.

 liblub is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 liblub is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with liblub.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef EDITOR_H
#define EDITOR_H

#define QT_NO_GRAPHICSEFFECT 1

#include <QHBoxLayout>
#include <QComboBox>

#include "Application/Qt/QtApplication.h"
#include "Renderer/RenderPasses.h"
#include "TargetModel.h"
#include "PassModel.h"


class Editor: public QtApplication {
    Q_OBJECT
public:

    Editor(int &argc, char **argv);

    ~Editor();

    void scene();
    void renderFrame();

    string scenePath;

    void initWidgets(QSplitter *mainSplitter);

  QComboBox * renderTargetSelector;
  QComboBox* transparencyModeSrc;
  QComboBox* transparencyModeDest;

  DebugPlane * selectedPlane;
  PassModel * passModel;
  TargetModel * targetModel;

  QMap<QString, GLuint> transparencyModes;

  QVBoxLayout *passLayout;
  QWidget *passLayoutWidget;

  GLuint srcMode;
  GLuint destMode;


  void updateTransparency();

public slots:
  void setSelectedTransparencySrc(const QString &srcModeName);
  void setSelectedTransparencyDest(const QString &destModeName);

  void setSelectedPlane(const QModelIndex &index);
  void setSelectedPass(const QModelIndex &index);
  void setSelectedTexture(const QModelIndex &index);
  void changePlaneSource(const QString &name);
  void setTransparency(bool transparent);

  void setOffSetFactor(double factor);
  void setOffSetUnits(double units);
};

#endif
