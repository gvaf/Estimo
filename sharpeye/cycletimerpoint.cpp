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

#include <QString>
#include <QTime>
#include <QtGui>
#include <cassert>
#include "cycletimerpoint.h"


//////////////////////
// helper functions //
//////////////////////

namespace {

QString
enabledDisabled(bool b)
{
	return b ? QObject::tr("enabled") : QObject::tr("disabled");
}

void
addLabelDesc(QGridLayout* layout, int row, int col, const QString& text)
{
	QLabel* label = new QLabel(text);
	QFont font;
	font.setBold(true);
	label->setFont(font);
	layout->addWidget(label, row, col, 1, 2);
}

QLabel*
addLabelPair(QGridLayout* layout, int row, int col,
	     const QString& text1, const QString& text2)
{
	QLabel* label = new QLabel(text1);
	QFontMetrics fm = label->fontMetrics();
	int xwidth = fm.width(QLatin1Char('x'));
	label->setIndent(xwidth * 4);
	QLabel* box = new QLabel(text2);
	label->setBuddy(box);
	box->setFrameStyle(QFrame::Sunken | QFrame::Panel);
	layout->addWidget(label, row, col);
	layout->addWidget(box, row, col + 1);
	return box;
}

} // namespace


///////////////
// functions //
///////////////

QString
CT::varLabel(CT::Variable var, bool units)
{
	switch (var) {
	case VarLabel:
		return QObject::tr("Configuration");
	case VarProgMem:
		return QObject::tr("Program memory");
	case VarPointMem:
		return QObject::tr("Point memory");
	case VarFull:
		return QObject::tr("Full-pel execution units");
	case VarQuarter:
		return QObject::tr("Sub-pel execution units");
	case VarPartitions:
		return QObject::tr("Smallest partition size");
	case VarMVCost:
		return QObject::tr("MV cost optimization");
	case VarMVCand:
		return QObject::tr("MV candidates");
	case VarLUT:
		return QObject::tr("Logic cells");
	case VarVideoFile:
		return QObject::tr("Video file");
	case VarResX:
		return QObject::tr("X resolution");
	case VarResY:
		return QObject::tr("Y resolution");
	case VarFrames:
		return QObject::tr("Number of frames");
	case VarQP:
		return QObject::tr("QP");
	case VarRefFrames:
		return QObject::tr("Reference frames");
	case VarRes:
		return QObject::tr("Resolution");
	case VarBitRate:
		if (units)
			return QObject::tr("Bit rate (kbit/s)");
		return QObject::tr("Bit rate");
	case VarPSNR:
		if (units)
			return QObject::tr("PSNR (dB)");
		return QObject::tr("PSNR");
	case VarEnergy:
		if (units)
			return QObject::tr("Energy (nJ)");
		return QObject::tr("Energy");
	case VarCycles:
		return QObject::tr("Cycles");
	case VarCyclesParallel:
		return QObject::tr("Cycles (parallel)");
	case VarEnergyMB:
		if (units)
			return QObject::tr("Energy / macroblock (nJ)");
		return QObject::tr("Energy / macroblock");
	case VarCyclesMB:
		return QObject::tr("Cycles / macroblock");
	case VarCyclesMBParallel:
		return QObject::tr("Cycles / macroblock (parallel)");
	case VarFPS:
		return QObject::tr("Frames / second");
	case VarFPSParallel:
		return QObject::tr("Frames / second (parallel)");
	default:
		if (units)
			return QObject::tr("-");
		return QObject::tr("None");
	};
}

qreal
CT::varMinStep(CT::Variable var)
{
	switch (var) {
	case VarFull:
	case VarQuarter:
	case VarPartitions:
	case VarMVCost:
	case VarMVCand:
	case VarResX:
	case VarResY:
	case VarFrames:
	case VarQP:
	case VarRefFrames:
	case VarCycles:
	case VarCyclesParallel:
	case VarCyclesMB:
	case VarCyclesMBParallel:
		return 1;

	case VarLUT:
		return 100;

	case VarBitRate:
	case VarPSNR:
	case VarEnergy:
	case VarEnergyMB:
	case VarFPS:
	case VarFPSParallel:
		return 0.01;

	default:
		return 0;
	};
}

//////////////////////
// struct PointConf //
//////////////////////

int CT::PointConf::intVal(Variable var) const
{
	switch (var) {
	case VarFull:
		return full;
	case VarQuarter:
		return quarter;
	case VarPartitions:
		return partitions;
	case VarMVCost:
		return int(mvCost);
	case VarMVCand:
		return int(mvCand);
	case VarLUT:
		return lut;
	case VarResX:
		return resX;
	case VarResY:
		return resY;
	case VarFrames:
		return frames;
	case VarQP:
		return qp;
	case VarRefFrames:
		return refFrames;
	default:
		return 0;
	};
}

qreal CT::PointConf::realVal(Variable var) const
{
	switch (var) {
	case VarFull:
	case VarQuarter:
	case VarPartitions:
	case VarMVCost:
	case VarMVCand:
	case VarLUT:
	case VarResX:
	case VarResY:
	case VarFrames:
	case VarQP:
	case VarRefFrames:
		return intVal(var);

	default:
		return 0;
	};
}

QString CT::PointConf::strVal(Variable var) const
{
	switch (var) {
	case VarFull:
	case VarQuarter:
	case VarLUT:
	case VarResX:
	case VarResY:
	case VarFrames:
	case VarQP:
	case VarRefFrames:
		return QString::number(intVal(var));
	case VarLabel:
		return label;
	case VarProgMem:
		return progMem;
	case VarPointMem:
		return pointMem;
	case VarVideoFile:
		return videoFile;
	case VarPartitions:
		{
			QString s = QString::number(partitions);
			return s + QObject::tr("x") + s;
		}
	case VarMVCost:
		return enabledDisabled(mvCost);
	case VarMVCand:
		return enabledDisabled(mvCand);
	case VarRes:
		return strVal(VarResX) + QObject::tr("x") + strVal(VarResY);
	default:
		return QString();
	};
}


//////////////////////
// struct PointData //
//////////////////////

int CT::PointData::intVal(Variable var) const
{
	switch (var) {
	case VarFull:
	case VarQuarter:
	case VarPartitions:
	case VarMVCost:
	case VarMVCand:
	case VarLUT:
	case VarResX:
	case VarResY:
	case VarFrames:
	case VarQP:
	case VarRefFrames:
		return PointConf::intVal(var);
	default:
		return 0;
	};
}

qreal CT::PointData::realVal(Variable var) const
{
	int mb_per_frame;
	switch (var) {
	case VarFull:
	case VarQuarter:
	case VarPartitions:
	case VarMVCost:
	case VarMVCand:
	case VarLUT:
	case VarResX:
	case VarResY:
	case VarFrames:
	case VarQP:
	case VarRefFrames:
		return PointConf::realVal(var);
	case VarBitRate:
		return bitRate;
	case VarPSNR:
		return psnr;
	case VarEnergy:
		return energy;
	case VarCycles:
		return cycles;
	case VarCyclesParallel:
		return cyclesParallel;
	case VarEnergyMB:
		mb_per_frame = ((resX + 15) / 16) * ((resY + 15) / 16);
		return energy / mb_per_frame / frames;
	case VarCyclesMB:
		mb_per_frame = ((resX + 15) / 16) * ((resY + 15) / 16);
		return qreal(cycles) / mb_per_frame / frames;
	case VarCyclesMBParallel:
		mb_per_frame = ((resX + 15) / 16) * ((resY + 15) / 16);
		return qreal(cyclesParallel) / mb_per_frame / frames;
	case VarFPS:
		return cycles <= 0 ? 0 : 2e8 * frames / cycles; // 200 MHz
	case VarFPSParallel:
		return cyclesParallel <= 0 ? 0 : 2e8 * frames / cyclesParallel;
	default:
		return 0;
	};
}

QString CT::PointData::strVal(Variable var) const
{
	switch (var) {
	case VarLabel:
	case VarProgMem:
	case VarPointMem:
	case VarVideoFile:
	case VarFull:
	case VarQuarter:
	case VarPartitions:
	case VarMVCost:
	case VarMVCand:
	case VarLUT:
	case VarResX:
	case VarResY:
	case VarFrames:
	case VarQP:
	case VarRefFrames:
	case VarRes:
		return PointConf::strVal(var);
	case VarBitRate:
	case VarPSNR:
	case VarEnergy:
	case VarEnergyMB:
	case VarCyclesMB:
	case VarCyclesMBParallel:
	case VarFPS:
	case VarFPSParallel:
		return QString::number(realVal(var));
	case VarCycles:
		return QString::number(cycles);
	case VarCyclesParallel:
		return QString::number(cyclesParallel);
	default:
		return QString();
	};
}


//////////////////////
// struct PointItem //
//////////////////////

CT::PointItem::PointItem(const PointConf& conf)
	: x264(0),
	  x264NoOutput(true)
{
	p.label = conf.label;
	p.progMem = conf.progMem;
	p.pointMem = conf.pointMem;
	p.videoFile = conf.videoFile;
	p.full = conf.full;
	p.quarter = conf.quarter;
	p.partitions = conf.partitions;
	p.mvCost = conf.mvCost;
	p.mvCand = conf.mvCand;
	p.lut = conf.lut;
	p.resX = conf.resX;
	p.resY = conf.resY;
	p.frames = conf.frames;
	p.qp = conf.qp;
	p.refFrames = conf.refFrames;

	p.bitRate = 0;
	p.psnr = 0;
	p.energy = 0;
	p.cycles = 0;
	p.cyclesParallel = 0;
}

void CT::PointItem::start()
{
	assert(x264 == 0);
	x264 = new QProcess(this);
	connect(x264, SIGNAL(readyReadStandardOutput()),
		this, SLOT(x264Output()));
	connect(x264, SIGNAL(finished(int, QProcess::ExitStatus)),
		this, SLOT(x264Ready()));
	x264Run();
}

void CT::PointItem::stop()
{
	if (x264 == 0)
		return;
	x264->kill();
}

const CT::PointData& CT::PointItem::data() const
{
	return p;
}

bool CT::PointItem::hasData() const
{
	return !x264NoOutput;
}

bool CT::PointItem::plottable() const
{
	return !x264NoOutput && x264 == 0;
}

void CT::PointItem::remove()
{
	emit removed(this);
	deleteLater();
}

void CT::PointItem::rename(const QString& text)
{
	p.label = text;
	emit changed(this);
}

void CT::PointItem::details(QWidget* parent)
{
	DetailsDialog* d = new DetailsDialog(parent, this);
	d->show();
}

void CT::PointItem::exportConf(QWidget *parent)
{
	static QString defPath;

	QString fileName = QFileDialog::getSaveFileName
		(parent, tr("Save Configuration"), defPath,
		 tr("VHDL files (*.vhd);;All files (*)"));
	if (fileName.isEmpty())
		return;
	defPath = QFileInfo(fileName).path();

	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		QMessageBox::warning(parent, tr("Export Failed"),
				     tr("Could not open file."));
		return;
	}
	const PointData& pt = data();
	QTextStream s(&file);
	s << "----------------------------------------------------------------------------\n"
	  << "--  This file is a part of the ME engine VHDL model\n"
	  << "--  Copyright (C) 2003  Jose Luis Nunez\n"
	  << "--\n"
	  << "\n"
	  << "library IEEE;\n"
	  << "use IEEE.std_logic_1164.all;\n"
	  << "use IEEE.std_logic_arith.all;\n"
	  << "\n"
	  << "\n"
	  << "package config is\n"
	  << "\n"
	  << "constant CFG_PIPELINE_COUNT : integer := (" << pt.full << ");\n"
	  << "constant CFG_PIPELINE_COUNT_QP : integer := (" << pt.quarter << ");\n"
	  << "constant CFG_CM : integer := (0); --compact memory configuration switch\n"
	  << "constant CFG_MV_COST : integer := (" << pt.mvCost <<  "); -- set to one to enable adding the cost of the motion vector to the SAD\n"
	  << "constant CFG_USE_MVC : integer := (" << pt.mvCand <<"); -- set to use motion vector candidates\n"
	  << "constant CFG_USE_REF : integer := (" << pt.refFrames << "); -- define how many reference frames to use\n"

	  << "\n"
	  << "type rest_type_points is array(CFG_PIPELINE_COUNT-1 downto 0) of std_logic_vector(7 downto 0); \n"
	  << "type rest_type_displacement is array(CFG_PIPELINE_COUNT-1 downto 0) of std_logic_vector(15 downto 0);\n"
	  << "type rest_type_points_qp is array(CFG_PIPELINE_COUNT_QP-1 downto 0) of std_logic_vector(7 downto 0); \n"
	  << "type rest_type_displacement_qp is array(CFG_PIPELINE_COUNT_QP-1 downto 0) of std_logic_vector(15 downto 0);  \n"
	  << "type mode_type is (m16x16,m8x8,m16x8,m8x16,m8x4,m4x8,m4x4);\n"
	  << "--type mode_type is (m16x16);\n"
	  << "\n"
	  << "\n"
	  << "end;\n";
}

void CT::PointItem::x264Run()
{
	QString program = QApplication::applicationDirPath() + "/x264";
	QStringList args;
	args << "--caside";
	switch (p.partitions) {
	case 4:
		args << "--partitions" << "p8x8,p4x4";
		break;
	case 8:
		args << "--partitions" << "p8x8";
		break;
	default:
		args << "--partitions" << "none";
	}
	args << "--me" << "cas"
	     << "--subme" << "1"
	     << "--casfeu" << QString::number(p.full)
	     << "--casqeu" << QString::number(p.quarter);
	if (!p.mvCost)
		args << "--no-casmvcost";
	if (!p.mvCand)
		args << "--no-casmvcand";
	args << "--casprog" << p.progMem
	     << "--caspoint" << p.pointMem
	     << "--qp" << QString::number(p.qp)
	     << "--ref" << QString::number(p.refFrames)
	     << "--output" << "/dev/null";
	if (p.frames)
		args << "--frames" << QString::number(p.frames);
	QString resText = (QString::number(p.resX)
			   + "x" + QString::number(p.resY));
	args << p.videoFile << resText;
	x264NoOutput = true;
	x264String.clear();
	p.frames = 0;
	lastUpdate.start();
	emit changed(this);

	x264->start(program, args);
}

void CT::PointItem::processLine(const QString& line)
{
	QStringList l = line.split('\t');
	if (l.size() < 9)
		return;
	bool ok;
	qint64 frame, mb;
	double bitRate, psnr;
	qint64 cycles, fcycles, qcycles, couldSaveCycles, energypJ;
	if ((frame = l[0].toLongLong(&ok)), !ok)
		return;
	if ((mb = l[1].toLongLong(&ok)), !ok)
		return;
	if ((bitRate = l[2].toDouble(&ok)), !ok)
		return;
	if ((psnr = l[3].toDouble(&ok)), !ok)
		return;
	if ((cycles = l[4].toLongLong(&ok)), !ok)
		return;
	if ((fcycles = l[5].toLongLong(&ok)), !ok)
		return;
	if ((qcycles = l[6].toLongLong(&ok)), !ok)
		return;
	if ((couldSaveCycles = l[7].toLongLong(&ok)), !ok)
		return;
	if ((energypJ = l[8].toLongLong(&ok)), !ok)
		return;

	x264NoOutput = false;
	p.frames = frame;
	p.bitRate = bitRate;
	p.psnr = psnr;
	p.cycles = cycles;
	p.cyclesParallel = cycles - couldSaveCycles;
	p.energy = energypJ / 1000.0;

	if (lastUpdate.elapsed() > 500) {
		lastUpdate.restart();
		emit changed(this);
	}
}

void CT::PointItem::x264Output()
{
	int n;
	x264String.append(x264->readAllStandardOutput());
	while ((n = x264String.indexOf('\n')) >= 0) {
		QString line = x264String.left(n);
		x264String.remove(0, n + 1);
		processLine(line);
	}
}

void CT::PointItem::x264Ready()
{
	x264Output();
	x264->deleteLater();
	x264 = 0;
	if (!plottable()) {
		remove();
	} else {
		emit changed(this);
		emit finished(this);
	}
}


/////////////////////////
// class DetailsDialog //
/////////////////////////

CT::DetailsDialog::DetailsDialog(QWidget* parent, PointItem* point)
	: QDialog(parent)
{
	const PointData& pt = point->data();
	QGridLayout* layout = new QGridLayout;
	layout->setColumnMinimumWidth(2, 12);
	int y = 0, x = 0;
	addLabelDesc(layout, y++, x, tr("Processor configuration:"));
	bLabel = addLabelPair(layout, y++, x, tr("Label:"),
			      pt.strVal(VarLabel));
	bProgMem = addLabelPair(layout, y++, x, tr("Program memory:"),
				pt.strVal(VarProgMem));
	bPointMem = addLabelPair(layout, y++, x, tr("Point memory:"),
				 pt.strVal(VarPointMem));
	bFull = addLabelPair(layout, y++, x, tr("Full-pel units:"),
			     pt.strVal(VarFull));
	bQuarter = addLabelPair(layout, y++, x, tr("Sub-pel units:"),
				pt.strVal(VarQuarter));
	bPartitions = addLabelPair(layout, y++, x, tr("Smallest partition:"),
				   pt.strVal(VarPartitions));
	bMVCost = addLabelPair(layout, y++, x, tr("MV cost optimization:"),
			       pt.strVal(VarMVCost));
	bMVCand = addLabelPair(layout, y++, x, tr("MV candidates:"),
			       pt.strVal(VarMVCand));
	bLUT = addLabelPair(layout, y++, x, tr("Logic cells:"),
			    pt.strVal(VarLUT));
	addLabelDesc(layout, y++, x, tr("Video data:"));
	bVideoFile = addLabelPair(layout, y++, x, tr("Video file:"),
				  pt.strVal(VarVideoFile));
	bRes = addLabelPair(layout, y++, x, tr("Resolution:"),
			    pt.strVal(VarRes));
	bFrames = addLabelPair(layout, y++, x, tr("Frames processed:"),
			       pt.strVal(VarFrames));
	bQP = addLabelPair(layout, y++, x, tr("QP (0 is lossless):"),
			   pt.strVal(VarQP));
	bRefFrames = addLabelPair(layout, y++, x, tr("Reference frames:"),
				  pt.strVal(VarRefFrames));
	y = 0;
	x = 3;
	addLabelDesc(layout, y++, x, tr("Results:"));
	bBitRate = addLabelPair(layout, y++, x, tr("Bit rate (kbit/s):"),
				pt.strVal(VarBitRate));
	bPSNR = addLabelPair(layout, y++, x, tr("PSNR (dB):"),
			     pt.strVal(VarPSNR));
	bEnergyMB = addLabelPair(layout, y++, x, tr("Energy / macroblock (nJ):"),
				 pt.strVal(VarEnergyMB));
	bFPS = addLabelPair(layout, y++, x, tr("FPS:"),
			    pt.strVal(VarFPS));
	bCyclesMB = addLabelPair(layout, y++, x, tr("Cycles / macroblock:"),
				 pt.strVal(VarCyclesMB));
	addLabelDesc(layout, y++, x, tr("Full- and sub-pel in parallel:"));
	bFPSParallel = addLabelPair(layout, y++, x, tr("FPS:"),
				    pt.strVal(VarFPSParallel));
	bCyclesMBParallel = addLabelPair(layout, y++, x, tr("Cycles / macroblock:"),
					 pt.strVal(VarCyclesMBParallel));

	connect(point, SIGNAL(removed(PointItem*)), this, SLOT(close()));
	connect(point, SIGNAL(changed(PointItem*)),
		this, SLOT(changed(PointItem*)));
	setAttribute(Qt::WA_DeleteOnClose, true);
	setLayout(layout);
	setWindowTitle(pt.strVal(VarLabel));
}

void CT::DetailsDialog::changed(CT::PointItem* point)
{
	const PointData& pt = point->data();
	bLabel->setText(pt.strVal(VarLabel));
	bProgMem->setText(pt.strVal(VarProgMem));
	bPointMem->setText(pt.strVal(VarPointMem));
	bFull->setText(pt.strVal(VarFull));
	bQuarter->setText(pt.strVal(VarQuarter));
	bPartitions->setText(pt.strVal(VarPartitions));
	bMVCost->setText(pt.strVal(VarMVCost));
	bMVCand->setText(pt.strVal(VarMVCand));
	bLUT->setText(pt.strVal(VarLUT));
	bVideoFile->setText(pt.strVal(VarVideoFile));
	bRes->setText(pt.strVal(VarRes));
	bFrames->setText(pt.strVal(VarFrames));
	bQP->setText(pt.strVal(VarQP));
	bRefFrames->setText(pt.strVal(VarRefFrames));
	bBitRate->setText(pt.strVal(VarBitRate));
	bPSNR->setText(pt.strVal(VarPSNR));
	bEnergyMB->setText(pt.strVal(VarEnergyMB));
	bFPS->setText(pt.strVal(VarFPS));
	bCyclesMB->setText(pt.strVal(VarCyclesMB));
	bFPSParallel->setText(pt.strVal(VarFPSParallel));
	bCyclesMBParallel->setText(pt.strVal(VarCyclesMBParallel));
	setWindowTitle(pt.strVal(VarLabel));
}
