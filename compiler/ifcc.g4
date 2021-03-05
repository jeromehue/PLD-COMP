grammar ifcc;

axiom : prog       
      ;

prog : 'int' 'main' '(' ')' '{' affectation* RETURN CONST ';' '}' ;

affectation : TYPE ID '=' CONST ';';

RETURN : 'return' ;
TYPE : 'int32_t';
CONST : [0-9]+ ;
ID : [a-z]+ ;
COMMENT : '/*' .*? '*/' -> skip ;
DIRECTIVE : '#' .*? '\n' -> skip ;
WS    : [ \t\r\n] -> channel(HIDDEN);
