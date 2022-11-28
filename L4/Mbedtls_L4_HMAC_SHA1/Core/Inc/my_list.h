/*
*********************************************************************************************************
*                                            	 Notes
* Filename: list.h
* Notes:
* Date: 2019-12
* Author: 崔洪玮
*********************************************************************************************************
*/
#ifndef __MY_LIST_H
#define __MY_LIST_H

/*
*********************************************************************************************************
*                                               INCLUDES
*********************************************************************************************************
*/
#include "ram_mgmt.h"

/*
*********************************************************************************************************
*                                             DEFINATIONS
*********************************************************************************************************
*/
typedef struct node{
    void *content;
    struct node* next;
} node;

#define NOT_FOUND -1

#define LIST_SRAM	SRAM1

/*
*********************************************************************************************************
*                                              FUNCTIONS
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
node* CreatList(void *content);

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
int isLast(node *position);

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
node* SearchList(void *content, node *list, void *(*fun)(node*));

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
node* InsertList(void *content, node *list, node *position);

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
node* AppendList(void *content, node *(*list));

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
node *DeleteNode(void *content, node *list);

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

node *EditList(void *content, void *newContent, node *list);

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
int GetNodeIndex(node *aNode, node *list);

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
void DeleteList(node *list);
#endif
