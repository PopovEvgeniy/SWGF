/*
Simple windows game framework was create by Popov Evgeniy Alekseyevich
Some code was taken from wglext.h(https://www.khronos.org/registry/OpenGL/api/GL/wglext.h) by The Khronos Group Inc

Simple windows game framework license

Copyright © 2016–2018, Popov Evgeniy Alekseyevich

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

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
#pragma comment(lib,"xinput.lib")

//Uncomment follow lines if you will compile the code under Visual C++ 2017
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
#include <windows.h>
#include <unknwn.h>
#include <dshow.h>
#include <xinput.h>
#include <GL\gl.h>

#define SWGF_KEYBOARD 256
#define SWGF_MOUSE 3

#define SWGFKEY_NONE 0
#define SWGFKEY_PRESS 1
#define SWGFKEY_RELEASE 2

#define SWGF_MOUSE_LEFT 0
#define SWGF_MOUSE_RIGHT 1
#define SWGF_MOUSE_MIDDLE 2

#define SWGF_GAMEPAD_AMOUNT 4
#define SWGF_GAMEPAD_BATTERY_ERROR 0
#define SWGF_GAMEPAD_BATTERY_ALKAINE 1
#define SWGF_GAMEPAD_BATTERY_NIMH 2
#define SWGF_GAMEPAD_BATTERY_UNKNOW 3
#define SWGF_GAMEPAD_BATTERY_EMPTY 4
#define SWGF_GAMEPAD_BATTERY_LOW 5
#define SWGF_GAMEPAD_BATTERY_MEDIUM 6
#define SWGF_GAMEPAD_BATTERY_FULL 7
#define SWGF_GAMEPAD_UP XINPUT_GAMEPAD_DPAD_UP
#define SWGF_GAMEPAD_DOWN XINPUT_GAMEPAD_DPAD_DOWN
#define SWGF_GAMEPAD_LEFT XINPUT_GAMEPAD_DPAD_LEFT
#define SWGF_GAMEPAD_RIGHT XINPUT_GAMEPAD_DPAD_RIGHT
#define SWGF_GAMEPAD_A XINPUT_GAMEPAD_A
#define SWGF_GAMEPAD_B XINPUT_GAMEPAD_B
#define SWGF_GAMEPAD_X XINPUT_GAMEPAD_X
#define SWGF_GAMEPAD_Y XINPUT_GAMEPAD_Y
#define SWGF_GAMEPAD_LEFT_BUMPER XINPUT_GAMEPAD_LEFT_SHOULDER
#define SWGF_GAMEPAD_RIGHT_BUMPER XINPUT_GAMEPAD_RIGHT_SHOULDER
#define SWGF_GAMEPAD_START XINPUT_GAMEPAD_START
#define SWGF_GAMEPAD_BACK XINPUT_GAMEPAD_BACK
#define SWGF_GAMEPAD_LEFT_TRIGGER 0
#define SWGF_GAMEPAD_RIGHT_TRIGGER 1
#define SWGF_GAMEPAD_LEFT_STICK 2
#define SWGF_GAMEPAD_RIGHT_STICK 3

extern BOOL WINAPI wglSwapIntervalEXT (int interval); // This code was taken from wglext.h by The Khronos Group Inc
typedef BOOL (WINAPI * PFNWGLSWAPINTERVALEXTPROC) (int interval); // This code was taken from wglext.h by The Khronos Group Inc

struct SWGF_Color
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

struct SWGF_Box
{
 unsigned long int x:32;
 unsigned long int y:32;
 unsigned long int width:32;
 unsigned long int height:32;
};

LRESULT CALLBACK SWGF_Process_Message(HWND window,UINT Message,WPARAM wParam,LPARAM lParam);

class SWGF_Synchronization
{
 private:
 HANDLE timer;
 protected:
 void create_timer();
 void set_timer(const unsigned long int interval);
 void wait_timer();
 public:
 SWGF_Synchronization();
 ~SWGF_Synchronization();
};

class SWGF_Engine
{
 protected:
 HWND window;
 WNDCLASS window_class;
 unsigned long int width;
 unsigned long int height;
 void prepare_engine();
 void create_window();
 void capture_mouse();
 bool process_message();
 public:
 SWGF_Engine();
 ~SWGF_Engine();
 unsigned long int get_width();
 unsigned long int get_height();
};

class SWGF_Frame
{
 protected:
 unsigned long int frame_width;
 unsigned long int frame_height;
 size_t buffer_length;
 unsigned long int *buffer;
 void create_render_buffer();
 unsigned long int get_rgb(const unsigned long int red,const unsigned long int green,const unsigned long int blue);
 public:
 SWGF_Frame();
 ~SWGF_Frame();
 void draw_pixel(const unsigned long int x,const unsigned long int y,const unsigned char red,const unsigned char green,const unsigned char blue);
 void clear_screen();
 unsigned long int get_frame_width();
 unsigned long int get_frame_height();
};

class SWGF_Display:public SWGF_Engine
{
 protected:
 DEVMODE display;
 DEVMODE get_video_mode();
 void set_video_mode(DEVMODE mode);
 void check_video_mode();
 public:
 SWGF_Display();
 ~SWGF_Display();
};

class SWGF_Render:public SWGF_Display, public SWGF_Frame
{
 protected:
 HDC context;
 HGLRC render;
 PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT;
 unsigned int texture;
 unsigned int surface;
 bool check_common_setting(const PIXELFORMATDESCRIPTOR &setting);
 bool check_acceleration(const PIXELFORMATDESCRIPTOR &setting);
 int get_pixel_format();
 void set_pixel_format(const int format);
 void create_render_context();
 void set_render();
 void set_perfomance_setting();
 void set_perspective();
 void clear_stage();
 void check_videocard();
 void prepare_surface();
 void create_texture();
 void disable_vsync();
 void create_render();
 void refresh();
 public:
 SWGF_Render();
 ~SWGF_Render();
};

class SWGF_Screen:public SWGF_Synchronization, public SWGF_Render
{
 public:
 void initialize();
 bool sync();
 SWGF_Screen* get_handle();
};

class SWGF_Keyboard
{
 private:
 unsigned char *preversion;
 public:
 SWGF_Keyboard();
 ~SWGF_Keyboard();
 void initialize();
 bool check_hold(const unsigned char code);
 bool check_press(const unsigned char code);
 bool check_release(const unsigned char code);
};

class SWGF_Mouse
{
 private:
 unsigned char preversion[SWGF_MOUSE];
 public:
 SWGF_Mouse();
 ~SWGF_Mouse();
 void show();
 void hide();
 void set_position(const unsigned long int x,const unsigned long int y);
 unsigned long int get_x();
 unsigned long int get_y();
 bool check_hold(const unsigned char button);
 bool check_press(const unsigned char button);
 bool check_release(const unsigned char button);
};

class SWGF_Gamepad
{
 private:
 XINPUT_BATTERY_INFORMATION battery;
 XINPUT_STATE current;
 XINPUT_STATE preversion;
 XINPUT_VIBRATION vibration;
 unsigned long int active;
 size_t length;
 bool read_battery_status();
 void clear_state();
 bool read_state();
 bool write_state();
 void set_motor(const unsigned short int left,const unsigned short int right);
 bool check_button(XINPUT_STATE &target,const unsigned short int button);
 bool check_trigger(XINPUT_STATE &target,const unsigned char trigger);
 public:
 SWGF_Gamepad();
 ~SWGF_Gamepad();
 void set_active(const unsigned long int gamepad);
 unsigned long int get_active();
 unsigned long int get_amount();
 bool check_connection();
 bool is_wireless();
 unsigned char get_battery_type();
 unsigned char get_battery_level();
 void update();
 bool check_button_hold(const unsigned short int button);
 bool check_button_press(const unsigned short int button);
 bool check_button_release(const unsigned short int button);
 bool check_trigger_hold(const unsigned char trigger);
 bool check_trigger_press(const unsigned char trigger);
 bool check_trigger_release(const unsigned char trigger);
 bool set_vibration(const unsigned short int left,const unsigned short int right);
 char get_stick_x(const unsigned char stick);
 char get_stick_y(const unsigned char stick);
};

class SWGF_Multimedia
{
 private:
 IGraphBuilder *loader;
 IMediaControl *player;
 IMediaSeeking *controler;
 IVideoWindow *video;
 wchar_t *convert_file_name(const char *target);
 void open(const wchar_t *target);
 bool is_end();
 void rewind();
 public:
 SWGF_Multimedia();
 ~SWGF_Multimedia();
 void initialize();
 void load(const char *target);
 bool check_playing();
 void stop();
 void play();
};

class SWGF_Memory
{
 private:
 MEMORYSTATUSEX memory;
 void get_status();
 public:
 SWGF_Memory();
 ~SWGF_Memory();
 unsigned long long int get_total_memory();
 unsigned long long int get_free_memory();
};

class SWGF_System
{
 public:
 SWGF_System();
 ~SWGF_System();
 unsigned long int get_random(const unsigned long int number);
 void quit();
 void run(const char *command);
 char* read_environment(const char *variable);
 void enable_logging(const char *name);
};

class SWGF_Timer
{
 private:
 unsigned long int interval;
 time_t start;
 public:
 SWGF_Timer();
 ~SWGF_Timer();
 void set_timer(const unsigned long int seconds);
 bool check_timer();
};

class SWGF_Primitive
{
 private:
 SWGF_Screen *surface;
 public:
 SWGF_Primitive();
 ~SWGF_Primitive();
 void initialize(SWGF_Screen *Screen);
 void draw_line(const unsigned long int x1,const unsigned long int y1,const unsigned long int x2,const unsigned long int y2,const unsigned char red,const unsigned char green,const unsigned char blue);
 void draw_rectangle(const unsigned long int x,const unsigned long int y,const unsigned long int width,const unsigned long int height,const unsigned char red,const unsigned char green,const unsigned char blue);
 void draw_filled_rectangle(const unsigned long int x,const unsigned long int y,const unsigned long int width,const unsigned long int height,const unsigned char red,const unsigned char green,const unsigned char blue);
};

class SWGF_Image
{
 private:
 unsigned long int width;
 unsigned long int height;
 unsigned char *data;
 unsigned char *create_buffer(const unsigned long int length);
 void clear_buffer();
 FILE *open_image(const char *name);
 unsigned long int get_file_size(FILE *target);
 public:
 SWGF_Image();
 ~SWGF_Image();
 void load_tga(const char *name);
 void load_pcx(const char *name);
 unsigned long int get_width();
 unsigned long int get_height();
 unsigned long int get_data_length();
 unsigned char *get_data();
 void destroy_image();
};

class SWGF_Canvas
{
 protected:
 unsigned long int width;
 unsigned long int height;
 unsigned long int frames;
 SWGF_Screen *surface;
 SWGF_Color *image;
 SWGF_Color *create_buffer(const unsigned long int image_width,const unsigned long int image_height);
 public:
 SWGF_Canvas();
 ~SWGF_Canvas();
 SWGF_Color *get_image();
 unsigned long int get_width();
 unsigned long int get_height();
 void set_frames(const unsigned long int amount);
 unsigned long int get_frames();
 void initialize(SWGF_Screen *Screen);
 void load_image(SWGF_Image &buffer);
 void mirror_image(const unsigned char kind);
 void resize_image(const unsigned long int new_width,const unsigned long int new_height);
};

class SWGF_Background:public SWGF_Canvas
{
 public:
 void draw_horizontal_background(const unsigned long int frame);
 void draw_vertical_background(const unsigned long int frame);
 void draw_background();
};

class SWGF_Sprite:public SWGF_Canvas
{
 private:
 unsigned long int current_x;
 unsigned long int current_y;
 bool compare_pixels(const SWGF_Color &first,const SWGF_Color &second);
 public:
 SWGF_Sprite();
 ~SWGF_Sprite();
 void clone(SWGF_Sprite &target);
 void draw_sprite_frame(const unsigned long int x,const unsigned long int y,const unsigned long int frame);
 void draw_sprite(const unsigned long int x,const unsigned long int y);
 unsigned long int get_x();
 unsigned long int get_y();
 unsigned long int get_sprite_width();
 unsigned long int get_sprite_height();
 SWGF_Sprite* get_handle();
 SWGF_Box get_box();
};

class SWGF_Text
{
 private:
 unsigned long int current_x;
 unsigned long int current_y;
 SWGF_Sprite *sprite;
 public:
 SWGF_Text();
 ~SWGF_Text();
 void set_position(const unsigned long int x,const unsigned long int y);
 void load_font(SWGF_Sprite *font);
 void draw_text(const char *text);
};

class SWGF_Collision
{
 public:
 bool check_horizontal_collision(const SWGF_Box &first,const SWGF_Box &second);
 bool check_vertical_collision(const SWGF_Box &first,const SWGF_Box &second);
 bool check_collision(const SWGF_Box &first,const SWGF_Box &second);
};