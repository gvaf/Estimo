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
#include "projecttree.h"

/// Constructor
ProjectTree::ProjectTree()
{
  setColumnCount(1);

  QTreeWidgetItem *root = new QTreeWidgetItem((QTreeWidget*)0, 
                                     QStringList(QString("Project files")));
  root->setIcon(0, QIcon(":/SharpEye/images/folder.png"));

  estimosource = new QTreeWidgetItem(root, QStringList(QString("Estimo Source Files")));
  estimosource->setIcon(0, QIcon(":/SharpEye/images/folder.png"));

  QTreeWidgetItem *output = new QTreeWidgetItem(root, QStringList(QString("Output Files")));
  output->setIcon(0, QIcon(":/SharpEye/images/folder.png"));

  QTreeWidgetItem *asmdoc = new QTreeWidgetItem(output, QStringList(QString("Assembly.asm")));
  asmdoc->setIcon(0, QIcon(":/SharpEye/images/document.png"));

  QTreeWidgetItem *patdoc = new QTreeWidgetItem(output, QStringList(QString("patterns.mif")));
  patdoc->setIcon(0, QIcon(":/SharpEye/images/document.png"));

  QTreeWidgetItem *prgdoc = new QTreeWidgetItem(output, QStringList(QString("program.mif")));
  prgdoc->setIcon(0, QIcon(":/SharpEye/images/document.png"));

  QList<QTreeWidgetItem *> fileitems;

  fileitems.append(root);
  insertTopLevelItems(0, fileitems); 
  setHeaderLabels( QStringList() << "Solution Files");
  root->setExpanded(true);
  output->setExpanded(true);
  estimosource->setExpanded(true);
}

/// Destructor
ProjectTree::~ProjectTree()
{
}

/// Open File
void ProjectTree::openFile(QString fileName)
{
  QTreeWidgetItem *srcdoc = new QTreeWidgetItem(estimosource, QStringList(fileName));
  srcdoc->setIcon(0, QIcon(":/SharpEye/images/document.png"));

  allsources[fileName] = srcdoc;
}

/// Close file
void ProjectTree::closeFile(QString fileName)
{
  int index = estimosource->indexOfChild( allsources[fileName] );
  allsources.remove(fileName);
  estimosource->takeChild(index);
}
