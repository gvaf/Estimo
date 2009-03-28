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
 *  included in all copies or substantial portions of the Software
 **************************************************************************
 */

#include <QByteArray>
#include <QHash>
#include <QList>
#include <QString>

#include "patterntable.h"

/// Return true if the input name is a variable
bool PatternTable::isDefined(const QString & name)
    { return memory.contains(name); }

/// Returns a reference to the variable's value
QList<PatternTable::Point> & PatternTable::operator[](const QString & name)
   { return memory[name]; }

/// Clear all variables
void PatternTable::clear()
  { memory.clear(); }


/// Provides a hash function for the point objects
uint qHash(const PatternTable::Point &p)
{
 QByteArray x;
 QByteArray y;
 
  x.setNum(p.x);
  y.setNum(p.y);
  x.append(y);

  return qHash(x);
}
