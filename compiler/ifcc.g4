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
            declaration*
            statement*
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
    :   initDeclarator*
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


// To be used in expressions and return 
primaryExpression
    : CONST
    | ID
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
ID          : [_a-zA-Z][_a-zA-Z0-9]* ;
COMMENT     : '/*' .*? '*/' -> skip ;
DIRECTIVE   : '#' .*? '\n' -> skip ;
WS          : [ \t\r\n] -> channel(HIDDEN);
