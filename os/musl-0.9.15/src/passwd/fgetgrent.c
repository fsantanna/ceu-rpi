#define _GNU_SOURCE
#include "pwf.h"

struct group *fgetgrent(FILE *f)
{
    static char *line=NULL, **mem=NULL;
    static struct group gr = { 0 };
	size_t size=0, nmem=0;
	return __getgrent_a(f, &gr, &line, &size, &mem, &nmem);
}
