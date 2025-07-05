#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

void execute_grep(char *pattern, char *file) {
  if (pattern == NULL || file == NULL) {
    fprintf(stderr, "grep: expected pattern and file\n");
    return;
  }

  FILE *fp = fopen(file, "r");
  if (fp == NULL) {
    perror("grep");
    return;
  }

  char line[1024];
  while (fgets(line, sizeof(line), fp) != NULL) {
    if (strstr(line, pattern) != NULL) {
      printf("%s", line);
    }
  }

  fclose(fp);
}

void execute_mv(char *src, char *dest) {
  if (src == NULL || dest == NULL) {
    fprintf(stderr, "mv: expected source and destination\n");
    return;
  }

  if (rename(src, dest) != 0) {
    perror("mv");
  }
}

void execute_cp(char *src, char *dest) {
  if (src == NULL || dest == NULL) {
    fprintf(stderr, "cp: expected source and destination\n");
    return;
  }

  FILE *src_file = fopen(src, "r");
  if (src_file == NULL) {
    perror("cp");
    return;
  }

  FILE *dest_file = fopen(dest, "w");
  if (dest_file == NULL) {
    perror("cp");
    fclose(src_file);
    return;
  }

  char buffer[1024];
  size_t bytes_read;
  while ((bytes_read = fread(buffer, 1, sizeof(buffer), src_file)) > 0) {
    fwrite(buffer, 1, bytes_read, dest_file);
  }

  fclose(src_file);
  fclose(dest_file);
}

void split_input(char *input, char **tokens) {
  int i = 0;
  tokens[i] = strtok(input, " \n");  // Split input by spaces and newline
  while (tokens[i] != NULL) {
    i++;
    tokens[i] = strtok(NULL, " \n");
  }
}

void execute_ls() {
  DIR *dir;
  struct dirent *entry;

  dir = opendir(".");
  if (dir == NULL) {
    perror("ls");
    return;
  }

  while ((entry = readdir(dir)) != NULL) {
    printf("%s  ", entry->d_name);
  }
  printf("\n");
  closedir(dir);
}

void execute_pwd() {
  char cwd[1024];
  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    printf("%s\n", cwd);
  } else {
    perror("pwd");
  }
}

void execute_cd(char *path) {
  if (path == NULL) {
    fprintf(stderr, "cd: expected argument\n");
  } else {
    if (chdir(path) != 0) {
      perror("cd");
    }
  }
}

void execute_mkdir(char *path) {
  if (path == NULL) {
    fprintf(stderr, "mkdir: expected argument\n");
  } else {
    if (mkdir(path, 0777) != 0) {  // 0777 gives full permissions
      perror("mkdir");
    }
  }
}

void execute_rmdir(char *path) {
  if (path == NULL) {
    fprintf(stderr, "rmdir: expected argument\n");
  } else {
    if (rmdir(path) != 0) {
      perror("rmdir");
    }
  }
}

void execute_rm(char *path) {
  if (path == NULL) {
    fprintf(stderr, "rm: expected argument\n");
  } else {
    if (remove(path) != 0) {
      perror("rm");
    }
  }
}

void execute_touch(char *path) {
  if (path == NULL) {
    fprintf(stderr, "touch: expected argument\n");
  } else {
    FILE *file = fopen(path, "w");
    if (file == NULL) {
      perror("touch");
    } else {
      fclose(file);
    }
  }
}

int main() {
  char *input = NULL;     // Pointer for getline
  size_t input_size = 0;  // Size of the input buffer
  char *tokens[100];      // Array to store tokens (command + arguments)

  while (1) {
    printf("irtaza's-shell>> ");

    if (getline(&input, &input_size, stdin) == -1) {
      printf("\n");
      break;
    }

    split_input(input, tokens);

    if (tokens[0] != NULL && strcmp(tokens[0], "exit") == 0) {
      break;
    }

    if (tokens[0] != NULL) {
      if (strcmp(tokens[0], "cd") == 0) {
        execute_cd(tokens[1]);
        continue;
      } else if (strcmp(tokens[0], "pwd") == 0) {
        execute_pwd();
        continue;
      } else if (strcmp(tokens[0], "ls") == 0) {
        execute_ls();
        continue;
      } else if (strcmp(tokens[0], "mkdir") == 0) {
        execute_mkdir(tokens[1]);
        continue;
      } else if (strcmp(tokens[0], "rmdir") == 0) {
        execute_rmdir(tokens[1]);
        continue;
      } else if (strcmp(tokens[0], "rm") == 0) {
        execute_rm(tokens[1]);
        continue;
      } else if (strcmp(tokens[0], "touch") == 0) {
        execute_touch(tokens[1]);
        continue;
      } else if (strcmp(tokens[0], "cp") == 0) {
        execute_cp(tokens[1], tokens[2]);
        continue;
      } else if (strcmp(tokens[0], "mv") == 0) {
        execute_mv(tokens[1], tokens[2]);
        continue;
      } else if (strcmp(tokens[0], "grep") == 0) {
        execute_grep(tokens[1], tokens[2]);
        continue;
      }
    }

    pid_t pid = fork();
    if (pid == 0) {
      execvp(tokens[0], tokens);

      perror("execvp");
      exit(1);
    } else if (pid > 0) {
      wait(NULL);
    } else {
      perror("fork");
    }
  }
  free(input);

  return 0;
}