#pragma once
#ifndef ITOKCOLLECTION_H
#define ITOKCOLLECTION_H
/////////////////////////////////////////////////////////////////////
//  ITokCollection.h - package for the ITokCollection interface    //
//  ver 1.0                                                        //
//  Language:      Visual C++ 2015                                 //
//  Platform:      Lenovo Yoga 2 Pro, Windows 10                   //
//  Application:   Prototype for CSE687 Pr1, Sp16                  //
//  Author:        Arjun Vinod, MS in CE (H/w)                     //
//  Source:        Jim Fawcett, CST 4-187, Syracuse University     //
//                 jfawcett@twcny.rr.com                           //
/////////////////////////////////////////////////////////////////////
/*
Module Purpose:
===============
ITokCollection is an interface that supports substitution of different
types of scanners for parsing.  In this solution, we illustrate that
by binding two different types of collections, SemiExp and XmlParts,
to this interface.  This is illustrated in the test stubs for the
SemiExpression and XmlElementParts modules.

Maintenance History:
====================
ver 1.0 : 11 Feb 16
- first release
*/
#include<string>

struct ITokCollection
{
	virtual bool get(bool clear = true) = 0;
	virtual size_t length() = 0;
	virtual std::string& operator[](int n) = 0;
	virtual size_t find(const std::string& tok) = 0;
	virtual void push_back(const std::string& tok) = 0;
	virtual bool merge(const std::string& firstTok, const std::string& secondTok) = 0; //To add an additional token if neded
	virtual bool remove(const std::string& tok) = 0;
	virtual bool remove(size_t i) = 0;
	virtual void toLower() = 0; //
	virtual void trimFront() = 0;
	virtual void clear() = 0;
	virtual std::string show(bool showNewLines = false) = 0;
	virtual ~ITokCollection() {};
};

#endif