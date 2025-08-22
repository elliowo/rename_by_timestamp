#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <glob.h>
#include <unistd.h>

void print_usage(const char *program_name) {
    fprintf(stderr, "Usage: %s <filepath>\n", program_name);
}

int is_digit(char c) {
    return (c >= '0' && c <= '9');
}

int is_timestamp_formatted(const char *filename) {
  if (strlen(filename) < 19) return 0;

  for (int i = 0; i < 4; i++) {
    if (!is_digit(filename[i])) return 0;
  }

  if (filename[4] != '-' || filename[7] != '-' || filename[10] != '_' ||
      filename[13] != '-' || filename[16] != '-') return 0;

  for (int i = 5; i < 19; i += 3) {
    if (!is_digit(filename[i]) || !is_digit(filename[i + 1])) return 0;
  }

  return 1;
}

int process_file(const char *filepath) {
    struct stat st;
    if (stat(filepath, &st) == -1) {
	perror("stat");
	return 1;
    }
    if (S_ISDIR(st.st_mode)) {
	printf("Skipping directory: %s\n", filepath);
	return 0;
    }

    time_t t = st.st_mtime;
    struct tm *tm_info = localtime(&t);
    char timestamp[25];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d_%H-%M-%S", tm_info);

    const char *last_slash = strrchr(filepath, '/');
    const char *filename = last_slash ? last_slash + 1 : filepath;

    if (is_timestamp_formatted(filename)) {
	char existing[25];
	strncpy(existing, filename, 19);
	existing[19] = '\0';
	if (strcmp(existing, timestamp) == 0) {
	    printf("File is already named with the correct timestamp: %s\n", filepath);
	    return 0;
	}
    }

    char new_name[256];
    const char *dot = strrchr(filepath, '.');
    if (dot && *(dot + 1)) {
	snprintf(new_name, sizeof(new_name), "%s%.*s", timestamp, (int)(dot - filepath), filepath);
	strcat(new_name, dot);
    } else {
	strcpy(new_name, timestamp);
    }

    if (rename(filepath, new_name) == -1) {
	perror("rename");
	return 1;
    }

    printf("File renamed to: %s\n", new_name);
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
      //fprintf(stderr, "Error: Invalid number of arguments\n");
	print_usage(argv[0]);
	return 1;
    }

    for (int i = 1; i < argc; i++) {
	const char *pattern = argv[i];
	int has_wildcard = strpbrk(pattern, "*?[") != NULL;

	if (has_wildcard) {
	    glob_t g;
	    if (glob(pattern, GLOB_TILDE, NULL, &g) == 0) {
		for (size_t j = 0; j < g.gl_pathc; j++) {
		    struct stat st;
		    if (stat(g.gl_pathv[j], &st) == 0 && S_ISREG(st.st_mode)) {
			process_file(g.gl_pathv[j]);
		    }
		}
		globfree(&g);
	    }
	} else {
	    process_file(pattern);
	}
    }

    return 0;
}
