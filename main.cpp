
// main.cpp


#include "diftor.h"


int main ()
{
	char buf[100]     = {};
	char buf_str[100] = {};

	ReadFile ("test.txt", buf);
	printf ("<%s>\n", buf);

	Node_t *root = NULL;
	MakeTree (&root, buf, buf_str);

	Node_t *root_copy = NULL;
	CopyBranch (root, &root_copy);

	Node_t *root_dif = NULL;
	Diftor (root_copy, &root_dif);

	InOrder (root_dif);
	Graph_Dump (root_dif);

	DeleteBranch (&root);
	DeleteBranch (&root_copy);

	return 0;
}



