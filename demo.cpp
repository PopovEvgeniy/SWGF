#include "SWGF.h"

int main(void)
{
 long int x,y,screen_width,screen_height;
 unsigned char frame;
 SWGF_Timer timer;
 SWGF_Screen screen;
 SWGF_Keyboard keyboard;
 SWGF_Mouse mouse;
 SWGF_Gamepad gamepad;
 SWGF_Multimedia media;
 SWGF_Image image;
 SWGF_Canvas space,ship,font;
 screen.initialize();
 space.initialize(screen.get_handle());
 ship.initialize(screen.get_handle());
 font.initialize(screen.get_handle());
 screen.set_mode(800,600);
 screen_width=screen.get_frame_width();
 screen_height=screen.get_frame_height();
 x=screen_width/2;
 y=screen_height/2;
 image.load_tga("space.tga",space);
 image.load_tga("ship.tga",ship);
 image.load_tga("font.tga",font);
 screen.clear_screen();
 space.resize_image(screen_width,screen_height);
 frame=1;
 timer.set_timer(1);
 media.load("space.mp3");
 media.play();
 while(screen.begin_sync()==false)
 {
  if(media.check_playing()==false) media.play();
  if(mouse.get_pushed_button()==SWGF_MOUSE_LEFT) break;
  if(keyboard.check_push(57)==true) break;
  if(keyboard.check_push(72)==true) y-=2;
  if(keyboard.check_push(80)==true) y+=2;
  if(keyboard.check_push(75)==true) x-=2;
  if(keyboard.check_push(77)==true) x+=2;
  if(keyboard.check_push(71)==true) ship.mirror_image(0);
  if(keyboard.check_push(79)==true) ship.mirror_image(1);
  if(gamepad.check_button(SWGF_GAMEPAD_X)==true) break;
  if(gamepad.check_button(SWGF_GAMEPAD_UP)==true) y-=2;
  if(gamepad.check_button(SWGF_GAMEPAD_DOWN)==true) y+=2;
  if(gamepad.check_button(SWGF_GAMEPAD_LEFT)==true) x-=2;
  if(gamepad.check_button(SWGF_GAMEPAD_RIGHT)==true) x+=2;
  if(gamepad.check_trigger(SWGF_GAMEPAD_LEFT_TRIGGER)==true) gamepad.set_vibration(65535,65535);
  if(gamepad.check_trigger(SWGF_GAMEPAD_RIGHT_TRIGGER)==true) gamepad.set_vibration(0,0);
  if(gamepad.check_button(SWGF_GAMEPAD_A)==true) ship.mirror_image(0);
  if(gamepad.check_button(SWGF_GAMEPAD_B)==true) ship.mirror_image(1);
  if((x<=0)||(x>=screen_width)) x=screen_width/2;
  if((y<=0)||(y>=screen_height)) y=screen_height/2;
  space.draw_background();
  font.draw_text(screen_width/2,font.get_height(),"Technical demonstration");
  ship.draw_sprite_frame(x,y,frame,2);
  if (timer.check_timer()==true)
  {
   frame++;
   if (frame>2) frame=1;
  }
  screen.end_sync();
 }
 return 0;
}