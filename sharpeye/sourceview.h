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

#ifndef SOURCEVIEW_H
#define SOURCEVIEW_H

#include <QWidget>
#include <QTextEdit>

class EstimoHighLight;

class SourceView : public QWidget
{
	Q_OBJECT
public:
     SourceView();
    ~SourceView();

     bool loadFile(const QString &fileName);
     bool save(QString fname = "");
     bool isModified() const;
     QString filename() const;
     QTextDocument * document();
     QTextEdit * getEditor();

private slots:
     void whenModified(bool);

signals:
     void modified(QWidget *);
     void saved(QWidget *);

private:
   QTextEdit * editor;
   QString fileName;
   EstimoHighLight *highlighter;

};

#endif // SOURCEVIEW_H
