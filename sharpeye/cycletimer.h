/**************************************************************************
 *  SharpEye Studio - Integrated Development Environment
 **************************************************************************
 *  University of Bristol
 *  Department of Electrical and Electronic Engineering
 *
 *  Copyright (C) 2009 Trevor Spiteri  <trevor.spiteri@bristol.ac.uk>
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

#ifndef CYCLETIMER_H
#define CYCLETIMER_H

#include <QList>
#include <QMainWindow>
#include <QQueue>

#include "cycletimerpoint.h"

class QCheckBox;
class QComboBox;
class QLabel;
class QPushButton;
class QSpinBox;

namespace CT {

class PointItem;
class Table;

class CycleTimer : public QMainWindow
{
	Q_OBJECT

public:
	CycleTimer(QWidget* parent = 0);

signals:
	void addPoint(PointItem*);

private:
	QList<PointItem*> runPoints;
	QList<PointItem*> points;
	int confNext;

	int maxConcurrent;
	int pointId;
	int yuvX, yuvY;

	PointItem* displayedItem;

	Table* tableWindow;

	QComboBox* cProgMem;
	QPushButton* browseProgMem;
	QComboBox* cPointMem;
	QPushButton* browsePointMem;
	QSpinBox* cFull;
	QSpinBox* cQuarter;
	QComboBox* cPartitions;
	QCheckBox* cMVCost;
	QCheckBox* cMVCand;

	QComboBox* cVideoFile;
	QPushButton* browseVideoFile;
	QSpinBox* cResX;
	QSpinBox* cResY;
	QSpinBox* cFrames;
	QSpinBox* cQP;
	QSpinBox* cRefFrames;

	QLabel* bLabel;
	QLabel* bFull;
	QLabel* bQuarter;
	QLabel* bPartitions;
	QLabel* bMVCost;
	QLabel* bMVCand;
	QLabel* bLUT;
	QLabel* bFrames;
	QLabel* bQP;
	QLabel* bRefFrames;
	QLabel* bBitRate;
	QLabel* bPSNR;
	QLabel* bEnergyMB;
	QLabel* bFPS;
	QLabel* bCyclesMB;
	QLabel* bFPSParallel;
	QLabel* bCyclesMBParallel;

	QLabel* bQueue;

	QPushButton* runButton;
	QPushButton* stopButton;
	QPushButton* plotButton;
	QPushButton* tableButton;
	QPushButton* clearButton;

	void updateResults(PointItem* po);
	PointItem* getConf();
	void checkQueue();

private slots:
	void browseProgram();
	void browsePoints();
	void browseVideo();
	void run();
	void stop();
	void plot();
	void table();
	void pointChanged(PointItem* src);
	void pointFinished(PointItem* src);
	void pointRemoved(PointItem* src);
	void videoBoxIndexChanged(int i);
};

} // namespace CT

#endif // !CYCLETIMER_H
