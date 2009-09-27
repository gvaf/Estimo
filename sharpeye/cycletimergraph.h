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

#ifndef CYCLETIMERGRAPH_H
#define CYCLETIMERGRAPH_H

#include <QList>
#include <QMainWindow>
#include <QPair>
#include <QWidget>

#include "cycletimerpoint.h"

class QComboBox;
class QLineEdit;

namespace CT {

class Graph;

class GraphRenderer : public QWidget
{
	Q_OBJECT

public:
	GraphRenderer(QWidget* parent, Graph* g);

	QSize minimumSizeHint() const;
	QSize sizeHint() const;

	void setXAxis(Variable var);
	void setYAxis(Variable var);
	void setPointArea(Variable var);
	void setLabel(Variable var);
	void setTitle(const QString& text);

	void paintOn(QPainter& painter, qreal x, qreal y, qreal w, qreal h);

public slots:
	void updatePoints();

protected:
	void paintEvent(QPaintEvent* event);

private:
	Graph* graph;
	Variable xAxis, yAxis, zAxis, label;
	QString title;

	QFont font;
	qreal ticLength;
	qreal titleH, axisLH, ticLH, ticLW;
	qreal titlePad, labelPad, ticPad;
	qreal pointLH, pointLW;
	qreal crossL, circR;

	qreal xMin, xMax, yMin, yMax, zMin, zMax;

	qreal axMin, axMax, ayMin, ayMax;
	qreal pxMin, pxMax, pyMin, pyMax;
	int axInt, ayInt, axLog10, ayLog10;

	void drawAxis(QPainter& painter);
	void drawPoints(QPainter& painter);
};

class Graph : public QMainWindow
{
	Q_OBJECT

	friend class GraphRenderer;

public:
	Graph(QWidget* parent);

public slots:
	void addPoint(PointItem* ptObj);

signals:
	void updateVisible();

private:
	QList<QPair<PointItem*, bool> > points;

	GraphRenderer* graphRenderer;

	QComboBox* xCombo;
	QComboBox* yCombo;
	QComboBox* zCombo;
	QComboBox* labelCombo;
	QLineEdit* titleBox;

	QPushButton* exportPdfButton;
	QPushButton* printButton;

private slots:
	void xChanged();
	void yChanged();
	void zChanged();
	void labelChanged();
	void titleChanged(const QString& text);
	void exportPdf();
	void print();
	void ptChanged(PointItem*);
	void ptRemoved(PointItem*);
};

} // namespace CT

#endif // !CYCLETIMERGRAPH_H
