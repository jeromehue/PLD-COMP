grammar ifcc;

/*
    C standard available here :
    http://port70.net/~nsz/c/c11/
*/

axiom
        :  prog
        ;

procedure
	: 'void' ID '(' parameterlist? ')' '{'
		declaration*
		statement*
        '}'
        ;

parameterlist
	:   TYPE ID (',' TYPE ID)*
	;

function
	:
	TYPE ID '(' parameterlist? ')' '{'
		declaration*
		statement*
		returnInstr
        '}'
        ;

prog
	: (function | procedure)+
	;

declaration
	: TYPE initDeclaratorList ';'
	;

initDeclaratorList
	:   initDeclarator (',' initDeclarator)*
	;

initDeclarator
	: ID
	| ID '=' arithExpr
	| ID '[' INT_CONST ']' '=' arrayInitialisation
	;

arrayInitialisation
        : '{' (INT_CONST (',' INT_CONST)*)? '}'
        ;

returnInstr
	:   RETURN arithExpr ';'
	;

primaryExpression
	: INT_CONST     # number
	| CHAR_CONST    # char
	| ID            # var
	| ID '[' (INT_CONST | ID) ']' #tab 
        ;

relationalExpression
	: left=primaryExpression
          relOp=('<'|'>'|'=='|'!=')
          right=primaryExpression    #RelExpr
        ;

statement
	: assignmentExpr ';'
	| ifStatement
	| whileStatement	
	;

blockStatement
	:statement* 
	;

ifStatement
	:'if' '(' relationalExpression  ')' '{'
		thenBloc = blockStatement
    '}'
	'else' '{'
		elseBloc = blockStatement 
	'}'#ifElse 
	| 'if' '(' relationalExpression  ')' '{'
		thenBloc = blockStatement
    '}'#if 
	;

whileStatement
	: 'while' '(' relationalExpression ')' '{'
		whileBlockStatement = blockStatement
	'}'
	;

assignmentExpr
	: ID '=' arithExpr                      # assignArithExpr
	| ID '=' relationalExpression           # assignRelExpr
        | ID '=' ID '(' primaryExpression? ')'                 # assignFunction
        | ID '=' ID '(' primaryExpression (',' primaryExpression)+ ')'# assignFunction
	;

arithExpr
	: primaryExpression                                         # prExpr
	| '(' primaryExpression ')'                                 # prExpr
	| left=arithExpr op=('*'|'/') right=arithExpr               # Expr
	| left=arithExpr op=('+'|'-') right=arithExpr               # Expr
	| left=arithExpr op=('&'|'^'|'|') right=arithExpr           # Expr
	| '(' left=arithExpr op=('*'|'/') right=arithExpr   ')'     # Expr
	| '(' left=arithExpr op=('+'|'-') right=arithExpr   ')'     # Expr
	| '(' left=arithExpr op=('&'|'^'|'|') right=arithExpr   ')' # Expr
	;

RETURN          : 'return' ;
TYPE            : 'int' | 'char';
INT_CONST       : [-]?[0-9]+;
CHAR_CONST      : '\'' [a-zA-Z0-9] '\'' ;
ID              : [_a-zA-Z][_a-zA-Z0-9]* ;
MULTINE_COMMENT : ('/*' .*? '*/') -> skip ;
INLINE_COMMENT  : '//' ~[\r\n]* [\r\n] -> skip ;
DIRECTIVE       : '#' .*? '\n' -> skip ;
WS              : [ \t\r\n] -> channel(HIDDEN);
