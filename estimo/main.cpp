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
 *  included in all copies or substantial portions of the Software
 **************************************************************************
 */

#include <iostream>
using namespace std;

#include <QApplication>
#include <QFileInfo>
#include "interpreter.h"
#include "estimodule.h"
#include "binaryword.h"
#include "settings.h"

int main(int argc, char **argv)
{
  QApplication app(argc, argv);

  QString conFile = QApplication::applicationDirPath() + "/configuration.ini";

  QFileInfo info(conFile);

  if( !info.exists() )
  {
	  cout << "Error: Cannot find configuration file..." << endl;
	  return -1;
  }

  try {
  cout << "Reading 'configuration.ini'..." << endl;

  const Settings & settings = getSettings();
  cout << "Using configuration version " << settings.version.toStdString() << endl;
  
  EstimoBash bash;

  if( argc == 2 )
    bash.run(argv[1]);
  else
  {
	  cout << "Estimo C Compiler: USAGE estimo <source-file.est>" << endl;
  }

  } catch(char *er)
  {
    cout << er << endl;
  };

  return 0;
}

