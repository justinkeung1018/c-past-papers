#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "image.h"
#include "dragon.h"

#define LEVEL_0_GREY_VALUE 100
#define LEVEL_1_GREY_VALUE 120
#define LEVEL_2_GREY_VALUE 150
#define LEVEL_3_GREY_VALUE 180
#define LEVEL_4_GREY_VALUE 200
#define DEFAULT_GREY_VALUE 255

#define DRAGON_AXIOM "FX"
#define DRAGON_X     "X+YF+"
#define DRAGON_Y     "-FX-Y"
#define DRAGON_SCALE 1
#define DRAGON_PATH "../output/jurassicdragon.pgm"

#define TWINDRAGON_AXIOM "FX+FX+"
#define TWINDRAGON_X     "X+YF"
#define TWINDRAGON_Y     "FX-Y"
#define TWINDRAGON_SCALE 2
#define TWINDRAGON_PATH "../output/twindragon.pgm"

#define NUM_ORIENTATIONS 8

/* x, y: coordinates of turtle */
static long x, y;

/* When drawing a pixel to the image, x and y must be divided by this value.
 * This enables both the dragon curve and twin dragon to rendered without
 * clipping.
 */
static long scale;

/* drawn_pixels: the length of the path travelled. */
static long drawn_pixels;

/* direction: the current direction of the turtle. */
static vector_t direction;

/* Returns a vector that describes the initial direction of the turtle. Each
 * iteration corresponds to a 45 degree rotation of the turtle anti-clockwise.  */
vector_t starting_direction(int total_iterations)
{
  static vector_t direction_map[] = {
    { 1, 0 },
    { 1, 1 },
    { 0, 1 },
    { -1, 1 },
    { -1, 0 },
    { -1, -1 },
    { 0, -1 },
    { 1, -1 },
  };

  return direction_map[total_iterations % NUM_ORIENTATIONS];
}

/* Draws a pixel to dst at location (x, y). The pixel intensity is chosen as a
 * function of image size and the number of pixels drawn.
 *
 * The gray level changes over entire size of path; the pixel value oscillates
 * along the path to give some contrast between segments close to each other
 * spatially.
 */
void draw_greyscale(image_t *dst, long x, long y)
{
  int level = LEVEL * drawn_pixels / (dst->height * dst->height);

  uint8_t colour;

  switch (level) {
    case 0:
      colour = LEVEL_0_GREY_VALUE;
      break;

    case 1:
      colour = LEVEL_1_GREY_VALUE;
      break;

    case 2:
      colour = LEVEL_2_GREY_VALUE;
      break;

    case 3:
      colour = LEVEL_3_GREY_VALUE;
      break;

    case 4:
      colour = LEVEL_4_GREY_VALUE;
      break;

    default:
      colour = DEFAULT_GREY_VALUE;
      break;
  }

  set_pixel(dst, x, y, colour);
}

/* Iterates though the characters of str, recusively calling string_iteration()
 * until rules have been applied iterations times, or no rule is applicable.
 * and update the image.
 */
void string_iteration(image_t *dst, const char *str, int iterations)
{
  if (iterations < 0) {
    return;
  }

  for (int i = 0; i < strlen(str); i++) {
    long temp;
    switch (str[i]) {
      case 'F':
        drawn_pixels++;
        draw_greyscale(dst, x / scale, y / scale);
        x += direction.dx;
        y += direction.dy;
        break;

      case 'X':
        string_iteration(dst, TWINDRAGON_X, iterations - 1);
        break;

      case 'Y':
        string_iteration(dst, TWINDRAGON_Y, iterations - 1);
        break;

      case '+':
        temp = direction.dx;
        direction.dx = direction.dy;
        direction.dy = -temp;
        break;

      case '-':
        temp = direction.dx;
        direction.dx = -direction.dy;
        direction.dy = temp;
        break;
    }
  }
}

/* Creates an image of requested size, calls starting_direction() to compute
 * initial turtle direction then calls string_iteration() to construct the
 * image. The constructed image is saved to a file in the output directory.
 */
void dragon(long size, int total_iterations)
{
  image_t *dst;
  image_error_t init_status = init_image(&dst, 1.5 * size, size, 1, 255);

  if (init_status != IMG_OK) {
    image_print_error(init_status);
    exit(EXIT_FAILURE);
  }

  drawn_pixels = 0;
  x = size;
  y = size;
  scale = TWINDRAGON_SCALE;
  direction = starting_direction(total_iterations);

  string_iteration(dst, TWINDRAGON_AXIOM, total_iterations);

  image_error_t write_status = image_write(TWINDRAGON_PATH, dst, PGM_FORMAT);

  if (write_status != IMG_OK) {
    image_print_error(write_status);
    image_free(dst);
    exit(EXIT_FAILURE);
  }

  image_free(dst);
}

/* The main function. When called with an argument, this should be considered
 * the number of iterations to execute. Otherwise, it is assumed to be 9. Image
 * size is computed from the number of iterations then dragon() is used to
 * generate the dragon image. */
int main(int argc, char **argv)
{
  int num_iterations;

  if (argc == 2) {
    num_iterations = atoi(argv[1]);
  } else {
    num_iterations = 9;
  }

  long size = 1;
  for (int i = 0; i < num_iterations; i++) {
    size *= 2;
  }

  dragon(size, 2 * num_iterations);

	return EXIT_SUCCESS;
}

