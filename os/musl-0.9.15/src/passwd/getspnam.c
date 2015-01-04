#include "pwf.h"

#define LINE_LIM 256

struct spwd *getspnam(const char *name)
{
    static struct spwd sp = {0};
    static char *line = NULL;
	struct spwd *res;
	int e;

	if (!line) line = malloc(LINE_LIM);
	if (!line) return 0;
	e = getspnam_r(name, &sp, line, LINE_LIM, &res);
	if (e) errno = e;
	return res;
}
