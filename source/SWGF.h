/*
Simple windows game framework made by Popov Evgeniy Alekseyevich
Some code taken from wglext.h(https://www.khronos.org/registry/OpenGL/api/GL/wglext.h) by The Khronos Group Inc

Simple windows game framework license

Copyright (C) 2016 - 2022 Popov Evgeniy Alekseyevich

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

Third-party license

Copyright (c) 2007-2010 The Khronos Group Inc.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and/or associated documentation files (the
"Materials"), to deal in the Materials without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Materials, and to
permit persons to whom the Materials are furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Materials.

THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
*/

#ifndef SWGF_H
#define SWGF_H

#if defined _MSC_VER && _MSC_VER>=1500
  #pragma warning(disable : 4996)
#endif

#if !defined __GNUC__
 #pragma comment(lib,"kernel32.lib")
 #pragma comment(lib,"user32.lib")
 #pragma comment(lib,"gdi32.lib")
 #pragma comment(lib,"opengl32.lib")
 #pragma comment(lib,"ole32.lib")
 #pragma comment(lib,"strmiids.lib")
 #pragma comment(lib,"xinput.lib")
#endif

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <cwchar>
#include <new>
#include <windows.h>
#include <unknwn.h>
#include <dshow.h>
#include <xinput.h>
#include <GL\gl.h>

namespace SWGF
{

 typedef enum
 {
  STATIC_IMAGE=0,
  HORIZONTAL_ANIMATED=1,
  VERTICAL_ANIMATED=2
 } IMAGE_KIND;

 typedef enum
 {
  HORIZONTAL_TEXT=0,
  VERTICAL_TEXT=1
 } TEXT_KIND;

 typedef enum
 {
  MOUSE_LEFT=0,
  MOUSE_RIGHT=1,
  MOUSE_MIDDLE=2
 } MOUSE_BUTTON;

typedef enum
{
 GAMEPAD_NEUTRAL_DIRECTION=0,
 GAMEPAD_NEGATIVE_DIRECTION=-1,
 GAMEPAD_POSITIVE_DIRECTION=1
} GAMEPAD_DIRECTION;

 typedef enum
 {
  GAMEPAD_LEFT_STICK=0,
  GAMEPAD_RIGHT_STICK=1
 } GAMEPAD_STICKS;

 typedef enum
 {
  GAMEPAD_LEFT_TRIGGER=0,
  GAMEPAD_RIGHT_TRIGGER=1
 } GAMEPAD_TRIGGERS;

 typedef enum
 {
  GAMEPAD_UP=XINPUT_GAMEPAD_DPAD_UP,
  GAMEPAD_DOWN=XINPUT_GAMEPAD_DPAD_DOWN,
  GAMEPAD_LEFT=XINPUT_GAMEPAD_DPAD_LEFT,
  GAMEPAD_RIGHT=XINPUT_GAMEPAD_DPAD_RIGHT,
  GAMEPAD_A=XINPUT_GAMEPAD_A,
  GAMEPAD_B=XINPUT_GAMEPAD_B,
  GAMEPAD_X=XINPUT_GAMEPAD_X,
  GAMEPAD_Y=XINPUT_GAMEPAD_Y,
  GAMEPAD_LEFT_BUMPER=XINPUT_GAMEPAD_LEFT_SHOULDER,
  GAMEPAD_RIGHT_BUMPER=XINPUT_GAMEPAD_RIGHT_SHOULDER,
  GAMEPAD_START=XINPUT_GAMEPAD_START,
  GAMEPAD_BACK=XINPUT_GAMEPAD_BACK
 } GAMEPAD_BUTTONS;

 typedef struct
 {
  unsigned int x;
  unsigned int y;
  unsigned int width;
  unsigned int height;
 } BOX;

 void Halt(const char *message);

 namespace Internal
 {

  typedef BOOL (WINAPI * PFNWGLSWAPINTERVALEXTPROC) (int interval); // This code taken from wglext.h by The Khronos Group Inc

  LRESULT CALLBACK Process_Message(HWND window,UINT Message,WPARAM wParam,LPARAM lParam);

  class Synchronization
  {
   private:
   HANDLE timer;
   protected:
   void create_timer();
   void set_timer(const unsigned long int interval);
   void wait_timer();
   public:
   Synchronization();
   ~Synchronization();
  };

  class Display
  {
   private:
   DEVMODE display;
   protected:
   void set_video_mode();
   void get_video_mode();
   void correct_depth();
   void set_setting(const unsigned long int width,const unsigned long int height);
   unsigned long int get_depth() const;
   unsigned long int get_display_width() const;
   unsigned long int get_display_height() const;
   public:
   Display();
   ~Display();
  };

  class Engine
  {
   private:
   WNDCLASSEX window_class;
   HWND window;
   HDC context;
   void get_instance();
   void set_backgrond_color();
   void load_icon();
   void load_cursor();
   void register_window_class();
   void take_context();
   void create_window();
   protected:
   HDC get_context();
   void prepare_engine();
   bool process_message();
   public:
   Engine();
   ~Engine();
  };

  class WINGL
  {
   private:
   HDC device;
   HGLRC render;
   PIXELFORMATDESCRIPTOR setting;
   PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
   bool check_flag(const unsigned long int flag) const;
   int get_pixel_format(HDC target,const unsigned long int color);
   void set_pixel_format(const int format);
   void create_render_context();
   void disable_vsync();
   protected:
   void set_render(HDC target,const unsigned long int color);
   void Swap();
   bool is_software_render() const;
   public:
   WINGL();
   ~WINGL();
  };

 }

 namespace Resource
 {

  template <class RESOURCE>
  RESOURCE *create()
  {
   RESOURCE *target=NULL;
   try
   {
    target=new RESOURCE;
   }
   catch (...)
   {
    SWGF::Halt("Can't allocate memory");
   }
   return target;
  }

  template <class RESOURCE>
  RESOURCE *create_array(const size_t amount)
  {
   RESOURCE *target=NULL;
   try
   {
    target=new RESOURCE[amount];
   }
   catch (...)
   {
    SWGF::Halt("Can't allocate memory");
   }
   return target;
  }

  template <class RESOURCE>
  void destroy(RESOURCE *target)
  {
   if (target!=NULL)
   {
    delete target;
   }

  }

  template <class RESOURCE>
  void destroy_array(RESOURCE *target)
  {
   if (target!=NULL)
   {
    delete[] target;
   }

  }

 }

 namespace Core
 {

  typedef struct
 {
  int x;
  int y;
 } Vertex;

  typedef struct
 {
  double u;
  double v;
 } Point;

 double get_start_offset(const double current,const double total);
 double get_end_offset(const double current,const double total);

 template <class DATA_TYPE>
 class Buffer
 {
  private:
  DATA_TYPE *buffer;
  size_t length;
  public:

  Buffer()
  {
   buffer=NULL;
   length=0;
  }

  ~Buffer()
  {
   Resource::destroy_array(buffer);
   buffer=NULL;
   length=0;
  }

  void set_length(const size_t amount)
  {
   length=amount;
  }

  void destroy_buffer()
  {
   Resource::destroy_array(buffer);
   buffer=NULL;
   length=0;
  }

  void fill_buffer(const DATA_TYPE value)
  {
   size_t index;
   for (index=0;index<length;++index)
   {
    buffer[index]=value;
   }

  }

  void create_buffer()
  {
   buffer=Resource::create_array<DATA_TYPE>(length);
  }

  size_t get_length() const
  {
   return length;
  }

  DATA_TYPE *get_buffer()
  {
   return buffer;
  }

  DATA_TYPE& operator[](const size_t index)
  {
   return buffer[index];
  }

 };

  class Unicode_Convertor
  {
   private:
   Buffer<wchar_t> target;
   void convert_string(const char *source);
   public:
   Unicode_Convertor();
   ~Unicode_Convertor();
   wchar_t *convert(const char *source);
  };

  class Resizer
  {
   private:
   Buffer<unsigned int> image;
   unsigned int size_limit;
   unsigned int source_width;
   unsigned int source_height;
   unsigned int target_width;
   unsigned int target_height;
   size_t get_source_offset(const unsigned int x,const unsigned int y) const;
   void resize_image(const unsigned int *target);
   void set_setting(const unsigned int width,const unsigned int height,const unsigned int limit);
   void calculate_size();
   void correct_size();
   void create_texture();
   public:
   Resizer();
   ~Resizer();
   void make_texture(const unsigned int *target,const unsigned int width,const unsigned int height,const unsigned int limit);
   unsigned int get_width() const;
   unsigned int get_height() const;
   unsigned int *get_buffer();
  };

  class FPS
  {
   private:
   time_t start;
   unsigned int current;
   unsigned int fps;
   protected:
   void update_counter();
   unsigned int get_fps_amount() const;
   public:
   FPS();
   ~FPS();
  };

  class Shape
  {
   private:
   unsigned int target_width;
   unsigned int target_height;
   unsigned int total_width;
   unsigned int total_height;
   unsigned int current_x;
   unsigned int current_y;
   protected:
   Vertex vertex[4];
   Point point[4];
   void set_data();
   unsigned int get_total_width() const;
   unsigned int get_total_height() const;
   public:
   Shape();
   ~Shape();
   void set_total_size(const unsigned int width,const unsigned int height);
   void set_size(const unsigned int width,const unsigned int height);
   void set_position(const unsigned int x,const unsigned int y);
   void set_tile_offset(const double row,const double rows,const double column,const double columns);
   void set_horizontal_offset(const double current,const double total);
   void set_vertical_offset(const double current,const double total);
  };

  class Rectangle:public Shape
  {
   private:
   unsigned int texture;
   void create_texture(const unsigned int *buffer);
   void delete_texture();
   void check_texture();
   void load_data();
   void draw_rectangle();
   public:
   Rectangle();
   ~Rectangle();
   void enable_transparent();
   void disable_transparent();
   void prepare(const unsigned int *buffer);
   void draw();
   void destroy_texture();
   bool is_texture_exist() const;
  };

  class Render
  {
   private:
   unsigned int get_maximum_texture_size() const;
   void set_perfomance_setting();
   void set_render_hints();
   void set_common_setting();
   void set_matrix_setting();
   void set_perspective(const unsigned int width,const unsigned int height);
   void create_render(const unsigned int width,const unsigned int height);
   protected:
   void clear_stage();
   void start_render(const unsigned int width,const unsigned int height);
   public:
   Render();
   ~Render();
  };

 }

 namespace Misc
 {

  class Multimedia
  {
   private:
   IGraphBuilder *loader;
   IMediaControl *player;
   IMediaSeeking *controler;
   IVideoWindow *video;
   void com_setup();
   void set_screen_mode();
   void load_content(const wchar_t *target);
   void open(const wchar_t *target);
   bool is_play();
   void rewind();
   void play_content();
   void create_loader();
   void create_player();
   void create_controler();
   void create_video_player();
   public:
   Multimedia();
   ~Multimedia();
   void initialize();
   bool check_playing();
   void stop();
   void play();
   void play_loop();
   void load(const char *target);
   void initialize(const char *target);
  };

  class Memory
  {
   private:
   MEMORYSTATUSEX memory;
   void get_status();
   public:
   Memory();
   ~Memory();
   unsigned long long int get_total_physical();
   unsigned long long int get_free_physical();
   unsigned long long int get_total_virtual();
   unsigned long long int get_free_virtual();
   unsigned long int get_usage();
  };

 }

 namespace Input
 {

  class Keyboard
  {
   private:
   Core::Buffer<unsigned char> preversion;
   bool check_state(const unsigned char code,const unsigned char state);
   public:
   Keyboard();
   ~Keyboard();
   void initialize();
   bool check_hold(const unsigned char code);
   bool check_press(const unsigned char code);
   bool check_release(const unsigned char code);
   bool is_ready() const;
  };

  class Mouse
  {
   private:
   unsigned char preversion[3];
   POINT position;
   void get_position();
   bool check_state(const SWGF::MOUSE_BUTTON button,const unsigned char state);
   public:
   Mouse();
   ~Mouse();
   void show();
   void hide();
   void set_position(const unsigned int x,const unsigned int y);
   unsigned int get_x();
   unsigned int get_y();
   bool check_hold(const SWGF::MOUSE_BUTTON button);
   bool check_press(const SWGF::MOUSE_BUTTON button);
   bool check_release(const SWGF::MOUSE_BUTTON button);
  };

  class Gamepad
  {
   private:
   XINPUT_STATE current;
   XINPUT_STATE preversion;
   XINPUT_VIBRATION vibration;
   unsigned int active;
   void clear_state();
   bool read_state();
   bool write_state();
   void set_motor(const unsigned short int left,const unsigned short int right);
   bool check_current_button(const SWGF::GAMEPAD_BUTTONS button) const;
   bool check_preversion_button(const SWGF::GAMEPAD_BUTTONS button) const;
   bool check_current_trigger(const SWGF::GAMEPAD_TRIGGERS trigger) const;
   bool check_preversion_trigger(const SWGF::GAMEPAD_TRIGGERS trigger) const;
   public:
   Gamepad();
   ~Gamepad();
   void set_active(const unsigned int gamepad);
   unsigned int get_active() const;
   bool check_connection();
   void update();
   bool check_button_hold(const SWGF::GAMEPAD_BUTTONS button);
   bool check_button_press(const SWGF::GAMEPAD_BUTTONS button);
   bool check_button_release(const SWGF::GAMEPAD_BUTTONS button);
   bool check_trigger_hold(const SWGF::GAMEPAD_TRIGGERS trigger);
   bool check_trigger_press(const SWGF::GAMEPAD_TRIGGERS trigger);
   bool check_trigger_release(const SWGF::GAMEPAD_TRIGGERS trigger);
   unsigned char get_left_trigger() const;
   unsigned char get_right_trigger() const;
   bool set_vibration(const unsigned short int left,const unsigned short int right);
   bool disable_vibration();
   unsigned short int get_left_motor() const;
   unsigned short int get_right_motor() const;
   SWGF::GAMEPAD_DIRECTION get_stick_x(const SWGF::GAMEPAD_STICKS stick) const;
   SWGF::GAMEPAD_DIRECTION get_stick_y(const SWGF::GAMEPAD_STICKS stick) const;
   SWGF::GAMEPAD_DIRECTION get_left_stick_x() const;
   SWGF::GAMEPAD_DIRECTION get_left_stick_y() const;
   SWGF::GAMEPAD_DIRECTION get_right_stick_x() const;
   SWGF::GAMEPAD_DIRECTION get_right_stick_y() const;
  };

 }

 namespace File
 {

  class Binary_File
  {
   private:
   FILE *target;
   protected:
   FILE *get_target();
   void set_target(FILE *point);
   public:
   Binary_File();
   ~Binary_File();
   void close();
   void set_position(const long int offset);
   long int get_position();
   long int get_length();
   bool check_error();
   bool is_open() const;
  };

  class Input_File:public Binary_File
  {
   public:
   Input_File();
   ~Input_File();
   void open(const char *name);
   void read(void *buffer,const size_t length);
  };

  class Output_File:public Binary_File
  {
   public:
   Output_File();
   ~Output_File();
   void open(const char *name);
   void create_temp();
   void write(const void *buffer,const size_t length);
   void flush();
  };

 }

 namespace Graphics
 {

  class Screen: public Core::FPS, public Core::Render, public Internal::Display, public Internal::Engine, public Internal::WINGL, public Internal::Synchronization
 {
   private:
   void check_video_mode();
   void set_resolution(const unsigned long int width,const unsigned long int height);
   void screen_setup();
   public:
   Screen();
   ~Screen();
   void clear_screen();
   void initialize();
   void initialize(const unsigned int width,const unsigned int height);
   bool update();
   bool sync();
   bool is_ready();
   bool is_accelerated() const;
   bool is_software() const;
   unsigned long int get_color() const;
   unsigned int get_fps() const;
   unsigned int get_width() const;
   unsigned int get_height() const;
   Screen* get_handle();
  };

   class Image
  {
   private:
   Core::Buffer<unsigned char> data;
   unsigned int width;
   unsigned int height;
   void uncompress_tga_data(const unsigned char *target);
   void load_tga(File::Input_File &target);
   public:
   Image();
   ~Image();
   unsigned int get_width() const;
   unsigned int get_height() const;
   size_t get_length() const;
   unsigned char *get_data();
   Image* get_handle();
   void destroy_image();
   unsigned char *load_tga(const char *name);
  };

  class Picture
  {
   private:
   Core::Buffer<unsigned int> image;
   unsigned int image_width;
   unsigned int image_height;
   protected:
   void set_image_size(const unsigned int width,const unsigned int height);
   void create_storage();
   void load_image(Image *buffer);
   public:
   Picture();
   ~Picture();
   void destroy_image();
   bool is_storage_empty() const;
   unsigned int get_image_width() const;
   unsigned int get_image_height() const;
   size_t get_image_length() const;
   unsigned int *get_image();
  };

  class Animation
  {
   private:
   unsigned int frames;
   unsigned int frame;
   void correct_frame();
   protected:
   void reset_animation_setting();
   void increase_frame();
   void set_frame(const unsigned int target);
   void set_frames(const unsigned int amount);
   public:
   Animation();
   ~Animation();
   unsigned int get_frames() const;
   unsigned int get_frame() const;
  };

  class Billboard
  {
   private:
   bool transparent;
   unsigned int current_x;
   unsigned int current_y;
   unsigned int sprite_width;
   unsigned int sprite_height;
   void check_transparent();
   void draw_sprite_image();
   protected:
   Core::Rectangle billboard;
   void reset_billboard_setting();
   void prepare(const unsigned int width,const unsigned int height,const unsigned int *picture);
   public:
   Billboard();
   ~Billboard();
   void set_transparent(const bool enabled);
   bool get_transparent() const;
   void set_width(const unsigned int width);
   void set_height(const unsigned int height);
   void set_size(const unsigned int width,const unsigned int height);
   void set_position(const unsigned int x,const unsigned int y);
   void set_x(const unsigned int x);
   void set_y(const unsigned int y);
   unsigned int increase_x();
   unsigned int decrease_x();
   unsigned int increase_y();
   unsigned int decrease_y();
   unsigned int increase_x(const unsigned int increment);
   unsigned int decrease_x(const unsigned int decrement);
   unsigned int increase_y(const unsigned int increment);
   unsigned int decrease_y(const unsigned int decrement);
   unsigned int get_x() const;
   unsigned int get_y() const;
   unsigned int get_width() const;
   unsigned int get_height() const;
   SWGF::BOX get_box() const;
   void draw();
   void draw(const unsigned int x,const unsigned int y);
   void draw(const bool transparency);
   void draw(const bool transparency,const unsigned int x,const unsigned int y);
  };

  class Sprite:public Billboard,public Animation,public Picture
  {
   private:
   SWGF::IMAGE_KIND current_kind;
   void reset_sprite_setting();
   void set_sprite_setting();
   void configure_sprite();
   void set_sprite_frame();
   void set_kind(const SWGF::IMAGE_KIND kind);
   public:
   Sprite();
   ~Sprite();
   Sprite* get_handle();
   SWGF::IMAGE_KIND get_kind() const;
   void set_setting(const SWGF::IMAGE_KIND kind,const unsigned int frames);
   void load(Image *buffer,const SWGF::IMAGE_KIND kind,const unsigned int frames);
   void load(Image *buffer);
   void load(Image &buffer,const SWGF::IMAGE_KIND kind,const unsigned int frames);
   void load(Image &buffer);
   void load(const char *name,const SWGF::IMAGE_KIND kind,const unsigned int frames);
   void load(const char *name);
   void set_target(const unsigned int target);
   void step();
   void clone(Sprite *target);
   void clone(Sprite &target);
   void destroy();
  };

  class Sheet:public Billboard,public Picture,public Animation
  {
   private:
   unsigned int rows;
   unsigned int columns;
   void reset_sheet_setting();
   void prepare_sheet();
   public:
   Sheet();
   ~Sheet();
   unsigned int get_row(const unsigned int target) const;
   unsigned int get_column(const unsigned int target) const;
   unsigned int calculate(const unsigned int row,const unsigned int column) const;
   unsigned int get_rows() const;
   unsigned int get_columns() const;
   void destroy();
   void select(const unsigned int row,const unsigned int column);
   void select(const unsigned int target);
   void step();
   void load(Image *sheet,const unsigned int row_amount,const unsigned int column_amount);
   void load(Image &sheet,const unsigned int row_amount,const unsigned int column_amount);
   void load(const char *name,const unsigned int row_amount,const unsigned int column_amount);
  };

  class Background
  {
   private:
   Graphics::Sprite stage;
   public:
   Background();
   ~Background();
   void prepare(const Screen *screen);
   void prepare(Screen &screen);
   void prepare(const unsigned int width,const unsigned int height);
   void set_setting(const SWGF::IMAGE_KIND kind,const unsigned int frames);
   void load(Image *background,const IMAGE_KIND kind,const unsigned int frames);
   void load(Image *background);
   void load(Image &background,const SWGF::IMAGE_KIND kind,const unsigned int frames);
   void load(Image &background);
   void load(const char *name,const SWGF::IMAGE_KIND kind,const unsigned int frames);
   void load(const char *name);
   void set_target(const unsigned int target);
   void step();
   void draw();
   void destroy_image();
   void destroy();
   unsigned int get_frame() const;
   unsigned int get_frames() const;
   unsigned int get_width() const;
   unsigned int get_height() const;
   SWGF::IMAGE_KIND get_kind() const;
  };

  class Text
  {
   private:
   Graphics::Sheet text;
   SWGF::TEXT_KIND orientation;
   unsigned int current_x;
   unsigned int current_y;
   void increase_position();
   void restore_position();
   public:
   Text();
   ~Text();
   SWGF::TEXT_KIND get_orientation() const;
   void set_orientation(const SWGF::TEXT_KIND target);
   unsigned int get_font_width() const;
   unsigned int get_font_height() const;
   void set_position(const unsigned int x,const unsigned int y);
   void set_size(const unsigned int width,const unsigned int height);
   void set_width(const unsigned int width);
   void set_height(const unsigned int height);
   void set_setting(const unsigned int width,const unsigned int height,const SWGF::TEXT_KIND kind);
   void load_font(Image *font);
   void load_font(Image &font);
   void load_font(const char *name);
   void print(const char target);
   void print(const char *target);
   void print(const unsigned int x,const unsigned int y,const char target);
   void print(const unsigned int x,const unsigned int y,const char *target);
   void destroy_image();
   void destroy_font();
  };

 }

 namespace Common
 {

  class Timer
  {
   private:
   double interval;
   time_t start;
   public:
   Timer();
   ~Timer();
   void set_timer(const double seconds);
   bool check_timer();
  };

  class Collision
  {
   private:
   SWGF::BOX first;
   SWGF::BOX second;
   bool check_horizontal_collision() const;
   bool check_vertical_collision() const;
   public:
   Collision();
   ~Collision();
   void set_target(const SWGF::BOX &first_target,const SWGF::BOX &second_target);
   bool check_collision() const;
   bool check_collision(const SWGF::BOX &first_target,const SWGF::BOX &second_target);
  };

 }

 namespace Filesystem
 {
  bool delete_file(const char *name);
  bool file_exist(const char *name);
 }

 namespace Tools
 {
  SWGF::BOX generate_box(const unsigned int x,const unsigned int y,const unsigned int width,const unsigned int height);
  void quit();
  bool enable_logging(const char *name);
  void randomize();
  unsigned int get_random(const unsigned int number);
  unsigned int get_texture_size();
 }

}

#endif