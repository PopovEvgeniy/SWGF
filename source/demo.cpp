#include "SWGF.h"

int main()
{
 char perfomance[8];
 SWGF::Common::Timer timer;
 SWGF::Input::Keyboard keyboard;
 SWGF::Input::Gamepad gamepad;
 SWGF::Input::Mouse mouse;
 SWGF::Misc::Multimedia media;
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
 media.initialize();
 media.load("space.mp3");
 memset(perfomance,0,8);
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
  if (gamepad.check_button_hold(SWGF::GAMEPAD_A)==true)
  {
   break;
  }
  if (gamepad.check_button_hold(SWGF::GAMEPAD_UP)==true)
  {
   ship.decrease_y();
  }
  if (gamepad.check_button_hold(SWGF::GAMEPAD_DOWN)==true)
  {
   ship.increase_y();
  }
  if (gamepad.check_button_hold(SWGF::GAMEPAD_LEFT)==true)
  {
   ship.decrease_x();
  }
  if (gamepad.check_button_hold(SWGF::GAMEPAD_RIGHT)==true)
  {
   ship.increase_x();
  }

  if (gamepad.get_left_stick_x()==SWGF::GAMEPAD_NEGATIVE_DIRECTION)
  {
   ship.decrease_x();
  }
  if (gamepad.get_left_stick_x()==SWGF::GAMEPAD_POSITIVE_DIRECTION)
  {
   ship.increase_x();
  }
  if (gamepad.get_left_stick_y()==SWGF::GAMEPAD_NEGATIVE_DIRECTION)
  {
   ship.decrease_y();
  }
  if (gamepad.get_left_stick_y()==SWGF::GAMEPAD_POSITIVE_DIRECTION)
  {
   ship.increase_y();
  }
  if (gamepad.check_trigger_hold(SWGF::GAMEPAD_LEFT_TRIGGER)==true)
  {
   gamepad.set_vibration(USHRT_MAX,USHRT_MAX);
  }
  if (gamepad.check_trigger_hold(SWGF::GAMEPAD_RIGHT_TRIGGER)==true)
  {
   gamepad.disable_vibration();
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