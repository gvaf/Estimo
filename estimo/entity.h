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
#ifndef ENTITY_H
#define ENTITY_H

#include <iostream>
using namespace std;

#pragma warning( disable : 4100 )

#include "assemblydoc.h"
class NumTable;
class PatternTable;
class Entity;

enum E_PARTITIONS
{
	E_MODE_16x16 = 0,   
	E_MODE_8x16  = 1,
	E_MODE_16x8  = 2, 
	E_MODE_8x8   = 3   
};

//---------------------------------------------------------------------------------

/// Holding all allocated objects
class MemoryPool
{
  public: 
    /// Constructor
    MemoryPool() {}

    /// Destructor
    ~MemoryPool() {MemoryPool::garbageCollect();}

    /// Monitor object's life
    static void monitor(Entity *item);

    /// Stop monitoring object probably delete manually
    static void stop_monitor(Entity *item);

    /// Deallocate all objects
    static void garbageCollect();

   /// List with all objects
    static QList<Entity *> allEntities;
};

//---------------------------------------------------------------------------------

/// Generic Parameters Class used when calling the eval() at tree
class EvalParam
{
  public:

	  enum TYPES
	  {
       NORMAL,
	   DELEGATE
	  };

	  /// Default Constructor
	  EvalParam() { type = DELEGATE; inLoop = false; counts = 1;}

	  EvalParam(const EvalParam *p)
	  {
         if( p != 0 )
		 {
		   type = p->type;
		   parent = p->parent;
		   origin = p->origin;
		   counts = p->counts;
		 }
	  }

     void moveOrigin(PatternTable::Point point)
	 {
		 origin = origin + point;
	 }

	static EvalParam * getParam(EvalParam * param = 0);

	TYPES type;
    QSet<PatternTable::Point> parent;
	PatternTable::Point origin;

 public:
    int counts;
    bool inLoop;
	QString endofLoop;

	void insideLoop(QString mylabel)
	{ endofLoop = mylabel; inLoop = true; }

 public:
	 QString label;
};

//---------------------------------------------------------------------------------

/// A code entity inherited by all code entities
class Entity
{
 public:

  /// Constructor
  Entity();

  /// Destructor
  virtual ~Entity() { MemoryPool::stop_monitor(this); }

  /// Virtual Evaluate 'program entity'
  virtual float eval(EvalParam *param) const { return 0;}

  /// Stop execution if necessary
  bool notStopped() const;
 
  virtual QString className() { return "Entity"; };

 public:
 

  static NumTable     variables;       ///< Holding the numerical variables
  static PatternTable patterns;        ///< Holding the pattern variables
  static AssemblyDoc doc;              ///< Assembly doc
  static void init();                            ///< Initialize internal structures
  static void force_update(EvalParam * param);   ///< call and update command
  static bool stopEx;                  ///< Stop Execution flag
};

#endif
