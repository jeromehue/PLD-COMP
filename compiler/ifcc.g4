grammar ifcc;

axiom : prog       
      ;

prog : 'int' 'main' '(' ')' '{' affectation* expr_affectation* RETURN value ';' '}' ;

affectation : TYPE ID '=' CONST ';';

expr_affectation : ID '=' expression ';';

expression  : CONST                                             # number
            | left=expression op=('+'|'-'|'/'|'*') right=expression # Expr 
            ;
            


value   : CONST 
        | ID
        ;

RETURN : 'return' ;
TYPE : 'int32_t';
CONST : [0-9]+ ;
ID : [a-z]+ ;
COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);
