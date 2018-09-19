#ifndef LEXER_PARSER_HPP
# define LEXER_PARSER_HPP

#include "expertSystem.hpp"

class ExpertSystem;
class Rule;

class LexerParser
{
	typedef	std::vector<std::pair<std::string, t_lexem>> t_vector;

    typedef void (LexerParser::*addLexemFunction)(t_vector::iterator, std::vector<void *>);
    typedef struct              s_ash_lexem
    {
        std::string             value;
        t_lexem                 index;
        std::string             debug_value;
        addLexemFunction        f;
    }                           t_ash_lexem;

	private:
		t_vector					_lexedFile;
		std::vector<t_ash_lexem>	_lexem;
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
		void			addOBracket(t_vector::iterator, std::vector<void *>);
		void			addNegative(t_vector::iterator,  std::vector<void *>);
		void			addCBracket(t_vector::iterator, std::vector<void *>);
		void			addImplies(t_vector::iterator, std::vector<void *>);
		void			addOperator(t_vector::iterator, std::vector<void *>);
		void			addFacts(t_vector::iterator,  std::vector<void *>);
		void			addQueries(t_vector::iterator,  std::vector<void *>);
		void			addOperand(t_vector::iterator,  std::vector<void *>);

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
