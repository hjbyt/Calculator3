/** Grammars always start with a grammar header. This grammar is called
 *  SPCalculator and must match the filename: SPCalculator.g4
 */
grammar SPCalculator;
/**
* Our parse code will reside in the package SP alongside our implementation of a tree.
*/
@header{
	package SP;
}

@parser::members{
	//We define a new method in our praser. This method will return a string
	//Representing our tree in LISP-Style
	public String getLisp(){
		StatContext ctx = (StatContext)stat();
		if(ctx.exception!=null){
			return new String("Invalid Expression!");		
		}else{
			return ctx.tree.getLisp();
		}
	}
}

//Valid statement is either a termination command || an arithmetical expression followed by a semicolon
stat returns [SPTree tree] : e1=TERMINATION SEMICOLON {$tree = new SPTree($e1.text);}
			   | e2=exp SEMICOLON {$tree = $e2.tree;}
 ; 

exp returns [SPTree tree] : 
			  // Number
			   n=NUMBER {$tree = new SPTree($n.text);}
			  // Sub-expression
			  | LEFT_PARENTHESIS e1=exp RIGHT_PARENTHESIS {$tree = $e1.tree;}
			  // Unary operation
			  | c=(PLUS|MINUS) e1=exp               {$tree = new SPTree($c.text); $tree.insertChild($e1.tree);}
			  // Binary operation
			  | e1=exp c=SUM_RANGE 			 e2=exp {$tree = new SPTree($c.text); $tree.insertChild($e1.tree); $tree.insertChild($e2.tree);}
			  | e1=exp c=(MULPTIPLY|DIVIDE)  e2=exp {$tree = new SPTree($c.text); $tree.insertChild($e1.tree); $tree.insertChild($e2.tree);}
			  | e1=exp c=(PLUS|MINUS)        e2=exp {$tree = new SPTree($c.text); $tree.insertChild($e1.tree); $tree.insertChild($e2.tree);}
			  ;

// parser rules start with lowercase letters, lexer rules with uppercase
TERMINATION: '<>';
SEMICOLON: ';';

// Numbers
NUMBER: [0-9]+;

// Parentheses
LEFT_PARENTHESIS: '(';
RIGHT_PARENTHESIS: ')';

// Operators
PLUS: '+';
MINUS : '-';
MULPTIPLY: '*';
DIVIDE: '/';
SUM_RANGE: '$';

// Ignore whitespace
WHITESPACE: [ \t\r\n]+ -> skip;
