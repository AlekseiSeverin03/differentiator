#include <stdio.h>
#include <assert.h>

#ifndef INCLUDE_FILEOPERATIONS_H
#define INCLUDE_FILEOPERATIONS_H


enum OUT_FUN_WORK_FILE
{
OK_OPEN        =  50,
OK_CLOSE       =  51,
ERR_OPEN_FILE  = -50,
ERR_CLOSE_FILE = -51,
};

extern FILE *LOG_FILE;

const char PREAMBLE[] = "This is log file of the project \"Stack\"\n"
                        "Here will be write errors and warnings\n\n";


int Open_File  (FILE **stream, const char *name_file, const char mode[] = "a");
int Close_File (FILE *stream,  const char *name_file);


#define OPEN_LOG_FILE()                                          \
{                                                                \
    if (Open_File (&LOG_FILE, "log_file.txt") == ERR_OPEN_FILE)  \
    {                                                            \
        printf ("Fail to open the file \"log_file.txt\"\n\n");   \
        exit (10);                                               \
    }                                                            \
																 \
	if (is_log == 1)											 \
	{															 \
		fprintf (LOG_FILE, "\n$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");  \
		fprintf (LOG_FILE, "\nFile open: " __DATE__ ", " __TIME__ "\n\n%s", PREAMBLE);  \
	}	                                                         \
}

#define CLOSE_LOG_FILE()                                          \
{                                                                 \
	if (is_log == 1)											  \
	{															  \
		fprintf (stream, "\nFile close: " __DATE__ ", " __TIME__ "\n\n");  \
		fprintf (stream, "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n");  \
	}	                                                          \
																  \
    if (Close_File (LOG_FILE, "log_file.txt") == ERR_CLOSE_FILE)  \
    {                                                             \
        printf ("Fail to close the file \"log_file.txt\"\n\n");   \
        exit (11);                                                \
    }                                                             \
}


#endif  // INCLUDE_FILEOPERATIONS_H
