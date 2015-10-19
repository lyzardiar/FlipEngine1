#include "Token.h"


Token::Token( void )
{

}

Token::Token( const Token *token )
{

}

Token::~Token( void )
{

}

void Token::Reset() 
{
	_type = TT_UNKOWN;
	_data.Clear();
}

void Token::AppendData( char c )
{
	_data += c;
}
