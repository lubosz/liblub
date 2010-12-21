/*
 * Config.cpp
 *
 *  Created on: Dec 20, 2010
 *      Author: bmonkey
 */

#include "Config.h"
#include "Logger.h"
#include <QSettings>
#include <QSize>
#include <QStringList>

Config::Config() {
	// TODO Auto-generated constructor stub
	Logger::Instance().log("Init Settings");

	 QSettings settings("Lubosz", "liblub");
	 settings.setValue("fridge/color", Qt::white);
	 settings.setValue("fridge/size", QSize(32, 96));
	 settings.setValue("sofa", true);
	 settings.setValue("tv", false);
	 QStringList foo;
	 foo.append("foo");
	 foo.append("bar");
	 settings.setValue("myASS", foo);
	 QStringList bar = settings.property("myASS").toStringList();



}

Config::~Config() {
	// TODO Auto-generated destructor stub
}
