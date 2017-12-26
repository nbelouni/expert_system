#ifndef LEXER_PARSER_HPP
# define LEXER_PARSER_HPP

#include "expertSystem.hpp"

class LexerParser
{
	typedef	std::vector<std::pair<std::string, t_lexem>> t_vector;

	private:
		t_vector					_lexedFile;
		std::vector<std::string>	_lexem;

	public:
		LexerParser();
		LexerParser(LexerParser const &);
		~LexerParser();

		LexerParser		&operator=(LexerParser const &lex);
		t_vector		const &getLexedFile() const;

		void			clear(void);
		void			printLexedFile();

		void			Lexer(char const *fileName);
//		ExpertSystem	Parser();

		class	InvalidLineException : public std::exception
		{
			private:
				std::string		_message;

			public:
				InvalidLineException(std::string);
				InvalidLineException(InvalidLineException const &);
				virtual ~InvalidLineException() throw();
				virtual const char *what() const throw();
		
			private:
			InvalidLineException &operator=(InvalidLineException const &);
		};
};

#endif
