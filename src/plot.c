#include "plot.h"
#include "colorscheme.h"
#include "common_function.h"
#include "global.h"
#include "text.h"

char const plot_background_png[] = "res/plot_back.png";

struct plot {
  SDL_Rect position;
  SDL_Texture *background;
  struct text *name;

  struct fft {
    float *data;
    size_t length;
    float dx;
    float x0;
  } fft;
};

struct plot *plot_build(SDL_Point position, char const *name) {

  SDL_Surface *sur = IMG_Load(plot_background_png);
  if (sur == NULL) {
    display_error_img("Can't load plot_back.png");
    return NULL;
  }

  SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, sur);
  if (tex == NULL) {
    display_error_sdl("Can't create texture from surface plot_back");
    SDL_FreeSurface(sur);
    return NULL;
  }

  struct plot *new_plot = malloc(sizeof *new_plot);
  new_plot->position.x = position.x;
  new_plot->position.y = position.y;
  new_plot->position.w = sur->w;
  new_plot->position.h = sur->h;
  new_plot->background = tex;
  new_plot->fft.dx = 0.0f;
  new_plot->fft.x0 = 0.0f;
  new_plot->fft.length = 0;
  new_plot->fft.data = malloc(sizeof(float) * 1);

  int32_t const plot_size_name = 20;
  SDL_Rect pos_name = {.x = position.x + 9, .y = position.y - 11};
  new_plot->name = text_build(text_get_font_type(TEXT_FONT_BOLD),
                              plot_size_name, COLOR_PLOT_NAME, pos_name, name);

  SDL_FreeSurface(sur);

  return new_plot;
}

void plot_free(struct plot *plot) {
  if (plot == NULL) {
    printf("plot == NULL");
    return;
  }

  SDL_DestroyTexture(plot->background);
  plot->background = NULL;

  free(plot->fft.data);
  text_free(plot->name);

  free(plot);
  plot = NULL;
}

int plot_get_pos_w(struct plot *plot) { return plot->position.w; }

int plot_get_pos_h(struct plot *plot) { return plot->position.h; }

int plot_get_pos_x(struct plot *plot) { return plot->position.x; }

int plot_get_pos_y(struct plot *plot) { return plot->position.y; }

struct text const *plot_get_name(struct plot *plot) {
  return plot->name;
}

void plot_fft_update(struct plot *plot, float *data, int length, float dx,
                     float x0) {
  free(plot->fft.data);
  plot->fft.data = malloc(sizeof *data * length);

  for (size_t i = 0; i < length; ++i) {
    plot->fft.data[length - 1 - i] = data[i];
  }

  plot->fft.length = length;
  plot->fft.dx = dx;
  plot->fft.x0 = x0;
}
