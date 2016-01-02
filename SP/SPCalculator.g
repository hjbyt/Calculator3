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
	
	public void AddAll(SPTree root, List<SPTree> children) {
		for (int i = 0; i < children.size(); i++) {
			root.insertChild(children.get(i));
		}
	}
}

//Valid statement is either a termination command || an arithmetical expression
stat returns [SPTree tree] : e1=TERMINATION {$tree = new SPTree($e1.text);}
			   | e2=exp {$tree = $e2.tree;}
                           ; 

exp returns [SPTree tree] : 
			  // Number
			   n=NUMBER {$tree = new SPTree($n.text);}
			  // Sub-expression
			  | LEFT_PARENTHESIS e1=exp RIGHT_PARENTHESIS {$tree = $e1.tree;}
			  // Min\Max operations
			  | c=(MIN|MAX) e=expList {$tree = new SPTree($c.text); AddAll($tree, $e.children); }
			  // Unary operation
			  | c=(PLUS|MINUS) e1=exp               {$tree = new SPTree($c.text); $tree.insertChild($e1.tree);}
			  // Binary operation
			  | e1=exp c=SUM_RANGE 			 e2=exp {$tree = new SPTree($c.text); $tree.insertChild($e1.tree); $tree.insertChild($e2.tree);}
			  | e1=exp c=(MULPTIPLY|DIVIDE)  e2=exp {$tree = new SPTree($c.text); $tree.insertChild($e1.tree); $tree.insertChild($e2.tree);}
			  | e1=exp c=(PLUS|MINUS)        e2=exp {$tree = new SPTree($c.text); $tree.insertChild($e1.tree); $tree.insertChild($e2.tree);}
			  ;
			  
expList returns [ ArrayList<SPTree> children ]
    @init
    {
    	$children = new ArrayList<SPTree>();
    } :
    LEFT_PARENTHESIS e1=exp {$children.add(new SPTree($e1.text));} (COMMA e2=exp {$children.add(new SPTree($e2.text));})* RIGHT_PARENTHESIS
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
MIN: 'min';
MAX: 'max';
COMMA: ',';

// Ignore whitespace
WHITESPACE: [ \t\r\n]+ -> skip;
