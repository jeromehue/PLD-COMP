grammar ifcc;

axiom : prog       
      ;

prog : 
    'int' 'main' '(' ')' '{' 
        declaration*
        affectation* 
        expr_affectation* 
        RETURN retval ';' 
    '}' ;

declaration : TYPE ID (',' ID)* ';';

affectation : TYPE ID '=' CONST ';';

expr_affectation : ID '=' expression ';';

expression  : CONST                                             # number
            | ID                                                # var
            | left=expression op=('*'|'/') right=expression     # Expr 
            | left=expression op=('+'|'-') right=expression     # Expr 
            ;
            


retval  : CONST 
        | ID
        ;

RETURN : 'return' ;
TYPE : 'int';
CONST : [0-9]+ ;
ID : [a-z]+ ;
COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);
