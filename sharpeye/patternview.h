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

#ifndef PATTERN_VIEW_H
#define PATTERN_VIEW_H

#include <QtGui>

class PatternView;

/// Pattern Dialog
class PatternDialog : public QDialog
{
  Q_OBJECT
 public:

  PatternDialog();
 ~PatternDialog() {}

  struct Point
  { 
    Point(double x_=0, double y_=0) : x(x_), y(y_) {}
   double x; double y; 
  };

 public slots:
  void newPoint();
  void savePoint();
  void deletePoint();
  void copyClipboard();
  void fix();
  void currentRowChanged(int currentRow);

 private:
   QDoubleSpinBox *xbox;
   QDoubleSpinBox *ybox; 
   QList<Point> pattern;
   QListWidget *points;
   PatternView *pview;
   QLineEdit *pname;
};

/// Grid For pattern specification
class PatternView : public QWidget
{
  Q_OBJECT
  public:
  
   /// Constructor
   PatternView(QList<PatternDialog::Point> & pat, QWidget *parent = 0);

  public slots:
    void changedSelection(int);

  protected:
     void paintEvent(QPaintEvent *); 
     void drawPoint(QPainter & painter, double x, double y);

  private:
     QList<PatternDialog::Point> &pattern;
     int selection;
};


/// Compiler Output Result
class OutputDialog : public QDialog
{
  Q_OBJECT
 public:

  OutputDialog(QString outpath,  QString result, QWidget * parent);
 ~OutputDialog() {}

 private:
     QString outputPath;

	 bool loadFile(QString filename, QTextEdit *view);
     void loadInfo(QString arg1, QString filename, QTextEdit *viewer);
};


#endif
