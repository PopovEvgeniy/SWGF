/*
Simple windows game framework was create by Popov Evgeniy Alekseyevich
Some code was taken from wglext.h(https://www.khronos.org/registry/OpenGL/api/GL/wglext.h) by The Khronos Group Inc

Simple windows game framework license

Copyright (C) 2016-2021 Popov Evgeniy Alekseyevich

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

#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"ole32.lib")
#pragma comment(lib,"strmiids.lib")
#pragma comment(lib,"winmm.lib")

//Uncomment follow lines if you will compile the code under Visual C++ 2017 or higher
/*
#pragma warning(disable : 4995)
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_WARNINGS
*/

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>
#include <new>
#include <windows.h>
#include <unknwn.h>
#include <dshow.h>
#include <mmsystem.h>
#include <GL\gl.h>

#define GETSCANCODE(argument) ((argument >> 16)&0x7f)
#define MOUSE 3

enum MIRROR_TYPE {MIRROR_HORIZONTAL=0,MIRROR_VERTICAL=1};
enum BACKGROUND_TYPE {NORMAL_BACKGROUND=0,HORIZONTAL_BACKGROUND=1,VERTICAL_BACKGROUND=2};
enum SPRITE_TYPE {SINGLE_SPRITE=0,HORIZONTAL_STRIP=1,VERTICAL_STRIP=2};
enum SURFACE {SURFACE_SMALL=0,SURFACE_LARGE=1};
enum MOUSE_BUTTON {MOUSE_LEFT=0,MOUSE_RIGHT=1,MOUSE_MIDDLE=2};
enum GAMEPAD_DIRECTION {GAMEPAD_NEUTRAL_DIRECTION=0,GAMEPAD_NEGATIVE_DIRECTION=-1,GAMEPAD_POSITIVE_DIRECTION=1};
enum GAMEPAD_STICKS {GAMEPAD_LEFT_STICK=0,GAMEPAD_RIGHT_STICK=1};
enum GAMEPAD_DPAD {GAMEPAD_NONE=0,GAMEPAD_UP=1,GAMEPAD_DOWN=2,GAMEPAD_LEFT=3,GAMEPAD_RIGHT=4,GAMEPAD_UPLEFT=5,GAMEPAD_UPRIGHT=6,GAMEPAD_DOWNLEFT=7,GAMEPAD_DOWNRIGHT=8};
enum GAMEPAD_BUTTONS {GAMEPAD_BUTTON1=JOY_BUTTON1,GAMEPAD_BUTTON2=JOY_BUTTON2,GAMEPAD_BUTTON3=JOY_BUTTON3,GAMEPAD_BUTTON4=JOY_BUTTON4,GAMEPAD_BUTTON5=JOY_BUTTON5,GAMEPAD_BUTTON6=JOY_BUTTON6,GAMEPAD_BUTTON7=JOY_BUTTON7,GAMEPAD_BUTTON8=JOY_BUTTON8,GAMEPAD_BUTTON9=JOY_BUTTON9,GAMEPAD_BUTTON10=JOY_BUTTON10,GAMEPAD_BUTTON11=JOY_BUTTON11,GAMEPAD_BUTTON12=JOY_BUTTON12,GAMEPAD_BUTTON113=JOY_BUTTON13,GAMEPAD_BUTTON14=JOY_BUTTON14,GAMEPAD_BUTTON15=JOY_BUTTON15,GAMEPAD_BUTTON16=JOY_BUTTON16,GAMEPAD_BUTTON17=JOY_BUTTON17,GAMEPAD_BUTTON18=JOY_BUTTON18,GAMEPAD_BUTTON19=JOY_BUTTON19,GAMEPAD_BUTTON20=JOY_BUTTON20,GAMEPAD_BUTTON21=JOY_BUTTON21,GAMEPAD_BUTTON22=JOY_BUTTON22,GAMEPAD_BUTTON23=JOY_BUTTON23,GAMEPAD_BUTTON24=JOY_BUTTON24,GAMEPAD_BUTTON25=JOY_BUTTON25,GAMEPAD_BUTTON26=JOY_BUTTON26,GAMEPAD_BUTTON27=JOY_BUTTON27,GAMEPAD_BUTTON28=JOY_BUTTON28,GAMEPAD_BUTTON29=JOY_BUTTON29,GAMEPAD_BUTTON30=JOY_BUTTON30,GAMEPAD_BUTTON31=JOY_BUTTON31,GAMEPAD_BUTTON32=JOY_BUTTON32};

extern BOOL WINAPI wglSwapIntervalEXT (int interval); // This code was taken from wglext.h by The Khronos Group Inc
typedef BOOL (WINAPI * PFNWGLSWAPINTERVALEXTPROC) (int interval); // This code was taken from wglext.h by The Khronos Group Inc

struct IMG_Pixel
{
 unsigned char blue:8;
 unsigned char green:8;
 unsigned char red:8;
};

struct TGA_head
{
 unsigned char id:8;
 unsigned char color_map:8;
 unsigned char type:8;
};

struct TGA_map
{
 unsigned short int index:16;
 unsigned short int length:16;
 unsigned char map_size:8;
};

struct TGA_image
{
 unsigned short int x:16;
 unsigned short int y:16;
 unsigned short int width:16;
 unsigned short int height:16;
 unsigned char color:8;
 unsigned char alpha:3;
 unsigned char direction:5;
};

struct PCX_head
{
 unsigned char vendor:8;
 unsigned char version:8;
 unsigned char compress:8;
 unsigned char color:8;
 unsigned short int min_x:16;
 unsigned short int min_y:16;
 unsigned short int max_x:16;
 unsigned short int max_y:16;
 unsigned short int vertical_dpi:16;
 unsigned short int horizontal_dpi:16;
 unsigned char palette[48];
 unsigned char reversed:8;
 unsigned char planes:8;
 unsigned short int plane_length:16;
 unsigned short int palette_type:16;
 unsigned short int screen_width:16;
 unsigned short int screen_height:16;
 unsigned char filled[54];
};

struct Collision_Box
{
 unsigned long int x:32;
 unsigned long int y:32;
 unsigned long int width:32;
 unsigned long int height:32;
};

LRESULT CALLBACK Process_Message(HWND window,UINT Message,WPARAM wParam,LPARAM lParam);

namespace SWGF
{

struct Vertex
{
 int x;
 int y;
};

struct Point
{
 float u;
 float v;
};

void Halt(const char *message);

class COM_Base
{
 public:
 COM_Base();
 ~COM_Base();
};

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
 protected:
 HDC get_context();
 void prepare_engine();
 void destroy_window();
 void take_context();
 void create_window();
 void capture_mouse();
 bool process_message();
 public:
 Engine();
 ~Engine();
};

class Frame
{
 private:
 size_t pixels;
 unsigned long int frame_width;
 unsigned long int frame_height;
 unsigned int *buffer;
 unsigned int *shadow;
 void calculate_buffer_length();
 unsigned int *get_memory(const char *error);
 void clear_buffer(unsigned int *target);
 unsigned int *create_buffer(const char *error);
 protected:
 size_t get_offset(const unsigned long int x,const unsigned long int y,const unsigned long int target_width);
 size_t get_offset(const unsigned long int x,const unsigned long int y) const;
 void set_size(const unsigned long int surface_width,const unsigned long int surface_height);
 void set_size(const SURFACE surface);
 void create_buffers();
 public:
 Frame();
 ~Frame();
 unsigned int *get_buffer();
 size_t get_pixels() const;
 bool draw_pixel(const unsigned long int x,const unsigned long int y,const unsigned long int red,const unsigned long int green,const unsigned long int blue);
 void clear_screen();
 void save();
 void restore();
 void restore(const unsigned long int x,const unsigned long int y,const unsigned long int width,const unsigned long int height);
 unsigned long int get_frame_width() const;
 unsigned long int get_frame_height() const;
};

class Plane: public Frame
{
 private:
 unsigned int *plane;
 unsigned int *target;
 unsigned long int target_width;
 unsigned long int target_height;
 float x_ratio;
 float y_ratio;
 public:
 Plane();
 ~Plane();
 void create_plane(const unsigned long int width,const unsigned long int height,const unsigned long int surface_width,const unsigned long int surface_height,unsigned int *surface_buffer);
 void transfer();
 Plane* get_handle();
};

class Timer
{
 private:
 unsigned long int interval;
 time_t start;
 public:
 Timer();
 ~Timer();
 void set_timer(const unsigned long int seconds);
 bool check_timer();
};

class FPS
{
 private:
 Timer timer;
 unsigned long int current;
 unsigned long int fps;
 protected:
 void update_counter();
 public:
 FPS();
 ~FPS();
 unsigned long int get_fps() const;
};

class Unicode_Convertor
{
 private:
 wchar_t *target;
 void get_memory(const size_t length);
 void clear_buffer(const size_t length);
 void create_buffer(const size_t length);
 void convert_string(const char *source);
 public:
 Unicode_Convertor();
 ~Unicode_Convertor();
 wchar_t *convert(const char *source);
};

class Display
{
 private:
 DEVMODE display;
 void get_video_mode();
 void set_video_mode();
 protected:
 void check_video_mode();
 void set_display_mode(const unsigned long int screen_width,const unsigned long int screen_height);
 public:
 Display();
 ~Display();
 unsigned long int get_color() const;
 unsigned long int get_width() const;
 unsigned long int get_height() const;
};

class WINGL:public Display, public Engine
{
 private:
 HGLRC render;
 PIXELFORMATDESCRIPTOR setting;
 PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
 bool check_base_setting() const;
 bool check_advanced_setting() const;
 bool check_common_setting() const;
 bool check_acceleration() const;
 int get_pixel_format();
 void set_pixel_format(const int format);
 void create_render_context();
 protected:
 void destroy_render_context();
 void set_render();
 void disable_vsync();
 void Swap();
 public:
 WINGL();
 ~WINGL();
};

class Render:public WINGL, public Frame
{
 private:
 unsigned int texture;
 Vertex vertex[4];
 Point point[4];
 void set_perfomance_setting();
 void set_perspective();
 void clear_stage();
 void check_videocard();
 void set_vertex_coordinates();
 void create_texture();
 void load_surface_data();
 void create_render();
 void draw();
 protected:
 void destroy_render();
 void start_render();
 void refresh();
 public:
 Render();
 ~Render();
};

class Screen:public FPS, public Synchronization, public Render
{
 public:
 Screen();
 ~Screen();
 void initialize();
 void initialize(const SURFACE surface);
 void set_mode(const unsigned long int screen_width,const unsigned long int screen_height);
 bool update();
 bool sync();
 Screen* get_handle();
};

class Keyboard
{
 private:
 unsigned char *preversion;
 void create_buffer();
 void clear_buffer();
 bool check_state(const unsigned char code,const unsigned char state);
 public:
 Keyboard();
 ~Keyboard();
 void initialize();
 bool check_hold(const unsigned char code);
 bool check_press(const unsigned char code);
 bool check_release(const unsigned char code);
};

class Mouse
{
 private:
 unsigned char preversion[MOUSE];
 POINT position;
 void get_position();
 bool check_state(const MOUSE_BUTTON button,const unsigned char state);
 public:
 Mouse();
 ~Mouse();
 void show();
 void hide();
 void set_position(const unsigned long int x,const unsigned long int y);
 unsigned long int get_x();
 unsigned long int get_y();
 bool check_hold(const MOUSE_BUTTON button);
 bool check_press(const MOUSE_BUTTON button);
 bool check_release(const MOUSE_BUTTON button);
};

class Gamepad
{
 private:
 JOYINFOEX current;
 JOYINFOEX preversion;
 JOYCAPS configuration;
 unsigned int active;
 unsigned int max_amount;
 bool read_configuration();
 bool read_state();
 void clear_state();
 bool check_button(const GAMEPAD_BUTTONS button,const JOYINFOEX &target);
 public:
 Gamepad();
 ~Gamepad();
 unsigned int get_amount();
 unsigned int get_button_amount();
 unsigned int get_last_index();
 bool check_connection();
 void update();
 unsigned long int get_sticks_amount();
 void set_active(const unsigned int gamepad);
 unsigned int get_max_amount() const;
 unsigned int get_active() const;
 GAMEPAD_DPAD get_dpad() const;
 GAMEPAD_DIRECTION get_stick_x(const GAMEPAD_STICKS stick);
 GAMEPAD_DIRECTION get_stick_y(const GAMEPAD_STICKS stick);
 bool check_hold(const GAMEPAD_BUTTONS button);
 bool check_press(const GAMEPAD_BUTTONS button);
 bool check_release(const GAMEPAD_BUTTONS button);
};

class Multimedia:public COM_Base
{
 private:
 IGraphBuilder *loader;
 IMediaControl *player;
 IMediaSeeking *controler;
 IVideoWindow *video;
 void open(const wchar_t *target);
 bool is_play();
 void rewind();
 void create_loader();
 void create_player();
 void create_controler();
 void create_video_player();
 public:
 Multimedia();
 ~Multimedia();
 void initialize();
 void load(const char *target);
 bool check_playing();
 void stop();
 void play();
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

class System
{
 public:
 System();
 ~System();
 unsigned long int get_random(const unsigned long int number);
 void quit();
 void run(const char *command);
 char* read_environment(const char *variable);
 void enable_logging(const char *name);
};

class Filesystem
{
 private:
 bool status;
 public:
 Filesystem();
 ~Filesystem();
 void file_exist(const char *name);
 void delete_file(const char *name);
 bool get_status() const;
};

class Binary_File
{
 protected:
 FILE *target;
 void open_file(const char *name,const char *mode);
 public:
 Binary_File();
 ~Binary_File();
 void close();
 void set_position(const long int offset);
 long int get_position();
 long int get_length();
 bool check_error();
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
 void write(void *buffer,const size_t length);
 void flush();
};

class Primitive
{
 private:
 IMG_Pixel color;
 Screen *surface;
 public:
 Primitive();
 ~Primitive();
 void initialize(Screen *screen);
 void set_color(const unsigned char red,const unsigned char green,const unsigned char blue);
 void draw_line(const unsigned long int x1,const unsigned long int y1,const unsigned long int x2,const unsigned long int y2);
 void draw_rectangle(const unsigned long int x,const unsigned long int y,const unsigned long int width,const unsigned long int height);
 void draw_filled_rectangle(const unsigned long int x,const unsigned long int y,const unsigned long int width,const unsigned long int height);
};

class Image
{
 private:
 unsigned long int width;
 unsigned long int height;
 unsigned char *data;
 unsigned char *create_buffer(const size_t length);
 void clear_buffer();
 public:
 Image();
 ~Image();
 void load_tga(const char *name);
 void load_pcx(const char *name);
 unsigned long int get_width() const;
 unsigned long int get_height() const;
 size_t get_length() const;
 unsigned char *get_data();
 void destroy_image();
};

class Surface
{
 private:
 Screen *surface;
 IMG_Pixel *image;
 unsigned long int width;
 unsigned long int height;
 protected:
 void save();
 void restore();
 void clear_buffer();
 IMG_Pixel *create_buffer(const unsigned long int image_width,const unsigned long int image_height);
 void load_from_buffer(Image &buffer);
 void set_width(const unsigned long int image_width);
 void set_height(const unsigned long int image_height);
 void set_buffer(IMG_Pixel *buffer);
 size_t get_offset(const unsigned long int start,const unsigned long int x,const unsigned long int y,const unsigned long int target_width);
 size_t get_offset(const unsigned long int start,const unsigned long int x,const unsigned long int y) const;
 void draw_image_pixel(const size_t offset,const unsigned long int x,const unsigned long int y);
 bool compare_pixels(const size_t first,const size_t second) const;
 unsigned long int get_surface_width() const;
 unsigned long int get_surface_height() const;
 public:
 Surface();
 ~Surface();
 void initialize(Screen *screen);
 size_t get_length() const;
 IMG_Pixel *get_image();
 unsigned long int get_image_width() const;
 unsigned long int get_image_height() const;
 void mirror_image(const MIRROR_TYPE kind);
 void resize_image(const unsigned long int new_width,const unsigned long int new_height);
 void horizontal_mirror();
 void vertical_mirror();
};

class Canvas:public Surface
{
 private:
 unsigned long int frames;
 unsigned long int frame;
 protected:
 unsigned long int start;
 void set_frame(const unsigned long int target);
 void increase_frame();
 public:
 Canvas();
 ~Canvas();
 void set_frames(const unsigned long int amount);
 unsigned long int get_frames() const;
 unsigned long int get_frame() const;
 void load_image(Image &buffer);
};

class Background:public Canvas
{
 private:
 unsigned long int background_width;
 unsigned long int background_height;
 unsigned long int maximum_width;
 unsigned long int maximum_height;
 unsigned long int current;
 BACKGROUND_TYPE current_kind;
 void get_maximum_width();
 void get_maximum_height();
 void slow_draw_background();
 void configure_background();
 public:
 Background();
 ~Background();
 unsigned long int get_width() const;
 unsigned long int get_height() const;
 void set_kind(const BACKGROUND_TYPE kind);
 void set_setting(const BACKGROUND_TYPE kind,const unsigned long int frames);
 void set_target(const unsigned long int target);
 void step();
 void draw_background();
};

class Sprite:public Canvas
{
 private:
 bool transparent;
 unsigned long int current_x;
 unsigned long int current_y;
 unsigned long int sprite_width;
 unsigned long int sprite_height;
 SPRITE_TYPE current_kind;
 void draw_transparent_sprite();
 void draw_normal_sprite();
 public:
 Sprite();
 ~Sprite();
 void load_sprite(Image &buffer,const SPRITE_TYPE kind,const unsigned long int frames);
 void set_transparent(const bool enabled);
 bool get_transparent() const;
 void set_x(const unsigned long int x);
 void set_y(const unsigned long int y);
 void increase_x();
 void decrease_x();
 void increase_y();
 void decrease_y();
 void increase_x(const unsigned long int increment);
 void decrease_x(const unsigned long int decrement);
 void increase_y(const unsigned long int increment);
 void decrease_y(const unsigned long int decrement);
 unsigned long int get_x() const;
 unsigned long int get_y() const;
 unsigned long int get_width() const;
 unsigned long int get_height() const;
 Sprite* get_handle();
 Collision_Box get_box() const;
 void set_kind(const SPRITE_TYPE kind);
 SPRITE_TYPE get_kind() const;
 void set_target(const unsigned long int target);
 void step();
 void set_position(const unsigned long int x,const unsigned long int y);
 void clone(Sprite &target);
 void draw_sprite();
 void draw_sprite(const unsigned long int x,const unsigned long int y);
 void draw_sprite(const bool transparency);
 void draw_sprite(const bool transparency,const unsigned long int x,const unsigned long int y);
};

class Tileset:public Surface
{
 private:
 size_t offset;
 unsigned long int tile_width;
 unsigned long int tile_height;
 unsigned long int rows;
 unsigned long int columns;
 public:
 Tileset();
 ~Tileset();
 unsigned long int get_tile_width() const;
 unsigned long int get_tile_height() const;
 unsigned long int get_rows() const;
 unsigned long int get_columns() const;
 void select_tile(const unsigned long int row,const unsigned long int column);
 void draw_tile(const unsigned long int x,const unsigned long int y);
 void draw_tile(const unsigned long int row,const unsigned long int column,const unsigned long int x,const unsigned long int y);
 void load_tileset(Image &buffer,const unsigned long int row_amount,const unsigned long int column_amount);
};

class Text
{
 private:
 unsigned long int current_x;
 unsigned long int current_y;
 Sprite *font;
 void increase_position();
 void restore_position();
 public:
 Text();
 ~Text();
 void set_position(const unsigned long int x,const unsigned long int y);
 void load_font(Sprite *target);
 void draw_character(const char target);
 void draw_text(const char *text);
 void draw_character(const unsigned long int x,const unsigned long int y,const char target);
 void draw_text(const unsigned long int x,const unsigned long int y,const char *text);
};

class Transformation
{
 private:
 float screen_x_factor;
 float screen_y_factor;
 float surface_x_factor;
 float surface_y_factor;
 public:
 Transformation();
 ~Transformation();
 void initialize(const float screen_width,const float screen_height,const float surface_width,const float surface_height);
 float get_screen_x(const float surface_x) const;
 float get_screen_y(const float surface_y) const;
 float get_surface_x(const float screen_x) const;
 float get_surface_y(const float screen_y) const;
};

class Collision
{
 private:
 Collision_Box first;
 Collision_Box second;
 public:
 Collision();
 ~Collision();
 void set_target(const Collision_Box &first_target,const Collision_Box &second_target);
 bool check_horizontal_collision() const;
 bool check_vertical_collision() const;
 bool check_collision() const;
 bool check_horizontal_collision(const Collision_Box &first_target,const Collision_Box &second_target);
 bool check_vertical_collision(const Collision_Box &first_target,const Collision_Box &second_target);
 bool check_collision(const Collision_Box &first_target,const Collision_Box &second_target);
 Collision_Box generate_box(const unsigned long int x,const unsigned long int y,const unsigned long int width,const unsigned long int height) const;
};

}
