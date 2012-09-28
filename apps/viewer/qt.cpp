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
#include "Application/Qt/QtApplication.h"

#include "DemoLauncher.h"

class LoadApp: public QtApplication {
public:

    DemoLauncher * launcher;

    LoadApp(int &argc, char **argv) : QtApplication(argc, argv) {
        launcher = new DemoLauncher(argc, argv);
    }

    void scene() {
        launcher->init();
    }

    void renderFrame() {
        launcher->draw();
    }

};

int main(int argc, char **argv) {
    LoadApp(argc,argv).run();
}
