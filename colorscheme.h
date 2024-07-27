/* Taken from https://github.com/djpohly/dwl/issues/466 */
#define COLOR(hex)                                                             \
  {                                                                            \
    ((hex >> 24) & 0xFF) / 255.0f, ((hex >> 16) & 0xFF) / 255.0f,              \
        ((hex >> 8) & 0xFF) / 255.0f, (hex & 0xFF) / 255.0f                    \
  }

enum {
  ROSEWATER,
  FLAMINGO,
  PINK,
  MAUVE,
  RED,
  MAROON,
  PEACH,
  YELLOW,
  GREEN,
  TEAL,
  SKY,
  SAPPHIRE,
  BLUE,
  LAVENDER,
  TEXT,
  SUBTEXT1,
  SUBTEXT0,
  OVERLAY2,
  OVERLAY1,
  OVERLAY0,
  SURFACE2,
  SURFACE1,
  SURFACE0,
  BASE,
  MANTLE,
  CRUST,
  LAST_COLOR
};

enum { CATPPUCCIN_MOCHA, CATPPUCCIN_LATTE };

static const float colorschemes[][LAST_COLOR][4] = {
    [CATPPUCCIN_MOCHA] =
        {
            [ROSEWATER] = COLOR(0xf5e0dcff), [FLAMINGO] = COLOR(0xf2cdcdff),
            [PINK] = COLOR(0xf5c2e7ff),      [MAUVE] = COLOR(0xcba6f7ff),
            [RED] = COLOR(0xf38ba8ff),       [MAROON] = COLOR(0xeba0acff),
            [PEACH] = COLOR(0xfab387ff),     [YELLOW] = COLOR(0xf9e2afff),
            [GREEN] = COLOR(0xa6e3a1ff),     [TEAL] = COLOR(0x94e2d5ff),
            [SKY] = COLOR(0x89dcebff),       [SAPPHIRE] = COLOR(0x74c7ecff),
            [BLUE] = COLOR(0x89b4faff),      [LAVENDER] = COLOR(0xb4befeff),
            [TEXT] = COLOR(0xcdd6f4ff),      [SUBTEXT1] = COLOR(0xbac2deff),
            [SUBTEXT0] = COLOR(0xa6adc8ff),  [OVERLAY2] = COLOR(0x9399b2ff),
            [OVERLAY1] = COLOR(0x7f849cff),  [OVERLAY0] = COLOR(0x6c7086ff),
            [SURFACE2] = COLOR(0x585b70ff),  [SURFACE1] = COLOR(0x45475aff),
            [SURFACE0] = COLOR(0x313244ff),  [BASE] = COLOR(0x1e1e2eff),
            [MANTLE] = COLOR(0x181825ff),    [CRUST] = COLOR(0x11111bff),
        },
    [CATPPUCCIN_LATTE] = {
        [ROSEWATER] = COLOR(0xdc8a78ff), [FLAMINGO] = COLOR(0xdd7878ff),
        [PINK] = COLOR(0xea76cbff),      [MAUVE] = COLOR(0x8839efff),
        [RED] = COLOR(0xd20f39ff),       [MAROON] = COLOR(0xe64553ff),
        [PEACH] = COLOR(0xfe640bff),     [YELLOW] = COLOR(0xdf8e1dff),
        [GREEN] = COLOR(0x40a02bff),     [TEAL] = COLOR(0x179299ff),
        [SKY] = COLOR(0x04a5e5ff),       [SAPPHIRE] = COLOR(0x209fb5ff),
        [BLUE] = COLOR(0x1e66f5ff),      [LAVENDER] = COLOR(0x7287fdff),
        [TEXT] = COLOR(0x4c4f69ff),      [SUBTEXT1] = COLOR(0x5c5f77ff),
        [SUBTEXT0] = COLOR(0x6c6f85ff),  [OVERLAY2] = COLOR(0x7c7f93ff),
        [OVERLAY1] = COLOR(0x8c8fa1ff),  [OVERLAY0] = COLOR(0x9ca0b0ff),
        [SURFACE2] = COLOR(0xacb0beff),  [SURFACE1] = COLOR(0xbcc0ccff),
        [SURFACE0] = COLOR(0xccd0daff),  [BASE] = COLOR(0xeff1f5ff),
        [MANTLE] = COLOR(0xe6e9efff),    [CRUST] = COLOR(0xdce0e8ff),

    }};

static const int colorscheme = CATPPUCCIN_MOCHA;
