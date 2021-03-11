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
    | ID '=' arithExpr
    ;

statement       
    : assignmentExpr ';' 
    ;

assignmentExpr  
    : ID '=' arithExpr
    ;
                 

arithExpr    
    : CONST     # number
    | ID        # var
    | left=arithExpr op=('*'|'/') right=arithExpr     # Expr 
    | left=arithExpr op=('+'|'-') right=arithExpr     # Expr 
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
