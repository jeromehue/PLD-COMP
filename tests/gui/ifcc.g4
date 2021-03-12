grammar ifcc;

/*
    C standard available here :
    http://port70.net/~nsz/c/c11/
*/

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

relationalExpression
    : CONST     #re_number
    | ID        #re_var
    | relationalExpression '<' relationalExpression    #RelExpr
    | relationalExpression '>' relationalExpression    #RelExpr
    | relationalExpression '<=' relationalExpression   #RelExpr
    | relationalExpression '>=' relationalExpression   #RelExpr
    ;

equalityExpression 
    : relationalExpression
    | equalityExpression '==' relationalExpression     
    | equalityExpression '!=' relationalExpression 
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
