// Builtins at http://fastled.io/docs/3.1/colorpalettes_8cpp_source.html
DEFINE_GRADIENT_PALETTE( flame_palette ) {
  0,     0,  0,  0,   //cold
 64,    32,  0,  0,
192,   255,  0,  0,
240,   255,  8,  0,
255,   255, 32,  0};  //hot

DEFINE_GRADIENT_PALETTE( blue_flame_palette ) {
  0,     0,  0,  0,   //min
 64,     0,  0, 32,
192,     0,  0,255,
240,     4,  4,255,
255,     8,  8,255};  //max

DEFINE_GRADIENT_PALETTE( purple_flame_palette ) {
  0,     0,  0,  0,   //min
 64,    19,  0, 32,
192,    32,  0, 32,
240,    35, 10, 55,
255,    64,  0, 64};  //max

DEFINE_GRADIENT_PALETTE( green_flame_palette ) {
  0,      0,  0,  0,   //min
 255,     0,255,  0};  //max

DEFINE_GRADIENT_PALETTE( white_flame_palette ) {
  0,      1,  1,   1,   //min
 255,     255,255,255};  //max

DEFINE_GRADIENT_PALETTE( inv_flame_palette ) {
  0,    255,  0,   0,   //min
  4,     64,  0,   0,
 255,     0,  0,   0};  //max

DEFINE_GRADIENT_PALETTE( inv_purple_flame_palette ) {
  0,     85,  0, 171,   //min
 255,     0,  0,   0};  //max
