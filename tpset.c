#include <dirent.h>
#include <getopt.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *get_trackpoint_path() {
  FILE *pipe_stream =
      popen("find /sys/devices/platform/i8042 -name name | xargs grep -Fl "
            "TrackPoint | sed 's/\\/input\\/input[0-9]*\\/name$//'",
            "r");
  char path[2048];
  if (pipe_stream == NULL) {
    printf("Failed to run command to find trackpoint path");
    return NULL;
  }
  fgets(path, sizeof(path), pipe_stream);
  pclose(pipe_stream);
  char *path_ptr = path;
  return path_ptr;
}

int set_nipple_sensitivity(int sensitivity) {
  if (sensitivity <= 0) {
    printf("Invalid value %d must be greater than 0", sensitivity);
    return 1;
  }

  char command[100];
  sprintf(command, "echo %d | sudo tee /sys/devices/platform/i8042/serio1/serio2/sensitivity", sensitivity);
  return system(command);
}

int set_nipple_speed(int speed) {
  if (speed <= 0) {
    printf("Invalid value %d speed must be greater than 0", speed);
    return 1;
  }

  char command[100];
  sprintf(command, "echo %d | sudo tee %s/serio2/speed", speed, get_trackpoint_path());
  pritpset$ make
gcc -o tpset tpset.c
froogal@froogal-thinkpad:~/programming/c/tpset$ ./tpset --speed 30
Ran: echo 30 | sudo tee /sys/devices/platform/i8042/serio1
[sudo] password for froogal: ntf("Ran: %s", command);
  return system(command);
}

int main(int argc, char *argv[]) {
  int c;
  int digit_optind = 0;

  int sensitivity;
  int speed;

  while (1) {
    int this_option_optind = optind ? optind : 1;
    int option_index = 0;

    static struct option long_opts[] = {{"help", no_argument, 0, 'h'},
                                        {"version", no_argument, 0, 'v'},
                                        {"sensitivity", required_argument, 0, 's'},
                                        {"speed", required_argument, 0, 'c'}}; // celeritas latin for speed

    c = getopt_long(argc, argv, "s:hv", long_opts, &option_index);

    if (c == -1)
      break;

    // Parse args
    switch (c) {
    case 'h':
      printf("Help!\n");
      break;
    case 'v':
      printf("Version!\n");
      break;
    case 's':
      sensitivity = atoi(optarg);
      if (sensitivity == 0) {
        printf("Invalid input: %s: Must be a number\n", optarg);
        break;
      }
      set_nipple_sensitivity(sensitivity);
      printf("Nip nop set to %i\n", sensitivity);
      break;
    case 'c':
      speed = atoi(optarg);
      if (speed == 0) {
        printf("Invalid input %s: Must be a number\n", optarg);
        break;
      }
      set_nipple_speed(speed);
      printf("Nipple speed set to %i", speed);
      break;
    default:
      printf("Invalid option\n");
      break;
    }
  }
  char *path = get_trackpoint_path();

  // printf("%s\n", path);
  return 0;
}