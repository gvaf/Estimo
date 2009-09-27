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

#ifndef CYCLETIMERTABLE_H
#define CYCLETIMERTABLE_H

#include <QList>
#include <QMainWindow>

class QPushButton;
class QTableWidget;

namespace CT {

class CycleTimer;
class PointItem;

class Table : public QMainWindow
{
	Q_OBJECT

public:
	Table(QWidget* parent, CycleTimer* ct);

public slots:
	void addPoint(PointItem* ptObj);

private:
	CycleTimer* cycleTimer;
	QList<PointItem*> selection;

	QTableWidget* table;
	QPushButton* renameButton;
	QPushButton* exportConfButton;
	QPushButton* detailsButton;
	QPushButton* deleteButton;
	QPushButton* exportTableButton;
	QPushButton* clearButton;

private slots:
	void rename();
	void exportConf();
	void details();
	void delItems();
	void exportTable();
	void clear();
	void selChanged();
	void ptChanged(PointItem*);
	void ptRemoved(PointItem*);
};

} // namespace CT

#endif // !CYCLETIMERTABLE_H
