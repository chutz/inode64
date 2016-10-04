#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fts.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <linux/limits.h>

int main(int argc, char **argv)
{
	int exit_code = 0;
	int retval = 0;

	FTS *file_system;
	FTSENT *child = NULL;
	FTSENT *parent = NULL;
	char file_path[PATH_MAX];
	char real_path[PATH_MAX];

	struct stat sb;

	if (argc < 1) {
		printf("Usage: %s <path> [path] ...\n", argv[0]);
	}

	file_system = fts_open(&argv[1], FTS_PHYSICAL | FTS_NOSTAT | FTS_XDEV, NULL);

	if (file_system != NULL) {
		while((parent = fts_read(file_system)) != NULL) {
			child = fts_children(file_system, 0);

			if (child == NULL) {
				perror("fts_children");
				continue;
			}
			while ((child != NULL) && (child->fts_link != NULL)) {
				child = child->fts_link;
				snprintf(file_path, PATH_MAX, "%s/%s", child->fts_path, child->fts_name);
				if (realpath(file_path, real_path) == NULL) {
					perror(file_path);
					exit_code = 1;
				} else if ((retval = lstat(real_path, &sb)) != 0) {
					perror(file_path);
					exit_code = retval;
				} else if (sb.st_ino > UINT_MAX) {
					printf("%s\n", real_path);
				}
			}
		}
		fts_close(file_system);
	}
	return exit_code;
}
