
// diftor.cpp


#include "diftor.h"


static FILE *file_out = NULL;


//_________________________======================________________________________
//                         |    Work to file    |
//                         ======================

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int NodeCtor_Dig (Node_t **node_ptr_ptr, double value)
{
	assert (node_ptr_ptr != NULL);

	*node_ptr_ptr = (Node_t *) calloc (1, sizeof (Node_t));  
	if (*node_ptr_ptr == NULL)
	{
		printf ("!!Allocate error!!\n");   // TODO log_file :)
		return 1;
	}

	(*node_ptr_ptr)->value.dig   = value;
	(*node_ptr_ptr)->type        = DIGIT;
	(*node_ptr_ptr)->left_node   = NULL;
	(*node_ptr_ptr)->right_node  = NULL;

	return 0;
}	


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int NodeCtor_Opr (Node_t **node_ptr_ptr, int cmd)
{
	assert (node_ptr_ptr != NULL);

	*node_ptr_ptr = (Node_t *) calloc (1, sizeof (Node_t));  
	if (*node_ptr_ptr == NULL)
	{
		printf ("!!Allocate error!!\n");   // TODO log_file :)
		return 1;
	}

	(*node_ptr_ptr)->value.cmd_opr = cmd;
	(*node_ptr_ptr)->type          = OPERATOR;
	(*node_ptr_ptr)->left_node     = NULL;
	(*node_ptr_ptr)->right_node    = NULL;

	return 0;
}	


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int NodeCtor_Var (Node_t **node_ptr_ptr, const char *str)
{
	assert (node_ptr_ptr != NULL);
//printf ("%s\n", str);
	*node_ptr_ptr = (Node_t *) calloc (1, sizeof (Node_t));  
	if (*node_ptr_ptr == NULL)
	{
		printf ("!!Allocate error!!\n");   // TODO log_file :)
		return 1;
	}

	(*node_ptr_ptr)->value.var   = str;
	(*node_ptr_ptr)->type        = VARIABLE;
	(*node_ptr_ptr)->left_node   = NULL;
	(*node_ptr_ptr)->right_node  = NULL;

	return 0;
}	


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int MakeTree (Node_t **root, char *buf, char *buf_str)
{
	assert (root     != NULL);
	assert (buf      != NULL);
	assert (buf_str  != NULL);
	
	int cur_pos = 0;
	MakeNode (root, buf, &cur_pos, buf_str);

	return 0;
}
	


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int MakeNode (Node_t **node_ptr_ptr, char *buf, int *cur_pos, char *buf_str)
{
	assert (node_ptr_ptr != NULL);
	assert (buf          != NULL);
	assert (cur_pos      != NULL);
	assert (buf_str      != NULL);
//printf ("\t|%s|\n", buf + *cur_pos);
	char    tmp         = 0;
	Node_t *left_child  = NULL;
	Node_t *right_child = NULL;

	sscanf (buf + *cur_pos, "%c", &tmp);
	if (tmp == '(')
	{
		(*cur_pos)++;
		sscanf (buf + *cur_pos, "%c", &tmp);
		if (tmp == '(')
		{
			MakeNode (&left_child, buf, cur_pos, buf_str);
			Processing_Operator (node_ptr_ptr, buf, cur_pos);
			MakeNode (&right_child, buf, cur_pos, buf_str);
		
			(*node_ptr_ptr)->left_node = left_child;
			(*node_ptr_ptr)->right_node = right_child;

			if (*(buf + *cur_pos) != ')')
			{
				printf ("!!Error char: <%c>!!\n", *(buf + *cur_pos));
				return 1;
			}	

			(*cur_pos)++;
		}
		else
		{
			Processing_Dig_Var (node_ptr_ptr, buf, cur_pos, buf_str);
		}
	}
	else
	{
		printf ("!!Error char: <%c>!!\n", tmp);
		return 1;
	}

	return 0;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int Processing_Dig_Var (Node_t **node_ptr_ptr, char *buf, 
						int *cur_pos, char *buf_str)
{
printf ("V\t|%s|\n", buf + *cur_pos);
	double dig = 0;	
	int read_c = 0;

	if ((read_c = sscanf (buf + *cur_pos, "%lf", &dig)) != 0)
	{
		NodeCtor_Dig (node_ptr_ptr, dig);
		*cur_pos += read_c;
	}
	else
	{
		read_c = sscanf (buf + *cur_pos, "%[a-z]", buf_str);
		NodeCtor_Var (node_ptr_ptr, buf_str);
		*cur_pos += read_c;
		buf_str += read_c + 1;
	}

	char tmp = 0;
	sscanf (buf + *cur_pos, "%c", &tmp);
	if (tmp != ')')
	{
		printf ("!!Error char: <%c>!!\n", tmp);
		return 1;
	}
	
	(*cur_pos)++;
	return 0;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int Processing_Operator (Node_t **node_ptr_ptr, char *buf, int *cur_pos)
{
printf ("O\t|%s|\n", buf + *cur_pos);
	char op[MAX_LEN_OP] = {};
	int  read_c = 0;

	read_c = sscanf (buf + *cur_pos, "%[^(]", op);

	if (!strcmp (op, "+"))
	{	
		NodeCtor_Opr (node_ptr_ptr, CMD_ADD);
	}
	else
	{
		printf ("!!Error operator <%s>!!\n", op);
	}

	*cur_pos += read_c;

	char tmp = 0;
	sscanf (buf + *cur_pos, "%c", &tmp);
	if (tmp != '(')
	{
		printf ("!!Error char: <%c>!!\n", tmp);
		return 1;
	}
	
	return 0;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int CopyBranch (Node_t *root_from_ptr, Node_t **root_to_ptr)
{
	assert (root_from_ptr != NULL);
	assert (root_to_ptr   != NULL);

	switch (root_from_ptr->type)
	{
		case DIGIT:
		{
			NodeCtor_Dig (root_to_ptr, root_from_ptr->value.dig);
			break;
		}
		case OPERATOR:
		{
			NodeCtor_Opr (root_to_ptr, root_from_ptr->value.cmd_opr);
			break;
		}
		case VARIABLE:
		{
			NodeCtor_Var (root_to_ptr, root_from_ptr->value.var);
			break;
		}
		default:
		{
			printf ("!!Error type value: <%d>!!\n", root_from_ptr->type);
			return 1;
		}
	}
			
	if (root_from_ptr->left_node != NULL)  
	{
		CopyBranch (root_from_ptr->left_node, &((*root_to_ptr)->left_node));
	}

	if (root_from_ptr->right_node != NULL)  
	{
		CopyBranch (root_from_ptr->right_node, &((*root_to_ptr)->right_node));
	}
	
	return 0;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int DeleteBranch (Node_t **node_ptr_ptr)
{
	assert (node_ptr_ptr != NULL);

	if ((*node_ptr_ptr)->left_node != NULL)
	{
		DeleteBranch (&((*node_ptr_ptr)->left_node));
	}

	if ((*node_ptr_ptr)->right_node != NULL)
	{
		DeleteBranch (&((*node_ptr_ptr)->right_node));
	}

	free (*node_ptr_ptr);
	*node_ptr_ptr = NULL;

	return 0;
}




//_________________________======================________________________________
//                         |     Print Tree     |
//                         ======================
	

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int PrintNode (Node_t *node_ptr)
{
	assert (node_ptr != NULL);
	
	if (node_ptr->type == DIGIT)
	{
		printf ("%.2f", node_ptr->value.dig);
	}
	else if (node_ptr->type == OPERATOR)
	{
		switch (node_ptr->value.cmd_opr) 
		{
			case CMD_ADD: printf ("+"); break;
			default: printf ("!!Error command <%d>!!\n", node_ptr->value.cmd_opr);
		}
	}
	else if (node_ptr->type == VARIABLE)
	{
		printf ("%s", node_ptr->value.var);
	}
	else
	{	
		printf ("!!Error type value!!\n");
		return 1;
	}

	return 0;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int InOrder (Node_t *node_ptr) 
{
	assert (node_ptr != NULL);

	printf ("(");
	if (node_ptr->left_node != NULL)   InOrder (node_ptr->left_node);
	PrintNode (node_ptr);
	if (node_ptr->right_node != NULL)  InOrder (node_ptr->right_node);
	printf (")");

	return 0;
}



//_________________________======================________________________________
//                         |    Work to file    |
//                         ======================


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int SizeFile (FILE *file_in)
{
	fseek (file_in, 0, SEEK_END);
	int size_file = ftell (file_in);
	fseek (file_in, 0, SEEK_SET);
	
	return size_file;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int FileToBuf (char buf[], int size_file, FILE *file_in)
{
	assert (buf != NULL);
	assert (file_in != NULL);	
	assert (size_file >=0);	

	return  fread (buf, sizeof (char), size_file, file_in);
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int ReadFile (const char *name_file, char *buf_ptr)
{
	assert (name_file != NULL);
	assert (buf_ptr != NULL);
	

	FILE *file_in = NULL;
	Open_File (&file_in, name_file, "r");
		
	int size_buf = SizeFile (file_in) + 2;

	FileToBuf (buf_ptr, size_buf, file_in);
	Close_File (file_in, name_file);

	return 0;
}



//_________________________======================________________________________
//                         | Graphics Functions |
//                         ======================


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int Start_Dot_File ()
{
	Open_File (&file_out, "dump.dot", "w");

	fprintf (file_out, "digraph List\n{\n"
					   "	rankdir = LR;\n");

	return 0;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int End_Dot_File ()
{
	assert (file_out != NULL);

	fprintf (file_out, "}\n");

	Close_File (file_out, "dump.dot");

	return 0;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int Sample_Nodes ()
{
	fprintf (file_out, "node [shape = record, style = filled];\n");

	return 0;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int Graph_Dump (Node_t *node_ptr)
{
	assert (node_ptr != NULL);

	Start_Dot_File ();

	Preamble (MAX_DEPTH);

	Sample_Nodes ();

	Print_Nodes (node_ptr, 0);
	Print_Connections (node_ptr, node_ptr);	

	End_Dot_File ();

	return 0;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int Print_Nodes (Node_t *node_ptr, int cur_depth)
{
	assert (node_ptr != NULL);
		
	if (node_ptr->left_node != NULL)
	{
		cur_depth++;
		Print_Nodes (node_ptr->left_node, cur_depth);
		cur_depth--;
	}		

	Print_Node_Info (node_ptr, cur_depth);

	if (node_ptr->right_node != NULL)
	{
		cur_depth++;
		Print_Nodes (node_ptr->right_node, cur_depth);
		cur_depth--;
	}

	return 0;
}	


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int Print_Node_Info (Node_t *node_ptr, int depth)
{
	if (node_ptr->type == DIGIT)
	{
		fprintf (file_out, "	{rank = same; \"%dd\"; \"%p\" [label = " 
							LABEL_DIG 
					       ", fillcolor = \"%s\"];}\n", 
			 depth, node_ptr, node_ptr->value.dig, "green");
	}
	else if (node_ptr->type == OPERATOR)
	{
		switch (node_ptr->value.cmd_opr) 
		{
			case CMD_ADD: 
			{		
				fprintf (file_out, "	{rank = same; \"%dd\"; \"%p\" [label = " 
									LABEL_OPR 
					    		   ", fillcolor = \"%s\"];}\n", 
						 depth, node_ptr, "+", "yellow");
				break;	
			}
			default : break;
		}
	}
	else if (node_ptr->type == VARIABLE)
	{
		fprintf (file_out, "	{rank = same; \"%dd\"; \"%p\" [label = " 
							LABEL_STR 
					       ", fillcolor = \"%s\"];}\n", 
			 depth, node_ptr, node_ptr->value.var, "red");
	}	

	return 0;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int Print_Connections (Node_t *node_child_ptr, Node_t *node_parent_ptr)
{
	assert (node_child_ptr  != NULL);
	assert (node_parent_ptr != NULL);
		
	if (node_child_ptr->left_node != NULL)
	{
		Print_Connections (node_child_ptr->left_node, node_child_ptr);
	}

	Print_Connection_Info (node_child_ptr, node_parent_ptr, "black", "grey");

	if (node_child_ptr->right_node != NULL)
	{
		Print_Connections (node_child_ptr->right_node, node_child_ptr);
	}
	
	return 0;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int Print_Connection_Info (Node_t *child_ptr, Node_t *parent_ptr, 
						   const char color[], const char frontcolor[])
{
	fprintf (file_out, "\t\"%p\"->\"%p\" [color = \"%s\","
                       " frontcolor = \"%s\","
					   " label = \"%s\"];\n",
						parent_ptr, child_ptr, color, frontcolor, "\0");

	return 0;
}







