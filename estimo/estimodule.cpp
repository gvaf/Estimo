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

#include "estimodule.h"
#include <iostream>
using namespace std;
#include <math.h>
#include "entity.h"
#include <QHash>
#include <QString>
#include "numtable.h"
#include "patterntable.h"
#include "semanticerror.h"
#include "estimo_yacc.h"
#include "assemblydoc.h"
#include "asmcommand.h"

 /// Return the variable's value
float VarClass::eval(EvalParam *param) const 
{  return Entity::variables[name]; }

/// Evaluate expression -- assign a value to the variable
float AssignClass::eval(EvalParam *param) const
{ 
    Entity::variables[name] = rhnd->eval(param);
    Entity::notStopped();
    return variables[name]; 
}

//------------------------------------------------------------------

/// Evaluate Recursively the expression
float ExprClass::eval(EvalParam *param) const
{
   float lnum, rnum = 0;
 
   Entity::notStopped();

   if( left != 0) lnum = left->eval(param);
   if( right != 0) rnum = right->eval(param);

   switch(opr)
    {
      case '+':
        return lnum + rnum;

      case '-':
        return lnum - rnum;

      case '*':
        return lnum * rnum;

      case '/':
        return lnum / rnum;

	  case '%':
		return (int)lnum % (int)rnum;

      case T_EQ:
        return lnum == rnum; 

      case T_GT:
        return lnum > rnum; 

      case T_LT:
       return lnum < rnum;

      case T_GE:
       return lnum >= rnum;

      case T_LE:
       return lnum <= rnum;

      case T_NE:
       return lnum != rnum;

      case T_AND:
       return lnum && rnum;

      case T_OR:
       return lnum || rnum;

      case T_NOT:
       return !(lnum);

	  case T_FUN_SQRT:
       return sqrt(lnum);

	   case T_FUN_LOG2:
        return log(lnum) / log(2.0f);

	   case T_FUN_LN:
        return log(lnum);

	   case T_FUN_LOG10:
        return log10(lnum);

	   case T_FUN_POW:
        return pow(lnum, rnum);

	   case T_FUN_PRINT:
		cout << lnum << endl; 
		return 0;
	   
    };

  return 0;
 }

//------------------------------------------------------------------

float PrintClass::eval(EvalParam *param) const
{
    char prev = 0;
    for(unsigned int i = 1; i < text.size()-1; ++i)
    {
      if( prev == '\\' && text[i] == 'n')
        cout << endl;
      else if( prev == '\\' && text[i] == 't')
        cout << "\t";
     else if( prev == '\\' && text[i] == '\\')
       cout << "\\";
     else
     if( text[i] != '\\' )
        cout << text[i];    
        
     prev = text[i];                                                 
    }

	return 0;
}

//------------------------------------------------------------------

 /// Loop while the condition is true and execute body
float WhileClass::eval(EvalParam *param) const 
{
	
 QString L2 = doc.newLabel();   

 try {
   while( cond->eval(param) == 1 && notStopped() )
   {
	  param = EvalParam::getParam(param);
      param->insideLoop(L2);
      body->eval(param);
   }
   
   Entity::doc.setLabel(L2);      ///< L2:  
  }
  catch(RunTimeException &ex)
  {
	  if( ex.getType() == RunTimeException::BREAK_LOOP )
	  {		  
        Entity::doc.setLabel(L2);      ///< L2:  
		return 0;
	  }
	  else
		throw ex;
  };

  return 0; 
}

//------------------------------------------------------------------

/// Loop while the condition is true
float DoWhileClass::eval(EvalParam *param) const 
{ 
  QString L2 = doc.newLabel();   
	
  try 
  {
    do
	{
	  param = EvalParam::getParam(param);
      param->insideLoop(L2);
      body->eval(param);
	}
    while( cond->eval(param) == 1 && Entity::notStopped() );
	
    Entity::doc.setLabel(L2);      ///< L2:  
  } 
  catch(RunTimeException &ex)
  {
	  if( ex.getType() == RunTimeException::BREAK_LOOP )
	  {
        Entity::doc.setLabel(L2);      ///< L2:  
		return 0;
	  }
	  else
		throw ex;
  };

   return 0; 
}

//------------------------------------------------------------------

/// for loop -- for(i = 0 to 12 step 1)
float ForClass::eval(EvalParam *param) const 
{
    Entity::variables[var] = start->eval(param);
    float upto = last->eval(param);
    float stp  = step->eval(param);

	QString L2 = doc.newLabel();   
	
	try {

    if( stp > 0 )
     {
      if( Entity::variables[var] > upto ) throw SemanticError("Error: non terminate for loop");

      while( Entity::variables[var] <= upto && Entity::notStopped() )
        {
			param = EvalParam::getParam(param);
            param->insideLoop(L2);
			body->eval(param);
	
            Entity::variables[var] = Entity::variables[var] + stp; 
        }
	  
      Entity::doc.setLabel(L2);      ///< L2:  
     }
    else
      { 
        if( Entity::variables[var] < upto )  throw SemanticError("Error: non terminate for loop");

        while( Entity::variables[var] >= upto && Entity::notStopped() )
         {
		   param = EvalParam::getParam(param);
           param->insideLoop(L2);
           body->eval(param);
           Entity::variables[var] = Entity::variables[var] + stp; 
         }
		
        Entity::doc.setLabel(L2);      ///< L2:  
     }
	}  
	catch(RunTimeException &ex)
    {
	  if( ex.getType() == RunTimeException::BREAK_LOOP )
	  {
        
        Entity::doc.setLabel(L2);      ///< L2:  
		return 0;
	  }
	  else
		throw ex;
  };

    return 0; 
}

//------------------------------------------------------------------

 /// Evaluate if command
float IFClass::eval(EvalParam *param) const 
{ 
   Entity::notStopped();

   if( cond->eval(param) == 1 )
     ifbody->eval(param);
   else if( elsebody != 0 )
     elsebody->eval(param);

   return 0; 
}

//------------------------------------------------------------------

/// Constructor -- Creates a new list appending the new item
CheckListClass::CheckListClass(Entity *elist, Entity *item)
{
   QList<Entity *>::const_iterator i; 

   if( elist != 0)
    {
      CheckListClass *lc = static_cast<CheckListClass *>(elist);
      QList<Entity *> & list = lc->checks;
      for (i = list.constBegin(); i != list.constEnd(); ++i)
         checks.push_back(*i);

      delete elist;  
    }

   checks.push_back(item);
}

//------------------------------------------------------------------

/// Constructor -- Creates a new list appending the new item 
CasesListClass::CasesListClass(Entity *clist, Entity *item, int win)
{
 QList<Entity *>::const_iterator i; 

   if( clist != 0)
    {
      CasesListClass *lc = static_cast<CasesListClass *>(clist);
	  caseBlocks = lc->caseBlocks;
      delete clist;  
    }

    caseBlocks[win] = item;
}


//------------------------------------------------------------------

/// Constructor -- update the Pattern Table with the new pattern
PatternClass::PatternClass(const char *name, Entity * list)
{
    Entity::patterns[name].clear();

    QList<Entity *>::const_iterator i; 

    CheckListClass *lc = static_cast<CheckListClass *>(list);
    QList<Entity *> & points = lc->checks;

    for (i = points.constBegin(); i != points.constEnd(); ++i)
      {
        const ConstCheckClass *item = static_cast<ConstCheckClass *>(*i); 
        PatternTable::Point p;
        p.x = item->getX();
        p.y = item->getY();
        Entity::patterns[name].push_back(p);
      }
}

//------------------------------------------------------------------

/// Evaluate -- first command then second command
float SeqClass::eval(EvalParam *param) const
{
  Entity::notStopped();
  if( first != 0)    first->eval(param);
  if( sec != 0 )     sec->eval(param);
  return 0;
}

//------------------------------------------------------------------

/// Return the operation
float AsmCondition::getOperation()
{
  return oper;
}

//------------------------------------------------------------------

/// Constructor -- deprecated
AsmCmp::AsmCmp(float type, float reg, float num) 
{
	if( num >= AssemblyDoc::CMP_MIN  && num <= AssemblyDoc::CMP_MAX )
	{   
		this->type = type;
		this->reg  = reg;
		this->num  = num;
	}
	else
	{
		throw SemanticError(QString("Error: %1 number is out of range %2 ... %3").arg(num).arg(AssemblyDoc::CMP_MIN).arg(AssemblyDoc::CMP_MAX));
	}
}


//------------------------------------------------------------------

AsmCondition::AsmCondition(float _reg, float _oper, float _num)
  : reg(_reg), oper(_oper), num(_num)
{
	switch((int)reg)
	{
	case T_LENGTH:
		{
			int xy = static_cast<int>(num);

			std::ostringstream s;
			s << setw( 4 ) << setfill( '0' ) << hex << xy;


			if( s.str().length() == 4 )				
			{				
				this->reg  = reg;
				this->num  = num;
			}
			else
				throw SemanticError(QString("Error: LENGTH's compare argument should be two bytes, e.x. 0xaabb."));

		}
		break;

	case T_WINID:

		if( num < 0 || num > 255)
            throw SemanticError(QString("Error: In WINID comparison the number of points should be between [0, 255] ."));

		break;
	case T_COST:
		
		if( num < 0 ) 
		{
			float original_num = num;
			num = 0.0;
			cout << "Warning: In COST comparison " << original_num << " changed to " << num << "." << endl;
		}
		else
		if( num >= 16384 ) 
		{
			float original_num = num;
			num = 16383;
		    cout << "Warning: In COST comparison " << original_num << " changed to " << num << "." << endl;
		}
		

		break;
	default:

		if( num >= AssemblyDoc::CMP_MIN  && num <= AssemblyDoc::CMP_MAX )
		{   			
			this->reg  = reg;
			this->num  = num;
		}
		else
		{
			throw SemanticError(QString("Error: %1 number is out of range %2 ... %3").arg(num).arg(AssemblyDoc::CMP_MIN).arg(AssemblyDoc::CMP_MAX));
		}
	}
}

//------------------------------------------------------------------

/// Returns true if the condition is the operand "not_equal"
bool AsmCondition::isNotEqual()
{ return oper == T_NE; }

//------------------------------------------------------------------

/// compare evaluation #if, #while, #do-while
float AsmCondition::eval(EvalParam *param) const 
{
	if( reg == T_WINID )
	{
		/// We return special instructions in order to handle the missing not equal operator
		if( oper == T_EQ )	   
		{
			QString jumpLabel = param->label;

			new CheckJumpCmd( Entity::doc, QString("%1").arg(num), jumpLabel ); ///<   chkjmp 0,  L12

			return AsmCondition::WINNING_EQUAL;
		} 
		else if( oper == T_NE)
		{
			QString jumpLabel = param->label;

			new CheckJumpCmd( Entity::doc, QString("%1").arg(num), jumpLabel ); ///<   chkjmp 0,  L12

			return AsmCondition::WINNING_NOT_EQUAL; 
		}

	    // if oper is one of > < >= <=

		CmpCmd::REGISTER r;
		CmpCmd::OPERATOR op;
		float other = num;

		r = CmpCmd::WINID;

		// winid < 10  -not-> winid >= 10 -> winid > 9
		if( oper == T_LT ) { op = CmpCmd::GREATER; --other; }

		// winid >= 10 -not-> winid < 10
		if( oper == T_GE ) { op = CmpCmd::LESS; }

		// winid <= 10 -not-> winid > 10 
		if( oper == T_LE ) { op = CmpCmd::GREATER; }

		// winid > 10 -not-> winid <= 10 -> winid < 11
		if( oper == T_GT ) { op = CmpCmd::LESS; ++other; }
	
		new CmpCmd(doc, r, op, other);

		return AsmCondition::REGISTER;
	}
	else if ( reg == T_LENGTH )
	{
		CmpCmd::REGISTER r;
		CmpCmd::OPERATOR op;
		int xy = num, x, y;
		
		std::ostringstream sout;		
		sout << setw( 4 ) << std::setfill( '0' ) << std::hex << xy;
		std::string s = sout.str().substr(0,2) + " " + sout.str().substr(2,4); 		
		
	    std::istringstream sread(s);
		sread >> std::hex >> x >> y;

		r = CmpCmd::LENGTH;

		// sad < 10  -not-> sad >= 10 -> sad > 9
		if( oper == T_LT ) { op = CmpCmd::GREATER; --x; --y; }

		// sad >= 10 -not-> sad < 10
		if( oper == T_GE ) { op = CmpCmd::LESS; }

		// sad <= 10 -not-> sad > 10 
		if( oper == T_LE ) { op = CmpCmd::GREATER; }

		// sad > 10 -not-> sad <= 10 -> sad < 11
		if( oper == T_GT ) { op = CmpCmd::LESS; ++x; ++y; }

		// The not equal operator is emulated using the equal operator
		if( oper == T_EQ || oper == T_NE )  op = CmpCmd::EQUAL;				 

		xy = ( (x  & 0x7f) << 7 ) | (y & 0x7f);

		new CmpCmd(doc, r, op, xy);

		/// We return special instructions in order to handle the missing not equal operator
		if( oper == T_EQ ) return AsmCondition::EQUAL_OPERATOR;
		if( oper == T_NE ) return AsmCondition::NOT_EQUAL_OPERATOR;

		return AsmCondition::REGISTER;
	}
	else if( reg == T_COST )
	{
		CmpCmd::REGISTER r;
		CmpCmd::OPERATOR op;
		float other = num;

		r = CmpCmd::COST;

		// cost < 10  -not-> cost >= 10 -> cost > 9
		if( oper == T_LT ) { op = CmpCmd::GREATER; --other; }

		// cost >= 10 -not-> cost < 10
		if( oper == T_GE ) { op = CmpCmd::LESS; }

		// cost <= 10 -not-> cost > 10 
		if( oper == T_LE ) { op = CmpCmd::GREATER; }

		// cost > 10 -not-> cost <= 10 -> cost < 11
		if( oper == T_GT ) { op = CmpCmd::LESS; ++other; }

		if( oper == T_EQ || oper == T_NE )  op = CmpCmd::EQUAL;
		// disabled the not equal -- if( oper == T_NE )  op = CmpCmd::NOT_EQUAL;		 

		new CmpCmd(doc, r, op, other);

		/// We return special instructions in order to handle the missing not equal operator
		if( oper == T_EQ ) return AsmCondition::EQUAL_OPERATOR;
		if( oper == T_NE ) return AsmCondition::NOT_EQUAL_OPERATOR;

		return AsmCondition::REGISTER;
	}

	return AsmCondition::UNKNOWN_REGISTER;
}

//------------------------------------------------------------------

/// Evaluate if command if (cond) <code1> else <code2>
float IFSharp::eval(EvalParam *param) const 
{ 
   QString L1 = doc.newLabel();
   QString L2 = doc.newLabel();

   param = EvalParam::getParam(param);
   param->label = L1;

   /// Compare Command / ChkJump Command
   float type = cond->eval(param);   ///< cmp sad, num

   if( type == AsmCondition::WINNING_EQUAL )
   {
   /// if(cond) <code1> else <code2>
	if( elsebody != 0 )
	  {        
		elsebody->eval(param);          ///<   <code2>
		Entity::force_update(param);    ///<   call update
		new JumpCmd(doc, L2);           ///< b L2 	    
		Entity::doc.setLabel(L1);       ///< L1:	 
        ifbody->eval(param);           ///<   <winning code>
  	    Entity::force_update(param);   ///<   call update
	    Entity::doc.setLabel(L2);      ///< L2:
	  }
 	else  /// if(cond) <code1>
	{
        new JumpCmd(doc, L2);          ///< b L2 

	    Entity::doc.setLabel(L1);       ///< L1:	 

		ifbody->eval(param);           ///<   <winning code>
		
  	    Entity::force_update(param);   ///<   call update
		
	    Entity::doc.setLabel(L2);      ///< L2:
	}
   }
   else if( type == AsmCondition::WINNING_NOT_EQUAL)
   {
	   /// if(cond) <code1> else <code2>
	   if( elsebody != 0 )
	   {        		   
		   ifbody->eval(param);           ///<   <winning code>
		   Entity::force_update(param);    ///<   call update
		   new JumpCmd(doc, L2);           ///< b L2 	    
		   Entity::doc.setLabel(L1);       ///< L1:	 
		   elsebody->eval(param);          ///<   <code2>
		   Entity::force_update(param);   ///<   call update
		   Entity::doc.setLabel(L2);      ///< L2:
	   }
	   else  /// if(cond) <code1>
	   {
		   ifbody->eval(param);           ///<   <winning code>

		   Entity::force_update(param);   ///<   call update

		   Entity::doc.setLabel(L1);       ///< L1:	
	   }

   }
   else if( type == AsmCondition::REGISTER || type == AsmCondition::NOT_EQUAL_OPERATOR )
   {	   
	   /// if(cond) <code1> else <code2>
	   if( elsebody != 0 )
	   {
		 new JumpCmd(doc, L1, false);   ///< jmp L1	
			 ifbody->eval(param);            ///<   <code1>
			 Entity::force_update(param);    ///<   call update
		 new JumpCmd(doc, L2);          ///< b L2
		 Entity::doc.setLabel(L1);      ///< L1:
			 elsebody->eval(param);          ///<   <code2>
			 Entity::force_update(param);    ///<   call update
		 Entity::doc.setLabel(L2);      ///< L2:
	   }
	   else  /// if(cond) <code1>
	   {
		 QString L1 = doc.newLabel();

		 new JumpCmd(doc, L1, false);   ///< jmp L1	
			 ifbody->eval(param);            ///<   <code1>
			 Entity::force_update(param);    ///<   call update
		 Entity::doc.setLabel(L1);      ///< L1:
	   }
	}
   else if( type == AsmCondition::EQUAL_OPERATOR )
   {
	   /// if(cond) <code1> else <code2>
	   if( elsebody != 0 )
	   {
		   new JumpCmd(doc, L1, false);   ///< jmp L1	
		   elsebody->eval(param);          ///<   <code2>
		   Entity::force_update(param);    ///<   call update
		   new JumpCmd(doc, L2);          ///< b L2
		   Entity::doc.setLabel(L1);      ///< L1:
		   ifbody->eval(param);            ///<   <code1>
		   Entity::force_update(param);    ///<   call update		   		  
		   Entity::doc.setLabel(L2);      ///< L2:
	   }
	   else  /// if(cond) <code1>
	   {
		   QString L1 = doc.newLabel();

		   new JumpCmd(doc, L1, false);    ///< jmp L1	
           new JumpCmd(doc, L2);           ///< b L2
           Entity::doc.setLabel(L1);       ///< L1:

		   ifbody->eval(param);            ///<  <code1>
		   Entity::force_update(param);    ///<  call update

		   Entity::doc.setLabel(L2);       ///< L2:
	   }
   } 

   return 0; 
}

//------------------------------------------------------------------

/// Loop
float WhileSharp::eval(EvalParam *param) const 
{
   QString L1 = doc.newLabel();
   QString L2 = doc.newLabel();

   param = EvalParam::getParam(param);
   param->insideLoop(L2);
   param->label = L2;

   Entity::doc.setLabel(L1);      ///< L1: 
   
   float type = cond->eval(param);                  ///< Compare Command

   if( type == AsmCondition::WINNING_EQUAL )
   {
	   QString L3 = doc.newLabel();
	      	   
	   new JumpCmd(doc, L3, true);    ///<  b L3	

	   Entity::doc.setLabel(L2);      ///< L2: 

	   body->eval(param);                  ///< <code1>
	   Entity::force_update(param);        ///< call update

	   new JumpCmd(doc, L1, true);   ///<  b L1	
	   Entity::doc.setLabel(L3);      ///< L3: 
   }
   else  if( type == AsmCondition::WINNING_NOT_EQUAL)
   {
	   body->eval(param);                  ///< <code1>
	   Entity::force_update(param);        ///< call update

	   new JumpCmd(doc, L1, true);   ///<  b L1	

	   Entity::doc.setLabel(L2);      ///< L2: 
   }
   else if( type == AsmCondition::REGISTER || type == AsmCondition::NOT_EQUAL_OPERATOR )
   {
	   new JumpCmd(Entity::doc, L2, false);   ///< jmp L2	
	   body->eval(param);                  ///< <code1>
	   Entity::force_update(param);        ///< call update
	   new JumpCmd(doc, L1, true);   ///<  b L1	
	   Entity::doc.setLabel(L2);      ///< L2: 
   }
   else if( type == AsmCondition::EQUAL_OPERATOR )
   {
	   QString L3 = doc.newLabel();

	   new JumpCmd(Entity::doc, L2, false);   ///< jmp L2	      	   
       new JumpCmd(doc, L3, true);    ///<  b L3	

	   Entity::doc.setLabel(L2);      ///< L2: 

	   body->eval(param);                  ///< <code1>
	   Entity::force_update(param);        ///< call update

	   new JumpCmd(doc, L1, true);   ///<  b L1	
	   Entity::doc.setLabel(L3);      ///< L3: 
   }
   

   return 0; 
}

//------------------------------------------------------------------

/// Loop while the condition is true
float DoWhileSharp::eval(EvalParam *param) const 
{ 
   QString L1 = doc.newLabel();
   QString L2 = doc.newLabel();

   param = EvalParam::getParam(param);
   param->insideLoop(L2);

   Entity::doc.setLabel(L1);        ///< L1: 
   body->eval(param);                  ///< <code1>
   Entity::force_update(param);        ///< call update

   if( cond->className() == "AsmCondition")
   {
     AsmCondition * c = dynamic_cast<AsmCondition *>(cond);
	 if( c->isNotEqual() )
        param->label = L2;
	 else
        param->label = L1;
   }
   
   float type = cond->eval(param);     ///< Compare Command

   if( type == AsmCondition::WINNING_EQUAL )
   {	   
	   Entity::doc.setLabel(L2);	   
   }
   else  if( type == AsmCondition::WINNING_NOT_EQUAL)
   {
   	 new JumpCmd(doc, L1, true);   ///<  b L1	
     Entity::doc.setLabel(L2);

   } else if( type == AsmCondition::REGISTER || type == AsmCondition::NOT_EQUAL_OPERATOR )
   {
	  new JumpCmd(doc, L2, false);  ///< jmp L2	
	  new JumpCmd(doc, L1, true);   ///<  b L1	
	  Entity::doc.setLabel(L2);     ///< L2: 
   }
   else if( type == AsmCondition::EQUAL_OPERATOR )
   {
      new JumpCmd(doc, L1, false);  ///< jmp L1
	  Entity::doc.setLabel(L2);
   }

   return 0; 
}
 
//------------------------------------------------------------------

/// Emit Set MVC mode
float SetMVCmodeClass::eval(EvalParam *param) const
{
	new MVCModeCmd(doc, (int)mode_mvc->eval(param), (int)mode_refer->eval(param), mode_part  );
	return 0;
}

//------------------------------------------------------------------

/// Emit an assembly command for checking points add the new pattern to point memory
float UpdateClass::eval(EvalParam *param) const 
{
    Entity::force_update(param);

    return 0; 
}


//------------------------------------------------------------------

/// Emit the check command
float AsmChk::eval(EvalParam *param) const
{  
    if( ! Entity::patterns[patternName].isEmpty() )
     {
       if( !label.isEmpty() )
          Entity::doc.setLabel(label);

       new CheckCmd(Entity::doc, Entity::patterns[patternName]);
     }

    return 0;
}


//------------------------------------------------------------------

/// JMP/b command emit assembly
float AsmJmp::eval(EvalParam *param) const
{
   if( !label.isEmpty() )
     Entity::doc.setLabel(label);
   
   if( type == T_JMP )
            new JumpCmd(Entity::doc, target, false);
   else if( type == T_BRUNCH )
            new JumpCmd(Entity::doc, target, true);

   return 0;
}

//------------------------------------------------------------------

/// Compare Command
float AsmCmp::eval(EvalParam *param) const
{
   CmpCmd::REGISTER r;
   CmpCmd::OPERATOR op;

   if( reg == T_LENGTH ) r = CmpCmd::LENGTH;
   if( reg == T_COST ) r = CmpCmd::COST;

   if( type == T_GE ) op = CmpCmd::GREATER;
   if( type == T_LE ) op = CmpCmd::LESS;
   if( type == T_GT ) op = CmpCmd::GREATER;
   if( type == T_LT ) op = CmpCmd::LESS;
   if( type == T_EQ ) op = CmpCmd::EQUAL;
   if( type == T_NE ) op = CmpCmd::NOT_EQUAL;

   if( !label.isEmpty() )
     Entity::doc.setLabel(label);
 
   new CmpCmd(Entity::doc, r, op, num);

   return 0;
}

//------------------------------------------------------------------

/// Halt command
float AsmHalt::eval(EvalParam *param) const
{
   if( !label.isEmpty() )
     Entity::doc.setLabel(label);
   
    new HaltCmd(Entity::doc);

    return 0;
}


//------------------------------------------------------------------

 /// Evaluate 
float CheckJumpClass::eval(EvalParam *param) const 
{ 
  if( type == CheckJumpClass::E_CASE_CHECK )
  {
	  /// Get a reference to hash of cases
      CasesListClass * caseBlock = static_cast<CasesListClass *>(codeBlock);
      QHash<int, Entity *> & blockTable = caseBlock->caseBlocks;

	  /// Get the name of the patterns
      QList<PatternTable::Point>::const_iterator i; 
	  const QList<PatternTable::Point> & points = Entity::patterns[name];

	  int currPoint = 1;
      for (i = points.constBegin(); i != points.constEnd(); ++i)
		  {
            Entity * winblock = 0;

			if( blockTable.contains(currPoint) )
				winblock = blockTable[currPoint];
			
			PatternTable::Point item = *i;
			Entity::patterns["::pattern::"].push_back(PatternTable::Point(item.x, item.y, winblock) );
			++currPoint;
		 }

		Entity::force_update(param);   

  }
  else
  {
	  if( name.isEmpty() )  // Single Check point
	  {
		  Entity::patterns["::pattern::"].push_back( PatternTable::Point( x->eval(param), y->eval(param), codeBlock ) );	
	  }
	  else  // if it is a named pattern push all the values
	  {
		QList<PatternTable::Point>::const_iterator i; 
		const QList<PatternTable::Point> & points = Entity::patterns[name];

		for (i = points.constBegin(); i != points.constEnd(); ++i)
		  {
			PatternTable::Point item = *i;
			Entity::patterns["::pattern::"].push_back(PatternTable::Point(item.x, item.y) );
		 }

		Entity::force_update(param);   
	  }
  }

  return 0;
}

//------------------------------------------------------------------


float BreakClass::eval(EvalParam *param) const
{
  if( param != 0 )
	  if( param->inLoop )
	  {
        if( isEmbed )
		{
		  new JumpCmd(Entity::doc, param->endofLoop, true);   ///<  b L1
		  return 0;
		}
	  }

  throw RunTimeException(RunTimeException::BREAK_LOOP);
}