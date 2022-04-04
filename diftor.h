
// diftor.h


#include <stdio.h>
#include <string.h>
#include "file_operations.h"


#define LABEL_DIG  "\"data: %0.1lf \""
#define LABEL_OPR  "\"data: %s \""
#define LABEL_STR  "\"data: %s \""


//#define LABEL_DIG  "\"data: %f | { <fl%d> left child | <fr%d> right child } \""
//#define LABEL_STR  "\"data: %s | { <fl%d> left child | <fr%d> right child } \""


const int MAX_LEN_VAR = 10;
const int MAX_LEN_OP  = 10;
const int MAX_DEPTH   = 10;


enum TYPE_VALUE
{
	EMPTY    = 0,
	DIGIT    = 1,
	OPERATOR = 2,
	VARIABLE = 3,
};

enum COMMANDS
{
	CMD_ADD = 1,
	CMD_SUB = 2,
	CMD_MUL = 3,
	CMD_DIV = 4,
	CMD_POW = 5,
};


typedef struct Node_t   Node_t;
typedef struct Value_t  Value_t;


struct Value_t
{
	double       dig;
	const char  *var;
	int          cmd_opr;
};

struct Node_t
{
	int      type;
	Value_t  value;
	Node_t  *left_node;
	Node_t  *right_node;
};



//__________________________=====================_______________________________
//                          |   Work to Tree    |
//                          =====================

int NodeCtor_Dig (Node_t **node_ptr_ptr, double       value);
int NodeCtor_Opr (Node_t **node_ptr_ptr, int          cmd);
int NodeCtor_Var (Node_t **node_ptr_ptr, const char  *var);

int Processing_Dig_Var  (Node_t **node_ptr_ptr, char *buf, 
						 int *cur_pos, char *buf_str);
int Processing_Operator (Node_t **node_ptr_ptr, char *buf, int *cur_pos);
int MakeNode            (Node_t **node_ptr_ptr, char *buf, 
						 int *cur_pos, char *buf_str);
int MakeTree            (Node_t **root, char *buf, char *buf_str);
int CopyBranch          (Node_t *src_root_ptr, Node_t **dst_root_ptr);
int DeleteBranch        (Node_t **node_ptr_ptr);


//__________________________=====================_______________________________
//                          |   Differetiation  |
//                          =====================

int Diftor (Node_t *src_node_ptr, Node_t **dst_node_ptr_ptr);
int d_Const (Node_t **node_ptr_ptr);
int d_Var (Node_t **node_ptr_ptr);
int D_Opr (Node_t *src_node_ptr, Node_t **dst_node_ptr_ptr);
int d_Add (Node_t *src_node_ptr, Node_t **dst_node_ptr_ptr);
int d_Sub (Node_t *src_node_ptr, Node_t **dst_node_ptr_ptr);
int d_Mul (Node_t *src_node_ptr, Node_t **dst_node_ptr_ptr);
int d_Div (Node_t *src_node_ptr, Node_t **dst_node_ptr_ptr);
int d_Pow (Node_t *src_node_ptr, Node_t **dst_node_ptr_ptr);


//__________________________=====================_______________________________
//                          |    Print Tree     |
//                          =====================

int PrintNode    (Node_t *node_ptr);
int InOrder      (Node_t *node_ptr);


//__________________________=====================_______________________________
//                          |   Work to File    |
//                          =====================

int SizeFile  (FILE *file_in);
int FileToBuf (char buf[], int size_file, FILE *file_in);
int ReadFile  (const char *name_file, char *buf_ptr);


//__________________________=====================_______________________________
//                          | Graphics Funtions |
//                          =====================

int Start_Dot_File ();
int End_Dot_File   ();
int Sample_Nodes   ();
int Preamble       (int depth);

int Graph_Dump            (Node_t *node_ptr);
int Print_Nodes           (Node_t *node_ptr, int cur_depth);
int Print_Node_Info       (Node_t *node_ptr, int depth);
int Print_Connections     (Node_t *node_chikd_ptr, Node_t *node_parent_ptr);
int Print_Connection_Info (Node_t *child_ptr, Node_t *parent_ptr,
						   const char color[], const char frontcolor[]);




