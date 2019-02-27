///////////////////////////////////////////////////////////////////////
// SemiExpression.cpp - collect tokens for analysis                  //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Parser component, CSE687 - Object Oriented Design    //
// Author:      Arjun Vinod, MS in CE (H/w)                          //
// Source:      Jim Fawcett, Syracuse University,                    //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <exception>
#include "SemiExp.h"
#include "../Tokenizer/Tokenizer.h"

using namespace Scanner;

SemiExp::SemiExp(Toker* pToker) : _pToker(pToker) {}

bool SemiExp::get()
{
	if (_pToker == nullptr)
		throw(std::logic_error("no Toker reference"));
	_tokens.clear();
	while (true)
	{
		std::string token = _pToker->getTok();
		if (token == "")
			break;
		_tokens.push_back(token);

		if ((token == "public" || token == "private" || token == "protected") && _pToker->getTok() == ":")
		{
			
			 token = ":";
		    _tokens.push_back(token);
			 return true;
		}

		if (token == "#")     //For making header file as a single Semi Expression
		{
			while (token != "\n")
			{
				token=_pToker->getTok();
				_tokens.push_back(token);
				if (token == "\n")
					return true;
			}
		}

		if (token == "for")     //For making "for" statement as a single Semi Expression
		{
			while (token != ")")
			{
				token=_pToker->getTok();
				_tokens.push_back(token);
	
			}
		}

		if (token == "{")       //Terminating conditions
			return true;
		if (token == "}")
			return true;
		if (token == ";")
			return true;
	}
		return false;
	
}

Token SemiExp::operator[](size_t n)
{
	if (n < 0 || n >= _tokens.size())
		throw(std::invalid_argument("index out of range"));
	return _tokens[n];
}

size_t SemiExp::length()
{
	return _tokens.size();
}

void SemiExp::show()
{
	std::cout << "\n  ";
	for (auto token : _tokens)
		if (token != "\n")
			std::cout << token << " ";
	std::cout << "\n";
}

int main()
{
	Toker toker;
	//std::string fileSpec = "../Tokenizer/Tokenizer.cpp";
	std::string fileSpec = "./SemiExp/Text.txt";
	std::fstream in(fileSpec);
	if (!in.good())
	{
		std::cout << "\n  can't open file " << fileSpec << "\n\n";
		return 1;
	}
	toker.attach(&in);

	SemiExp semi(&toker);
	while (semi.get())
	{
		std::cout << "\n  -- semiExpression --";
		semi.show();
	}
	/*
	May have collected tokens, but reached end of stream
	before finding SemiExp terminator.
	*/
	if (semi.length() > 0)
	{
		std::cout << "\n  -- semiExpression --";
		semi.show();
		std::cout << "\n\n";
	}
	return 0;
}