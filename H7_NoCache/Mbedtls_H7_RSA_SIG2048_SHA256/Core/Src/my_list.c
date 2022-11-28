/*
*********************************************************************************************************
*                                            	 Notes
* Filename: list.c
* Notes:
* Date: 2019-12
* Author: 崔洪玮
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*                                            INCLUDE FILES
*********************************************************************************************************
*/
#include <stdio.h>
#include "ram_mgmt.h"
#include "my_list.h"
/*
*********************************************************************************************************
*                                             DEFINATIONS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                              FUNCTIONS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             GLOBAL_VARS
*********************************************************************************************************
*/

/*
**********************************************************************************
* Name: CreatList(void *content) 	
* Description:
* Parameter(s):
*	void *content
* Return:
*	NULL if malloc error
*	Pointer to the list head
* Date: 2020-2
**********************************************************************************
*/
node* CreatList(void *content) {
	node *listHead = my_malloc(SRAM12, sizeof(node));
	if (listHead != NULL){
		listHead->content = content;
		listHead->next = NULL;
		return (listHead);
	} else {
		return NULL;
	}
}

/*
**********************************************************************************
* Name: isLast(node *position)
* Description:
* Parameter(s):
*	node *position
* Return:
*	1: the node "position" is the last one of the list
*	0: the node is not the last one
* Date: 2020-2
**********************************************************************************
*/
int isLast(node *position) {
    return (position->next == NULL);
}

/*
**********************************************************************************
* Name: SearchList(void *content, node *list)	
* Description:
* Parameter(s):
*	void *content
*	node *list
*	void *(*fun)(node*)
* Return:
*	if matched, pointer to the node has "content"
*	else, NULL
* Date: 2020-2
**********************************************************************************
*/
node* SearchList(void *content, node *list, void *(*fun)(node*)) {
    node *position = list;
    while (position != NULL && fun(position) != content) {
        position = position->next;
    }
    return (position);
}

/*
**********************************************************************************
* Name: InsertList(void *content, node *list, node *position)	
* Description:
*	Insert a new node after node "position" in list.
* Parameter(s):
*	void *content
*	node *list
*	node *position
* Return:
*	NULL if malloc error
*	Pointer to the inserted new node
* Date: 2020-2
**********************************************************************************
*/
node* InsertList(void *content, node *list, node *position) {
    node *insertNode;

    insertNode = my_malloc(SRAM12, sizeof(struct node));

    if (insertNode == NULL) {
        return NULL;
    } else {
		// insert the new node after the node "position"
        insertNode->content = content;
        insertNode->next = position->next;
        position->next = insertNode;
		return (insertNode);
    }
}

/*
**********************************************************************************
* Name: AppendList(void *content, node *list)	
* Description:
*	append a new node to the list;
*	if params list == NULL, then create a new list with head node 
*	content = param "content"
* Parameter(s):
*	void *content
*	node **list
* Return:
*	NULL if malloc error
*	Pointer to the inserted new node
* Date: 2020-2
**********************************************************************************
*/
node* AppendList(void *content, node *(*list)) {
	if (*list == NULL) {
		*list = CreatList(content);
		if (*list != NULL) {
			return *list;
		} else {
			return NULL;
		}
	} else {
		node *tempNode = *list;
		
		while (!isLast(tempNode)) {
			tempNode = tempNode->next;
		}
		
		return (InsertList(content, *list, tempNode));
	}
}

/*
**********************************************************************************
* Name: findPrevious(void *content, node *list)	
* Description:
* Parameter(s):
*	void *content
*	node *list
* Return:
*	if NULL, the content of the head node is equal to the param "content" 
*	else, the previous node of target node
* Date: 2020-2
**********************************************************************************
*/
node *findPrevious(void *content, node *list) {
    node *position = list;
	if (list->content == content) {
		return NULL;
	}
    while (position->next != NULL && position->next->content != content) {
        position = position->next;
    }
	return (position);
}

/*
**********************************************************************************
* Name: DeleteNode(void *content, node *list)	
* Description:
* Parameter(s):
*	void *content
*	node *list
* Return:
*	NULL, no content matched
*	list, head node addr of the list
* Date: 2020-2
**********************************************************************************
*/
node *DeleteNode(void *content, node *list) {
    node *prevPosition, *deleteNode;

    prevPosition = findPrevious(content, list);
	if (prevPosition == NULL) {
		deleteNode = list;
		list = list->next;
		my_free(SRAMEX, deleteNode);
		return (list);
	}
    if (isLast(prevPosition)) {
       return NULL;
    } else {
        deleteNode = prevPosition->next;
        prevPosition->next = prevPosition->next->next;
        my_free(SRAM12, deleteNode->content);
		my_free(SRAMEX, deleteNode);
		return list;
    }
}

/*
**********************************************************************************
* Name: fun(node *) 	
* Description:
* Parameter(s):
* Return:
* Date: 2020-2
**********************************************************************************
*/
static void* fun(node *position) {
	return (position -> content);
}	

/*
**********************************************************************************
* Name: EditList(void *content, void *newContent, node *list)	
* Description:
* Parameter(s):
*	void *content
*	void *newContent
*	node *list
* Return:
*	NULL, no content matched
*	list, head node addr of the list
* Date: 2020-2
**********************************************************************************
*/

node *EditList(void *content, void *newContent, node *list) {
    node *tempNode = SearchList(content, list, fun);

    if (tempNode == NULL) {
        return NULL;
    } else {
        tempNode->content = newContent;
		return list;
    }
}

/*
**********************************************************************************
* Name: GetNodeIndex(node *aNode, node *list)	
* Description:
*	get the index of a node in the list
* Parameter(s):
*	node *aNode
*	node *list
* Return:
*	>=0: index of the node
*	-1 : not found
* Date: 2020-2
**********************************************************************************
*/
int GetNodeIndex(node *aNode, node *list) {
	node *temp = list;
	int counter = 0;
	while (temp != aNode) {
		if (temp->next == NULL) {
			return NOT_FOUND;
		} else {
			temp = temp->next;
			counter++;
		}
	}
	return counter;
}

/*
**********************************************************************************
* Name: DeleteList(node *list)	
* Description:
* Parameter(s):
*	node *list
* Return:
* Date: 2020-2
**********************************************************************************
*/
void DeleteList(node *list) {
	node *deleteNode = NULL;
	
	while (list != NULL) {
		deleteNode = list;
		list = list->next;
		my_free(SRAM12, deleteNode->content);
		my_free(SRAM12, deleteNode);
	}
}
