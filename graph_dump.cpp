
// graph_dump.cpp


#include "graph_dump.h"



static FILE *file_out = NULL;


//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
int Start_Dot_File (const char name_file[])
{
	assert (name_file != NULL);

	Open_File (&file_out, "dump.dot", "w");

	fprintf (file_out, "digraph List\n{\n"
					   "	rankdir = LR;\n");

	return 0;
}



// Functions for progect "LIST"
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
int Make_Info_List (int head, int tail, int size, int free)
{
	fprintf (file_out, "	{\n"
					   "		rankdir = LR;\n"
					   "		node [shape = record, style = filled];\n"
					   "		head [label = \"HEAD | %d\", fillcolor = \"green\"];\n"
					   "		tail [label = \"TAIL | %d\", fillcolor = \"green\"];\n"
					   "		size [label = \"SIZE | %d\", fillcolor = \"green\"];\n"
					   "		free [label = \"FREE | %d\", fillcolor = \"green\"];\n"
					   "	}\n", head, tail, size, free);
	return 0;
}


//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
int End_Dot_File (const char name_file[])
{
	assert (file_out != NULL);

	fprintf (file_out, "}\n");

	Close_File (file_out, name_file);

	return 0;
}


//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
int Make_Node_Name (const char name[], int prev, int data, int next,
					const char fillcolor[])
{
	assert (name != NULL);
	assert (fillcolor != NULL);

	fprintf (file_out, "%s [label = " LABEL_NODE_LIST_2 "fillcolor = \"%s\"];\n", 
			 name, name, prev, data, next, fillcolor);

	return 0;
}


//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
int Make_Node_ID (int  id,  int prev, int data, int next, 
				  const char fillcolor[])
{
	assert (fillcolor != NULL);

	fprintf (file_out, "%d [label = " LABEL_NODE_LIST_1 ", fillcolor = \"%s\"];\n", 
			 id, id, prev, data, next, fillcolor);

	return 0;
}


//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
int Make_Connection_Name (const char name_from[], const char name_to[],
					 	  const char color[],     const char frontcolor[],
					 	  const char label[])
{
	assert (name_from  != NULL);
	assert (name_to    != NULL);
	assert (color      != NULL);
	assert (frontcolor != NULL);
	assert (label      != NULL);

	fprintf (file_out, "\t%s->%s [color = \"%s\","
                       " frontcolor = \"%s\","
					   " label = \"%s\"];\n",
						name_from, name_to, color, frontcolor, label);

	return 0;
}
	

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
int Make_Connection_ID (int  id_from, int  id_to,
					 	const char color[], const char frontcolor[],
					 	const char label[])
{
//	assert (id_from    >= 0);
//	assert (id_to      >= 0);
	assert (color      != NULL);
	assert (frontcolor != NULL);
	assert (label      != NULL);

	fprintf (file_out, "\t%d->%d [color = \"%s\","
                       " frontcolor = \"%s\","
					   " label = \"%s\"];\n",
						id_from, id_to, color, frontcolor, label);

	return 0;
}


int Make_All ()
{
	fprintf (file_out, "node [shape = record, style = filled];\n");

	return 0;
}




// Functions for project "TREE"
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++




int Make_Info_Tree (int depth)
{
	return 0;
}


//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
int Make_Node (int depth, int data, const char fillcolor[])
{
	assert (fillcolor != NULL);

	fprintf (file_out, "	{rank = same; \"%dd\"; %d [label = " LABEL_NODE_TREE_2 
					   ", fillcolor = \"%s\"];}\n", 
			 depth, data, data, fillcolor);

	return 0;
}





//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
int Make_Connection (int parent_data, int child_data, const char color[],
					 const char frontcolor[])
{
	fprintf (file_out, "\t%d->%d [color = \"%s\","
                       " frontcolor = \"%s\","
					   " label = \"%s\"];\n",
						parent_data, child_data, color, frontcolor, "\0");

	return 0;
}
	


//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
int Preamble (int depth)
{
	fprintf (file_out, "	ranksep=.75; size = \"7.5,7.5\"\n"
					   "	{\n"
					   "		node [shape=plaintext, fontsize=16];\n"
					   "		\"0d\"");
	
	for (int count = 1; count <= depth; count++)
	{
		fprintf (file_out, "->\"%dd\"", count);
	}
	fprintf (file_out, ";\n"
					   "	}\n");

	return 0;
}
	
	





