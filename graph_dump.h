

// graph_dump.h

#ifndef INCLUDE_GRAPH_DUMP_H
#define INCLUDE_GRAPH_DUMP_H


#include <stdio.h>
#include <assert.h>
#include "file_operations.h"


#define CONNECTION_LIST_1  "\t%d->%d [color = \"%s\", frontcolor = \"%s\", label = \"%s\"];\\n"
#define LABEL_NODE_LIST_1  "\"index: %d | { prev\\n %d | data\\n %d | next\\n%d } \""
#define LABEL_NODE_LIST_2  "\"name: %s | { prev\\n %d | data\\n %d | next\\n%d } \""


#define LABEL_NODE_TREE_1  "\"data: %d | { <fl%d> lefr child | <fr%d> right child } \""
#define LABEL_NODE_TREE_2  "\"data: %d\""


const int MAX_LEN_STR = 20;


int Start_Dot_File		  (const char name_file[]);
int End_Dot_File 	      (const char name_file[]);



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Func for project "List"

int Make_Info_List        (int head, int tail, int size, int free);
int Make_Node_Name	      (const char *name, int prev, int data, int next,
					       const char *fillcolor);
int Make_Connection_Name  (const char *name_from, const char *name_to, 
				           const char *color, const char *frontcolor, const char *label);
int Make_Node_ID	      (int  id, int prev, int data, int next, 
					       const char *fillcolor);
int Make_Connection_ID    (int  id_from, int id_to, 
				           const char *color,  const char *frontcolor, const char *label);



//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Func for project "Tree"

int Preamble              (int depth);
int Make_Info_Tree        (int depth);
int Make_Node             (int depth, int data, const char fillcolor[]);
int Make_Connection       (int parent_data, int child_data, const char color[],
						   const char frontcolor[]); 


int Make_All              ();


#endif  // INCLUDE_GRAPH_DUMP_H
