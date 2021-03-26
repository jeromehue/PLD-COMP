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
            declaration?
            statement*
            returnInstr    
        '}' 
    ;

// Declaration

declaration     
    : TYPE initDeclaratorList ';'
    ;

initDeclaratorList
    :   initDeclarator (',' initDeclarator)*
    ;

initDeclarator  
    : ID 
    | ID '=' arithExpr 
    ;

returnInstr
    :   RETURN primaryExpression ';'
    ;

primaryExpression
    : CONST # number
    | ID    # var
    ;


relationalExpression
    : left=primaryExpression relOp=('<'|'>') right=primaryExpression    #RelExpr
    ;

equalityExpression 
    : relationalExpression
    | equalityExpression '==' relationalExpression     
    | equalityExpression '!=' relationalExpression 
    ;    
statement       
    : assignmentExpr ';'
    ;

assignmentExpr  
    : ID '=' arithExpr # assignArithExpr
    | ID '=' relationalExpression # assignRelExpr
    ;


arithExpr    
    : primaryExpression                                 # prExpr
    | '(' primaryExpression ')'                         # prExpr
    | left=arithExpr op=('*'|'/') right=arithExpr       # Expr 
    | left=arithExpr op=('+'|'-') right=arithExpr       # Expr 
    | '(' left=arithExpr op=('*'|'/') right=arithExpr   ')'      # Expr 
    | '(' left=arithExpr op=('+'|'-') right=arithExpr   ')'      # Expr 
    ;
            

RETURN      : 'return' ;
TYPE        : 'int';
CONST       : [-]?[0-9]+ ;
ID          : [_a-zA-Z][_a-zA-Z0-9]* ;
COMMENT     : '/*' .*? '*/' -> skip ;
DIRECTIVE   : '#' .*? '\n' -> skip ;
WS          : [ \t\r\n] -> channel(HIDDEN);
