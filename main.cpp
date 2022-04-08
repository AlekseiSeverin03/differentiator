
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
	Graph_Dump (root);

	Node_t *root_dif = NULL;
	Diftor (root, &root_dif);

//	InOrder (root_dif);
//	Graph_Dump (root_dif);

	for (int count = 0; count < 10; count++)
	{
		ArithmeticConst (&root_dif);
		MulOne          (&root_dif);
		MulNull         (&root_dif);
		PowOne          (&root_dif);
		PowNull         (&root_dif);
		AddNull         (&root_dif);
	}

	InOrder (root);
printf ("\n");
	InOrder (root_dif);
	Graph_Dump (root_dif);


	DeleteBranch (&root);
	return 0;
}



