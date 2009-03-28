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

#ifndef PROJECTTREE_H
#define PROJECTTREE_H

#include <QWidget>
#include <QTreeWidget>
#include <QHash>

class ProjectTree : public QTreeWidget
{
  Q_OBJECT
  public: 
     ProjectTree();
    ~ProjectTree();

  public slots:
     void openFile(QString fileName);
     void closeFile(QString fileName);

  private:
     QTreeWidgetItem *estimosource;
     QHash<QString, QTreeWidgetItem *> allsources;
};

#endif // PROJECTTREE_H
