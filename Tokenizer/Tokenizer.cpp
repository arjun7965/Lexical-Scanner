/////////////////////////////////////////////////////////////////////
// Tokenizer.cpp - read words from a std::stream                   //
// ver 3.2                                                         //
// Language:  C++, Visual Studio 2013                              //
// Application: Parser component, CSE687 - Object Oriented Design  //
// Author:      Arjun Vinod, MS in CE (H/W)                        //
// Source:      Jim Fawcett, Syracuse University,                  //
//              jfawcett@twcny.rr.com                              //
/////////////////////////////////////////////////////////////////////

#include "Tokenizer.h"
#include <iostream>
#include <cctype>
#include <string>
#include <vector>
#include <map>

using namespace std;

namespace Scanner
{
	class ConsumeState
	{
	public:
		ConsumeState();
		ConsumeState(const ConsumeState&) = delete;
		ConsumeState& operator=(const ConsumeState&) = delete;
		virtual ~ConsumeState();
		void attach(std::istream* pIn) { _pIn = pIn; }
		virtual void eatChars() = 0;
		void consumeChars() {
			_pState->eatChars();
			_pState = nextState();
		}
		bool canRead() { return _pIn->good(); }
		string getTok() { return token; }
		bool hasTok() { return token.size() > 0; }
		ConsumeState* nextState();
		map<char, char> SpecialTwoChar;
		vector<char>::iterator it;
		std :: vector<std::string> ConsumeState::SpecialTwoChar = { "<<", ">>", "::", "++", "--", "==", "+=", "-=", "*=", "/=" };


	protected:
		static string ConsumeState::token;
		static istream* ConsumeState::_pIn;
		static int prevChar;
		static int currChar;
		static vector<char> ConsumeState::SpecialOneChar;
		static ConsumeState* _pState;
		static ConsumeState* _pEatCppComment;
		static ConsumeState* _pEatCComment;
		static ConsumeState* _pEatWhitespace;
		static ConsumeState* _pEatPunctuator;
		static ConsumeState* _pEatAlphanum;
		static ConsumeState* _pEatNewline;
		static ConsumeState* _pEatSpecialSingleChar;
		static ConsumeState* _pEatSpecialDoubleChar;
		static ConsumeState* _pEatSingleQuote;
		static ConsumeState* _pEatDoubleQuote;

	};
}
using namespace Scanner; 

vector<char> ConsumeState::SpecialOneChar = { '<','>','[',']','{','}',':','=','+','-','*','(',')','\n' }; //Vector for Special Single Char
string ConsumeState::token;
istream* ConsumeState::_pIn;
int ConsumeState::prevChar;
int ConsumeState::currChar;
ConsumeState* ConsumeState::_pState = nullptr;
ConsumeState* ConsumeState::_pEatCppComment = nullptr;
ConsumeState* ConsumeState::_pEatCComment = nullptr;
ConsumeState* ConsumeState::_pEatWhitespace = nullptr;
ConsumeState* ConsumeState::_pEatPunctuator = nullptr;
ConsumeState* ConsumeState::_pEatAlphanum = nullptr;
ConsumeState* ConsumeState::_pEatSpecialSingleChar = nullptr;
ConsumeState* ConsumeState::_pEatSpecialDoubleChar = nullptr;
ConsumeState* ConsumeState::_pEatSingleQuote = nullptr;
ConsumeState* ConsumeState::_pEatDoubleQuote = nullptr;
ConsumeState* ConsumeState::_pEatNewline;

void testLog(const std::string& msg);

ConsumeState* ConsumeState::nextState()
{
	int chNext = _pIn->peek();
	if (std::isspace(currChar) && currChar != '\n')
	{
		testLog("state: eatWhitespace");
		return _pEatWhitespace;
	}

	if (currChar == '/' && chNext == '/')
	{
		testLog("state: eatCppComment");
		return _pEatCppComment;
	}
	if (currChar == '/' && chNext == '*')
	{
		testLog("state: eatCComment");
		return _pEatCComment;
	}
	if (currChar == '\n')
	{
		testLog("state: eatNewLine");
		return _pEatNewline;
	}
	if (currChar == '\'')
	{
		testLog("state: eatSingleQuote");
		return _pEatSingleQuote;
	}

	if (currChar == '"')
	{
		testLog("state: eatDoubleQuote");
		return _pEatDoubleQuote;
	}

	map<char, char>::iterator it = SpecialTwoChar.find(currChar);
	if (it != SpecialTwoChar.end() && it->second == chNext)
	{
		testLog("state: eatSpecialDoubleChar");
		return _pEatSpecialDoubleChar;
	}



	for (vector<char>::iterator it = SpecialOneChar.begin(); it != SpecialOneChar.end(); it++)
	{
		if (currChar == *it)
		{
			testLog("state: eatSpecialSingleChar");
			return _pEatSpecialSingleChar;
		}
	}

	if (ispunct(currChar))
	{

		testLog("state: eatPunctuator");
		return _pEatPunctuator;
	}

	if (isalnum(currChar))
	{
		testLog("state: eatAlphanum");
		return _pEatAlphanum;
	}


	if (!_pIn->good())
		return _pEatWhitespace;
	throw(std::logic_error("invalid type"));
}

//----------< Class for White Space>-----------------
class EatWhitespace : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating whitespace";
		do {
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (isspace(currChar) && currChar != '\n');
	}
};

//----------< Class for Cpp Comment>-----------------
class EatCppComment : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating C++ comment";
		do {
			token += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (currChar != '\n');
	}
};

//----------< Class for C Comment>-----------------
class EatCComment : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating C comment";
		do {
			token += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (currChar != '*' || _pIn->peek() != '/');
		token += currChar;
		currChar = _pIn->get();
		token += currChar;
		currChar = _pIn->get();

	}
};

//----------< Class for Single quoted strings>-----------------
class EatSingleQuote : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		currChar = _pIn->get();
		//std::cout << "\n  eating Single";

		do {
			if (currChar == '\\')
				currChar = _pIn->get();
			token += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();

		} while (currChar != '\'');
		//currChar = _pIn->get();	
		if (currChar = '\'')
			currChar = _pIn->get();
	}
};

//----------< Class for Double quoted string>-----------------
class EatDoubleQuote : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		currChar = _pIn->get();
		//token = currChar;
		//std::cout << "\n  eating Single";
		do {
			if (currChar == '\\')
				currChar = _pIn->get();
			token += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (currChar != '"');
		//currChar = _pIn->get();
		if (currChar = '"')
			currChar = _pIn->get();
	}
};

//----------< Class for Punctuator>-----------------
class EatPunctuator : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating punctuator";
		do {
			token += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (ispunct(currChar));
	}
};

//----------< Class for Alphanumerical>-----------------
class EatAlphanum : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();

		//std::cout << "\n  eating alphanum";

		do {
			token += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (isalnum(currChar));
	}
};

//----------< Class for Special single characters >-----------------
class EatSpecialSingleChar : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating SpecialSingleChar";

		token += currChar;

		if (!_pIn->good())  // end of stream
			return;
		currChar = _pIn->get();

	}
};

//----------< Class for Special double characters>-----------------
class EatSpecialDoubleChar : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		map<char, char>::iterator it = SpecialTwoChar.find(currChar);

		//std::cout << "\n  eating SpecialDoubleChar";
		
		if (it != SpecialTwoChar.end() && it->second == _pIn->peek())
		{
			token = currChar;
			currChar = _pIn->get();
			token += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		}
	}
};

//----------< Class for New line>-----------------
class EatNewline : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating alphanum";
		token += currChar;
		if (!_pIn->good())  // end of stream
			return;
		currChar = _pIn->get();
	}
};

ConsumeState::ConsumeState() //Constructor
{
	static bool first = true;
	if (first)
	{
		first = false;
		_pEatAlphanum = new EatAlphanum();
		_pEatCComment = new EatCComment();
		_pEatCppComment = new EatCppComment();
		_pEatPunctuator = new EatPunctuator();
		_pEatWhitespace = new EatWhitespace();
		_pEatNewline = new EatNewline();
		_pEatSpecialSingleChar = new EatSpecialSingleChar();
		_pEatSpecialDoubleChar = new EatSpecialDoubleChar();
		_pEatSingleQuote = new EatSingleQuote();
		_pEatDoubleQuote = new EatDoubleQuote();
		_pState = _pEatWhitespace;
	}

	SpecialTwoChar.insert(std::pair<char, char>('<', '<'));  //Insert function for map
	SpecialTwoChar.insert(std::pair<char, char>('>', '>'));
	SpecialTwoChar.insert(std::pair<char, char>(':', ':'));
	SpecialTwoChar.insert(std::pair<char, char>('+', '+'));
	SpecialTwoChar.insert(std::pair<char, char>('-', '-'));
	SpecialTwoChar.insert(std::pair<char, char>('=', '='));
	SpecialTwoChar.insert(std::pair<char, char>('+', '='));
	SpecialTwoChar.insert(std::pair<char, char>('-', '='));
	SpecialTwoChar.insert(std::pair<char, char>('*', '='));
	SpecialTwoChar.insert(std::pair<char, char>('/', '='));

	
}

ConsumeState::~ConsumeState()  //Destructor
{
	static bool first = true;
	if (first)
	{
		first = false;
		delete _pEatAlphanum;
		delete _pEatCComment;
		delete _pEatCppComment;
		delete _pEatPunctuator;
		delete _pEatWhitespace;
		delete _pEatNewline;
		delete _pEatSpecialSingleChar;
		delete _pEatSpecialDoubleChar;
		delete _pEatSingleQuote;
		delete _pEatDoubleQuote;
	}
}

Toker::Toker() : pConsumer(new EatWhitespace()) {}

Toker::~Toker() { delete pConsumer; }

bool Toker::attach(std::istream* pIn)
{
	if (pIn != nullptr && pIn->good())
	{
		pConsumer->attach(pIn);
		return true;
	}
	return false;
}

string Toker::getTok()
{
	while (true)
	{
		pConsumer->consumeChars();
		if (pConsumer->hasTok())
			break;
		if (!pConsumer->canRead())
			return "";
	}
	return pConsumer->getTok();
}

bool Toker::canRead() { return pConsumer->canRead(); }

void testLog(const std::string& msg)
{
#ifdef TEST_LOG
	std::cout << "\n  " << msg;
#endif
}

//----< test stub >--------------------------------------------------

#ifdef TEST_TOKENIZER

#include <fstream>

int main()
{
	//std::string fileSpec = "../Tokenizer/Tokenizer.cpp";
	//std::string fileSpec = "../Tokenizer/Tokenizer.h";
	std::string fileSpec = "./Tokenizer/Test.txt";

	std::ifstream in(fileSpec);
	if (!in.good())
	{
		std::cout << "\n  can't open " << fileSpec << "\n\n";
		return 1;
	}
	Toker toker;
	toker.attach(&in);
	while (in.good())
	{
		string tok = toker.getTok();
		if (tok == "\n")
			tok = "newline";
		std::cout << "\n -- " << tok;
	}
	std::cout << "\n\n";
	return 0;
}
#endif
