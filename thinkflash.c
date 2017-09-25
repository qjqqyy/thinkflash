#include <sys/types.h>
#include <sys/sysctl.h>

#include <err.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <unistd.h>

#define	THINKLIGHT_OID	"dev.acpi_ibm.0.thinklight"
#define	MIB_LENGTH	4
#define	I_HAVE_EPILEPSY	".OH_SHIT_I_HAVE_EPILEPSY"

void	toggle_thinklight(int *, size_t);
void	usage();

int
main(int argc, char *argv[])
{
	unsigned int	mseconds;
	int		mib[MIB_LENGTH];
	size_t		miblen;
	const char     *errstr;
	char	       *epilepsy_path;

	if (argc <= 1)
		usage();
	asprintf(&epilepsy_path, "%s/" I_HAVE_EPILEPSY, getenv("HOME"));
	if (epilepsy_path == NULL)
		err(EX_OSERR, "asprintf");
	if (access(epilepsy_path, F_OK) == 0)
		exit(0);	/* disabled */
	free(epilepsy_path);

	mseconds = strtonum(argv[1], 1, UINT_MAX, &errstr);
	if (errstr != NULL) {
		warn("strtonum");
		usage();
	}
	if (geteuid() != 0) {
		fprintf(stderr, "%s requires root privileges, trying anyway\n",
		    getprogname());
	}
	/*
	 * every invocation is 2 toggles so that even if we get spammed the
	 * even number of calls cancel each other out
	 */
	miblen = sizeof(mib);
	if (sysctlnametomib(THINKLIGHT_OID, mib, &miblen) == -1)
		err(EX_OSERR, "unknown oid '%s', is acpi_ibm module loaded?",
		    THINKLIGHT_OID);
	toggle_thinklight(mib, miblen);
	usleep(mseconds * 1000);
	toggle_thinklight(mib, miblen);
	exit(0);
}

void
toggle_thinklight(int *mib, size_t miblen)
{
	int	v;
	size_t	len;

	len = sizeof(v);
	/* get value */
	if (sysctl(mib, miblen, &v, &len, NULL, 0) == -1)
		err(EX_OSERR, "sysctl");
	v = !v;	/* flip it */
	if (sysctl(mib, miblen, NULL, NULL, &v, sizeof(v)) == -1)
		err(EX_OSERR, "sysctl");
}

void
usage()
{

	fprintf(stderr, "usage: %s [milliseconds]\n", getprogname());
	fprintf(stderr, "\tif file '~/" I_HAVE_EPILEPSY "' exists, "
	    "%s will exit without doing anything\n", getprogname());
	exit(EX_USAGE);
}
