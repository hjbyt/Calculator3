package SP;

import java.util.*;
import java.lang.StringBuffer;

//A simple tree data structure which supports LispStyle conversion method
public class SPTree{
	//Each node has a string value
	private String value;
	//A list of node's children
	private LinkedList<SPTree> childList;
	//Create a single node tree
	public SPTree(String value){
		this.value = value;
		childList = new LinkedList<SPTree>();
	}
	//Insert a subtree as a child
	public void insertChild(SPTree child){
		childList.addLast(child);
	}
	//Get the child at some index
	public SPTree getChildAtIndex(int index){
		return childList.get(index);
	}
	//Get a string representing the tree in LISP Style
	public String getLisp(){
		StringBuffer sBuf = new StringBuffer();
		sBuf.append("(" + value);
		for(SPTree child : childList){
			sBuf.append(child.getLisp());
		}
		sBuf.append(")");
		return sBuf.toString();
	}
}
