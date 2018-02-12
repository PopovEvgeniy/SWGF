/*
Simple windows game framework was create by Popov Evgeniy Alekseyevich
Some code was taken from wglext.h(https://www.khronos.org/registry/OpenGL/api/GL/wglext.h) by The Khronos Group Inc

Simple windows game framework license

Copyright � 2016�2018, Popov Evgeniy Alekseyevich

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
#pragma comment(lib,"winmm.lib")

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>
#include <windows.h>
#include <unknwn.h>
#include <dshow.h>
#include <mmsystem.h>
#include <GL\gl.h>

#define SWGF_KEYBOARD 256
#define SWGF_MOUSE 3

#define SWGFKEY_NONE 0
#define SWGFKEY_PRESS 1
#define SWGFKEY_RELEASE 2

#define SWGF_MOUSE_LEFT 0
#define SWGF_MOUSE_RIGHT 1
#define SWGF_MOUSE_MIDDLE 2

#define JOYSTICK_UPLEFT 31500
#define JOYSTICK_UPRIGHT 4500
#define JOYSTICK_DOWNLEFT 22500
#define JOYSTICK_DOWNRIGHT 13500
#define SWGF_GAMEPAD_LEFT_STICK 0
#define SWGF_GAMEPAD_RIGHT_STICK 1
#define SWGF_GAMEPAD_NONE 0
#define SWGF_GAMEPAD_UP 1
#define SWGF_GAMEPAD_DOWN 2
#define SWGF_GAMEPAD_LEFT 3
#define SWGF_GAMEPAD_RIGHT 4
#define SWGF_GAMEPAD_UPLEFT 5
#define SWGF_GAMEPAD_UPRIGHT 6
#define SWGF_GAMEPAD_DOWNLEFT 7
#define SWGF_GAMEPAD_DOWNRIGHT 8
#define SWGF_GAMEPAD1 JOYSTICKID1
#define SWGF_GAMEPAD2 2
#define SWGF_GAMEPAD3 3
#define SWGF_GAMEPAD4 4
#define SWGF_GAMEPAD5 5
#define SWGF_GAMEPAD6 6
#define SWGF_GAMEPAD7 7
#define SWGF_GAMEPAD8 8
#define SWGF_GAMEPAD9 9
#define SWGF_GAMEPAD10 10
#define SWGF_GAMEPAD11 11
#define SWGF_GAMEPAD12 12
#define SWGF_GAMEPAD13 13
#define SWGF_GAMEPAD14 14
#define SWGF_GAMEPAD15 15
#define SWGF_GAMEPAD_BUTTON1 JOY_BUTTON1
#define SWGF_GAMEPAD_BUTTON2 JOY_BUTTON2
#define SWGF_GAMEPAD_BUTTON3 JOY_BUTTON3
#define SWGF_GAMEPAD_BUTTON4 JOY_BUTTON4
#define SWGF_GAMEPAD_BUTTON5 JOY_BUTTON5
#define SWGF_GAMEPAD_BUTTON6 JOY_BUTTON6
#define SWGF_GAMEPAD_BUTTON7 JOY_BUTTON7
#define SWGF_GAMEPAD_BUTTON8 JOY_BUTTON8
#define SWGF_GAMEPAD_BUTTON9 JOY_BUTTON9
#define SWGF_GAMEPAD_BUTTON10 JOY_BUTTON10
#define SWGF_GAMEPAD_BUTTON11 JOY_BUTTON11
#define SWGF_GAMEPAD_BUTTON12 JOY_BUTTON12
#define SWGF_GAMEPAD_BUTTON113 JOY_BUTTON13
#define SWGF_GAMEPAD_BUTTON14 JOY_BUTTON14
#define SWGF_GAMEPAD_BUTTON15 JOY_BUTTON15
#define SWGF_GAMEPAD_BUTTON16 JOY_BUTTON16
#define SWGF_GAMEPAD_BUTTON17 JOY_BUTTON17
#define SWGF_GAMEPAD_BUTTON18 JOY_BUTTON18
#define SWGF_GAMEPAD_BUTTON19 JOY_BUTTON19
#define SWGF_GAMEPAD_BUTTON20 JOY_BUTTON20
#define SWGF_GAMEPAD_BUTTON21 JOY_BUTTON21
#define SWGF_GAMEPAD_BUTTON22 JOY_BUTTON22
#define SWGF_GAMEPAD_BUTTON23 JOY_BUTTON23
#define SWGF_GAMEPAD_BUTTON24 JOY_BUTTON24
#define SWGF_GAMEPAD_BUTTON25 JOY_BUTTON25
#define SWGF_GAMEPAD_BUTTON26 JOY_BUTTON26
#define SWGF_GAMEPAD_BUTTON27 JOY_BUTTON27
#define SWGF_GAMEPAD_BUTTON28 JOY_BUTTON28
#define SWGF_GAMEPAD_BUTTON29 JOY_BUTTON29
#define SWGF_GAMEPAD_BUTTON30 JOY_BUTTON30
#define SWGF_GAMEPAD_BUTTON31 JOY_BUTTON31
#define SWGF_GAMEPAD_BUTTON32 JOY_BUTTON32

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
 TIMECAPS resolution;
 unsigned long int start;
 unsigned long int delay;
 protected:
 void create_timer();
 void set_timer_resolution();
 void reset_timer_resolution();
 void pause(const unsigned long int interval);
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
 unsigned long int buffer_length;
 COLORREF *buffer;
 void create_render_buffer();
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
 bool check_common_setting(PIXELFORMATDESCRIPTOR setting);
 bool check_acceleration(PIXELFORMATDESCRIPTOR setting);
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
 JOYINFOEX current;
 JOYINFOEX preversion;
 JOYCAPS configuration;
 unsigned long int length[2];
 unsigned int active;
 bool read_configuration();
 bool read_state();
 void clear_state();
 bool check_button(const unsigned long int button,JOYINFOEX &target);
 public:
 SWGF_Gamepad();
 ~SWGF_Gamepad();
 void set_active(const unsigned int gamepad);
 unsigned int get_active();
 unsigned int get_amount();
 unsigned int get_button_amount();
 bool check_connection();
 void update();
 unsigned char get_dpad();
 unsigned long int get_sticks_amount();
 char get_stick_x(const unsigned char stick);
 char get_stick_y(const unsigned char stick);
 bool check_hold(const unsigned long int button);
 bool check_press(const unsigned long int button);
 bool check_release(const unsigned long int button);
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
 void draw_background();
 void draw_horizontal_background(const unsigned long int frame);
 void draw_vertical_background(const unsigned long int frame);
};

class SWGF_Sprite:public SWGF_Canvas
{
 private:
 unsigned long int current_x;
 unsigned long int current_y;
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
 bool check_horizontal_collision(SWGF_Box first,SWGF_Box second);
 bool check_vertical_collision(SWGF_Box first,SWGF_Box second);
 bool check_collision(SWGF_Box first,SWGF_Box second);
};