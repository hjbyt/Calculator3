all: antlr SPCalculatorMain.class
	

antlr: SPCalculator.g
	java org.antlr.v4.Tool SPCalculator.g 

SPCalculatorMain.class : SPCalculatorParser.java SPCalculatorListener.java SPCalculatorLexer.java SPTree.java SPCalculatorBaseListener.java SPCalculatorLexer.tokens SPCalculator.tokens SPCalculatorMain.java
	javac *.java
clean:
	rm -f SPCalculatorParser.class  SPCalculatorParser.java SPCalculatorListener.class SPCalculatorListener.java \
SPCalculatorLexer.class  SPCalculatorLexer.java SPTree.class SPCalculatorBaseListener.class  SPCalculatorBaseListener.java SPCalculatorMain.class \
SPCalculatorLexer.tokens SPCalculator.tokens
