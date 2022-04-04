
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

	InOrder (root_copy);
	Graph_Dump (root_copy);

	DeleteBranch (&root);
	DeleteBranch (&root_copy);

	return 0;
}



