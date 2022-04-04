
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
	else if (!strcmp (op, "-"))
	{	
		NodeCtor_Opr (node_ptr_ptr, CMD_SUB);
	}
	else if (!strcmp (op, "*"))
	{	
		NodeCtor_Opr (node_ptr_ptr, CMD_MUL);
	}
	else if (!strcmp (op, "/"))
	{	
		NodeCtor_Opr (node_ptr_ptr, CMD_DIV);
	}
	else if (!strcmp (op, "^"))
	{	
		NodeCtor_Opr (node_ptr_ptr, CMD_POW);
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
int CopyBranch (Node_t *src_root_ptr, Node_t **dst_root_ptr_ptr)
{
	assert (src_root_ptr   != NULL);
	assert (dst_root_ptr_ptr != NULL);

	switch (src_root_ptr->type)
	{
		case DIGIT:
		{
			NodeCtor_Dig (dst_root_ptr_ptr, src_root_ptr->value.dig);
			break;
		}
		case OPERATOR:
		{
			NodeCtor_Opr (dst_root_ptr_ptr, src_root_ptr->value.cmd_opr);
			break;
		}
		case VARIABLE:
		{
			NodeCtor_Var (dst_root_ptr_ptr, src_root_ptr->value.var);
			break;
		}
		default:
		{
			printf ("!!Error type value: <%d>!!\n", src_root_ptr->type);
			return 1;
		}
	}
			
	if (src_root_ptr->left_node != NULL)  
	{
		CopyBranch (src_root_ptr->left_node, &((*dst_root_ptr_ptr)->left_node));
	}

	if (src_root_ptr->right_node != NULL)  
	{
		CopyBranch (src_root_ptr->right_node, &((*dst_root_ptr_ptr)->right_node));
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
//                         |   Differentiation  |
//                         ======================


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int Diftor (Node_t *src_node_ptr, Node_t **dst_node_ptr_ptr)
{
	assert (src_node_ptr   != NULL);
	assert (dst_node_ptr_ptr != NULL);

	switch (src_node_ptr->type)
	{
		case DIGIT:
		{
			d_Const (dst_node_ptr_ptr);
			break;
		}
		case OPERATOR:
		{
			D_Opr (src_node_ptr, dst_node_ptr_ptr);
			break;
		}
		case VARIABLE:
		{
			d_Var (dst_node_ptr_ptr);
			break;
		}
		default:
		{
			printf ("!!Error type value: <%d>!!\n", src_node_ptr->type);
			return 1;
		}
	}
			
	return 0;
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int d_Const (Node_t **node_ptr_ptr)
{
	assert (node_ptr_ptr != NULL);

	NodeCtor_Dig (node_ptr_ptr, 0);

	return 0;
}
	
	
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int d_Var (Node_t **node_ptr_ptr)
{
	assert (node_ptr_ptr != NULL);

	NodeCtor_Dig (node_ptr_ptr, 1);

	return 0;
}
	
	
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int D_Opr (Node_t *src_node_ptr, Node_t **dst_node_ptr_ptr)
{
	assert (src_node_ptr   != NULL);
	assert (dst_node_ptr_ptr != NULL);

	switch (src_node_ptr->value.cmd_opr)
	{
		case CMD_ADD:
		{
			d_Add (src_node_ptr, dst_node_ptr_ptr);
			break;
		}
		case CMD_SUB:
		{
			d_Sub (src_node_ptr, dst_node_ptr_ptr);
			break;
		}
		case CMD_MUL:
		{
			d_Mul (src_node_ptr, dst_node_ptr_ptr);
			break;
		}
		case CMD_DIV:
		{
			d_Div (src_node_ptr, dst_node_ptr_ptr);
			break;
		}
		case CMD_POW:
		{
			d_Pow (src_node_ptr, dst_node_ptr_ptr);
			break;
		}
		default:
		{
			printf ("!!Error command: <%d>!!\n", src_node_ptr->value.cmd_opr);
			return 1;
		}
	}		

	return 0;
}
	
	
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int d_Add (Node_t *src_node_ptr, Node_t **dst_node_ptr_ptr)
{
	assert (src_node_ptr   != NULL);
	assert (dst_node_ptr_ptr != NULL);
	
	NodeCtor_Opr (dst_node_ptr_ptr, CMD_ADD);

	Diftor (src_node_ptr->left_node, &((*dst_node_ptr_ptr)->left_node));
	Diftor (src_node_ptr->right_node, &((*dst_node_ptr_ptr)->right_node));

	return 0;
}
	
	
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int d_Sub (Node_t *src_node_ptr, Node_t **dst_node_ptr_ptr)
{
	assert (src_node_ptr   != NULL);
	assert (dst_node_ptr_ptr != NULL);
	
	NodeCtor_Opr (dst_node_ptr_ptr, CMD_SUB);

	Diftor (src_node_ptr->left_node, &((*dst_node_ptr_ptr)->left_node));
	Diftor (src_node_ptr->right_node, &((*dst_node_ptr_ptr)->right_node));

	return 0;
}

	
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int d_Mul (Node_t *src_node_ptr, Node_t **dst_node_ptr_ptr)
{
	assert (src_node_ptr   != NULL);
	assert (dst_node_ptr_ptr != NULL);
	
	NodeCtor_Opr (dst_node_ptr_ptr, CMD_ADD);
	NodeCtor_Opr (&((*dst_node_ptr_ptr)->left_node), CMD_MUL);
	NodeCtor_Opr (&((*dst_node_ptr_ptr)->right_node), CMD_MUL);

	Diftor (src_node_ptr->left_node, 
			&((*dst_node_ptr_ptr)->left_node->left_node));
	CopyBranch (src_node_ptr->right_node,
			    &((*dst_node_ptr_ptr)->left_node->right_node));

	CopyBranch (src_node_ptr->left_node,
			    &((*dst_node_ptr_ptr)->right_node->left_node));
	Diftor (src_node_ptr->right_node, 
			&((*dst_node_ptr_ptr)->right_node->right_node));

	return 0;
}

	
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int d_Div (Node_t *src_node_ptr, Node_t **dst_node_ptr_ptr)
{
	assert (src_node_ptr   != NULL);
	assert (dst_node_ptr_ptr != NULL);
	
	NodeCtor_Opr (dst_node_ptr_ptr, CMD_DIV);
	NodeCtor_Opr (&((*dst_node_ptr_ptr)->left_node), CMD_SUB);
	NodeCtor_Opr (&((*dst_node_ptr_ptr)->left_node->left_node), CMD_MUL);
	NodeCtor_Opr (&((*dst_node_ptr_ptr)->left_node->right_node), CMD_MUL);
	NodeCtor_Opr (&((*dst_node_ptr_ptr)->right_node), CMD_POW);

	Diftor (src_node_ptr->left_node, 
			&((*dst_node_ptr_ptr)->left_node->left_node->left_node));
	CopyBranch (src_node_ptr->right_node,
			    &((*dst_node_ptr_ptr)->left_node->left_node->right_node));

	CopyBranch (src_node_ptr->left_node,
			    &((*dst_node_ptr_ptr)->left_node->right_node->left_node));
	Diftor (src_node_ptr->right_node, 
			&((*dst_node_ptr_ptr)->left_node->right_node->right_node));

	CopyBranch (src_node_ptr->right_node, 
				&((*dst_node_ptr_ptr)->right_node->left_node));
	NodeCtor_Dig (&((*dst_node_ptr_ptr)->right_node->right_node), 2);

	return 0;
}

	
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int d_Pow (Node_t *src_node_ptr, Node_t **dst_node_ptr_ptr)
{
	assert (src_node_ptr   != NULL);
	assert (dst_node_ptr_ptr != NULL);
	
	NodeCtor_Opr (dst_node_ptr_ptr, CMD_MUL);
	NodeCtor_Opr (&((*dst_node_ptr_ptr)->left_node), CMD_POW);
	NodeCtor_Opr (&((*dst_node_ptr_ptr)->right_node), CMD_MUL);

	CopyBranch (src_node_ptr->left_node,
				&((*dst_node_ptr_ptr)->left_node->left_node));
	NodeCtor_Dig (&((*dst_node_ptr_ptr)->left_node->right_node),
				  src_node_ptr->right_node->value.dig - 1);

	NodeCtor_Dig (&((*dst_node_ptr_ptr)->right_node->left_node),
				  src_node_ptr->right_node->value.dig);
	
	Diftor (src_node_ptr->left_node, 
			&((*dst_node_ptr_ptr)->right_node->right_node));
	
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
			case CMD_SUB: printf ("-"); break;
			case CMD_MUL: printf ("*"); break;
			case CMD_DIV: printf ("/"); break;
			case CMD_POW: printf ("^"); break;
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
			case CMD_SUB: 
			{		
				fprintf (file_out, "	{rank = same; \"%dd\"; \"%p\" [label = " 
									LABEL_OPR 
					    		   ", fillcolor = \"%s\"];}\n", 
						 depth, node_ptr, "-", "yellow");
				break;	
			}
			case CMD_MUL: 
			{		
				fprintf (file_out, "	{rank = same; \"%dd\"; \"%p\" [label = " 
									LABEL_OPR 
					    		   ", fillcolor = \"%s\"];}\n", 
						 depth, node_ptr, "*", "yellow");
				break;	
			}
			case CMD_DIV: 
			{		
				fprintf (file_out, "	{rank = same; \"%dd\"; \"%p\" [label = " 
									LABEL_OPR 
					    		   ", fillcolor = \"%s\"];}\n", 
						 depth, node_ptr, "/", "yellow");
				break;	
			}
			case CMD_POW: 
			{		
				fprintf (file_out, "	{rank = same; \"%dd\"; \"%p\" [label = " 
									LABEL_OPR 
					    		   ", fillcolor = \"%s\"];}\n", 
						 depth, node_ptr, "^", "yellow");
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







