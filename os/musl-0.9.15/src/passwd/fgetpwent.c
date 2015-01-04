#define _GNU_SOURCE
#include "pwf.h"

struct passwd *fgetpwent(FILE *f)
{
    static char *line = NULL;
    static struct passwd pw = {0};
	size_t size=0;
	return __getpwent_a(f, &pw, &line, &size);
}
