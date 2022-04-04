#include "file_operations.h"

//-------------------------------------------------------------------------------------------------------------------------
//! Open the file
//!
//! @param  [out] stream     pointer on stream
//! @param  [in]  name_file  name of file
//! @param  [in]  mode       mode to open file
//!
//! @return code error:
//!			 _50_ if there are no errors
//!			_-50_ if there are errors
//-------------------------------------------------------------------------------------------------------------------------

int Open_File (FILE **stream, const char *name_file, const char mode[])
{
	assert (stream != NULL);

	*stream = fopen (name_file, mode);

	if (*stream == NULL)
	{
		printf ("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		printf ("Failed to open the file with the name %s\n", name_file);
		printf ("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	
		return ERR_OPEN_FILE;
	}

	return OK_OPEN;
}

//-------------------------------------------------------------------------------------------------------------------------
//! Close the file
//!
//! @param  [in]  stream     stream
//! @param  [in]  name_file  name of file
//!
//! @return code error:
//!			 _51_ if there are no errors
//!			_-51_ if there are errors
//-------------------------------------------------------------------------------------------------------------------------

int Close_File (FILE *stream, const char *name_file)
{
	assert (stream != NULL);

	if (fclose (stream) == EOF)
	{
		printf ("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		printf ("Failed to close the file with the name %s\n", name_file);
		printf ("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	
		return ERR_CLOSE_FILE;
	}

	return OK_CLOSE;
}



