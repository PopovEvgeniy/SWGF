#include "SWGF.h"

#define MAP_WIDTH 16
#define MAP_HEIGHT 16

unsigned char MAP_ROWS[MAP_WIDTH]={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
unsigned char MAP_COLUMNS[MAP_HEIGHT]={2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2};

SWGF::Tileset tilemap;

void draw_map()
{
 unsigned long int x,y;
 size_t row,column;
 x=0;
 y=0;
 for (row=0;row<MAP_WIDTH;++row)
 {
  x=(unsigned long int)row*tilemap.get_tile_width();
  for (column=0;column<MAP_HEIGHT;++column)
  {
   y=(unsigned long int)column*tilemap.get_tile_height();
   tilemap.select_tile(MAP_ROWS[row],MAP_COLUMNS[column]);
   tilemap.draw_tile(x,y);
  }

 }

}

int main(void)
{
 SWGF::Screen screen;
 SWGF::Sprite font;
 SWGF::Text text;
 SWGF::Image image;
 SWGF::Keyboard keyboard;
 font.initialize(screen.get_handle());
 tilemap.initialize(screen.get_handle());
 keyboard.initialize();
 image.load_tga("font.tga");
 font.load_image(image);
 text.load_font(font.get_handle());
 text.set_position(0,0);
 image.load_tga("grass.tga");
 tilemap.load_tileset(image,6,3);
 screen.initialize();
 draw_map();
 screen.save();
 while (screen.sync()==false)
 {
  if (keyboard.check_hold(1)==true) break;
  screen.restore();
  text.draw_text("Press Esc to exit");
 }
 return 0;
}