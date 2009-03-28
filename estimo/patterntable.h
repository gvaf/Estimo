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

#ifndef PATTERN_TABLE_H
#define PATTERN_TABLE_H

#include <QHash>
#include <QList>
#include <QString>

class Entity;

/// Patterns
class PatternTable
{
 public:

   /// A single check point
   struct Point
   {
	 /// Default Constructor
     Point() : x(0) , y(0), codeBlock(0), isFullPel(true) {}

	 /// Copy Constructor
	 Point(const Point &p)
	 {
      x = p.x;
	  y = p.y;
	  isFullPel = p.isFullPel;
	  codeBlock = p.codeBlock;
	 }

	 /// Return a string representation
	 std::string toString()
	 { 
      QString res = QString("(%1, %2)").arg(x).arg(y);
		 return res.toStdString();
	 }

	 /// Overload Assignment operator
	 void operator=(const Point &p)
	 {
       x = p.x;
  	   y = p.y;
	   isFullPel = p.isFullPel;
	   codeBlock = p.codeBlock;
	 }

	 /// Constructor
     Point(float _x, float _y, Entity * _codeBlock = 0) : x(_x), y(_y), codeBlock(_codeBlock) {}

	 /// Overload add operator
	 Point operator+(const Point &p)
	 {
	   return Point(x + p.x, y + p.y);
	 }

	 /// Overload equality operator
     bool operator==(const Point &p) const
     { return (x == p.x) && (y == p.y); };

     /// Coordinates
     float x, y; 	 

	 bool isFullPel;

	 /// Block of Code attached to point, if it's the winning point then run this code 
	 Entity * codeBlock;
   };

   /// Return true if the input name is a variable
   bool isDefined(const QString & name);

   /// Returns a reference to the variable's value
   QList<Point> & operator[](const QString & name);

   /// Clear all variables
   void clear();

 private:
   QHash< QString, QList<Point> > memory; ///< Holding a table with all pattern type variables
};

/// The customized hash function
extern uint qHash(const PatternTable::Point &);

#endif
