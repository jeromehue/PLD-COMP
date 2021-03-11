grammar ifcc;

axiom           
    : prog       
    ;

prog            
    : 
        'int' 'main' '(' ')' '{' 
            blockItem*
            RETURN retval ';' 
        '}' 
    ;

blockItem       
    : declaration
    | statement
    ;

declaration     
    : TYPE initDeclaratorList ';'
    ;

initDeclaratorList
    :   initDeclarator
    |   initDeclaratorList ',' initDeclarator
    ;

initDeclarator  
    : ID 
    | ID '=' CONST 
    ;

statement       
    : assignmentExpr ';' 
    ;

assignmentExpr  : 
    ID '=' expression 
    ;
                 

expression      
    : CONST     # number
    | ID        # var
    | left=expression op=('*'|'/') right=expression     # Expr 
    | left=expression op=('+'|'-') right=expression     # Expr 
    ;
            
retval  
    : CONST 
    | ID
    ;



RETURN      : 'return' ;
TYPE        : 'int';
CONST       : [0-9]+ ;
ID          : [a-z]+ ;
COMMENT     : '/*' .*? '*/' -> skip ;
DIRECTIVE   : '#' .*? '\n' -> skip ;
WS          : [ \t\r\n] -> channel(HIDDEN);
