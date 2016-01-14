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
	
	public void AddAll(SPTree root, List<SPTree> children) {
		for (int i = 0; i < children.size(); i++) {
			root.insertChild(children.get(i));
		}
	}
}

//Valid statement is either a termination command || an arithmetical expression followed by a semicolon || an assignment command
stat returns [SPTree tree] 
			//@init {setTrace(true);}
			: 
			e1=TERMINATION SEMICOLON {$tree = new SPTree($e1.text);}
			| e2=exp SEMICOLON {$tree = $e2.tree;}
			| as=assignment SEMICOLON {$tree = $as.tree;}
 ; 

exp returns [SPTree tree] : 
			  // Number
			   n=NUMBER {$tree = new SPTree($n.text);}
			  // Sub-expression
			  | LEFT_PARENTHESIS e1=exp RIGHT_PARENTHESIS {$tree = $e1.tree;}
			  // List operations
			  | c=(MIN|MAX|MEDIAN|AVERAGE) LEFT_PARENTHESIS e=expList RIGHT_PARENTHESIS {$tree = new SPTree($c.text); AddAll($tree, $e.children); }
			  // Unary operation
			  | c=(PLUS|MINUS) e1=exp               {$tree = new SPTree($c.text); $tree.insertChild($e1.tree);}
			  // Binary operation
			  | e1=exp c=SUM_RANGE 			 e2=exp {$tree = new SPTree($c.text); $tree.insertChild($e1.tree); $tree.insertChild($e2.tree);}
			  | e1=exp c=(MULPTIPLY|DIVIDE)  e2=exp {$tree = new SPTree($c.text); $tree.insertChild($e1.tree); $tree.insertChild($e2.tree);}
			  | e1=exp c=(PLUS|MINUS)        e2=exp {$tree = new SPTree($c.text); $tree.insertChild($e1.tree); $tree.insertChild($e2.tree);}
			  // Variables
			  | v=VARIABLE {$tree = new SPTree($v.text);}
			  ;
			
assignment returns [SPTree tree] :
	v=VARIABLE op=EQUALS e=exp {$tree = new SPTree($op.text); $tree.insertChild(new SPTree($v.text)); $tree.insertChild($e.tree);}
	;
			  
expList returns [ ArrayList<SPTree> children ]
    @init
    {
    	$children = new ArrayList<SPTree>();
    } :
    e1=exp {$children.add($e1.tree);} (COMMA e2=exp {$children.add($e2.tree);})*
    ;

// parser rules start with lowercase letters, lexer rules with uppercase
TERMINATION: '<>';
SEMICOLON: ';';

// Numbers
NUMBER: [0-9]+;

// Variables
VARIABLE: [a-zA-Z]+;

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
MEDIAN: 'median';
AVERAGE: 'average';
COMMA: ',';
EQUALS: '=';

// Ignore whitespace
WHITESPACE: [ \t\r\n]+ -> skip;
