/*
 * Userspace program that communicates with the vga_ball device driver
 * through ioctls
 *
 * Stephen A. Edwards
 * Columbia University
 */

#include <stdio.h>
#include "vga_ball.h"
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int vga_ball_fd;

/* Read and print the background color */
void get_ball_coords(unsigned short *x, unsigned short *y) {
  vga_ball_arg_t vla;
  
  if (ioctl(vga_ball_fd, VGA_BALL_READ_COORDS, &vla)) {
      perror("ioctl(VGA_BALL_READ_COORDS) failed");
      return;
  }
  *x = vla.coords.x;
  *y = vla.coords.y;
  printf("Current ball coordinates: %u, %u\n", *x, *y);
}

/* Set the background color */
void set_ball_coords(unsigned short x, unsigned short y)
{
  vga_ball_arg_t vla;
  vla.coords.x = x;
  vla.coords.y = y;
  if (ioctl(vga_ball_fd, VGA_BALL_WRITE_COORDS, &vla)) {
      perror("ioctl(VGA_BALL_SET_COORDS) failed");
      return;
  }
}

int main()
{
  vga_ball_arg_t vla;
  int i;
  static const char filename[] = "/dev/vga_ball";

  unsigned short x =320, y= 240;
  short vx = 2, vy = 1;

  const unsigned short x_min = 20, x_max = 620; // horizontal boundaries
  const unsigned short y_min = 20, y_max = 460; // vertical boundaries

  
  printf("VGA ball Userspace program started\n");


  if ( (vga_ball_fd = open(filename, O_RDWR)) == -1) {
    fprintf(stderr, "could not open %s\n", filename);
    return -1;
  }

  while (1) {
    set_ball_coords(x,y);

    x += vx;
    y += vy;

    if(x <= x_min || x >= x_max) vx = -vx;

    if(y <= y_min || y >= y_max) vy= -vy;
    usleep(22000); //16000
  }

  
  printf("VGA BALL Userspace program terminating\n");
  close(vga_ball_fd);
  return 0;
}
