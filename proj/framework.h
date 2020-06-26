typedef struct window_s
{
  int width;
  int height;
  float asp;
} window_t;

typedef struct view_s
{
  int th;
  int ph;
  float dim;
  int fov;
} view_t;

typedef struct color_s
{
  unsigned int r: 8;
  unsigned int g: 8;
  unsigned int b: 8;
} color_t;

typedef struct player_s
{
  float x, y, z;
  float pitch, yaw;
  float speed;
} player_t;
