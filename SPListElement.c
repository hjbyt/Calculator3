#include "SPListElement.h"
#include <stdlib.h>
#include <string.h>


struct SPListElement_t {
	char* elementStr;
	double* elementValue;
};

char* copyStr(char* str){
	if(str == NULL){
		return NULL;
	}else{
		int sizeOfStr = strlen(str);
		char* tempStr = (char*)calloc((sizeOfStr+1),sizeof(char));
		strcpy(tempStr,str);
		tempStr[sizeOfStr] = '\0';
		return tempStr;
	}
}

double* copyDouble(double value){
	double* newValue = (double*)malloc(sizeof(*newValue));
	if(newValue==NULL){
		return NULL;
	}else{
		*newValue = value;
		return newValue;
	}
}

SPListElement createElement(char* str, double value){
	if(str==NULL){
		return NULL;
	}
	SPListElement temp = (SPListElement) malloc(sizeof(*temp));
	if(temp == NULL){//Allocation Fails
		return NULL;
	}else{
		temp->elementStr = copyStr(str);
		if(temp->elementStr == NULL){//Allocation fails
			free(temp);
			return NULL;
		}else{
			temp->elementValue = copyDouble(value);
			if(temp->elementValue == NULL){
				free(temp->elementStr);
				free(temp);
				return NULL;
			}
			return temp;
		}
	}
}
SPListElement copyElement(SPListElement data){
	if(data==NULL){
		return NULL;
	}
    SPListElement copyElement = (SPListElement) malloc(sizeof(*copyElement));
    if(copyElement==NULL){
    	return NULL;
    }
    char* str = data->elementStr;
    if(str==NULL){
    	return NULL;
    }else{
    	copyElement->elementStr = copyStr(data->elementStr);
    	if(copyElement->elementStr == NULL){
    		free(copyElement);
    		return NULL;
    	}
    	copyElement->elementValue = copyDouble(*data->elementValue);
    	if(copyElement->elementValue == NULL){
    		free(copyElement->elementStr);
    		free(copyElement);
    		return NULL;
    	}
    	return copyElement;
    }
}

void destroyElement(SPListElement data){
	if(data==NULL){
		return;
	}else{
		free(data->elementStr);
		free(data->elementValue);
		free(data);
		return;
	}
}

SPElementResult setElementStr(SPListElement data, char* str){
	if(data == NULL || str == NULL){//str value cannot be NULL
		return SP_ELEMENT_INVALID_ARGUMENT;
	}else{
		char* strCopy = copyStr(str);
		if(strCopy == NULL){//Allocation fails
			return SP_ELEMENT_OUT_OF_MEMORY;
		}else{
			free(data->elementStr);
			data->elementStr=strCopy;
			return SP_ELEMENT_SUCCESS;
		}
	}
}

char* getElementStr(SPListElement data){
	return (data == NULL ? NULL : copyStr(data->elementStr));
}

SPElementResult setELementValue(SPListElement data, double newValue){
	if(data==NULL){
		return SP_ELEMENT_INVALID_ARGUMENT;
	}else{
		double* valueCopy = copyDouble(newValue);
		if(valueCopy==NULL){
			return SP_ELEMENT_OUT_OF_MEMORY;
		}else{
			free(data->elementValue);
			data->elementValue = valueCopy;
			return SP_ELEMENT_SUCCESS;
		}
	}
}

double* getElementValue(SPListElement data){
	return (data==NULL ? NULL : copyDouble(*data->elementValue));
}

bool areElementsEqual(SPListElement data1,SPListElement data2){
	if(data1==NULL || data2==NULL){
		return false;
	}else{
		return (strcmp(data1->elementStr,data2->elementStr) == 0 ) &&
			   (*data1->elementValue == *data2->elementValue);
	}
}

bool isElementStrEquals(SPListElement data1,char* str){
	if(str==NULL || data1==NULL){
		return false;
	}else{
		return strcmp(data1->elementStr,str)==0 ? true : false;
	}
}

bool isElementValueEquals(SPListElement data1,double value){
	if(data1==NULL){
		return false;
	}else{
		return *data1->elementValue == value ? true : false;
	}
}