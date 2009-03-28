/**************************************************************************
 *  SharpEye Studio - Integrated Development Environment 
 **************************************************************************  
 *  University of Bristol
 *  Department of Electrical and Electronic Engineering 
 *
 *  Copyright (C) 2008 George Vafiadis  <vafiadis@ieee.org>
 *
 **************************************************************************
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2
 *  as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  The above copyright notice and this permission notice shall be 
 *  included in all copies or substantial portions of the Software.                    
 **************************************************************************
 */

#include <QtGui>
#include <QSplashScreen>
#include <time.h>
#include "sharpeye.h"
#include <QDialog>
#include "patternview.h"
#include "timedelay.h"

int main(int argc, char *argv[])
{
 Q_INIT_RESOURCE(sharpeye);

 QApplication app(argc, argv);
 
 QPixmap pixmap(":/SharpEye/images/splash.png");
 
 QSplashScreen splash(pixmap);
 splash.show();
 app.processEvents();

 gsleep(1);
 
 SharpEye mainwindow;

 if( argc == 2 ) mainwindow.openFile(argv[1]);

 
 mainwindow.show();


 splash.finish(&mainwindow);

 return app.exec();
}
