#include "SWGF.h"

int main(void)
{
 long int x,y,screen_width,screen_height;
 unsigned char frame;
 unsigned long int fps;
 char perfomance[8];
 SWGF_Timer timer;
 SWGF_Screen screen;
 SWGF_System System;
 SWGF_Keyboard keyboard;
 SWGF_Gamepad gamepad;
 SWGF_Mouse mouse;
 SWGF_Multimedia media;
 SWGF_Image image;
 SWGF_Background space;
 SWGF_Sprite ship,font;
 SWGF_Text text;
 System.enable_logging("log.txt");
 keyboard.initialize();
 screen.initialize();
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
 text.set_position(font.get_sprite_width(),font.get_sprite_width());
 ship.set_frames(2);
 screen.clear_screen();
 space.resize_image(screen_width,screen_height);
 frame=1;
 mouse.hide();
 timer.set_timer(1);
 media.initialize();
 media.load("space.mp3");
 fps=0;
 memset(perfomance,0,8);
 while(screen.sync()==false)
 {
  fps++;
  gamepad.update();
  if(media.check_playing()==false) media.play();
  if(mouse.check_press(SWGF_MOUSE_LEFT)==true) break;
  if(keyboard.check_hold(57)==true) break;
  if(keyboard.check_hold(72)==true) y-=2;
  if(keyboard.check_hold(80)==true) y+=2;
  if(keyboard.check_hold(75)==true) x-=2;
  if(keyboard.check_hold(77)==true) x+=2;
  if(keyboard.check_press(71)==true) ship.mirror_image(0);
  if(keyboard.check_press(79)==true) ship.mirror_image(1);
  if(gamepad.check_hold(SWGF_GAMEPAD_BUTTON2)==true) break;
  if(gamepad.check_press(SWGF_GAMEPAD_BUTTON4)==true) ship.mirror_image(0);
  if(gamepad.check_press(SWGF_GAMEPAD_BUTTON3)==true) ship.mirror_image(1);
  switch (gamepad.get_dpad())
  {
   case SWGF_GAMEPAD_UP:
   y--;
   break;
   case SWGF_GAMEPAD_DOWN:
   y++;
   break;
   case SWGF_GAMEPAD_UPLEFT:
   y--;
   x--;
   break;
   case SWGF_GAMEPAD_UPRIGHT:
   y--;
   x++;
   break;
   case SWGF_GAMEPAD_DOWNLEFT:
   y++;
   x--;
   break;
   case SWGF_GAMEPAD_DOWNRIGHT:
   y++;
   x++;
   break;
   case SWGF_GAMEPAD_LEFT:
   x--;
   break;
   case SWGF_GAMEPAD_RIGHT:
   x++;
   break;
  }
  if(gamepad.get_stick_x(SWGF_GAMEPAD_LEFT_STICK)==-1) x--;
  if(gamepad.get_stick_x(SWGF_GAMEPAD_LEFT_STICK)==1) x++;
  if(gamepad.get_stick_y(SWGF_GAMEPAD_LEFT_STICK)==-1) y--;
  if(gamepad.get_stick_y(SWGF_GAMEPAD_LEFT_STICK)==1) y++;
  if((x<=0)||(x>=screen_width)) x=screen_width/2;
  if((y<=0)||(y>=screen_height)) y=screen_height/2;
  space.draw_background();
  text.draw_text(perfomance);
  ship.draw_sprite_frame(x,y,frame);
  if (timer.check_timer()==true)
  {
   itoa(fps,perfomance,10);
   fps=0;
   frame++;
   if (frame>ship.get_frames()) frame=1;
  }

 }
 return 0;
}