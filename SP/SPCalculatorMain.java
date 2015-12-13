package SP;
import org.antlr.v4.runtime.*;
import org.antlr.v4.runtime.tree.*;
import java.io.*;
import java.util.*;

public class SPCalculatorMain {
	//Define all flags
	private final static HashSet<String> inputFlag =  new HashSet<String>(Arrays.asList("-i","-I"));
	private final static HashSet<String> outputFlag = new HashSet<String>(Arrays.asList("-o","-O"));
	private final static HashSet<String> errorFlag =  new HashSet<String>(Arrays.asList("-e","-E"));
	//Print a usage message to stderr	
	private static void usage(){
		System.err.println("Usage: -i [filename] -o [filename] -e [filename]");
	}
	//Checks if str is valid flag
	private static boolean isFlag(String str){
		return inputFlag.contains(str) || outputFlag.contains(str) || errorFlag.contains(str);
	}
	//Checks if all options are in a valid form
	private static void checkOptions(String[] args){
		for(int i =0;i<args.length;i++){
			if(isFlag(args[i])){
				if(++i>=args.length || isFlag(args[i])){
					usage();
					System.exit(-1);
				}
			}else{
				usage();
				System.exit(-1);
			}
		}
	}
	//Set stdin/stdout/stderr
	private static void setStandardIOE(String[] args){
		checkOptions(args);
		for(int i =0;i<args.length;i++){
			if(outputFlag.contains(args[i])){
				try{
				    System.setOut( new PrintStream(new FileOutputStream(args[++i])));
				 }catch(Exception e){
				    System.err.println(e.getMessage());
				    System.exit(-1);
				 }
			}else if(inputFlag.contains(args[i])){
				try{
					FileInputStream inputFile = new FileInputStream(args[++i]);
					System.setIn(inputFile);
				}catch(Exception e){
					System.err.println("File " + args[i] + " doesn't exist");
					System.exit(-1);
				}
			}else{ //Stderr
				try{
					System.setErr( new PrintStream(new FileOutputStream(args[++i])));
				}catch(Exception e){
					System.err.println(e.getMessage());
					System.exit(-1);
				}
			}
		}
	}
	//Check if the str matches a termination lisp tree
	private static boolean isTermination(String str){
		if(str.equals("(<>)")){
			return true;
		}else{
			return false;
		}
	}
	public static void main(String[] args) throws Exception {
		//set standard error/input/out based on the flags
		setStandardIOE(args);
		//Start reading from stdin
		BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
		String s;
		while((s = br.readLine())!=null){
			try{
				//Create a buffered string reader for the current line
				StringReader sReader = new StringReader(s);
				ANTLRInputStream input = new ANTLRInputStream(sReader);
				SPCalculatorLexer lexer = new SPCalculatorLexer(input);
				// create a buffer of tokens pulled from the lexer
				CommonTokenStream tokens = new CommonTokenStream(lexer);
				// create a parser that feeds off the tokens buffer
				SPCalculatorParser parser = new SPCalculatorParser(tokens);
				parser.setErrorHandler(new BailErrorStrategy());
				// Start parsing + return the tree in LispStyle -				
				String lispTree = parser.getLisp();
				System.out.println(lispTree); // print LISP-style tree
				if(isTermination(lispTree)){ //If we had termination command we need to terminate the java prog as well.
					break;
				}
			}catch(Exception e){ // Exception May occur during parsing!
				System.err.println("Invalid Expression : " + s);
			}
		}
		
	}
}	
