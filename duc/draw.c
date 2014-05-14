
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <stdint.h>

#include "duc.h"
#include "cmd.h"


static int draw_main(int argc, char **argv)
{
	int c;
	char *path_db = NULL;
	int size = 800;
	char *path_out = "duc.png";
	int depth = 4;

	struct option longopts[] = {
		{ "database",       required_argument, NULL, 'd' },
		{ "levels",         required_argument, NULL, 'l' },
		{ "output",         required_argument, NULL, 'o' },
		{ "size",           required_argument, NULL, 's' },
		{ NULL }
	};

	while( ( c = getopt_long(argc, argv, "d:f:l:o:s:", longopts, NULL)) != EOF) {

		switch(c) {
			case 'd':
				path_db = optarg;
				break;
			case 'l':
				depth = atoi(optarg);
				break;
			case 'o':
				path_out = optarg;
				break;
			case 's':
				size = atoi(optarg);
				break;
			default:
				return -2;
		}
	}

	argc -= optind;
	argv += optind;
	
	char *path = ".";
	if(argc > 0) path = argv[0];

        /* Open duc context */

        duc_errno err;
        duc *duc = duc_open(path_db, DUC_OPEN_RO, &err);
        if(duc == NULL) {
                fprintf(stderr, "%s\n", duc_strerror(err));
                return -1;
        }

        duc_dir *dir = duc_opendir(duc, path);
        if(dir == NULL) {
                fprintf(stderr, "%s\n", duc_strerror(duc_error(duc)));
                return -1;
        }

	FILE *f = fopen(path_out, "w");
	if(f == NULL) {
		return -1;
	}

	duc_graph(dir, size, depth, f);

	duc_closedir(dir);
	duc_close(duc);

	return 0;
}


	
struct cmd cmd_draw = {
	.name = "draw",
	.description = "Draw graph",
	.usage = "[options] [PATH]",
	.help = 
		"  -d, --database=ARG      use database file ARG [~/.duc.db]\n"
	        "  -l, --levels=ARG        draw up to ARG levels deep [4]\n"
		"  -o, --output=ARG        output file name [duc.png]\n"
	        "  -s, --size=ARG          image size [800]\n",
	.main = draw_main
};

/*
 * End
 */

