#include "SWGF.h"

int main()
{
 char perfomance[8];
 SWGF::Common::Timer timer;
 SWGF::Misc::Multimedia media;
 SWGF::Input::Keyboard keyboard;
 SWGF::Input::Gamepad gamepad;
 SWGF::Input::Mouse mouse;
 SWGF::Graphics::Screen screen;
 SWGF::Graphics::Background space;
 SWGF::Graphics::Sprite ship;
 SWGF::Graphics::Text text;
 SWGF::Tools::enable_logging("log.txt");
 keyboard.initialize();
 screen.initialize();
 space.load("space.tga");
 space.prepare(screen);
 ship.load("ship.tga",SWGF::HORIZONTAL_ANIMATED,2);
 ship.set_position(screen.get_width()/2,screen.get_height()/2);
 text.load_font("font.tga");
 text.set_position(text.get_font_width(),text.get_font_height());
 mouse.hide();
 timer.set_timer(1);
 memset(perfomance,0,8);
 media.load("space.mp3");
 while(screen.sync())
 {
  gamepad.update();
  media.play_loop();
  if (mouse.check_press(SWGF::MOUSE_LEFT)==true)
  {
   break;
  }
  if (keyboard.check_hold(57)==true)
  {
   break;
  }
  if (keyboard.check_hold(72)==true)
  {
   ship.decrease_y(2);
  }
  if (keyboard.check_hold(80)==true)
  {
   ship.increase_y(2);
  }
  if (keyboard.check_hold(75)==true)
  {
   ship.decrease_x(2);
  }
  if (keyboard.check_hold(77)==true)
  {
   ship.increase_x(2);
  }
  if (gamepad.check_hold(SWGF::GAMEPAD_BUTTON2)==true)
  {
   break;
  }
  switch (gamepad.get_dpad())
  {
   case SWGF::GAMEPAD_UP:
   ship.decrease_y();
   break;
   case SWGF::GAMEPAD_DOWN:
   ship.increase_y();
   break;
   case SWGF::GAMEPAD_UPLEFT:
   ship.decrease_y();
   ship.decrease_x();
   break;
   case SWGF::GAMEPAD_UPRIGHT:
   ship.decrease_y();
   ship.increase_x();
   break;
   case SWGF::GAMEPAD_DOWNLEFT:
   ship.increase_y();
   ship.decrease_x();
   break;
   case SWGF::GAMEPAD_DOWNRIGHT:
   ship.increase_y();
   ship.increase_x();
   break;
   case SWGF::GAMEPAD_LEFT:
   ship.decrease_x();
   break;
   case SWGF::GAMEPAD_RIGHT:
   ship.increase_x();
   break;
   default:
   ;
   break;
  }
  if (gamepad.get_stick_x(SWGF::GAMEPAD_LEFT_STICK)==SWGF::GAMEPAD_NEGATIVE_DIRECTION)
  {
   ship.decrease_x();
  }
  if (gamepad.get_stick_x(SWGF::GAMEPAD_LEFT_STICK)==SWGF::GAMEPAD_POSITIVE_DIRECTION)
  {
   ship.increase_x();
  }
  if (gamepad.get_stick_y(SWGF::GAMEPAD_LEFT_STICK)==SWGF::GAMEPAD_NEGATIVE_DIRECTION)
  {
   ship.decrease_y();
  }
  if (gamepad.get_stick_y(SWGF::GAMEPAD_LEFT_STICK)==SWGF::GAMEPAD_POSITIVE_DIRECTION)
  {
   ship.increase_y();
  }
  if (ship.get_x()>screen.get_width())
  {
   ship.set_x(screen.get_width()/2);
  }
  if (ship.get_y()>screen.get_height())
  {
   ship.set_y(screen.get_height()/2);
  }
  itoa(screen.get_fps(),perfomance,10);
  space.draw();
  text.print(perfomance);
  ship.draw();
  if (timer.check_timer()==true)
  {
   ship.step();
  }

 }
 return 0;
}