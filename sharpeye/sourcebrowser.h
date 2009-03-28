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

#ifndef SOURCE_BROWSER_H
#define SOURCE_BROWSER_H

#include <QtGui>
#include "sourceview.h"

class SourceBrowser : public QTabWidget
{
   Q_OBJECT
  public:
     SourceBrowser();
    ~SourceBrowser();
     QString getCurrentFile();

 public slots:
     void open(const QString & filepath);
     void save();
     void saveAs();
     void saveAll();
     void closeTab();
     void closeAll();
     void filePrintPreview();
     void filePrintPdf();
     void undo();
     void redo();
     void cut();
     void copy();
     void paste();

  private slots:
     void unsavedICO(QWidget *);
     void savedICO(QWidget *);
    
  signals:
     void modificationChanged(bool);
     void undoAvailable(bool);
     void redoAvailable(bool);
     void copyAvailable(bool);
     void fileOpen(QString fileName);
     void fileClose(QString fileName);

  private:
     QString icoType(const QString & fileName);

  private:
    QHash<QString, SourceView *> pages;
};

#endif
