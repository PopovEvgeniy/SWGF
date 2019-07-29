// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "SWGF.h"

int main(void)
{
 long int x,y,screen_width,screen_height;
 unsigned long int frame;
 char perfomance[8];
 SWGF::Timer timer;
 SWGF::Screen screen;
 SWGF::System System;
 SWGF::Keyboard keyboard;
 SWGF::Gamepad gamepad;
 SWGF::Mouse mouse;
 SWGF::Multimedia media;
 SWGF::Image image;
 SWGF::Background space;
 SWGF::Sprite ship,font;
 SWGF::Text text;
 System.enable_logging("log.txt");
 keyboard.initialize();
 space.initialize(screen.get_handle());
 ship.initialize(screen.get_handle());
 font.initialize(screen.get_handle());
 screen_width=screen.get_frame_width();
 screen_height=screen.get_frame_height();
 x=screen_width/2;
 y=screen_height/2;
 image.load_tga("space.tga");
 space.load_image(image);
 image.load_tga("ship.tga");
 ship.load_image(image);
 image.load_tga("font.tga");
 font.load_image(image);
 text.load_font(font.get_handle());
 text.set_position(font.get_width(),font.get_width());
 ship.set_frames(2);
 ship.set_kind(HORIZONTAL_STRIP);
 screen.clear_screen();
 space.resize_image(screen_width,screen_height);
 space.set_kind(NORMAL_BACKGROUND);
 frame=1;
 mouse.hide();
 timer.set_timer(1);
 media.initialize();
 media.load("space.mp3");
 memset(perfomance,0,8);
 screen.initialize();
 while(screen.sync())
 {
  gamepad.update();
  if(media.check_playing()==false) media.play();
  if(mouse.check_press(MOUSE_LEFT)==true) break;
  if(keyboard.check_hold(57)==true) break;
  if(keyboard.check_hold(72)==true) y-=2;
  if(keyboard.check_hold(80)==true) y+=2;
  if(keyboard.check_hold(75)==true) x-=2;
  if(keyboard.check_hold(77)==true) x+=2;
  if(keyboard.check_press(71)==true) ship.mirror_image(MIRROR_HORIZONTAL);
  if(keyboard.check_press(79)==true) ship.mirror_image(MIRROR_VERTICAL);
  if(gamepad.check_hold(GAMEPAD_BUTTON2)==true) break;
  if(gamepad.check_press(GAMEPAD_BUTTON4)==true) ship.mirror_image(MIRROR_HORIZONTAL);
  if(gamepad.check_press(GAMEPAD_BUTTON3)==true) ship.mirror_image(MIRROR_VERTICAL);
  switch (gamepad.get_dpad())
  {
   case GAMEPAD_UP:
   y--;
   break;
   case GAMEPAD_DOWN:
   y++;
   break;
   case GAMEPAD_UPLEFT:
   y--;
   x--;
   break;
   case GAMEPAD_UPRIGHT:
   y--;
   x++;
   break;
   case GAMEPAD_DOWNLEFT:
   y++;
   x--;
   break;
   case GAMEPAD_DOWNRIGHT:
   y++;
   x++;
   break;
   case GAMEPAD_LEFT:
   x--;
   break;
   case GAMEPAD_RIGHT:
   x++;
   break;
  }
  if(gamepad.get_stick_x(GAMEPAD_LEFT_STICK)==GAMEPAD_NEGATIVE_DIRECTION) x--;
  if(gamepad.get_stick_x(GAMEPAD_LEFT_STICK)==GAMEPAD_POSITIVE_DIRECTION) x++;
  if(gamepad.get_stick_y(GAMEPAD_LEFT_STICK)==GAMEPAD_NEGATIVE_DIRECTION) y--;
  if(gamepad.get_stick_y(GAMEPAD_LEFT_STICK)==GAMEPAD_POSITIVE_DIRECTION) y++;
  if((x<=0)||(x>=screen_width)) x=screen_width/2;
  if((y<=0)||(y>=screen_height)) y=screen_height/2;
  itoa(screen.get_fps(),perfomance,10);
  space.draw_background();
  text.draw_text(perfomance);
  ship.set_target(frame);
  ship.set_position(x,y);
  ship.draw_sprite();
  if (timer.check_timer()==true)
  {
   frame++;
   if (frame>ship.get_frames()) frame=1;
  }

 }
 return 0;
}