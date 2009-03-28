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

#ifndef BINARY_WORD_H
#define BINARY_WORD_H

class QString;

/// Binary Word Class
/*
 Binary Representation

         | 5 | 4 | 3 | 2 | 1 | 0 |
   MSB     <------------------       LSB
*/
class BinaryWord
{
 public:

   typedef char BIT;

   /// Constructor
   BinaryWord(int _size = 0);

   /// Copy Constructor
   BinaryWord(const BinaryWord &other);

   /// Destructor
   ~BinaryWord();

   /// Append two Binary Words
   BinaryWord operator+(const BinaryWord &other) const;

   /// Overload assignment operator
   const BinaryWord & operator=(const BinaryWord &other);

   /// Output for debug purpose
   void output() const;

   /// Return a string 
   QString getString() const;

   /// Reference to position 'index'
   BIT & operator[](int index);

   /// Return a binary representation of the input number  
   static BinaryWord toBinary(float num, int size, bool integral = false);

   /// Return a binary representation of the input number  
   static QString getBits(float num, const int size, bool fullpel);


   /// Return a binary representation of the input number  
   static BinaryWord BinaryWord::toBinary(const QString &number);

   /// Return a rounded number <int>.<frac> frac: 5, 25 75
   static float round(float num);

 private:
    int size;   ///< Size of the binary word
    BIT *bits;  ///< Array holding the actual bits
};


#endif
