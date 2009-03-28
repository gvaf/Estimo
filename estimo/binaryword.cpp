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
 *  included in all copies or substantial portions of the Software.                    
 **************************************************************************
 */

#include "binaryword.h"
#include <math.h>
#include <QString>
#include <vector>
#include <stdio.h>
#include <iostream>
using namespace std;

#define ONE 1
#define ZERO 0

/// Constructor
BinaryWord::BinaryWord(int _size) : size(_size)
{
 bits = 0;

 if(size != 0 )
  {
    bits = new BinaryWord::BIT[size];

    for(int i = 0; i < size; ++i)
         bits[i] = false;
  }
}

//------------------------------------------------------

/// Destructor
BinaryWord::~BinaryWord()
{  if(size != 0) delete bits; }


//------------------------------------------------------

/// Append two Binary Words
/**
    S11 S10 S9 S8 S7 S6   S5 S4 S3 S2 S1 S0
    X5 X4 X3 X2 X1 X0 +   Y5 Y4 Y3 Y2 Y1 Y0
        bits[]                 other[]
*/
BinaryWord BinaryWord::operator+(const BinaryWord &other) const
{
 BinaryWord sum(this->size + other.size);
 int i;

 for(i = 0; i < other.size ; ++i)
  sum[i] = other.bits[i];

 for(int j = 0; j < size; ++j)
   sum[i+j] = bits[j];

 return sum;
}

//------------------------------------------------------

/// Overload assignment operator
const BinaryWord & BinaryWord::operator=(const BinaryWord &other)
{
 if( &other == this ) return *this;

 if(size != 0) delete bits;

 size = other.size;

 bits = new BIT[size];
 
 for(int i = 0; i < size; ++i)
   bits[i] = other.bits[i];

 return *this;
}

//------------------------------------------------------

/// Copy Constructor
BinaryWord::BinaryWord(const BinaryWord &other) : size(0)
{
 if( &other == this ) return;

 size = other.size;

 bits = new BIT[size];

 for(int i = 0; i < size; ++i)
   bits[i] = other.bits[i];
}

//------------------------------------------------------

/// Reference to position 'index'
BinaryWord::BIT & BinaryWord::operator[](int index) 
{
  if( index < size && index >= 0 )
    return bits[index]; 
  else 
  {
    throw "Error: Out of bound index";
  }
}

//------------------------------------------------------

/// Return a binary representation of the string number
BinaryWord BinaryWord::toBinary(const QString &number)
{
 int numDigits = number.size();

 BinaryWord n(numDigits);

 for(int i = 0; i < numDigits; ++i)
	 if(number[numDigits - 1 - i] == '0')
		 n.bits[i] = 0;
	 else
         n.bits[i] = 1;

  return n;
}

 QString BinaryWord::getBits(float num, const int size, bool fullpel)
{
  vector<bool> b(size);
  bool isNeg = false;

  for(int i = 0; i < size; ++i)
    b[i] = false;

  int n = static_cast<int>(num);

    if( num < 0 )
    {
        isNeg = true;
        n = -n;
    }

    if( !fullpel )
    {
        bool first, second;
        float frac = fabs(num) - n;

              if( frac == 0.00) { first = 0; second = 0; }
         else if( frac == 0.25) { first = 0; second = 1; }
         else if( frac == 0.50) { first = 1; second = 0; }
         else if( frac == 0.75) { first = 1; second = 1; }

         b[size-1] = second;
         b[size-2] = first;
    }

    int nd = size - (fullpel ? 0 : 2);
    do
    {
      b[--nd] = (n % 2 ? 1 : 0);
      n >>= 1;
    } while( n != 0 );

    if( isNeg )
    {
     int curry = 1;
     for(int i = size - 1; i >= 0; --i )
     {
      int digit = !b[i];
      b[i] = (digit != curry);
      curry  = digit * curry;

     }
    }

	QString result;
    for(int i = 0; i < size; ++i)
		result.push_back( (b[i] ? '1' : '0') );

   return result;
}


/// Return a binary representation of the input number  
BinaryWord BinaryWord::toBinary(float num, int size, bool integral)
{
 BinaryWord n(size);
 float rnum = BinaryWord::round(num);
 float frac = fabs( rnum - (int)rnum );
 
 if( frac == 0.0 )
  { n[1] = ZERO; n[0] = ZERO; }
 else if( frac == 0.25 )
  { n[1] = ZERO; n[0] = ONE; }
 else if( frac == 0.5 )
  { n[1] = ONE; n[0] = ZERO; }
 else if( frac == 0.75 )
  { n[1] = ONE; n[0] = ONE; }

 unsigned int unum;
 bool nbin[32] = {0};
 int j = 0;

 unum = (unsigned int)fabs(rnum);

  do
    nbin[j++] = unum & 1;
  while( unum >>= 1 );

  for(int i = 0; i < size - (integral ? 0 : 2); ++i)
    n[i + (integral ? 0 : 2)] = nbin[i];

	/// Complement 2
  if(rnum < 0.0 )
   {
	   for(int i = 0; i < size; ++i)
        n[i] = !n[i];

    BinaryWord::BIT curry = 1;
    for(int i = 0; i < size; ++i)
       {
        BinaryWord::BIT digit = n[i];
        n[i]  =  (digit != curry);
        curry =   digit * curry;
       }
   }
  return n;
}

//------------------------------------------------------

/// Return a string 
QString BinaryWord::getString() const
{
 QString num;

  for(int i = size - 1; i >= 0; --i)
       num.push_back ( (bits[i] ? '1' : '0') );

 return num;
}

//------------------------------------------------------


/// Output for debug purpose
void BinaryWord::output() const
{ 
     cout << "SIZE = " << size << endl;

     for(int i = size - 1; i >= 0; --i)
       cout  << (bits[i] ? '1' : '0');

    cout << endl;
} 
 

//------------------------------------------------------

/// Return a rounded number <int>.<frac> frac: 5, 25 75
float BinaryWord::round(float num)
{
 float frac = num - (int)num;
 float pfrac = fabs(frac);

 if( 0.0 <= pfrac && pfrac <= 0.125 )
  pfrac = 0.0;
 else if( 0.125 < pfrac && pfrac <= 0.375 )
  pfrac = 0.25; 
 else if( 0.375 < pfrac && pfrac <= 0.625)
  pfrac = 0.5;
 else if( 0.625 < pfrac && pfrac < 0.875)
  pfrac = 0.75;
 else if( 0.875 <= pfrac && pfrac < 1)
  pfrac = 1;

 return (int)num + pfrac;
}
