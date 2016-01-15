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


//Valid statement is either a termination command || an arithmetical expression followed by a semicolon
stat returns [SPTree tree] : e1=TERMINATION SEMICOLON {$tree = new SPTree($e1.text);}
			   | a=assign  SEMICOLON {$tree = $a.tree;}
			   | e2=exp    SEMICOLON {$tree = $e2.tree;}
			   | g=graph   SEMICOLON {$tree = $g.tree;}
 ; 

exp returns [SPTree tree] : 
			  // Terminal
			   n=(NUMBER|VAR_NAME) {$tree = new SPTree($n.text);}
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
			  ;

function returns [SPTree tree]:
			LEFT_BRACKET e=exp COMMA c1=NUMBER COMMA c2=NUMBER COMMA ch=('$' | '%' | '&' | '*' | '~' | '+' | '#' | '@') RIGHT_BRACKET 
				{$tree = new SPTree("function"); $tree.insertChild($e.tree); $tree.insertChild(new SPTree($c1.text)); 
				$tree.insertChild(new SPTree($c2.text)); $tree.insertChild(new SPTree($ch.text));}
			;

graph returns [SPTree tree] :
			g=GRAPH {$tree = new SPTree("graph");} LEFT_PARENTHESIS f=VAR_NAME COMMA n=NUMBER COMMA 
				{$tree.insertChild(new SPTree($f.text)); $tree.insertChild(new SPTree($n.text));}
				func1=function {$tree.insertChild($func1.tree);} (COMMA func2=function {$tree.insertChild($func2.tree);})*
			;

assign returns [SPTree tree] :
			  v=VAR_NAME c=EQUALS e=exp {$tree = new SPTree($c.text); $tree.insertChild(new SPTree($v.text)); $tree.insertChild($e.tree);}
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

// Parentheses
LEFT_PARENTHESIS: '(';
RIGHT_PARENTHESIS: ')';
LEFT_BRACKET: '[';
RIGHT_BRACKET: ']';

// Operators
PLUS: '+';
MINUS : '-';
MULPTIPLY: '*';
DIVIDE: '/';
SUM_RANGE: '$';
EQUALS: '=';
MIN: 'min';
MAX: 'max';
MEDIAN: 'median';
AVERAGE: 'average';
COMMA: ',';
GRAPH: 'graph';

// Valid variable name
VAR_NAME: 
	  'm' 'i' ~'n'
	| 'm' 'a' ~'x'
	| 'a' 'v' 'e' 'r' 'a' 'g' ~'e'
	| 'm' 'e' 'd' 'i' 'a' ~'n'
	| [a-zA-Z]+;

// Ignore whitespace
WHITESPACE: [ \t\r\n]+ -> skip;