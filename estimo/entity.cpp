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

#include <QHash>
#include <QString>
#include <QList>

#include "entity.h"
#include "assemblydoc.h"
#include "semanticerror.h"
#include "numtable.h"
#include "patterntable.h"
#include "estimodule.h"

//----------------------------------------------------------------------------

/// Keeps all objects for garbage collecting purposes
QList<Entity *> MemoryPool::allEntities;

/// Delete all unused objects
void MemoryPool::garbageCollect()
{
  while (!allEntities.isEmpty())
     delete allEntities.takeFirst();
}

/// Monitor object's life
void MemoryPool::monitor(Entity *item)
{
  allEntities.push_back(item); 
}

/// Stop monitoring object probably deleted manually
void MemoryPool::stop_monitor(Entity *item)
{
  MemoryPool::allEntities.removeAll(item);
}

//----------------------------------------------------------------------------

/// Constructor -- add the new entity to the list
Entity::Entity()
{
  MemoryPool::monitor(this);
}

/// Initialize internal memory
void Entity::init()
{
  MemoryPool::garbageCollect();
  variables.clear();
  patterns.clear();
  doc.init();
  patterns["::pattern::"].clear();
}

/// Initialize Static Members
NumTable     Entity::variables;
PatternTable Entity::patterns;
AssemblyDoc Entity::doc;

bool Entity::stopEx = false;

/// Stop execution if necessary
bool Entity::notStopped() const 
{ 
  if( Entity::stopEx ) throw SemanticError("Execution stopped");  
  return true;
}

/// Stucture with jumps when we have a winning point
struct WinJumps
{
  WinJumps()
  {
    index = 0;
	codeLabel = "";
	wincode = 0;
  }

  WinJumps(int _index, QString _codeLabel, Entity * _wincode)
	  : index(_index), codeLabel(_codeLabel), wincode(_wincode)
  {}

  int index;          ///< Index of the point
  QString codeLabel;  ///< Label where the code starts
  Entity * wincode;   ///< The delegate code
};

/// Call the update command
void Entity::force_update(EvalParam * param)
{
  QString endofwin;          ///< Label end of winning code block
  QVector<WinJumps> jumps;   ///< List with all the winning code blocks

  QList<PatternTable::Point>::const_iterator iter; 
  QList<PatternTable::Point> points;
  QList<PatternTable::Point> currPoints = Entity::patterns["::pattern::"];
  
  /// Ignore useless updates
  if( Entity::patterns["::pattern::"].isEmpty() ) return;
  
  if( param != 0 )
  {
   if( param->type == EvalParam::DELEGATE )
    {
	  if( param->counts >= 2 )
	  {
          points = currPoints;

	  }
	  else
	  {
	  ++(param->counts);

	  QSet<PatternTable::Point> parent = param->parent;

	  for(int i = 0; i < currPoints.size(); ++i)	  
	     if( !parent.contains(currPoints[i] + param->origin ) )	   
	    	   points.push_back(currPoints[i]);


	  for(int i = 0; i < points.size(); ++i)	  
         parent.insert(points[i] + param->origin);	  

     if( points.size() == 0 )
 	 {
      //  cout << "Empty Optimum! " << endl;			 
		Entity::patterns["::pattern::"].clear();
		return;
	 }
	  }
    }
  }
  else
  {
   /// Create parameter object
   param = new EvalParam();   

   /// Set the points of the new pattern
   points = currPoints;

   /*
          cout << "\nNO INPUT:" << endl;

		  for(int i = 0; i < currPoints.size(); ++i)	  
				  cout << "\tCurrent Pattern: " << currPoints[i].toString() << endl;
 	*/

   /// Use the points for sub-patterns
   for(int i = 0; i < points.size(); ++i)	  
     param->parent.insert(points[i]);   
  }

  /// Append the command 'check Pattern' to assembly program
  new CheckCmd(doc, points);

  /// Keep the index of the current winning point
   int index = 0;

   /// Indicate there is at least a winning jump
   bool atLeastWin = false;

   /// For each code Block append assembly command 'check and jump'
   for(iter = points.constBegin();  iter != points.constEnd(); ++iter)
    {
        PatternTable::Point item = *iter;       

		/// if there is winning delegate
		if( item.codeBlock != 0 )
		{
          /// If there is no improvement then set the check-jump to the end of the blocks
          if( index == 0 )
		  {
			endofwin = Entity::doc.newLabel();			
            new CheckJumpCmd( Entity::doc, QString("%1").arg(0), endofwin );
		  }
		  
		  /// Get a new label for the code block
          QString codeLabel = doc.newLabel();
       
		  /// There is at least a winning jump, so append a branch later
		  atLeastWin = true;

		  /// Append the new codeblock
          jumps.push_back( WinJumps(index+1, codeLabel, item.codeBlock) );
    
		  /// Append a new 'check and jump' assembly instruction
          new CheckJumpCmd( Entity::doc, QString("%1").arg(index+1), codeLabel );
		}

		++index;
    }

    /// There is at least a winning jump, so append a branch later
    if( atLeastWin )
       new JumpCmd(Entity::doc, endofwin, true);   ///< b <end of code blocks>		 	

   /// Clear temporary checks buffer
   Entity::patterns["::pattern::"].clear();

   /// If there is at least a codeBlock delegate
   if( endofwin != "" )
   {  
     /// For each code block  emit  Label: block; b endofwin;
     for(int i = 0; i < jumps.size(); ++i)
      {         
		 /// Set block's label
		 Entity::doc.setLabel(jumps[i].codeLabel);
      		    
         /// Clear temporary checks buffer
 		 Entity::patterns["::pattern::"].clear();
		
		 /// Get the origin point
		 const PatternTable::Point & winpoint = points.at( jumps[i].index -1 );

		 /// Create a new instance of parameters for the subpattern
		 EvalParam * parameters = new EvalParam(param);

		 /// Set the new origin of the pattern	  	 
		 parameters->moveOrigin(winpoint);

		 /// Evalutate the code of the block using the parameters
	     jumps[i].wincode->eval(parameters);
		
		 /// delete the temporary patterns
		 delete parameters;

		 /// Emit a jump command to the end of the block
		 new JumpCmd(Entity::doc, endofwin, true);   ///< b <end of code blocks>		 		 	    
       }
    
	  /// Indicate the end of code block
      Entity::doc.setLabel(endofwin);
   }    
}

/// Check if the input is a valid param otherwise create a new parameter
EvalParam * EvalParam::getParam(EvalParam * param)
{
      if( param == 0 )
	    return new EvalParam();
	  else
        return param;
}
