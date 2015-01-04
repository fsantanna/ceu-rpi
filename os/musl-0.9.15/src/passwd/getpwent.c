#include "pwf.h"

static FILE *f = NULL;

void setpwent()
{
	if (f) fclose(f);
	f = 0;
}

weak_alias(setpwent, endpwent);

struct passwd *getpwent()
{
    static char *line = NULL;
    static struct passwd pw = {0};
	size_t size=0;
	if (!f) f = fopen("/etc/passwd", "rbe");
	if (!f) return 0;
	return __getpwent_a(f, &pw, &line, &size);
}

struct passwd *getpwuid(uid_t uid)
{
	struct passwd *pw;
	int errno_saved;
	setpwent();
	while ((pw=getpwent()) && pw->pw_uid != uid);
	errno_saved = errno;
	endpwent();
	errno = errno_saved;
	return pw;
}

struct passwd *getpwnam(const char *name)
{
	struct passwd *pw;
	int errno_saved;
	setpwent();
	while ((pw=getpwent()) && strcmp(pw->pw_name, name));
	errno_saved = errno;
	endpwent();
	errno = errno_saved;
	return pw;
}
