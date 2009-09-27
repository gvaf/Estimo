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

#ifndef CYCLETIMERPOINT_H
#define CYCLETIMERPOINT_H

#include <QDialog>
#include <QString>
#include <QTime>

class QLabel;
class QProcess;
class QWidget;

namespace CT {

enum Variable {
	VarNone = -1,

	VarLabel,
	VarProgMem,
	VarPointMem,
	VarFull,
	VarQuarter,
	VarPartitions,
	VarMVCost,
	VarMVCand,
	VarLUT,
	VarVideoFile,
	VarResX,
	VarResY,
	VarFrames,
	VarQP,
	VarRefFrames,

	VarRes,

	VarBitRate,
	VarPSNR,
	VarEnergy,
	VarCycles,
	VarCyclesParallel,

	VarEnergyMB,
	VarCyclesMB,
	VarCyclesMBParallel,
	VarFPS,
	VarFPSParallel
};

QString varLabel(Variable var, bool units = false);
qreal varMinStep(Variable var);

struct PointConf
{
	QString label;
	QString progMem;
	QString pointMem;
	int full;
	int quarter;
	int partitions;
	bool mvCost;
	bool mvCand;
	int lut;
	QString videoFile;
	int resX;
	int resY;
	int frames;
	int qp;
	int refFrames;

	int intVal(Variable var) const;
	qreal realVal(Variable var) const;
	QString strVal(Variable var) const;
};

struct PointData : public PointConf
{
	qreal bitRate;
	qreal psnr;
	qreal energy;
	qint64 cycles;
	qint64 cyclesParallel;

	int intVal(Variable var) const;
	qreal realVal(Variable var) const;
	QString strVal(Variable var) const;
};

class PointItem : public QObject
{
	Q_OBJECT

public:
	PointItem(const PointConf& conf);
	void start();
	void stop();
	const PointData& data() const;
	bool hasData() const;
	bool plottable() const;

	void remove();
	void rename(const QString& newName);
	void details(QWidget* parent);
	void exportConf(QWidget* parent);

signals:
	void changed(PointItem*);
	void finished(PointItem*);
	void removed(PointItem*);

private:
	PointData p;
	QTime lastUpdate;
	QProcess* x264;
	QString x264String;
	bool x264NoOutput;

	void x264Run();
	void processLine(const QString& line);

private slots:
	void x264Output();
	void x264Ready();

private:
	PointItem(const PointItem&);
	PointItem& operator=(const PointItem&);
};

class DetailsDialog : public QDialog
{
	Q_OBJECT

public:
	DetailsDialog(QWidget* parent, PointItem* point);

private:
	QLabel* bLabel;
	QLabel* bProgMem;
	QLabel* bPointMem;
	QLabel* bFull;
	QLabel* bQuarter;
	QLabel* bPartitions;
	QLabel* bMVCost;
	QLabel* bMVCand;
	QLabel* bLUT;
	QLabel* bVideoFile;
	QLabel* bRes;
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

private slots:
	void changed(PointItem* point);
};

} // namespace CT

#endif // !CYCLETIMERPOINT_H
