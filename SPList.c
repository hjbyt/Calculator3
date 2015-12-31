#include "SPList.h"
#include <stdlib.h>

typedef struct Node_t {
	SPListElement data;
	struct Node_t* next;
	struct Node_t* previous;
}*Node;

Node createNode(Node previous, Node next, SPListElement element) {
	SPListElement newElement = copyElement(element);
	if (newElement == NULL ) {
		return NULL ;
	}
	Node newNode = (Node) malloc(sizeof(*newNode));
	if (newNode == NULL ) {
		destroyElement(newElement);
		return NULL ;
	}
	newNode->data = newElement;
	newNode->previous = previous;
	newNode->next = next;
	return newNode;
}

void destroyNode(Node node){
	if(node==NULL){
		return;
	}
	if(node->data!=NULL){
		destroyElement(node->data);
	}
	free(node);
}

struct List_t {
	Node head;
	Node tail;
	Node current;
	int size;
};

SPList listCreate() {
	SPList list = (SPList) malloc(sizeof(*list));
	if (list == NULL ) {
		return NULL ;
	} else {
		list->head = (Node) malloc(sizeof(*list->head));
		if (list->head == NULL ) {
			free(list);
			return NULL ;
		}
		list->tail = (Node) malloc(sizeof(*list->tail));
		if (list->tail == NULL ) {
			free(list->head);
			free(list);
			return NULL ;
		}
		list->head->data = NULL;
		list->head->next = list->tail;
		list->head->previous = NULL;
		list->tail->data = NULL;
		list->tail->next = NULL;
		list->tail->previous = list->head;
		list->current = NULL;
		list->size = 0;
		return list;

	}
}

SPList listCopy(SPList list) {
	if(list==NULL){
		return NULL;
	}
	SPList copyList = listCreate();
	if(copyList == NULL){
		return NULL;
	}
	SPListElement currentElement = listGetFirst(list);
	while(currentElement){
		if(listInsertLast(copyList,currentElement)!=SP_LIST_SUCCESS){
			listDestroy(copyList);
			list->current=NULL;
			return NULL;
		}
		currentElement = listGetNext(list);
	}
	list->current=NULL;
	copyList->current =NULL;
	return copyList;
}

int listGetSize(SPList list) {
	return list == NULL ? -1 : list->size;
}

SPListElement listGetFirst(SPList list) {
	if (list == NULL || listGetSize(list) == 0) {
		return NULL ;
	} else {
		list->current = list->head->next;
		return list->current->data;
	}
}

SPListElement listGetNext(SPList list) {
	if (list == NULL || listGetSize(list) == 0 || list->current == NULL ) {
		return NULL ;
	} else {
		if (list->current->next == list->tail) {
			list->current = NULL;
			return NULL ;
		} else {
			list->current = list->current->next;
			return list->current->data;
		}
	}
}

SPListElement listGetCurrent(SPList list) {
	if (list == NULL || listGetSize(list) == 0 || list->current == NULL ) {
		return NULL ;
	} else {
		return list->current->data;
	}
}

ListResult listInsertFirst(SPList list, SPListElement element) {
	if (list == NULL || element == NULL ) {
		return SP_LIST_NULL_ARGUMENT;
	}
	Node newNode = createNode(list->head, list->head->next, element);
	if (newNode == NULL ) {
		return SP_LIST_OUT_OF_MEMORY;
	}
	list->head->next->previous = newNode;
	list->head->next = newNode;
	list->size++;
	return SP_LIST_SUCCESS;
}

ListResult listInsertLast(SPList list, SPListElement element) {
	if (list == NULL || element == NULL ) {
		return SP_LIST_NULL_ARGUMENT;
	}
	Node newNode = createNode(list->tail->previous, list->tail, element);
	if (newNode == NULL ) {
		return SP_LIST_OUT_OF_MEMORY;
	}
	list->tail->previous->next = newNode;
	list->tail->previous = newNode;
	list->size++;
	return SP_LIST_SUCCESS;
}

ListResult listInsertBeforeCurrent(SPList list, SPListElement element) {
	if (list == NULL || element == NULL ) {
		return SP_LIST_NULL_ARGUMENT;
	}
	if (list->current == NULL ) {
		return SP_LIST_INVALID_CURRENT;
	}
	Node newNode = createNode(list->current->previous, list->current, element);
	if (newNode == NULL ) {
		return SP_LIST_OUT_OF_MEMORY;
	}
	list->current->previous->next = newNode;
	list->current->previous = newNode;
	list->size++;
	return SP_LIST_SUCCESS;
}

ListResult listInsertAfterCurrent(SPList list, SPListElement element) {
	if (list == NULL || element == NULL) {
		return SP_LIST_NULL_ARGUMENT;
	}
	if (list->current == NULL ) {
		return SP_LIST_INVALID_CURRENT;
	} else if (list->current == list->tail->previous) {
		return listInsertLast(list, element);
	} else {
		Node tempCurrent = list->current;
		list->current = list->current->next;
		ListResult res = listInsertBeforeCurrent(list, element);
		list->current = tempCurrent;
		return res;
	}
}

ListResult listRemoveCurrent(SPList list){
	if(list == NULL){
		return SP_LIST_NULL_ARGUMENT;
	}
	if(list->current==NULL){
		return SP_LIST_INVALID_CURRENT;
	}
	list->current->previous->next = list->current->next;
	list->current->next->previous = list->current->previous;
	destroyNode(list->current);
	list->current=NULL;
	list->size--;
	return SP_LIST_SUCCESS;
}

ListResult listClear(SPList list){
	if(list==NULL){
		return SP_LIST_NULL_ARGUMENT;
	}
	while(listGetFirst(list)){
		listRemoveCurrent(list);
	}
	return SP_LIST_SUCCESS;
}

void listDestroy(SPList list){
	if(list==NULL){
		return;
	}
	listClear(list);
	destroyNode(list->head);
	destroyNode(list->tail);
	free(list);
}
