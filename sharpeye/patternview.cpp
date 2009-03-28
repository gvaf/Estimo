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
#include <math.h>
#include <fstream>
using namespace std;

#include "patternview.h"

/// Return a rounded number <int>.<frac> frac: 5, 25 75
float pround(float num)
{
 float frac = num - (int)num;
 float pfrac = fabs(frac);

 if( 0.0 <= pfrac && pfrac <= 0.125 )
  pfrac = 0.0;
 else if( 0.125 < pfrac && pfrac <= 0.375 )
  pfrac = 0.25; 
 else if( 0.375 < pfrac && pfrac <= 0.625)
  pfrac = 0.5;
 else if( 0.625 < pfrac && pfrac < 0.875)
  pfrac = 0.75;
 else if( 0.875 <= pfrac && pfrac < 1)
  pfrac = 1;

 return (int)num + pfrac;
}

/// Constructor
PatternView::PatternView(QList<PatternDialog::Point> & pat, QWidget *parent)
  : QWidget(parent), pattern(pat)
{
  //setBackgroundRole(QColor(Qt::red));
  setAutoFillBackground(true);

  setMouseTracking(true);
  setFixedSize(QSize(500,500) );
  setCursor(Qt::CrossCursor);
  selection = 0;
}

/// Constructor
void PatternView::paintEvent(QPaintEvent *)
{
  QPainter painter(this);

  int w = width() / 2;
  int h = height() / 2;

  painter.translate(width() / 2, height() / 2);

  painter.setRenderHint(QPainter::Antialiasing, true);

  QColor niceBlue(150, 150, 200);

  QLinearGradient backgradient(0, h, 0, -h);
  backgradient.setColorAt(0.2, Qt::white);
  backgradient.setColorAt(1, niceBlue);

  painter.setBrush(backgradient);
  painter.setPen(QPen(Qt::blue, 1));
  painter.drawRoundRect(-w, -h, width(), height(), 6, 6);

  int ysteps = 4;
  int xsteps = 4;
  int axisx = -32;
  int axisy =  32;

  QRect inside(-(width()-60)/2, -(height()-90)/2, width() - 58, height() -90);
  QLinearGradient ingradient(inside.x()+inside.width()/2, inside.y(), 
                                   inside.x()+inside.width()/2, inside.y() + inside.height() );
  ingradient.setColorAt(0.9, Qt::white);
  ingradient.setColorAt(0.0, QColor(26, 59, 105));
  painter.setBrush(ingradient);
  painter.setPen(QPen(QColor(179,187, 197), 0.9));

  painter.setPen(QPen(Qt::black));

  for(int ix = inside.x(); ix < inside.x() + inside.width(); ix += inside.width() / xsteps )
  {
   painter.setPen(QPen(Qt::black, 0.8)); //QColor(179,187, 197), 0.5));
   painter.drawLine(ix,  inside.y(),  ix,  inside.y() + inside.height()+8);
   painter.setPen(QPen(Qt::black));  
   painter.drawText(ix-3, inside.y() + inside.height()+ 20, QString("%1").arg(axisx) );
   axisx += 16;
  }

  for(int iy = inside.y(); iy <= inside.y() + inside.height();  iy += inside.height() / ysteps )
  {
    painter.setPen(QPen(QPen(Qt::black, 0.5)));
    painter.drawLine(inside.x()-8, iy,  inside.x() + inside.width()-2,  iy);
    painter.setPen(QPen(Qt::black));  
    painter.drawText(inside.x()-25, iy+3, QString("%1").arg(axisy) );
    axisy -= 16;
  }

  for(int i = 0; i < pattern.size(); ++i) 
     drawPoint(painter, pattern.at(i).x, pattern.at(i).y);
}

/// Draw a single Point 
void PatternView::drawPoint(QPainter & painter, double x, double y)
{
 int maxX =  220; 
 int maxY = -205;

 int intX = (int)x;
 int intY = (int)y;
 int fracX = (int)((x - intX) * 10.0);
 int fracY = (int)(y - intY) * 10;

 int x0 = (int)( x * ( maxX / 32.0) + fracX );
 int y0 = (int)( y * ( maxY / 32.0) + fracY );

 if( fracX == 0 && fracY == 0 )
   painter.setPen(QPen(Qt::red, 1));
 else
   painter.setPen(QPen(Qt::darkGreen, 1));

 if( pattern.at(selection).x == x &&
     pattern.at(selection).y == y )
   painter.setPen(QPen(Qt::yellow, 1));

  painter.drawLine( x0, y0-3, x0, y0+3);
  painter.drawLine( x0-3, y0, x0+3, y0);
}


/// Changed current point
void PatternView::changedSelection(int index)
{
  selection = index;
  update();
}

/// Default Constructor
PatternDialog::PatternDialog()
{
    QHBoxLayout *mainLayout    = new QHBoxLayout;
    QVBoxLayout *controlLayout = new QVBoxLayout;
    QVBoxLayout *topLayout     = new QVBoxLayout;
    QHBoxLayout *titleLayout   = new QHBoxLayout;
    QGridLayout *coords        = new QGridLayout;
    QVBoxLayout *vpoint        = new QVBoxLayout;
    QHBoxLayout *buttonsLayout = new QHBoxLayout;

    QPushButton *newButton    = new QPushButton("&New Point", this);
    QPushButton *deleteButton = new QPushButton("&Delete Point", this);
    QPushButton *saveButton   = new QPushButton("&Save Point", this);
    QPushButton *clipButton   = new QPushButton("&Copy Pattern to Clipboard...", this);
    QPushButton *cancelButton = new QPushButton("Cancel", this);

    connect(cancelButton, SIGNAL(released()), this, SLOT(reject()));
    connect(newButton,    SIGNAL(released()), this, SLOT(newPoint()));
    connect(deleteButton, SIGNAL(released()), this, SLOT(deletePoint()));
    connect(saveButton,   SIGNAL(released()), this, SLOT(savePoint()));
    connect(clipButton,   SIGNAL(released()), this, SLOT(copyClipboard()));

    QHBoxLayout *bLayout = new QHBoxLayout;
    bLayout->addStretch(1);
    bLayout->addWidget(clipButton);
    bLayout->addWidget(cancelButton);

    buttonsLayout->addWidget(newButton);
    buttonsLayout->addWidget(deleteButton);
    buttonsLayout->addWidget(saveButton);

    vpoint->addLayout(buttonsLayout);

    pview = new PatternView(pattern, this);

    points = new QListWidget(this);

    connect(points, SIGNAL(currentRowChanged(int)), this, SLOT(currentRowChanged(int)));
    connect(points, SIGNAL(currentRowChanged(int)), pview, SLOT(changedSelection(int)));

    QGroupBox *groupBox  = new QGroupBox(tr("Point Coordinates"));
    xbox = new QDoubleSpinBox(this);
    ybox = new QDoubleSpinBox(this);

    connect(xbox, SIGNAL(editingFinished()), this, SLOT(fix()));
    connect(ybox, SIGNAL(editingFinished()), this, SLOT(fix()));

    xbox->setRange(-32, 31.75);
    ybox->setRange(-32, 31.75);
    xbox->setSingleStep(0.25);
    ybox->setSingleStep(0.25);

    QLabel *xlabel = new QLabel("X-coord:", this);
    QLabel *ylabel = new QLabel("Y-coord:", this);

    QGroupBox *groupPoints = new QGroupBox(tr("Pattern Points"));

    vpoint->addWidget(points);
    groupBox->setLayout(coords);
    groupPoints->setLayout(vpoint);

    coords->addWidget(xlabel, 0,0);
    coords->addWidget(ylabel, 1,0);
    coords->addWidget(xbox,   0,1);
    coords->addWidget(ybox,  1,1);

    pname = new QLineEdit(this);
    QLabel    *pnameLbl = new QLabel("&Pattern Name:", this);
    pnameLbl->setBuddy(pname);

    titleLayout->addWidget(pnameLbl);
    titleLayout->addWidget(pname);

    topLayout->addLayout(titleLayout);
    topLayout->addLayout(mainLayout);
    topLayout->addLayout(bLayout);

    mainLayout->addWidget(pview);
    controlLayout->addWidget(groupPoints);
    controlLayout->addWidget(groupBox);
    mainLayout->addLayout(controlLayout);

    setLayout(topLayout);
    setWindowTitle(tr("Pattern Dialog"));
}

/// Create New point
void PatternDialog::newPoint()
{
  pattern.append(Point(0.0, 0.0));
  points->addItem("Point(0.00, 0.00)");
  points->setCurrentRow( points->count()-1 );
  points->setFocus();
  pview->update();
}

/// Save Current Point
void PatternDialog::savePoint()
{
  if( points->count() == 0 ) return;
  int cr = points->currentRow();

  pattern[cr] = Point(xbox->value(), ybox->value());
  pview->update();
}

/// Delete Point from the list
void PatternDialog::deletePoint()
{
  if( points->count() == 0 ) return;
  int cr = points->currentRow();
  points->takeItem(cr);
  pattern.removeAt(cr);
  pview->update();
}

/// Copy Pattern to clipboard
void PatternDialog::copyClipboard()
{
  QClipboard *clipboard = QApplication::clipboard();
  QString clip;
  QString pn = "??";

  if(pname->text() != "" )
   pn = pname->text();

  clip = QString("Pattern(%1)").arg(pn);
  clip += "\n{\n";

  for(int i = 0; i < pattern.size(); ++i) 
   {
    clip += QString("\tcheck(%1, %2)\n").arg(pattern.at(i).x).arg(pattern.at(i).y);
   }

  clip += "}\n";

  clipboard->setText(clip);
  accept();
}

/// Fix the values in order to be valid 
void PatternDialog::fix()
{ 
  xbox->setValue( pround( xbox->value()));
  ybox->setValue( pround( ybox->value()));

  if( points->count() == 0 ) return;
  QListWidgetItem *item = points->item(points->currentRow());
  item->setText( QString("Point (%1, %2)").arg(xbox->value()).arg(ybox->value()));
  pview->update();
}

/// Catch the current row changed
void PatternDialog::currentRowChanged(int currentRow)
{
  if( points->count() == 0 ) return; 
  Point crpoint = pattern.value(currentRow);
  xbox->setValue(crpoint.x);
  ybox->setValue(crpoint.y);
  pview->update();
}


/// Constructor
OutputDialog::OutputDialog(QString outpath, QString result, QWidget *parent)
  : outputPath(outpath), QDialog(parent)
{
	resize(780, 580);

    QVBoxLayout *topLayout = new QVBoxLayout(this);
   
	QTabWidget * tabs = new QTabWidget(this);
	QTextEdit * outText = new QTextEdit(this);
	QTextEdit * asmText = new QTextEdit(this);
	QTextEdit * pointsText = new QTextEdit(this);
	QTextEdit * programText = new QTextEdit(this);
	QTextEdit * confText = new QTextEdit(this);
 
    outText->setFrameStyle(QFrame::NoFrame); 
	asmText->setFrameStyle(QFrame::NoFrame); 
	pointsText->setFrameStyle(QFrame::NoFrame); 
    programText->setFrameStyle(QFrame::NoFrame); 
    confText->setFrameStyle(QFrame::NoFrame); 

	outText->setReadOnly(true);
	asmText->setReadOnly(true);
	pointsText->setReadOnly(true);
    programText->setReadOnly(true);
    confText->setReadOnly(true);

	topLayout->addWidget(tabs);

	tabs->addTab(outText,     "Output");
	tabs->addTab(asmText,     "Assembly");
	tabs->addTab(pointsText,  "Patterns Memory");
	tabs->addTab(programText, "Program");
	tabs->addTab(confText,    "Configuration");

	loadFile(outputPath + "/assembly.asm", asmText);
	loadInfo("-mem", outputPath + "/patterns.bin", pointsText);
	loadInfo("-pro", outputPath + "/program.bin", programText);

	outText->setPlainText(result);
	
	 QString appdir = QApplication::applicationDirPath(); 	
    loadFile(appdir + "/configuration.ini", confText);

	setLayout(topLayout);
    setWindowTitle(tr("Estimo C Output"));
}

void OutputDialog::loadInfo(QString arg1, QString filename, QTextEdit *viewer)
{
     QProcess sharpInfo;
	 QStringList args;
	 args << arg1 << filename;
	 
	 QString appdir = QApplication::applicationDirPath(); 

	 sharpInfo.start(appdir + "/sharpinfo", args);
	 sharpInfo.waitForFinished(2000);
     QString res(sharpInfo.readAll());
	 viewer->setPlainText(res);    
}

bool OutputDialog::loadFile(QString fileName, QTextEdit *viewer)
 {
  QFile file(fileName);
     if (!file.open(QFile::ReadOnly | QFile::Text)) {
         QMessageBox::warning(this, tr("SharpEye Studio"),
                              tr("Cannot read file %1:\n%2.")
                              .arg(fileName)
                              .arg(file.errorString()));
         return false;
     }

     QTextStream in(&file);
     QApplication::setOverrideCursor(Qt::WaitCursor);
     viewer->setPlainText(in.readAll());
     QApplication::restoreOverrideCursor();

	 return true;
 }