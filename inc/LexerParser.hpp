#ifndef LEXER_PARSER_HPP
# define LEXER_PARSER_HPP

#include "expertSystem.hpp"

class ExpertSystem;
class Rule;

class LexerParser
{
	typedef	std::vector<std::pair<std::string, t_lexem>> t_vector;

	private:
		t_vector					_lexedFile;
		std::vector<std::string>	_lexem;
		std::string 				_error;
		std::string 				_factsAndQueriesError;
		bool						_facts;
		bool						_queries;
		int							_brackets;
		std::vector<std::string>	_exception;

	public:
		LexerParser();
		LexerParser(LexerParser const &);
		~LexerParser();

		LexerParser		&operator=(LexerParser const &lex);
		t_vector		const &getLexedFile() const;

		void 			addExceptionMessage(std::string);
		bool 			exceptionEmpty(void);

		void			clear(void);
		void			printLexedFile();

		void			Lexer(char const *fileName);

		t_lexem			findNextLexem(t_vector::iterator i);
		ExpertSystem	Parser();

		void			addOBracket(t_vector::iterator, std::vector<Token> &, t_lexem);
		void			addNegative(t_lexem);
		void			addCBracket(t_vector::iterator, std::vector<Token> &, t_lexem, Rule &, ExpertSystem &);
		void			addImplies(std::vector<Token> &, t_lexem, Rule &, int);
		void			addOperator(t_vector::iterator, std::vector<Token> &, t_lexem);
		void			addFacts(t_lexem);
		void			addQueries(t_lexem);
		void			addOperand(t_vector::iterator, std::vector<Token> &, t_lexem, Rule &, ExpertSystem &, int, t_lexem);

		class	InvalidLineException : public std::exception
		{
			private:
				std::vector<std::string>	_messages;

			public:
				InvalidLineException(std::vector<std::string> erors);
				InvalidLineException(InvalidLineException const &);
				virtual ~InvalidLineException() throw();
				virtual const char *what() const throw();
		
			private:
			InvalidLineException &operator=(InvalidLineException const &);
		};


};

#endif
