#include <iostream>
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

#include <QList>
#include <QSet>
#include <QtGui>

#include "cycletimer.h"
#include "cycletimerpoint.h"
#include "cycletimertable.h"

//////////////////////
// helper functions //
//////////////////////

namespace {

QPushButton*
createButton(const QString& text,
	     QObject* receiver, const char* member)
{
	QPushButton* button = new QPushButton(text);
	QObject::connect(button, SIGNAL(clicked()), receiver, member);
	return button;
}

QString
csvField(const QString& text)
{
	QString ret(text);
	if (ret.isEmpty())
		return ret;
	if (!ret.contains(',')
	    && !ret.contains('\"')
	    && !ret.contains('\n')
	    && !ret.at(0).isSpace()
	    && !ret.at(ret.size() - 1).isSpace()) {

		return ret;
	}
	ret.replace('\"', "\"\"").prepend('\"').append('\"');
	return ret;
}

const CT::Variable tableColumns[] = {
	CT::VarLabel,
	CT::VarPSNR,
	CT::VarBitRate,
	CT::VarFPS
};
const int nTableColumns = sizeof(tableColumns) / sizeof(tableColumns[0]);

QColor lighter(const QColor& orig, qreal factor)
{
	qreal h, s, v;
	orig.getHsvF(&h, &s, &v);
	bool l = (v > 0.5);
	v = l ? v - 0.5 : 0.5 - v;
	v *= factor;
	v = l ? v + 0.5 : 0.5 - v;
	QColor ret = orig;
	ret.setHsvF(h, s, v);
	return ret;
}

} // namespace


/////////////////
// class Table //
/////////////////

CT::Table::Table(QWidget* parent, CycleTimer* ct)
	: QMainWindow(parent),
	  cycleTimer(ct)
{
	table = new QTableWidget(0, nTableColumns);
	QFontMetrics fm = table->fontMetrics();
	int xwidth = fm.width(QLatin1Char('x'));
	table->setSelectionBehavior(QAbstractItemView::SelectRows);
 	QStringList labels;
	QHeaderView* horHeader = table->horizontalHeader();
	for (int i = 0; i < nTableColumns; ++i) {
		labels << varLabel(tableColumns[i], true);
		table->setColumnWidth(i, xwidth * 20);
	}
	table->setMinimumSize(xwidth * 80, 0);
        horHeader->setResizeMode(0, QHeaderView::Interactive);
	horHeader->setStretchLastSection(true);
        table->setHorizontalHeaderLabels(labels);
        table->verticalHeader()->hide();

	connect(table, SIGNAL(itemSelectionChanged()),
		this, SLOT(selChanged()));

	renameButton = createButton(tr("&Rename..."), this, SLOT(rename()));
	exportConfButton = createButton(tr("E&xport configuration..."), this, SLOT(exportConf()));
	detailsButton = createButton(tr("D&etails"), this, SLOT(details()));
	deleteButton = createButton(tr("&Delete"), this, SLOT(delItems()));
	// exportTableButton = createButton(tr("Export &table..."), this, SLOT(exportTable()));
	clearButton = createButton(tr("&Clear"), this, SLOT(clear()));

	QDialogButtonBox* buttonBox = new QDialogButtonBox;
	buttonBox->addButton(renameButton, QDialogButtonBox::ActionRole);
	buttonBox->addButton(exportConfButton, QDialogButtonBox::ActionRole);
	buttonBox->addButton(detailsButton, QDialogButtonBox::ActionRole);
	buttonBox->addButton(deleteButton, QDialogButtonBox::ActionRole);
	// buttonBox->addButton(exportTableButton, QDialogButtonBox::ActionRole);
	buttonBox->addButton(clearButton, QDialogButtonBox::ActionRole);

	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addWidget(table);
	mainLayout->addWidget(buttonBox);

	QWidget* widget = new QWidget();
	widget->setLayout(mainLayout);
	setCentralWidget(widget);

	setWindowTitle(tr("Cycle Accurate Model"));
}

void CT::Table::addPoint(PointItem* ptObj)
{
	// check if already in table
	for (int i = 0; i < table->rowCount(); ++i) {
		QTableWidgetItem* item = table->item(i, 0);
		if (item->data(Qt::UserRole).value<void*>() == ptObj)
			return;
	}

	int row = table->rowCount();
	table->insertRow(row);
	const PointData& point = ptObj->data();
	QVariant v;
	// do not use QObject* for the variant, otherwise it will be set to 0 when object deleted
	v.setValue((void*) ptObj);
	for (int i = 0; i < nTableColumns; ++i) {
		QTableWidgetItem* item = new QTableWidgetItem(point.strVal(tableColumns[i]));
		if (!ptObj->hasData()) {
			QColor color = item->textColor();
			color = lighter(color, 0.0625);
			item->setTextColor(color);
		} else if (!ptObj->plottable()) {
			QColor color = item->textColor();
			color = lighter(color, 0.25);
			item->setTextColor(color);
		}
		item->setData(Qt::UserRole, v);
		item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
		Qt::Alignment al = Qt::AlignRight | Qt::AlignVCenter;
		if (tableColumns[i] == CT::VarLabel)
			al = Qt::AlignLeft | Qt::AlignVCenter;
		item->setTextAlignment(al);
		table->setItem(row, i, item);
	}
	connect(ptObj, SIGNAL(changed(PointItem*)),
		this, SLOT(ptChanged(PointItem*)));
	connect(ptObj, SIGNAL(removed(PointItem*)),
		this, SLOT(ptRemoved(PointItem*)));

	// exportTableButton->setEnabled(true);
	clearButton->setEnabled(true);
}

void CT::Table::rename()
{
	if (selection.size() != 1)
		return;
	PointItem* pt = selection.front();
	bool ok;
	QString old = pt->data().strVal(VarLabel);
	QString text = QInputDialog::getText(this, tr("Rename: ") + old,
					     tr("Label:"), QLineEdit::Normal,
					     old, &ok);
	if (ok && !text.isEmpty())
		pt->rename(text);
}

void CT::Table::exportConf()
{
	if (selection.size() != 1)
		return;
	PointItem* pt = selection.front();
	pt->exportConf(this);
}

void CT::Table::details()
{
	for (int i = 0; i < selection.size(); ++i)
		selection[i]->details(this);
}

void CT::Table::delItems()
{
	QList<PointItem*> toDel(selection);
	for (int i = 0; i < toDel.size(); ++i)
		toDel[i]->remove();
}

void CT::Table::exportTable()
{
	static QString defPath;
	QString fileName = QFileDialog::getSaveFileName
		(this, tr("Save Configuration"), defPath,
		 tr("Comma Seperated Values files (*.csv);;All files (*)"));
	if (fileName.isEmpty())
		return;
	defPath = QFileInfo(fileName).path();

	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		QMessageBox::warning(this, tr("Export Failed"),
				     tr("Could not open file."));
		return;
	}
	QTextStream s(&file);
	Variable exp[] = {
		VarLabel,
		VarProgMem, VarPointMem,
		VarFull, VarQuarter, VarPartitions,
		VarMVCost, VarMVCand,
		VarLUT,
		VarVideoFile,
		VarResX, VarResY,
		VarFrames,
		VarQP,
		VarRefFrames,

		VarBitRate, VarPSNR,
		VarEnergy,
		VarCycles, VarCyclesParallel,
		VarEnergyMB,
		VarCyclesMB, VarCyclesMBParallel,
		VarFPS, VarFPSParallel
	};
	int exp_n = sizeof(exp) / sizeof(exp[0]);
	for (int i = 0; i < exp_n; ++i)
		s << csvField(varLabel(exp[i], true))
		  << (i == exp_n - 1 ? "\n" : ",");
	for (int pi = 0; pi < table->rowCount(); ++pi) {
		QTableWidgetItem* item = table->item(pi, 0);
		PointItem* pt = (PointItem*) (item->data(Qt::UserRole).value<void*>());
		const PointData& p = pt->data();
		for (int i = 0; i < exp_n; ++i)
			s << csvField(p.strVal(exp[i]))
			  << (i == exp_n - 1 ? "\n" : ",");
	}
}

void CT::Table::clear()
{
	QList<PointItem*> l;
	for (int i = table->rowCount(); i-- > 0; ) {
		QTableWidgetItem* item = table->item(i, 0);
		void* p = item->data(Qt::UserRole).value<void*>();
		PointItem* pt = (PointItem*) p;
		l.append(pt);
	}
	for (int i = 0; i < l.size(); ++i)
		l[i]->remove();
}

void CT::Table::selChanged()
{
	QSet<void*> objs;
	selection.clear();
	QList<QTableWidgetItem*> list = table->selectedItems();
	for (int i = 0; i < list.size(); ++i) {
		void* obj = list.at(i)->data(Qt::UserRole).value<void*>();
		if (objs.contains(obj))
			continue;
		objs.insert(obj);
		selection.append((PointItem*) obj);
	}
	int nRows = selection.size();
	renameButton->setEnabled(nRows == 1);
	exportConfButton->setEnabled(nRows == 1);
	detailsButton->setEnabled(nRows > 0);
	deleteButton->setEnabled(nRows > 0);
}

void CT::Table::ptChanged(CT::PointItem* pt)
{
	for (int row = 0; row < table->rowCount(); ++row) {
		QTableWidgetItem* item = table->item(row, 0);
		if (item->data(Qt::UserRole).value<void*>() == pt) {
			const PointData& point = pt->data();
			QVariant v;
			// do not use QObject* for the variant, otherwise it will be set to 0 when object deleted
			v.setValue((void*) pt);
			for (int i = 0; i < nTableColumns; ++i) {
				QTableWidgetItem* item = new QTableWidgetItem(point.strVal(tableColumns[i]));
				if (!pt->hasData()) {
					QColor color = item->textColor();
					color = lighter(color, 0.0625);
					item->setTextColor(color);
				} else if (!pt->plottable()) {
					QColor color = item->textColor();
					color = lighter(color, 0.25);
					item->setTextColor(color);
				}
				item->setData(Qt::UserRole, v);
				item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
				Qt::Alignment al = Qt::AlignRight | Qt::AlignVCenter;
				if (tableColumns[i] == CT::VarLabel)
					al = Qt::AlignLeft | Qt::AlignVCenter;
				item->setTextAlignment(al);
				table->setItem(row, i, item);
			}
			break;
		}
	}
}

void CT::Table::ptRemoved(CT::PointItem* pt)
{
	for (int row = 0; row < table->rowCount(); ++row) {
		QTableWidgetItem* item = table->item(row, 0);
		if (item->data(Qt::UserRole).value<void*>() == pt) {
			table->removeRow(row);
			// exportTableButton->setEnabled(table->rowCount() > 0);
			clearButton->setEnabled(table->rowCount() > 0);
			break;
		}
	}
}
