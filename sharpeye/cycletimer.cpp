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

#include <cstring>

#include <QtGui>

#include "cycletimer.h"
#include "cycletimergraph.h"
#include "cycletimerpoint.h"
#include "cycletimertable.h"

#if defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
#   include <windows.h>
#elif defined(Q_OS_UNIX) && !defined(Q_OS_MACX)
#   include <unistd.h>
#elif defined(Q_OS_MACX)
#   include <mach/mach.h>
#   include <mach/machine.h>
#endif

//////////////////////
// helper functions //
//////////////////////

namespace {

int
getCpuCount()
{
	int cpuCount = 1;

#if defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	cpuCount = si.dwNumberOfProcessors;
#elif defined(Q_OS_UNIX) && !defined(Q_OS_MACX)
	cpuCount = sysconf(_SC_NPROCESSORS_ONLN);
#elif defined(Q_OS_MACX)
	kern_return_t kr;
	struct host_basic_info hostinfo;
	unsigned int count = HOST_BASIC_INFO_COUNT;
	kr = host_info(mach_host_self(), HOST_BASIC_INFO,
		       (host_info_t)&hostinfo, &count);
	if (kr == KERN_SUCCESS)
		cpuCount = hostinfo.avail_cpus;
#endif

	return cpuCount < 1 ? 1 : cpuCount;
}

QPushButton*
createButton(const QString& text,
	     QObject* receiver, const char* member)
{
	QPushButton* button = new QPushButton(text);
	QObject::connect(button, SIGNAL(clicked()), receiver, member);
	return button;
}

QComboBox*
createHistoryComboBox(int characters)
{
	QComboBox* comboBox = new QComboBox;
	comboBox->setEditable(true);
	comboBox->setDuplicatesEnabled(false);
	comboBox->setInsertPolicy(QComboBox::InsertAtTop);
	comboBox->setSizePolicy(QSizePolicy::Expanding,
				QSizePolicy::Preferred);
	if (characters > 0) {
		comboBox->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
		comboBox->setMinimumContentsLength(characters);
	}
	return comboBox;
}

QSpinBox*
createSpinBox(int min, int max, int step, int value)
{
	QSpinBox* spinBox = new QSpinBox;
	spinBox->setMinimum(min);
	spinBox->setMaximum(max);
	spinBox->setSingleStep(step);
	spinBox->setValue(value);
	return spinBox;
}

QLabel*
resBox(QGridLayout* layout, int row, int col, const QString& text,
       int indent = 4)
{
	QLabel* label = new QLabel(text);
	QFontMetrics fm = label->fontMetrics();
	int xwidth = fm.width(QLatin1Char('x'));
	if (indent)
		label->setIndent(xwidth * indent);
	QLabel* box = new QLabel();
	label->setBuddy(box);
	box->setMinimumWidth(xwidth * 10);
	box->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred);
	box->setFrameStyle(QFrame::Sunken | QFrame::Panel);

	layout->addWidget(label, row, col);
	layout->addWidget(box, row, col + 1);
	return box;
}

void
resLabel(QGridLayout* layout, int row, int col, const QString& text)
{
	QLabel* label = new QLabel(text);
	QFont font;
	font.setBold(true);
	label->setFont(font);
	layout->addWidget(label, row, col, 1, 2);
}

bool
hasQuarterCmds(const QString& filename)
{
	QFile file(filename);
	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		for (;;) {
			char line[32];
			qint64 len = file.readLine(line, sizeof(line));
			if (len < 20)
				break;

			if (!std::memcmp(line, "0001", 4))
				return true;
		}
	}
	return false;
}

} // namespace


//////////////////////
// class CycleTimer //
//////////////////////

CT::CycleTimer::CycleTimer(QWidget* parent)
	: QMainWindow(parent),
	  confNext(0),
	  maxConcurrent(getCpuCount()),
	  pointId(0),
	  yuvX(1920), yuvY(1080),
	  displayedItem(0),
	  tableWindow(0)
{
	int y, x;
	QGroupBox* confGroupBox = new QGroupBox(tr("Motion-estimation search configuration"));

	QLabel* lProgMem = new QLabel(tr("&Program memory:"));
	cProgMem = createHistoryComboBox(32);
	lProgMem->setBuddy(cProgMem);
	browseProgMem = createButton(tr("Browse..."), this, SLOT(browseProgram()));

	QLabel* lPointMem = new QLabel(tr("P&oints memory:"));
	cPointMem = createHistoryComboBox(32);
	lPointMem->setBuddy(cPointMem);
	browsePointMem = createButton(tr("Browse..."), this, SLOT(browsePoints()));

	QLabel* lFull = new QLabel(tr("&Full-pel execution units:"));
	cFull = createSpinBox(1, 16, 1, 1);
	lFull->setBuddy(cFull);

	QLabel* lQuarter = new QLabel(tr("&Sub-pel execution units:"));
	cQuarter = createSpinBox(0, 16, 1, 1);
	lQuarter->setBuddy(cQuarter);

	QLabel* lRefFrames = new QLabel(tr("&Reference frames:"));
	cRefFrames = createSpinBox(1, 8, 1, 1);
	lRefFrames->setBuddy(cRefFrames);

	QLabel* lParitions = new QLabel(tr("Smallest &partition:"));
	cPartitions = new QComboBox;
	lParitions->setBuddy(cPartitions);
	cPartitions->addItem(tr("16x16"), 16);
	cPartitions->addItem(tr("8x8"), 8);
	cPartitions->addItem(tr("4x4"), 4);
	cPartitions->setCurrentIndex(0);

	cMVCost = new QCheckBox(tr("Enable &Motion Vector cost optimization"));
	cMVCost->setChecked(true);
	cMVCand = new QCheckBox(tr("Enable &Motion Vector candidates"));
	cMVCand->setChecked(true);

	QGridLayout* confLayout = new QGridLayout();
	confLayout->setColumnStretch(1, 1);
	y = 0;
	confLayout->addWidget(lProgMem, y, 0);
	confLayout->addWidget(cProgMem, y, 1, 1, 2);
	confLayout->addWidget(browseProgMem, y++, 3);
	confLayout->addWidget(lPointMem, y, 0);
	confLayout->addWidget(cPointMem, y, 1, 1, 2);
	confLayout->addWidget(browsePointMem, y++, 3);
	confLayout->addWidget(lFull, y, 0, 1, 2);
	confLayout->addWidget(cFull, y++, 2);
	confLayout->addWidget(lQuarter, y, 0, 1, 2);
	confLayout->addWidget(cQuarter, y++, 2);
	confLayout->addWidget(lRefFrames, y, 0, 1, 2);
	confLayout->addWidget(cRefFrames, y++, 2);
	confLayout->addWidget(lParitions, y, 0, 1, 2);
	confLayout->addWidget(cPartitions, y++, 2);
	confLayout->addWidget(cMVCost, y++, 0, 1, 3);
	confLayout->addWidget(cMVCand, y++, 0, 1, 3);
	confGroupBox->setLayout(confLayout);


	QGroupBox* videoGroupBox = new QGroupBox(tr("Video data"));

	QLabel* lVideoFile = new QLabel(tr("&Video file:"));
	cVideoFile = createHistoryComboBox(32);
	lVideoFile->setBuddy(cVideoFile);
	browseVideoFile = createButton(tr("Browse..."), this, SLOT(browseVideo()));
	connect(cVideoFile, SIGNAL(currentIndexChanged(int)),
		this, SLOT(videoBoxIndexChanged(int)));

	QLabel* lRes = new QLabel(tr("Re&solution:"));
	cResX = createSpinBox(1, 9999, 1, 352);
	lRes->setBuddy(cResX);
	cResY = createSpinBox(1, 9999, 1, 288);

	QLabel* lFrames = new QLabel(tr("&Maximum number of frames:"));
	cFrames = createSpinBox(0, 10000, 10, 50);
	lFrames->setBuddy(cFrames);

	QLabel* lQP = new QLabel(tr("&QP (0 is lossless):"));
	cQP = createSpinBox(0, 51, 1, 26);
	lQP->setBuddy(cQP);

	QGridLayout* videoLayout = new QGridLayout();
	videoLayout->setColumnStretch(1, 1);
	y = 0;
	videoLayout->addWidget(lVideoFile, y, 0);
	videoLayout->addWidget(cVideoFile, y, 1, 1, 4);
	videoLayout->addWidget(browseVideoFile, y++, 5);
	videoLayout->addWidget(lRes, y, 0, 1, 2);
	videoLayout->addWidget(cResX, y, 2);
	videoLayout->addWidget(new QLabel(tr("x")), y, 3);
	videoLayout->addWidget(cResY, y++, 4);
	videoLayout->addWidget(lFrames, y, 0, 1, 2);
	videoLayout->addWidget(cFrames, y++, 4);
	videoLayout->addWidget(lQP, y, 0, 1, 2);
	videoLayout->addWidget(cQP, y++, 4);
	videoGroupBox->setLayout(videoLayout);


	QGroupBox* resultsGroupBox = new QGroupBox(tr("Results"));
	QGridLayout* resLayout = new QGridLayout();
	resLayout->setColumnStretch(1, 1);
	resLayout->setColumnStretch(4, 1);
	resLayout->setColumnMinimumWidth(2, 12);

	y = x = 0;
	resLabel(resLayout, y++, x, tr("Processor configuration:"));
	bLabel = resBox(resLayout, y++, x, tr("Label:"));
	bFull = resBox(resLayout, y++, x, tr("Full-pel units:"));
	bQuarter = resBox(resLayout, y++, x, tr("Sub-pel units:"));
	bPartitions = resBox(resLayout, y++, x, tr("Smallest partition:"));
	bMVCost = resBox(resLayout, y++, x, tr("MV cost optimization:"));
	bMVCand = resBox(resLayout, y++, x, tr("MV candidates:"));
	bLUT = resBox(resLayout, y++, x, tr("Logic cells:"));
	resLabel(resLayout, y++, x, tr("Video data:"));
	bFrames = resBox(resLayout, y++, x, tr("Frames processed:"));
	bQP = resBox(resLayout, y++, x, tr("QP:"));
	bRefFrames = resBox(resLayout, y++, x, tr("Reference frames:"));
	y = 0;
	x = 3;
	resLabel(resLayout, y++, x, tr("Results:"));
	bBitRate = resBox(resLayout, y++, x, tr("Bit rate (kbit/s):"));
	bPSNR = resBox(resLayout, y++, x, tr("PSNR (dB):"));
	bEnergyMB = resBox(resLayout, y++, x, tr("Energy / macroblock (nJ):"));
	bFPS = resBox(resLayout, y++, x, tr("FPS:"));
	bCyclesMB = resBox(resLayout, y++, x, tr("Cycles / macroblock:"));
	resLabel(resLayout, y++, x, tr("Full- and sub-pel in parallel:"));
	bFPSParallel = resBox(resLayout, y++, x, tr("FPS:"));
	bCyclesMBParallel = resBox(resLayout, y++, x, tr("Cycles / macroblock:"));

	y += 2;
	bQueue = resBox(resLayout, y++, x, tr("Run queue:"), 0);

	resultsGroupBox->setLayout(resLayout);


	runButton = createButton(tr("R&un"), this, SLOT(run()));
	runButton->setDefault(true);
	stopButton = createButton(tr("&Stop"), this, SLOT(stop()));
	stopButton->setEnabled(false);
	plotButton = createButton(tr("&New Plot"), this, SLOT(plot()));
	tableButton = createButton(tr("&Table"), this, SLOT(table()));

	QDialogButtonBox* buttonBox = new QDialogButtonBox;
	buttonBox->addButton(runButton, QDialogButtonBox::ActionRole);
	buttonBox->addButton(stopButton, QDialogButtonBox::ActionRole);
	buttonBox->addButton(plotButton, QDialogButtonBox::ActionRole);
	buttonBox->addButton(tableButton, QDialogButtonBox::ActionRole);


	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addWidget(confGroupBox);
	mainLayout->addWidget(videoGroupBox);
	mainLayout->addWidget(resultsGroupBox);
	mainLayout->addWidget(buttonBox);

	QWidget* widget = new QWidget();
	widget->setLayout(mainLayout);
	setCentralWidget(widget);

	checkQueue();

	setWindowTitle(tr("Cycle Accurate Model"));
}

void CT::CycleTimer::updateResults(PointItem* po)
{
	displayedItem = po;
	const PointData& pt = po->data();

	bLabel->setText(pt.label);
	bFull->setText(QString::number(pt.full));
	bQuarter->setText(QString::number(pt.quarter));
	bPartitions->setText(pt.strVal(VarPartitions));
	bMVCost->setText(pt.strVal(VarMVCost));
	bMVCand->setText(pt.strVal(VarMVCand));
	bLUT->setText(QString::number(pt.lut));
	bQP->setText(QString::number(pt.qp));
	bRefFrames->setText(QString::number(pt.refFrames));

	if (!po->hasData()) {
		bFrames->setText(QString::number(0));
		bBitRate->setText(QString());
		bPSNR->setText(QString());
		bEnergyMB->setText(QString());
		bFPS->setText(QString());
		bCyclesMB->setText(QString());
		bFPSParallel->setText(QString());
		bCyclesMBParallel->setText(QString());
	} else {
		bFrames->setText(pt.strVal(VarFrames));
		bBitRate->setText(pt.strVal(VarBitRate));
		bPSNR->setText(pt.strVal(VarPSNR));
		bEnergyMB->setText(pt.strVal(VarEnergyMB));
		bFPS->setText(pt.strVal(VarFPS));
		bCyclesMB->setText(pt.strVal(VarCyclesMB));
		bFPSParallel->setText(pt.strVal(VarFPSParallel));
		bCyclesMBParallel->setText(pt.strVal(VarCyclesMBParallel));
	}
}

CT::PointItem* CT::CycleTimer::getConf()
{
	PointConf pt;
	++pointId;
	pt.label = tr("Cfg %n", 0, pointId);
	pt.progMem = cProgMem->currentText();
	pt.pointMem = cPointMem->currentText();
	pt.full = cFull->value();
	pt.quarter = cQuarter->value();
	pt.partitions = cPartitions->itemData(cPartitions->currentIndex()).toInt();
	pt.mvCost = cMVCost->isChecked();
	pt.mvCand = cMVCand->isChecked();
	pt.lut = 743 + 1555 * pt.full;
	if (pt.quarter)
		pt.lut += 5097 + 1749 * pt.quarter;
	if (pt.mvCost)
		pt.lut += 329;
	if (pt.mvCand)
		pt.lut += 259;
	pt.videoFile = cVideoFile->currentText();
	pt.resX = cResX->value();
	pt.resY = cResY->value();
	pt.frames = cFrames->value();
	pt.qp = cQP->value();
	pt.refFrames = cRefFrames->value();

	if (pt.videoFile.endsWith(".yuv", Qt::CaseInsensitive)) {
		yuvX = pt.resX;
		yuvY = pt.resY;
	}
	QPoint res(pt.resX, pt.resY);
	cVideoFile->setItemData(cVideoFile->currentIndex(), res);

	return new PointItem(pt);
}

void CT::CycleTimer::checkQueue()
{
	int run = runPoints.size();
	int queued = points.size() - confNext;
	while (run < maxConcurrent && queued > 0) {
		PointItem* item = points[confNext++];
		--queued;
		runPoints.append(item);
		++run;
		item->start();
	}
	if (run > 0 && !runPoints.contains(displayedItem))
		updateResults(runPoints.front());

	int t = run + queued;
	stopButton->setEnabled(t > 0);
	bQueue->setText(QString::number(run) + tr(" / ") + QString::number(t));
}

void CT::CycleTimer::browseProgram()
{
	QString prog = cProgMem->currentText();
	QString pt = cPointMem->currentText();
	QString def;
	if (!prog.isEmpty())
		def = prog;
	else if (!pt.isEmpty())
		def = QFileInfo(pt).path();
	QString file = QFileDialog::getOpenFileName
		(this, tr("Program memory"), def,
		 tr("Memory files (*.bin *.mif);;All files (*)"));
	if (!file.isEmpty()) {
		int existing = cProgMem->findText(file);
		if (existing >= 0) {
			cProgMem->setCurrentIndex(existing);
		} else {
			cProgMem->insertItem(0, file);
			cProgMem->setCurrentIndex(0);
		}
	}
}

void CT::CycleTimer::browsePoints()
{
	QString prog = cProgMem->currentText();
	QString pt = cPointMem->currentText();
	QString def;
	if (!pt.isEmpty())
		def = pt;
	else if (!prog.isEmpty())
		def = QFileInfo(prog).path();
	QString file = QFileDialog::getOpenFileName
		(this, tr("Points memory"), cPointMem->currentText(),
		 tr("Memory files (*.bin *.mif);;All files (*)"));
	if (!file.isEmpty()) {
		int existing = cPointMem->findText(file);
		if (existing >= 0) {
			cPointMem->setCurrentIndex(existing);
		} else {
			cPointMem->insertItem(0, file);
			cPointMem->setCurrentIndex(0);
		}
	}
}

void CT::CycleTimer::browseVideo()
{
	QString filters[] = {
		tr("CIF files (*.cif)"),
		tr("QCIF files (*.qcif)"),
		tr("YUV files (*.yuv)"),
		tr("All files (*)")
	};
	QString allFilters = (filters[0]
			      + ";;" + filters[1]
			      + ";;" + filters[2]
			      + ";;" + filters[3]);
	QString sel = filters[0];
	QString curr = cVideoFile->currentText();
	if (curr.endsWith(".cif", Qt::CaseInsensitive)) {
		sel = filters[0];
	} else if (curr.endsWith(".qcif", Qt::CaseInsensitive)) {
		sel = filters[1];
	} else if (curr.endsWith(".yuv", Qt::CaseInsensitive)) {
		sel = filters[2];
	} else if (curr.contains('.')) {
		sel = filters[3];
	}
	QString file = QFileDialog::getOpenFileName
		(this, tr("Video file"), curr,
		 allFilters, &sel);
	if (!file.isEmpty()) {
		int existing = cVideoFile->findText(file);
		if (existing >= 0) {
			cVideoFile->setCurrentIndex(existing);
		} else {
			QPoint res(cResX->value(), cResY->value());
			if (file.endsWith(".qcif", Qt::CaseInsensitive)) {
				res.setX(176);
				res.setY(144);
			} else if (file.endsWith(".cif", Qt::CaseInsensitive)) {
				res.setX(352);
				res.setY(288);
			} else if (file.endsWith(".yuv", Qt::CaseInsensitive)) {
				res.setX(yuvX);
				res.setY(yuvY);
			}
			cVideoFile->insertItem(0, file, QVariant(res));
			cVideoFile->setCurrentIndex(0);
		}
	}
}

void CT::CycleTimer::run()
{
	static bool warnQuarter = true;
	if (warnQuarter && cQuarter->value() == 0
	    && hasQuarterCmds(cProgMem->currentText())) {

		QMessageBox mb(QMessageBox::Warning,
			       tr("Warning"),
			       tr("The program has sub-pel instructions,\nbut there are no sub-pel execution units."),
			       QMessageBox::Ignore | QMessageBox::Cancel,
			       this);
		QCheckBox* cb = new QCheckBox(tr("Do not show this dialog again."));
		cb->setChecked(false);
		QGridLayout* l =  dynamic_cast<QGridLayout*>(mb.layout());
		if (l)
			l->addWidget(cb, 1, 1);
		if (mb.exec() != QMessageBox::Ignore)
			return;
		if (l && cb->isChecked())
			warnQuarter = false;
	}

	PointItem* item = getConf();
	points.append(item);
	connect(item, SIGNAL(changed(PointItem*)),
		this, SLOT(pointChanged(PointItem*)));
	connect(item, SIGNAL(finished(PointItem*)),
		this, SLOT(pointFinished(PointItem*)));
	connect(item, SIGNAL(removed(PointItem*)),
		this, SLOT(pointRemoved(PointItem*)));
	emit addPoint(item);

	checkQueue();
}

void CT::CycleTimer::stop()
{
	while (confNext < points.size()) {
		PointItem* item = points[confNext++];
		item->remove();
	}

	QList<PointItem*> l;
	for (int i = runPoints.size(); i-- > 0; )
		l.append(runPoints[i]);
	for (int i = 0; i < l.size(); ++i)
		l[i]->stop();

	checkQueue();
}

void CT::CycleTimer::plot()
{
	Graph* graph = new Graph(0);
	for (int i = 0; i < points.size(); ++i)
		graph->addPoint(points[i]);
	connect(this, SIGNAL(addPoint(PointItem*)),
		graph, SLOT(addPoint(PointItem*)));
	graph->show();
}

void CT::CycleTimer::table()
{
	if (!tableWindow) {
		tableWindow = new Table(0, this);
		for (int i = 0; i < points.size(); ++i)
			tableWindow->addPoint(points[i]);
		connect(this, SIGNAL(addPoint(PointItem*)),
			tableWindow, SLOT(addPoint(PointItem*)));
	}
	tableWindow->show();
	tableWindow->activateWindow();
}

void CT::CycleTimer::pointChanged(PointItem* src)
{
	if (displayedItem == src)
		updateResults(src);
}

void CT::CycleTimer::pointFinished(PointItem* src)
{
	runPoints.removeOne(src);
	checkQueue();
}

void CT::CycleTimer::pointRemoved(PointItem* src)
{
	int p = points.indexOf(src);
	if (p < 0)
		return;
	points.removeAt(p);
	if (confNext > p)
		--confNext;

	if (points.size() == 0)
		pointId = 0;

	if (displayedItem == src) {
		displayedItem = 0;
		bLabel->setText(QString());
		bFull->setText(QString());
		bQuarter->setText(QString());
		bPartitions->setText(QString());
		bMVCost->setText(QString());
		bMVCand->setText(QString());
		bLUT->setText(QString());
		bFrames->setText(QString());
		bQP->setText(QString());
		bRefFrames->setText(QString());
		bBitRate->setText(QString());
		bPSNR->setText(QString());
		bEnergyMB->setText(QString());
		bFPS->setText(QString());
		bCyclesMB->setText(QString());
		bFPSParallel->setText(QString());
		bCyclesMBParallel->setText(QString());
	}

	// if point was running, remove it from run queue
	runPoints.removeOne(src);

	checkQueue();
}

void CT::CycleTimer::videoBoxIndexChanged(int i)
{
	QPoint res = cVideoFile->itemData(i).toPoint();
	cResX->setValue(res.x());
	cResY->setValue(res.y());
}
