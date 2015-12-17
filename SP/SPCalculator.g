/** Grammars always start with a grammar header. This grammar is called
 *  SPCalculator and must match the filename: SPCalculator.g4
 */
grammar SPCalculator;

/**
* All generated files must reside in the package SP alongside our implementation
* of the tree and the main function.
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

//Valid statement is either a termination command || an arithmetical expression
stat returns [SPTree tree] : e1=TERMINATION {$tree = new SPTree($e1.text);}
			   | e2=exp {$tree = $e2.tree;}
                           ; 

exp returns [SPTree tree] : 
			    n=NUMBER {$tree = new SPTree($n.text);}
			  | LEFT_PARENTHESIS e1=exp RIGHT_PARENTHESIS {$tree = $e1.tree;}
			  | PLUS e1=exp {$tree = new SPTree("+"); $tree.insertChild($e1.tree);}
			  | MINUS e1=exp {$tree = new SPTree("-"); $tree.insertChild($e1.tree);}
			  | e1=exp SUM_RANGE e2=exp {$tree = new SPTree("$"); $tree.insertChild($e1.tree); $tree.insertChild($e2.tree);}
			  | e1=exp MULPTIPLY e2=exp {$tree = new SPTree("*"); $tree.insertChild($e1.tree); $tree.insertChild($e2.tree);}
			  | e1=exp DIVIDE    e2=exp {$tree = new SPTree("/"); $tree.insertChild($e1.tree); $tree.insertChild($e2.tree);}
			  | e1=exp PLUS      e2=exp {$tree = new SPTree("+"); $tree.insertChild($e1.tree); $tree.insertChild($e2.tree);}
			  | e1=exp MINUS     e2=exp {$tree = new SPTree("-"); $tree.insertChild($e1.tree); $tree.insertChild($e2.tree);}
			  ;

// parser rules start with lowercase letters, lexer rules with uppercase
TERMINATION: '<>';

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
