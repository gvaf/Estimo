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

#include <cmath>

#include <QtGui>
#include <QList>

#include "cycletimergraph.h"
#include "cycletimerpoint.h"


//////////////////////
// helper functions //
//////////////////////

namespace {

void
calculateAxis(qreal min, qreal max, qreal min_step, int max_intervals,
	      qreal& start, qreal& end, int& intervals, int& log10)
{
	if (min > max) {
		qreal t = min;
		min = max;
		max = t;
	}
	if (min_step > 0) {
		min = min_step * std::floor(min / min_step);
		max = min_step * std::ceil(max / min_step);
	}

	qreal range = max - min;
	if (range == 0) { // min == max
		qreal step = min_step;
		if (step <= 0) {
			step = 1;
			min = std::floor(min);
			max = std::ceil(max);
			range = max - min;
		}
		if (range != 0) {
			// do nothing
		} else if (max <= -step || min >= step) {
			min -= step;
			max += step;
		} else if (min < 0) {
			min -= step;
		} else {
			max += step;
		}
		range = max - min;
	}

	// find initial interval size
	int exp10 = (int) std::floor(std::log10(range));
	qreal p10 = std::pow(10.0, exp10);
	qreal mant = range / p10;
	int one25 = 1;
	if (mant > 5) {
		++exp10;
		p10 *= 10;
	} else if (mant > 2) {
		one25 = 5;
	} else if (mant > 1) {
		one25 = 2;
	}
	int rmin = (int) std::floor(min / p10 / one25);
	int rmax = (int) std::ceil(max / p10 / one25);

	// in every iteration, reduce the interval size and see if it is ok
	for (;;) {
		int next_exp10 = exp10;
		qreal next_p10 = p10;
		int next_one25 = one25;
		if (one25 == 5) {
			next_one25 = 2;
		} else if (one25 == 2) {
			next_one25 = 1;
		} else {
			next_one25 = 5;
			--next_exp10;
			next_p10 /= 10;
		}
		int next_rmin = (int) std::floor(min / next_p10 / next_one25);
		int next_rmax = (int) std::ceil(max / next_p10 / next_one25);
		int next_ints = next_rmax - next_rmin;
		int ints = rmax - rmin;
		if ((min_step <= 0 || next_one25 * next_p10 >= min_step)
		    && (next_ints <= max_intervals || next_ints <= ints)) {
			exp10 = next_exp10;
			p10 = next_p10;
			one25 = next_one25;
			rmin = next_rmin;
			rmax = next_rmax;
		} else {
			start = rmin * p10 * one25;
			end = rmax * p10 * one25;
			intervals = ints;
			break;
		}
	}
	log10 = exp10 / 3 * 3;
}

qreal
drawBoundedTextSuper(QPainter& painter, const QRectF& rect, int flags,
		     const QString& text, const QString& super, const QBrush& bg)
{
	QFont font = painter.font();
	QFont supFont = font;
	supFont.setPixelSize((int) (supFont.pixelSize() * 0.7));

	int supRaise = (int) (supFont.pixelSize() * .4);

	QRectF r, supr;
	QRectF bound = painter.boundingRect(rect, flags, text);
	painter.setFont(supFont);
	QRectF supBound = painter.boundingRect(rect, flags, super);
	painter.setFont(font);
	qreal w;
	bool scaled = false;
	qreal reqWidth = bound.width() + supBound.width();
	painter.save();
	painter.translate(rect.topLeft());
	if (reqWidth > rect.width()) {
		scaled = true;
		painter.scale(rect.width() / reqWidth, 1);
		r.setRect(0, 0, bound.width(), rect.height());
		supr.setRect(bound.width(), -supRaise,
			     supBound.width(), rect.height());
		w = rect.width();
	} else {
		// first set for Qt::AlignLeft
		r.setRect(0, 0, bound.width(), rect.height());
		supr.setRect(bound.width(), -supRaise,
			     supBound.width(), rect.height());
		if (flags & Qt::AlignRight) {
			r.moveRight(rect.width() - supBound.width());
			supr.moveRight(rect.width());
		} else if (flags & Qt::AlignHCenter) {
			qreal o = (rect.width() - reqWidth) / 2;
			r.moveLeft(r.left() + o);
			supr.moveLeft(supr.left() + o);
		}
		w = reqWidth;
	}
	if (bg != Qt::NoBrush) {
		painter.save();
		painter.setPen(Qt::NoPen);
		painter.setBrush(bg);
		painter.drawRect(QRectF(r.left(), r.top(), w, r.height()));
		painter.restore();
	}
	painter.drawText(r, flags, text);
	painter.setFont(supFont);
	painter.drawText(supr, flags, super);
	painter.restore();
	return w;
}

qreal
drawBoundedText(QPainter& painter, const QRectF& rect, int flags,
		const QString& text, const QBrush& bg = Qt::NoBrush)
{
	if (text.contains('^')) {
		int i = text.lastIndexOf('^');
		QString t = text.left(i);
		QString sup = text.mid(i + 1);
		return drawBoundedTextSuper(painter, rect, flags, t, sup, bg);
	}

	QRectF r(rect);
	QRectF bound = painter.boundingRect(rect, flags, text);
	qreal w;
	bool scaled = false;
	if (bound.width() > rect.width()) {
		scaled = true;
		painter.save();
		painter.translate(rect.topLeft());
		painter.scale(rect.width() / bound.width(), 1);
		r.setRect(0, 0, bound.width(), rect.height());
		w = rect.width();
	} else {
		if (flags & (Qt::AlignLeft | Qt::AlignHCenter))
			r.setRight(bound.right());
		if (flags & (Qt::AlignRight | Qt::AlignHCenter))
			r.setLeft(bound.left());
		w = bound.width();
	}
	if (bg != Qt::NoBrush) {
		painter.save();
		painter.setPen(Qt::NoPen);
		painter.setBrush(bg);
		painter.drawRect(r);
		painter.restore();
	}
	painter.drawText(r, flags, text);
	if (scaled) {
		painter.restore();
	}
	return w;
}

QPushButton*
createButton(const QString& text,
	     QObject* receiver, const char* member)
{
	QPushButton* button = new QPushButton(text);
	QObject::connect(button, SIGNAL(clicked()), receiver, member);
	return button;
}

QDoubleSpinBox*
createDoubleSpinBox(double min, double max, double step, double value)
{
	QDoubleSpinBox* spinBox = new QDoubleSpinBox;
	spinBox->setMinimum(min);
	spinBox->setMaximum(max);
	spinBox->setSingleStep(step);
	spinBox->setValue(value);
	return spinBox;
}

CT::Variable xDefault = CT::VarBitRate;
CT::Variable yDefault = CT::VarPSNR;
CT::Variable zDefault = CT::VarLUT;
CT::Variable labelDefault = CT::VarLabel;

void
addToCombo(QComboBox* combo, CT::Variable var, CT::Variable defVar)
{
	combo->addItem(varLabel(var), var);
	if (var == defVar)
		combo->setCurrentIndex(combo->count() - 1);
}

QDoubleSpinBox*
addMmItem(QGridLayout* layout, int row, int column, const QString& text,
	  double min, double max, double value)
{
	QLabel* label = new QLabel(text);
	QDoubleSpinBox* box = createDoubleSpinBox(min, max, 1.0, value);
	box->setDecimals(1);
	label->setBuddy(box);
	QLabel* mm = new QLabel(QObject::tr("mm"));

	layout->addWidget(label, row, column);
	layout->addWidget(box, row, column + 1);
	layout->addWidget(mm, row, column + 2);
	return box;
}

void
printOn(CT::GraphRenderer* renderer, QPrinter* printer, bool exprt,
	qreal pw = 0, qreal ph = 0, qreal gw = 0, qreal gh = 0)
{
	QRectF paper = printer->paperRect();
	QRectF page = exprt ? paper : printer->pageRect();
	int res = printer->resolution();

	if (!exprt) {
		if (paper.width() < res || paper.height() < res)
			return;
		page = page.intersected(QRectF(paper.left() + res * 0.25,
					       paper.top() + res * 0.25,
					       paper.width() - res * 0.5,
					       paper.height() - res * 0.5));
		if (page.width() < res * 0.5 || page.height() < res * 0.5)
			return;
	}

	QPainter painter(printer);

	qreal scale = res / 72.0;
	if (exprt) {
		qreal maxDim = paper.width() > paper.height()
			? paper.width() : paper.height();
		if (maxDim < 8 * res)
			scale = maxDim / 72 / 8;
	}
	painter.scale(scale, scale);
	paper.setRect(paper.left() / scale, paper.top() / scale,
		      paper.width() / scale, paper.height() / scale);
	page.setRect(page.left() / scale, page.top() / scale,
		     page.width() / scale, page.height() / scale);

	qreal w, h;
	if (exprt) {
		w = page.width() * gw / pw;
		h = page.height() * gh / ph;
	} else {
		w = page.height() / 0.75;
		if (w > page.width())
			w = page.width();
		if (w > 576)
			w = 576;
		h = w * 0.75;
	}
	qreal left = (paper.width() - w) / 2;
	qreal top = (paper.height() - h) / 2;
	if (!exprt) {
		if (left < page.left())
			left = page.left();
		else if (left + w > page.right())
			left = page.right() - w;
		if (top < page.top())
			top = page.top();
		else if (top + h > page.bottom())
			top = page.bottom() - h;
	}

	renderer->paintOn(painter, left, top, w, h);
}

} // namespace


/////////////////////////
// class GraphRenderer //
/////////////////////////

CT::GraphRenderer::GraphRenderer(QWidget* parent, Graph* g)
	: QWidget(parent),
	  graph(g),
	  xAxis(VarNone),
	  yAxis(VarNone),
	  zAxis(VarNone),
	  label(VarNone),
	  xMin(0), xMax(0),
	  yMin(0), yMax(0),
	  zMin(0), zMax(0)
{
	setBackgroundRole(QPalette::Base);
	setAutoFillBackground(true);
}

QSize CT::GraphRenderer::minimumSizeHint() const
{
	return QSize(400, 300);
}

QSize CT::GraphRenderer::sizeHint() const
{
	return QSize(600, 450);
}

void CT::GraphRenderer::setXAxis(Variable var)
{
	xAxis = var;
	updatePoints();
}

void CT::GraphRenderer::setYAxis(Variable var)
{
	yAxis = var;
	updatePoints();
}

void CT::GraphRenderer::setPointArea(Variable var)
{
	zAxis = var;
	updatePoints();
}

void CT::GraphRenderer::setLabel(Variable var)
{
	label = var;
	update();
}

void CT::GraphRenderer::setTitle(const QString& text)
{
	title = text;
	update();
}

// measurements should be in pts (1 inch = 72 pts)
void CT::GraphRenderer::paintOn(QPainter& painter,
				qreal x, qreal y, qreal w, qreal h)
{
	font.setFamily("Helvetica");

	qreal marTop, marBottom, marLeft, marRight;
	marTop = 10;
	marBottom = 10;
	marLeft = 10;
	marRight = 30;
	ticLength = 5;
	titleH = 30;
	axisLH = 20;
	ticLH = 20;
	ticLW = 50;
	titlePad = 20;
	labelPad = 10;
	ticPad = 9;
	pointLH = 20;
	pointLW = 60;
        crossL = 4;
	circR = 9;

	if (title.isEmpty()) {
		marTop = 20;
		titleH = titlePad = 0;
	}

	pxMin = x + marLeft + axisLH + labelPad + ticLW + ticPad;
	pxMax = x + w - marRight;
	pyMin = y + h - marBottom - axisLH - labelPad - ticLH - ticPad;
	pyMax = y + marTop + titleH + titlePad;

	int aInt = (int) std::floor((pxMax - pxMin) / ticLW * .75);
	if (aInt > 10)
		aInt = 10;
	calculateAxis(xMin, xMax, varMinStep(xAxis), aInt,
		      axMin, axMax, axInt, axLog10);
	aInt = (int) std::floor((pyMin - pyMax) / ticLH * .5);
	if (aInt > 10)
		aInt = 10;
	calculateAxis(yMin, yMax, varMinStep(yAxis), aInt,
		      ayMin, ayMax, ayInt, ayLog10);

	drawAxis(painter);
	drawPoints(painter);
}

void CT::GraphRenderer::updatePoints()
{
	xMin = xMax = yMin = yMax = zMin = zMax = 0;
	bool init = false;
	for (int i = 0; i < graph->points.size(); ++i) {
		if (!graph->points[i].second)
			continue;
		const PointData& point = graph->points[i].first->data();
		qreal x = point.realVal(xAxis);
		qreal y = point.realVal(yAxis);
		qreal z = point.realVal(zAxis);
		if (!init) {
			xMin = xMax = x;
			yMin = yMax = y;
			zMin = zMax = z;
			init = true;
		} else {
			if (x < xMin)
				xMin = x;
			if (x > xMax)
				xMax = x;
			if (y < yMin)
				yMin = y;
			if (y > yMax)
				yMax = y;
			if (z < zMin)
				zMin = z;
			if (z > zMax)
				zMax = z;
		}
	}
	update();
}

void CT::GraphRenderer::paintEvent(QPaintEvent* /* event */)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	painter.translate(+0.5, +0.5);

	painter.setPen(palette().dark().color());
	painter.setBrush(palette().light());
	painter.drawRect(QRect(0, 0, width() - 1, height() - 1));

	paintOn(painter, 0, 0, width(), height());
}

void CT::GraphRenderer::drawAxis(QPainter& painter)
{
	QRectF r;

	painter.setPen(QPen(Qt::black, 1));
	painter.setBrush(QBrush(Qt::white));

	// draw axis
	painter.drawRect(QRectF(pxMin, pyMin, pxMax - pxMin, pyMax - pyMin));

	// draw tic marks

	font.setPixelSize((int) (axisLH * 0.75));
	painter.setFont(font);
	painter.setBrush(Qt::NoBrush);

	r.setRect(0, pyMin + ticPad, ticLW, ticLH);
	for (int i = 0; i <= axInt; ++i) {
		qreal ax = axMin + (axMax - axMin) * i / axInt;
		qreal px = pxMin + (pxMax - pxMin) * i / axInt;
		painter.drawLine(QLineF(px, pyMin, px, pyMin - ticLength));
		painter.drawLine(QLineF(px, pyMax, px, pyMax + ticLength));
		r.moveLeft(px - ticLW / 2);
		qreal n = ax;
		if (axLog10)
			n /= std::pow(10.0, axLog10);
		if (std::abs(n) < 1e-5)
			n = 0;
		drawBoundedText(painter, r, Qt::AlignHCenter | Qt::AlignTop,
				QString::number(n));
	}

	// remember largest label width
	r.setRect(pxMin - ticPad - ticLW, 0, ticLW, ticLH);
	qreal maxWidth = 0;
	for (int i = 0; i <= ayInt; ++i) {
		qreal ay = ayMin + (ayMax - ayMin) * i / ayInt;
		qreal py = pyMin + (pyMax - pyMin) * i / ayInt;
		painter.drawLine(QLineF(pxMin, py, pxMin + ticLength, py));
		painter.drawLine(QLineF(pxMax, py, pxMax - ticLength, py));
		r.moveTop(py - ticLH / 2);
		qreal n = ay;
		if (ayLog10)
			n /= std::pow(10.0, ayLog10);
		if (std::abs(n) < 1e-5)
			n = 0;
		QString text(QString::number(n));
		int flags = Qt::AlignRight | Qt::AlignVCenter;
		qreal w = drawBoundedText(painter, r, flags, text);
		if (w > maxWidth)
			maxWidth = w;
	}

	// label axis
	font.setPixelSize((int) (axisLH * 0.75));
	painter.setFont(font);

	QString text = varLabel(xAxis, true);
	if (axLog10)
		text += " x 10^" + QString::number(axLog10);
	r.setRect(pxMin, pyMin + ticPad + ticLH + labelPad,
		  pxMax - pxMin, axisLH);
	drawBoundedText(painter, r, Qt::AlignCenter, text);

	painter.save();
	painter.rotate(-90);
	text = varLabel(yAxis, true);
	if (ayLog10)
		text += " x 10^" + QString::number(ayLog10);
	r.setRect(-pyMin, pxMin - ticPad - maxWidth - labelPad - axisLH,
		  pyMin - pyMax, axisLH);
	drawBoundedText(painter, r, Qt::AlignCenter, text);
	painter.restore();

	// label plot
	font.setPixelSize((int) (titleH * 0.75));
	painter.setFont(font);
	r.setRect(pxMin, pyMax - titlePad - titleH, pxMax - pxMin, titleH);
	drawBoundedText(painter, r, Qt::AlignCenter, title);
}

void CT::GraphRenderer::drawPoints(QPainter& painter)
{
	qreal xMult = (pxMax - pxMin) / (axMax - axMin);
	qreal xAdd = pxMin - xMult * axMin;
	qreal yMult = (pyMax - pyMin) / (ayMax - ayMin);
	qreal yAdd = pyMin - yMult * ayMin;
	qreal absZMin = std::abs(zMin);
	qreal absZMax = std::abs(zMax);
	qreal zDivider = absZMin > absZMax ? absZMin : absZMax;
	if (label != VarNone) {
		for (int i = 0; i < graph->points.size(); ++i) {
			if (!graph->points[i].second)
				continue;
			const PointData& point = graph->points[i].first->data();
			qreal x = point.realVal(xAxis);
			qreal y = point.realVal(yAxis);
			qreal px = x * xMult + xAdd;
			qreal py = y * yMult + yAdd;

			qreal z = point.realVal(zAxis);
			qreal radius = 0;
			if (zDivider > 0)
				radius = circR * std::sqrt(z / zDivider);

			QRectF r(px + radius + 1, py - pointLH - 1,
				 pointLW, pointLH);
			int hflag = Qt::AlignLeft;
			int vflag = Qt::AlignBottom;
			if (r.right() >= pxMax) {
				r.moveRight(px - radius - 1);
				hflag = Qt::AlignRight;
			}
			if (r.top() <= pyMax) {
				r.moveTop(py + 1);
				vflag = Qt::AlignTop;
			}
			painter.setPen(QPen(Qt::black, 1));
			painter.setBrush(Qt::NoBrush);
			font.setPixelSize((int) (pointLH * 0.75));
			painter.setFont(font);
			drawBoundedText(painter, r, vflag | hflag,
					point.strVal(label));//,
			//					QBrush(Qt::white));
		}
	}
	for (int i = 0; i < graph->points.size(); ++i) {
		if (!graph->points[i].second)
			continue;
		const PointData& point = graph->points[i].first->data();
		qreal x = point.realVal(xAxis);
		qreal y = point.realVal(yAxis);
		qreal px = x * xMult + xAdd;
		qreal py = y * yMult + yAdd;

		qreal z = point.realVal(zAxis);
		qreal radius = 0;
		if (zDivider > 0)
			radius = circR * std::sqrt(z / zDivider);

		painter.setPen(QPen(Qt::blue, 1));
		painter.setBrush(QBrush(Qt::blue));
		if (radius >= 2) {
			painter.drawEllipse(QRectF(px - radius, py - radius,
						   radius * 2, radius * 2));
		} else {
			radius = crossL;
			painter.drawLine(QLineF(px - crossL, py,
						px + crossL, py));
			painter.drawLine(QLineF(px, py - crossL,
						px, py + crossL));
		}
	}
}


/////////////////
// class Graph //
/////////////////

CT::Graph::Graph(QWidget* parent)
	: QMainWindow(parent)
{
	setAttribute(Qt::WA_DeleteOnClose, true);

	graphRenderer = new GraphRenderer(0, this);
	connect(this, SIGNAL(updateVisible()),
		graphRenderer, SLOT(updatePoints()));

	xCombo = new QComboBox;
	addToCombo(xCombo, VarFull, xDefault);
	addToCombo(xCombo, VarQuarter, xDefault);
	addToCombo(xCombo, VarFrequency, xDefault);
	addToCombo(xCombo, VarLUT, xDefault);
	addToCombo(xCombo, VarBitRate, xDefault);
	addToCombo(xCombo, VarPSNR, xDefault);
	addToCombo(xCombo, VarPower, xDefault);
	addToCombo(xCombo, VarFPS, xDefault);
	addToCombo(xCombo, VarCyclesMB, xDefault);
	addToCombo(xCombo, VarEnergyMB, xDefault);
	addToCombo(xCombo, VarFPSParallel, xDefault);
	addToCombo(xCombo, VarCyclesMBParallel, xDefault);
	addToCombo(xCombo, VarEnergyMBParallel, xDefault);

	yCombo = new QComboBox;
	addToCombo(yCombo, VarFull, yDefault);
	addToCombo(yCombo, VarQuarter, yDefault);
	addToCombo(yCombo, VarFrequency, yDefault);
	addToCombo(yCombo, VarLUT, yDefault);
	addToCombo(yCombo, VarBitRate, yDefault);
	addToCombo(yCombo, VarPSNR, yDefault);
	addToCombo(yCombo, VarPower, yDefault);
	addToCombo(yCombo, VarFPS, yDefault);
	addToCombo(yCombo, VarCyclesMB, yDefault);
	addToCombo(yCombo, VarEnergyMB, yDefault);
	addToCombo(yCombo, VarFPSParallel, yDefault);
	addToCombo(yCombo, VarCyclesMBParallel, yDefault);
	addToCombo(yCombo, VarEnergyMBParallel, yDefault);

	zCombo = new QComboBox;
	addToCombo(zCombo, VarNone, zDefault);
	addToCombo(zCombo, VarFull, zDefault);
	addToCombo(zCombo, VarQuarter, zDefault);
	addToCombo(zCombo, VarFrequency, zDefault);
	addToCombo(zCombo, VarHadamard, zDefault);
	addToCombo(zCombo, VarMVCost, zDefault);
	addToCombo(zCombo, VarMVCand, zDefault);
	addToCombo(zCombo, VarLUT, zDefault);
	addToCombo(zCombo, VarBitRate, zDefault);
	addToCombo(zCombo, VarPSNR, zDefault);
	addToCombo(zCombo, VarPower, zDefault);
	addToCombo(zCombo, VarFPS, zDefault);
	addToCombo(zCombo, VarCyclesMB, zDefault);
	addToCombo(zCombo, VarEnergyMB, zDefault);
	addToCombo(zCombo, VarFPSParallel, zDefault);
	addToCombo(zCombo, VarCyclesMBParallel, zDefault);
	addToCombo(zCombo, VarEnergyMBParallel, zDefault);

	labelCombo = new QComboBox;
	addToCombo(labelCombo, VarNone, labelDefault);
	addToCombo(labelCombo, VarLabel, labelDefault);

	titleBox = new QLineEdit("Plot title");

	QGridLayout* optionsLayout = new QGridLayout();
	optionsLayout->setColumnStretch(1, 1);
	optionsLayout->setColumnMinimumWidth(2, 12);
	optionsLayout->setColumnStretch(4, 1);
	QLabel* label;
	int y = 0;

	label = new QLabel(tr("Plot &title:"));
	label->setBuddy(titleBox);
	optionsLayout->addWidget(label, y, 0);
	optionsLayout->addWidget(titleBox, y++, 1);
	label = new QLabel(tr("Point &labels:"));
	label->setBuddy(labelCombo);
	optionsLayout->addWidget(label, y, 0);
	optionsLayout->addWidget(labelCombo, y++, 1);
	y = 0;
	label = new QLabel(tr("&X-axis:"));
	label->setBuddy(xCombo);
	optionsLayout->addWidget(label, y, 3);
	optionsLayout->addWidget(xCombo, y++, 4);
	label = new QLabel(tr("&Y-axis:"));
	label->setBuddy(yCombo);
	optionsLayout->addWidget(label, y, 3);
	optionsLayout->addWidget(yCombo, y++, 4);
	label = new QLabel(tr("&Area:"));
	label->setBuddy(zCombo);
	optionsLayout->addWidget(label, y, 3);
	optionsLayout->addWidget(zCombo, y++, 4);

	xChanged();
	yChanged();
	zChanged();
	labelChanged();
	titleChanged(titleBox->text());
	connect(xCombo, SIGNAL(activated(int)), this, SLOT(xChanged()));
	connect(yCombo, SIGNAL(activated(int)), this, SLOT(yChanged()));
	connect(zCombo, SIGNAL(activated(int)), this, SLOT(zChanged()));
	connect(labelCombo, SIGNAL(activated(int)),
		this, SLOT(labelChanged()));
	connect(titleBox, SIGNAL(textChanged(const QString&)),
		this, SLOT(titleChanged(const QString&)));

	exportPdfButton = createButton(tr("&Export..."), this, SLOT(exportPdf()));
	printButton = createButton(tr("&Print..."), this, SLOT(print()));
	QDialogButtonBox* buttonBox = new QDialogButtonBox;
	buttonBox->addButton(exportPdfButton, QDialogButtonBox::ActionRole);
	buttonBox->addButton(printButton, QDialogButtonBox::ActionRole);

	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addWidget(graphRenderer);
	mainLayout->addLayout(optionsLayout);
	mainLayout->addWidget(buttonBox);

	QWidget* widget = new QWidget();
	widget->setLayout(mainLayout);
	setCentralWidget(widget);

	titleBox->setFocus();
	titleBox->selectAll();
}

void CT::Graph::addPoint(PointItem* point)
{
	// check if already in list
	for (int i = 0; i < points.size(); ++i) {
		if (points[i].first == point)
			return;
	}

	connect(point, SIGNAL(changed(PointItem*)),
		this, SLOT(ptChanged(PointItem*)));
	connect(point, SIGNAL(removed(PointItem*)),
		this, SLOT(ptRemoved(PointItem*)));

	QPair<PointItem*, bool> p(point, point->plottable());
	points.append(p);
	if (p.second)
		emit updateVisible();
}

void CT::Graph::xChanged()
{
	Variable var = Variable(xCombo->itemData(xCombo->currentIndex()).toInt());
	graphRenderer->setXAxis(var);
	xDefault = var;
}

void CT::Graph::yChanged()
{
	Variable var = Variable(yCombo->itemData(yCombo->currentIndex()).toInt());
	graphRenderer->setYAxis(var);
	yDefault = var;
}

void CT::Graph::zChanged()
{
	Variable var = Variable(zCombo->itemData(zCombo->currentIndex()).toInt());
	graphRenderer->setPointArea(var);
	zDefault = var;
}

void CT::Graph::labelChanged()
{
	Variable var = Variable(labelCombo->itemData(labelCombo->currentIndex()).toInt());
	graphRenderer->setLabel(var);
	labelDefault = var;
}

void CT::Graph::titleChanged(const QString& text)
{
	setWindowTitle(text);
	graphRenderer->setTitle(text);
}

void CT::Graph::exportPdf()
{
	static qreal defPWidth = 80, defPHeight = 60;
	static bool defLandscape = false;
	static qreal defGWidth = 80, defGHeight = 60;
	static bool defGrayscale = false;
	static QString defPath;

	QGridLayout* layout = new QGridLayout;
	QDoubleSpinBox* bPW = addMmItem(layout, 0, 0, tr("&Paper width:"),
				       30, 1000, defPWidth);
	QDoubleSpinBox* bPH = addMmItem(layout, 1, 0, tr("P&aper height:"),
				       30, 1000, defPHeight);
	QCheckBox* bL = new QCheckBox(tr("&Landscape"));
	bL->setChecked(defLandscape);
	layout->addWidget(bL, 2, 0, 1, 3);
	QDoubleSpinBox* bGW = addMmItem(layout, 3, 0, tr("Graph &width:"),
				       30, 1000, defGWidth);
	QDoubleSpinBox* bGH = addMmItem(layout, 4, 0, tr("Graph &height:"),
				       30, 1000, defGHeight);
	QCheckBox* bG = new QCheckBox(tr("&Grayscale"));
	bG->setChecked(defGrayscale);
	layout->addWidget(bG, 5, 0, 1, 3);

	QDialog* d = new QDialog(this);
	QDialogButtonBox* box = new QDialogButtonBox(QDialogButtonBox::Cancel
						     | QDialogButtonBox::Ok);
	connect(box, SIGNAL(accepted()), d, SLOT(accept()));
	connect(box, SIGNAL(rejected()), d, SLOT(reject()));
	layout->addWidget(box, 6, 0, 1, 3);

	d->setLayout(layout);
	d->setWindowTitle(tr("Export to PDF"));
	if (d->exec() != QDialog::Accepted)
		return;

	qreal pw = defPWidth = bPW->value();
	qreal ph = defPHeight = bPH->value();
	bool l = defLandscape = bL->isChecked();
	qreal gw = defGWidth = bGW->value();
	qreal gh = defGHeight = bGH->value();
	bool g = defGrayscale = bG->isChecked();

	QString fileName = QFileDialog::getSaveFileName
		(this, tr("Export PDF"), defPath,
		 tr("PDF files (*.pdf);;EPS files (*.eps)"));
	if (fileName.isEmpty())
		return;
	defPath = QFileInfo(fileName).path();

	QPrinter printer(QPrinter::HighResolution);
	//	printer.setOutputFormat(QPrinter::PdfFormat);
	printer.setOutputFileName(fileName);
	if (fileName.endsWith(QLatin1String(".eps")))
		printer.setOutputFormat(QPrinter::PostScriptFormat);
	printer.setOrientation(l ? QPrinter::Landscape : QPrinter::Portrait);
	printer.setPaperSize(QSizeF(pw, ph), QPrinter::Millimeter);
	printer.setFullPage(true);
	printer.setColorMode(g ? QPrinter::GrayScale : QPrinter::Color);
	printOn(graphRenderer, &printer, true, pw, ph, gw, gh);
}

void CT::Graph::print()
{
	QPrinter printer(QPrinter::HighResolution);

	QPrintDialog* dialog = new QPrintDialog(&printer, this);
	dialog->setWindowTitle(tr("Print Plot"));
	dialog->setEnabledOptions(dialog->enabledOptions()
				  & ~(QAbstractPrintDialog::PrintSelection
				      | QAbstractPrintDialog::PrintPageRange));
	if (dialog->exec() != QDialog::Accepted)
		return;

	printOn(graphRenderer, &printer, false);
}

void CT::Graph::ptChanged(PointItem* point)
{
	for (int i = 0; i < points.size(); ++i) {
		if (points[i].first != point)
			continue;
		bool display = point->plottable() || points[i].second;
		points[i].second = point->plottable();
		if (display)
			emit updateVisible();
		break;
	}
}

void CT::Graph::ptRemoved(PointItem* point)
{
	for (int i = 0; i < points.size(); ++i) {
		if (points[i].first != point)
			continue;
		bool displayed = points[i].second;
		points.removeAt(i);
		if (displayed)
			emit updateVisible();
		break;
	}
}
