/*
Simple windows game framework license

Copyright © 2016–2017, Popov Evgeniy Alekseyevich

This software provide without any warranty.
Any person or company can redistribute original distributive of this software.
Any person or company can use source code of this software for making third–party product.
Documentation or interface of third–party product must contain this remark:
<Software> built on top Simple windows game framework from Popov Evgeniy Alekseyevich.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>
#include <windows.h>
#include <unknwn.h>
#include <d2d1.h>
#include <dshow.h>
#include <xinput.h>

#define SWGF_MOUSE_NONE 0
#define SWGF_MOUSE_LEFT 1
#define SWGF_MOUSE_RIGHT 2
#define SWGF_MOUSE_MIDDLE 3

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

struct SWGF_Pixel
{
 unsigned char blue:8;
 unsigned char green:8;
 unsigned char red:8;
 unsigned char alpha:8;
};

LRESULT CALLBACK SWGF_Process_Message(HWND window,UINT Message,WPARAM wParam,LPARAM lParam)
{
 if(Message==WM_DESTROY) PostQuitMessage(0);
 return DefWindowProc(window,Message,wParam,lParam);
}

class SWGF_Base
{
 public:
 SWGF_Base();
 ~SWGF_Base();
};

SWGF_Base::SWGF_Base()
{
 HRESULT status;
 status=CoInitialize(NULL);
 if(status!=S_OK)
 {
  if(status!=S_FALSE)
  {
   puts("Can't initialize COM");
   exit(EXIT_FAILURE);
  }

 }

}

SWGF_Base::~SWGF_Base()
{
 CoUninitialize();
}

class SWGF_Engine
{
 protected:
 HANDLE timer;
 HWND window;
 WNDCLASS window_class;
 unsigned long int width;
 unsigned long int height;
 unsigned char color;
 void destroy_window();
 void create_window();
 void capture_mouse();
 bool process_message();
 void set_timer();
 void wait_timer();
 void set_video_mode(DEVMODE mode);
 DEVMODE get_video_mode();
 void check_video_mode();
 public:
 SWGF_Engine();
 ~SWGF_Engine();
 unsigned long int get_width();
 unsigned long int get_height();
 unsigned char get_color();
};

SWGF_Engine::SWGF_Engine()
{
 unsigned char cursor[2];
 window_class.lpszClassName=TEXT("SWGF");
 window_class.hInstance=GetModuleHandle(NULL);
 window_class.style=CS_NOCLOSE;
 window_class.lpfnWndProc=(WNDPROC)SWGF_Process_Message;
 window_class.hbrBackground=NULL;
 window_class.hIcon=NULL;
 window_class.hCursor=NULL;
 window_class.cbClsExtra=0;
 window_class.cbWndExtra=0;
 cursor[0]=0;
 cursor[1]=0;
 window_class.hCursor=CreateCursor(window_class.hInstance,0,0,1,1,cursor,cursor);
 if (window_class.hCursor==NULL)
 {
  puts("Can't create a cursor");
  exit(EXIT_FAILURE);
 }
 if (RegisterClass(&window_class)==0)
 {
  puts("Can't register window class");
  exit(EXIT_FAILURE);
 }
 timer=CreateWaitableTimer(NULL,TRUE,NULL);
 if (timer==NULL)
 {
  puts("Can't create synchronization timer");
  exit(EXIT_FAILURE);
 }
 width=0;
 height=0;
 color=0;
}

SWGF_Engine::~SWGF_Engine()
{
 if(window!=NULL) CloseWindow(window);
 DestroyCursor(window_class.hCursor);
 UnregisterClass(window_class.lpszClassName,window_class.hInstance);
 ChangeDisplaySettings(NULL,0);
 CancelWaitableTimer(timer);
 CloseHandle(timer);
}

void SWGF_Engine::destroy_window()
{
 if(window!=NULL)
 {
  CloseWindow(window);
  window=NULL;
 }

}

void SWGF_Engine::create_window()
{
 window=CreateWindow(window_class.lpszClassName,NULL,WS_MAXIMIZE|WS_VISIBLE|WS_POPUP,0,0,width,height,NULL,NULL,window_class.hInstance,NULL);
 if (window==NULL)
 {
  puts("Can't create window");
  exit(EXIT_FAILURE);
 }
 EnableWindow(window,TRUE);
 SetFocus(window);
}

void SWGF_Engine::capture_mouse()
{
 RECT border;
 if(GetClientRect(window,&border)==FALSE)
 {
  puts("Can't capture window");
  exit(EXIT_FAILURE);
 }
 if(ClipCursor(&border)==FALSE)
 {
  puts("Can't capture cursor");
  exit(EXIT_FAILURE);
 }

}

bool SWGF_Engine::process_message()
{
 bool quit;
 MSG Message;
 quit=false;
 while(PeekMessage(&Message,window,0,0,PM_NOREMOVE)==TRUE)
 {
  if(Message.message==WM_QUIT)
  {
   quit=true;
   break;
  }
  if(GetMessage(&Message,window,0,0)==TRUE) DispatchMessage(&Message);
 }
 return quit;
}

void SWGF_Engine::set_timer()
{
 LARGE_INTEGER interval;
 interval.QuadPart=-166666;
 if(SetWaitableTimer(timer,&interval,17,NULL,NULL,FALSE)==FALSE)
 {
  puts("Can't set timer");
  exit(EXIT_FAILURE);
 }

}

void SWGF_Engine::wait_timer()
{
 WaitForSingleObject(timer,INFINITE);
}

void SWGF_Engine::set_video_mode(DEVMODE mode)
{
 if (ChangeDisplaySettings(&mode,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
 {
  puts("Can't change video mode");
  exit(EXIT_FAILURE);
 }

}

DEVMODE SWGF_Engine::get_video_mode()
{
 DEVMODE mode;
 memset(&mode,0,sizeof(DEVMODE));
 mode.dmSize=sizeof(DEVMODE);
 if (EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS,&mode)==FALSE)
 {
  puts("Can't get display setting");
  exit(EXIT_FAILURE);
 }
 return mode;
}

void SWGF_Engine::check_video_mode()
{
 DEVMODE mode;
 mode=this->get_video_mode();
 if (mode.dmBitsPerPel<32)
 {
  mode.dmBitsPerPel=32;
  this->set_video_mode(mode);
 }
 width=mode.dmPelsWidth;
 height=mode.dmPelsHeight;
 color=mode.dmBitsPerPel;
}

unsigned long int SWGF_Engine::get_width()
{
 return width;
}

unsigned long int SWGF_Engine::get_height()
{
 return height;
}

unsigned char SWGF_Engine::get_color()
{
 return color;
}

class SWGF_Draw
{
 protected:
 unsigned long int frame_width;
 unsigned long int frame_height;
 unsigned long int frame_line;
 SWGF_Pixel *buffer;
 void create_render_buffer();
 public:
 SWGF_Draw();
 ~SWGF_Draw();
 unsigned long int get_frame_width();
 unsigned long int get_frame_height();
 void draw_pixel(const unsigned long int x,const unsigned long int y,const unsigned char red,const unsigned char green,const unsigned char blue);
 void clear_screen();
};

SWGF_Draw::SWGF_Draw()
{
 frame_width=640;
 frame_height=480;
 frame_line=0;
 buffer=NULL;
}

SWGF_Draw::~SWGF_Draw()
{
 free(buffer);
}

void SWGF_Draw::create_render_buffer()
{
 if(buffer!=NULL)
 {
  free(buffer);
  buffer=NULL;
 }
 frame_line=frame_width*sizeof(SWGF_Pixel);
 buffer=(SWGF_Pixel*)calloc(frame_height*frame_line,1);
 if (buffer==NULL)
 {
  puts("Can't create render buffer");
  exit(EXIT_FAILURE);
 }

}

unsigned long int SWGF_Draw::get_frame_width()
{
 return frame_width;
}

unsigned long int SWGF_Draw::get_frame_height()
{
 return frame_height;
}

void SWGF_Draw::draw_pixel(const unsigned long int x,const unsigned long int y,const unsigned char red,const unsigned char green,const unsigned char blue)
{
 unsigned long int offset;
 if ((x<frame_width)&&(y<frame_height))
 {
  offset=x+y*frame_width;
  buffer[offset].red=red;
  buffer[offset].green=green;
  buffer[offset].blue=blue;
  buffer[offset].alpha=0;
 }

}

void SWGF_Draw::clear_screen()
{
 unsigned long int x,y;
 for (x=0;x<frame_width;x++)
 {
  for (y=0;y<frame_height;y++)
  {
   this->draw_pixel(x,y,0,0,0);
  }

 }

}

class SWGF_Screen:public SWGF_Base, public SWGF_Engine, public SWGF_Draw
{
 private:
 ID2D1Factory *render;
 ID2D1HwndRenderTarget *target;
 ID2D1Bitmap *surface;
 D2D1_RENDER_TARGET_PROPERTIES setting;
 D2D1_HWND_RENDER_TARGET_PROPERTIES configuration;
 D2D1_RECT_U source;
 D2D1_RECT_F destanation;
 D2D1_RECT_F texture;
 void create_factory();
 void create_target();
 void create_surface();
 void set_render_setting();
 void set_render();
 void destroy_render();
 void prepare_surface();
 void create_render();
 void update_surface();
 void refresh();
 public:
 SWGF_Screen();
 ~SWGF_Screen();
 void initialize();
 void set_mode(const unsigned long int new_width,const unsigned long int new_height);
 bool begin_sync();
 void end_sync();
 SWGF_Screen* get_handle();
};

SWGF_Screen::SWGF_Screen()
{
 render=NULL;
 target=NULL;
 surface=NULL;
}

SWGF_Screen::~SWGF_Screen()
{
 if(surface!=NULL) surface->Release();
 if(target!=NULL) target->Release();
 if(render!=NULL) render->Release();
}

void SWGF_Screen::create_factory()
{
 if(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED,&render)!=S_OK)
 {
  puts("Can't create render");
  exit(EXIT_FAILURE);
 }

}

void SWGF_Screen::create_target()
{
 if(render->CreateHwndRenderTarget(setting,configuration,&target)!=S_OK)
 {
  puts("Can't create render target");
  exit(EXIT_FAILURE);
 }
 target->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
}

void SWGF_Screen::create_surface()
{
 if(target->CreateBitmap(D2D1::SizeU(frame_width,frame_height),D2D1::BitmapProperties(setting.pixelFormat,96.0,96.0),&surface)!=S_OK)
 {
  puts("Can't create render surface");
  exit(EXIT_FAILURE);
 }

}

void SWGF_Screen::set_render_setting()
{
 setting=D2D1::RenderTargetProperties();
 setting.type=D2D1_RENDER_TARGET_TYPE_HARDWARE;
 setting.pixelFormat=D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,D2D1_ALPHA_MODE_IGNORE);
 setting.usage=D2D1_RENDER_TARGET_USAGE_NONE;
 setting.minLevel=D2D1_FEATURE_LEVEL_9;
 configuration.hwnd=window;
 configuration.pixelSize=D2D1::SizeU(width,height);
 configuration.presentOptions=D2D1_PRESENT_OPTIONS_IMMEDIATELY;
}

void SWGF_Screen::set_render()
{
 this->create_factory();
 this->create_target();
 this->create_surface();
}

void SWGF_Screen::destroy_render()
{
 if(surface!=NULL)
 {
  surface->Release();
  surface=NULL;
 }
 if(target!=NULL)
 {
  target->Release();
  target=NULL;
 }
 if(render!=NULL)
 {
  render->Release();
  render=NULL;
 }

}

void SWGF_Screen::prepare_surface()
{
 source=D2D1::RectU(0,0,frame_width,frame_height);
 destanation=D2D1::RectF(0,0,width,height);
 texture=D2D1::RectF(0,0,frame_width,frame_height);
}

void SWGF_Screen::create_render()
{
 this->destroy_render();
 this->set_render_setting();
 this->set_render();
 this->prepare_surface();
 this->create_render_buffer();
}

void SWGF_Screen::update_surface()
{
 surface->CopyFromMemory(&source,buffer,frame_line);
}

void SWGF_Screen::refresh()
{
 target->BeginDraw();
 target->DrawBitmap(surface,destanation,1.0,D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,texture);
 target->EndDraw();
}

void SWGF_Screen::initialize()
{
 this->check_video_mode();
 this->destroy_render();
 this->create_window();
 this->create_render();
 this->capture_mouse();
}

void SWGF_Screen::set_mode(const unsigned long int new_width,const unsigned long int new_height)
{

 DEVMODE mode;
 mode=this->get_video_mode();
 mode.dmPelsWidth=new_width;
 mode.dmPelsHeight=new_height;
 this->destroy_render();
 this->destroy_window();
 this->set_video_mode(mode);
 this->initialize();
}

bool SWGF_Screen::begin_sync()
{
 this->update_surface();
 this->set_timer();
 this->refresh();
 return this->process_message();
}

void SWGF_Screen::end_sync()
{
 this->wait_timer();
}

SWGF_Screen* SWGF_Screen::get_handle()
{
 return this;
}

class SWGF_Keyboard
{
 public:
 bool check_press(const unsigned char code);
 bool check_release(const unsigned char code);
 bool check_push(const unsigned char code);
};

bool SWGF_Keyboard::check_press(const unsigned char code)
{
 bool result;
 result=false;
 if(GetAsyncKeyState(MapVirtualKey(code,MAPVK_VSC_TO_VK))>0) result=true;
 return result;
}

bool SWGF_Keyboard::check_release(const unsigned char code)
{
 bool result;
 result=false;
 if(GetAsyncKeyState(MapVirtualKey(code,MAPVK_VSC_TO_VK))<0) result=true;
 return result;
}

bool SWGF_Keyboard::check_push(const unsigned char code)
{
 bool result;
 result=false;
 if(GetAsyncKeyState(MapVirtualKey(code,MAPVK_VSC_TO_VK))!=0) result=true;
 return result;
}

class SWGF_Mouse
{
 private:
 POINT position;
 public:
 SWGF_Mouse();
 ~SWGF_Mouse();
 unsigned char get_pressed_button();
 unsigned char get_released_button();
 unsigned char get_pushed_button();
 void set_position(const unsigned long int x,const unsigned long int y);
 unsigned long int get_x();
 unsigned long int get_y();
};

SWGF_Mouse::SWGF_Mouse()
{
 position.x=0;
 position.y=0;
}

SWGF_Mouse::~SWGF_Mouse()
{

}

unsigned char SWGF_Mouse::get_pressed_button()
{
 unsigned char result;
 result=SWGF_MOUSE_NONE;
 if(GetAsyncKeyState(VK_LBUTTON)<0) result=SWGF_MOUSE_LEFT;
 if(GetAsyncKeyState(VK_RBUTTON)<0) result=SWGF_MOUSE_RIGHT;
 if(GetAsyncKeyState(VK_MBUTTON)<0) result=SWGF_MOUSE_MIDDLE;
 return result;
}

unsigned char SWGF_Mouse::get_released_button()
{
 unsigned char result;
 result=SWGF_MOUSE_NONE;
 if(GetAsyncKeyState(VK_LBUTTON)>0) result=SWGF_MOUSE_LEFT;
 if(GetAsyncKeyState(VK_RBUTTON)>0) result=SWGF_MOUSE_RIGHT;
 if(GetAsyncKeyState(VK_MBUTTON)>0) result=SWGF_MOUSE_MIDDLE;
 return result;
}

unsigned char SWGF_Mouse::get_pushed_button()
{
 unsigned char result;
 result=SWGF_MOUSE_NONE;
 if(GetAsyncKeyState(VK_LBUTTON)!=0) result=SWGF_MOUSE_LEFT;
 if(GetAsyncKeyState(VK_RBUTTON)!=0) result=SWGF_MOUSE_RIGHT;
 if(GetAsyncKeyState(VK_MBUTTON)!=0) result=SWGF_MOUSE_MIDDLE;
 return result;
}

void SWGF_Mouse::set_position(const unsigned long int x,const unsigned long int y)
{
 if(SetCursorPos(x,y)==FALSE)
 {
  puts("Can't set the mouse cursor position");
  exit(EXIT_FAILURE);
 }

}

unsigned long int SWGF_Mouse::get_x()
{
 if(GetCursorPos(&position)==FALSE)
 {
  puts("Can't get the mouse cursor position");
  exit(EXIT_FAILURE);
 }
 return position.x;
}

unsigned long int SWGF_Mouse::get_y()
{
 if(GetCursorPos(&position)==FALSE)
 {
  puts("Can't get the mouse cursor position");
  exit(EXIT_FAILURE);
 }
 return position.y;
}

class SWGF_Gamepad
{
 private:
 XINPUT_STATE state;
 XINPUT_VIBRATION vibration;
 unsigned long int active;
 bool read_state();
 bool write_state();
 void set_motor(unsigned short int left,unsigned short int right);
 public:
 SWGF_Gamepad();
 ~SWGF_Gamepad();
 void set_active(unsigned long int gamepad);
 bool check_connection();
 bool check_button(unsigned short int button);
 bool check_trigger(unsigned char trigger);
 bool set_vibration(unsigned short int left,unsigned short int right);
 char get_stick_x(unsigned char stick);
 char get_stick_y(unsigned char stick);
};

SWGF_Gamepad::SWGF_Gamepad()
{
 XInputEnable(TRUE);
 memset(&state,0,sizeof(XINPUT_STATE));
 memset(&vibration,0,sizeof(XINPUT_VIBRATION));
 active=0;
}

SWGF_Gamepad::~SWGF_Gamepad()
{
 XInputEnable(FALSE);
}

bool SWGF_Gamepad::read_state()
{
 bool result;
 result=false;
 if(XInputGetState(active,&state)==ERROR_SUCCESS) result=true;
 return result;
}

bool SWGF_Gamepad::write_state()
{
 bool result;
 result=false;
 if(XInputSetState(active,&vibration)==ERROR_SUCCESS) result=true;
 return result;
}

void SWGF_Gamepad::set_motor(unsigned short int left,unsigned short int right)
{
 vibration.wLeftMotorSpeed=left;
 vibration.wRightMotorSpeed=right;
}

void SWGF_Gamepad::set_active(unsigned long int gamepad)
{
 active=gamepad;
}

bool SWGF_Gamepad::check_connection()
{
 return this->read_state();
}

bool SWGF_Gamepad::check_button(unsigned short int button)
{
 bool result;
 result=false;
 if(this->read_state()==true)
 {
  if(state.Gamepad.wButtons&button) result=true;
 }
 return result;
}

bool SWGF_Gamepad::check_trigger(unsigned char trigger)
{
 bool result;
 result=false;
 if(this->read_state()==true)
 {
  if((trigger==SWGF_GAMEPAD_LEFT_TRIGGER)&&(state.Gamepad.bLeftTrigger>=XINPUT_GAMEPAD_TRIGGER_THRESHOLD)) result=true;
  if((trigger==SWGF_GAMEPAD_RIGHT_TRIGGER)&&(state.Gamepad.bRightTrigger>=XINPUT_GAMEPAD_TRIGGER_THRESHOLD)) result=true;
 }
 return result;

}

bool SWGF_Gamepad::set_vibration(unsigned short int left,unsigned short int right)
{
 this->set_motor(left,right);
 return this->write_state();
}

char SWGF_Gamepad::get_stick_x(unsigned char stick)
{
 char result;
 short int control;
 result=0;
 if(this->read_state()==true)
 {
  if(stick==SWGF_GAMEPAD_LEFT_STICK)
  {
   control=32767-XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
   if(state.Gamepad.sThumbLX>=control) result=1;
   if(state.Gamepad.sThumbLX<=-1*control) result=-1;
  }
  if(stick==SWGF_GAMEPAD_RIGHT_STICK)
  {
   control=32767-XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
   if(state.Gamepad.sThumbRX>=control) result=1;
   if(state.Gamepad.sThumbRX<=-1*control) result=-1;
  }

 }
 return result;
}

char SWGF_Gamepad::get_stick_y(unsigned char stick)
{
 char result;
 short int control;
 result=0;
 if(this->read_state()==true)
 {
  if(stick==SWGF_GAMEPAD_LEFT_STICK)
  {
   control=32767-XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
   if(state.Gamepad.sThumbLY>=control) result=1;
   if(state.Gamepad.sThumbLY<=-1*control) result=-1;
  }
  if(stick==SWGF_GAMEPAD_RIGHT_STICK)
  {
   control=32767-XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
   if(state.Gamepad.sThumbRY>=control) result=1;
   if(state.Gamepad.sThumbRY<=-1*control) result=-1;
  }

 }
 return result;
}

class SWGF_Multimedia: public SWGF_Base
{
 private:
 IGraphBuilder *loader;
 IMediaControl *player;
 IMediaSeeking *controler;
 IVideoWindow *video;
 wchar_t *convert_file_name(const char *target);
 void open(const wchar_t *target);
 public:
 SWGF_Multimedia();
 ~SWGF_Multimedia();
 void load(const char *target);
 void play();
 bool check_playing();
};

SWGF_Multimedia::SWGF_Multimedia()
{
 if(CoCreateInstance(CLSID_FilterGraph,NULL,CLSCTX_INPROC_SERVER,IID_IGraphBuilder,(void**)&loader)!=S_OK)
 {
  puts("Can't create a multimedia loader");
  exit(EXIT_FAILURE);
 }
 if(loader->QueryInterface(IID_IMediaControl,(void**)&player)!=S_OK)
 {
  puts("Can't create a multimedia player");
  exit(EXIT_FAILURE);
 }
 if(loader->QueryInterface(IID_IMediaSeeking,(void**)&controler)!=S_OK)
 {
  puts("Can't create a player controler");
  exit(EXIT_FAILURE);
 }
 if(loader->QueryInterface(IID_IVideoWindow,(void**)&video)!=S_OK)
 {
  puts("Can't create a video player");
  exit(EXIT_FAILURE);
 }

}


SWGF_Multimedia::~SWGF_Multimedia()
{
 player->Stop();
 video->Release();
 controler->Release();
 player->Release();
 loader->Release();
}

wchar_t *SWGF_Multimedia::convert_file_name(const char *target)
{
 wchar_t *name;
 unsigned long int index,length;
 length=strlen(target);
 name=(wchar_t*)calloc(length+1,sizeof(wchar_t));
 if(name==NULL)
 {
  puts("Can't allocate memory");
  exit(EXIT_FAILURE);
 }
 for(index=0;index<length;index++) name[index]=target[index];
 return name;
}

void SWGF_Multimedia::open(const wchar_t *target)
{
 player->Stop();
 if(loader->RenderFile(target,NULL)!=S_OK)
 {
  puts("Can't load a multimedia file");
  exit(EXIT_FAILURE);
 }
 if(controler->SetRate(1)!=S_OK)
 {
  puts("Can't set playing rate");
  exit(EXIT_FAILURE);
 }
 video->put_FullScreenMode(OATRUE);
}

void SWGF_Multimedia::load(const char *target)
{
 wchar_t *name;
 name=this->convert_file_name(target);
 this->open(name);
 free(name);
}

void SWGF_Multimedia::play()
{
 long long position;
 position=0;
 player->Stop();
 if(controler->SetPositions(&position,AM_SEEKING_AbsolutePositioning,NULL,AM_SEEKING_NoPositioning)!=S_OK)
 {
  puts("Can't set start position");
  exit(EXIT_FAILURE);
 }
 player->Run();
}

bool SWGF_Multimedia::check_playing()
{
 bool result;
 long long current,stop;
 result=false;
 if(controler->GetPositions(&current,&stop)==S_OK)
 {
  if(current<stop) result=true;
 }
 return result;
}

class SWGF_Memory
{
 private:
 MEMORYSTATUSEX memory;
 public:
 SWGF_Memory();
 ~SWGF_Memory();
 unsigned long long int get_total_memory();
 unsigned long int long get_free_memory();
};

SWGF_Memory::SWGF_Memory()
{
 memory.dwLength=sizeof(MEMORYSTATUSEX);
}

SWGF_Memory::~SWGF_Memory()
{

}

unsigned long long int SWGF_Memory::get_total_memory()
{
 GlobalMemoryStatusEx(&memory);
 return memory.ullTotalPhys;
}

unsigned long long int SWGF_Memory::get_free_memory()
{
 GlobalMemoryStatusEx(&memory);
 return memory.ullAvailPhys;
}

class SWGF_System
{
 public:
 SWGF_System();
 ~SWGF_System();
 unsigned long int get_random(const unsigned long int number);
 void pause(const unsigned int long second);
 void quit();
 void run(const char *command);
 char* read_environment(const char *variable);
};

SWGF_System::SWGF_System()
{
 srand(time(NULL));
}

SWGF_System::~SWGF_System()
{

}

unsigned long int SWGF_System::get_random(const unsigned long int number)
{
 return rand()%number;
}

void SWGF_System::pause(const unsigned int long second)
{
 time_t start,stop;
 start=time(NULL);
 do
 {
  stop=time(NULL);
 } while(difftime(stop,start)<second);

}

void SWGF_System::quit()
{
 exit(EXIT_SUCCESS);
}

void SWGF_System::run(const char *command)
{
 system(command);
}

char* SWGF_System::read_environment(const char *variable)
{
 return getenv(variable);
}

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

SWGF_Timer::SWGF_Timer()
{
 interval=0;
 start=time(NULL);
}

SWGF_Timer::~SWGF_Timer()
{

}

void SWGF_Timer::set_timer(const unsigned long int seconds)
{
 interval=seconds;
 start=time(NULL);
}

bool SWGF_Timer::check_timer()
{
 bool result;
 time_t stop;
 result=false;
 stop=time(NULL);
 if(difftime(stop,start)>=interval)
 {
  result=true;
  start=time(NULL);
 }
 return result;
}

class SWGF_Primitive
{
 private:
 SWGF_Screen *surface;
 public:
 SWGF_Primitive();
 ~SWGF_Primitive();
 void initialize(SWGF_Screen *Screen);
 void draw_line(const unsigned long int x1,const unsigned long int y1,const unsigned long int x2,const unsigned long int y2,const unsigned char red,const unsigned char green,const unsigned char blue);
 void draw_rectangle(const unsigned long int *vertex,const unsigned char red,const unsigned char green,const unsigned char blue);
 void draw_filled_rectangle(const unsigned long int *vertex,const unsigned char red,const unsigned char green,const unsigned char blue);
 void draw_trianlge(const unsigned long int *vertex,const unsigned char red,const unsigned char green,const unsigned char blue);
 void draw_filled_trianlge(const unsigned long int *vertex,const unsigned char red,const unsigned char green,const unsigned char blue);
};

SWGF_Primitive::SWGF_Primitive()
{
 surface=NULL;
}

SWGF_Primitive::~SWGF_Primitive()
{
 surface=NULL;
}

void SWGF_Primitive::initialize(SWGF_Screen *Screen)
{
 surface=Screen;
}

void SWGF_Primitive::draw_line(const unsigned long int x1,const unsigned long int y1,const unsigned long int x2,const unsigned long int y2,const unsigned char red,const unsigned char green,const unsigned char blue)
{
 unsigned long int delta_x,delta_y,index,steps;
 float x,y,shift_x,shift_y;
 if (x1>x2)
 {
  delta_x=x1-x2;
 }
 else
 {
  delta_x=x2-x1;
 }
 if (y1>y2)
 {
  delta_y=y1-y2;
 }
 else
 {
  delta_y=y2-y1;
 }
 steps=delta_x;
 if (steps<delta_y) steps=delta_y;
 x=x1;
 y=y1;
 shift_x=(float)delta_x/(float)steps;
 shift_y=(float)delta_y/(float)steps;
 for (index=steps;index>0;index--)
 {
  x+=shift_x;
  y+=shift_y;
  surface->draw_pixel(x,y,red,green,blue);
 }

}

void SWGF_Primitive::draw_rectangle(const unsigned long int *vertex,const unsigned char red,const unsigned char green,const unsigned char blue)
{
 this->draw_line(vertex[0],vertex[1],vertex[2],vertex[3],red,green,blue);
 this->draw_line(vertex[0],vertex[3],vertex[2],vertex[3],red,green,blue);
 this->draw_line(vertex[0],vertex[1],vertex[0],vertex[3],red,green,blue);
 this->draw_line(vertex[2],vertex[1],vertex[2],vertex[3],red,green,blue);
}

void SWGF_Primitive::draw_filled_rectangle(const unsigned long int *vertex,const unsigned char red,const unsigned char green,const unsigned char blue)
{
 unsigned long int x,y,stop_x,stop_y;
 if (vertex[0]>vertex[2])
 {
  stop_x=vertex[0]+(vertex[0]-vertex[2]);
 }
 else
 {
  stop_x=vertex[0]+(vertex[2]-vertex[0]);
 }
 if (vertex[1]>vertex[3])
 {
  stop_y=vertex[1]+(vertex[1]-vertex[3]);
 }
 else
 {
  stop_y=vertex[1]+(vertex[3]-vertex[1]);
 }
 for (x=vertex[0];x<=stop_x;x++)
 {
  for (y=vertex[1];y<=stop_y;y++)
  {
   surface->draw_pixel(x,y,red,green,blue);
  }

 }

}

void SWGF_Primitive::draw_trianlge(const unsigned long int *vertex,const unsigned char red,const unsigned char green,const unsigned char blue)
{
 this->draw_line(vertex[0],vertex[1],vertex[2],vertex[3],red,green,blue);
 this->draw_line(vertex[2],vertex[3],vertex[4],vertex[5],red,green,blue);
 this->draw_line(vertex[0],vertex[1],vertex[4],vertex[5],red,green,blue);
}

void SWGF_Primitive::draw_filled_trianlge(const unsigned long int *vertex,const unsigned char red,const unsigned char green,const unsigned char blue)
{
 unsigned long int x,y;
 this->draw_trianlge(vertex,red,green,blue);
 for (x=vertex[0];x<vertex[4];x++)
 {
  for (y=vertex[2];y<=vertex[5];y++)
  {
   this->draw_line(vertex[0],vertex[1],x,y,red,green,blue);
  }

 }

}

class SWGF_Canvas
{
 private:
 unsigned long int width;
 unsigned long int height;
 unsigned long int current_x;
 unsigned long int current_y;
 SWGF_Screen *surface;
 SWGF_Color *image;
 public:
 SWGF_Canvas();
 ~SWGF_Canvas();
 unsigned long int get_width();
 unsigned long int get_height();
 unsigned long int get_x();
 unsigned long int get_y();
 void initialize(SWGF_Screen *Screen);
 void load_image(const unsigned char *buffer,const unsigned long int image_width,const unsigned long int image_height);
 void mirror_image(const unsigned char kind);
 void resize_image(const unsigned long int new_width,const unsigned long int new_height);
 void draw_background();
 void draw_sprite_frame(const unsigned long int x,const unsigned long int y,const unsigned long int frame,const unsigned long int amount);
 void draw_sprite(const unsigned long int x,const unsigned long int y);
 unsigned long int draw_text(const unsigned long int x,const unsigned long int y,const char *text);
};

SWGF_Canvas::SWGF_Canvas()
{
 image=NULL;
 surface=NULL;
 width=0;
 height=0;
 current_x=0;
 current_y=0;
}

SWGF_Canvas::~SWGF_Canvas()
{
 surface=NULL;
 if(image!=NULL) free(image);
}

unsigned long int SWGF_Canvas::get_width()
{
 return width;
}

unsigned long int SWGF_Canvas::get_height()
{
 return height;
}

unsigned long int SWGF_Canvas::get_x()
{
 return current_x;
}

unsigned long int SWGF_Canvas::get_y()
{
 return current_y;
}

void SWGF_Canvas::initialize(SWGF_Screen *Screen)
{
 surface=Screen;
}

void SWGF_Canvas::load_image(const unsigned char *buffer,const unsigned long int image_width,const unsigned long int image_height)
{
 unsigned long int length;
 length=image_width*image_height*3;
 width=image_width;
 height=image_height;
 if(image!=NULL) free(image);
 image=(SWGF_Color*)calloc(length,1);
 if (image==NULL)
 {
  puts("Can't allocate memory for image buffer");
  exit(EXIT_FAILURE);
 }
 memmove(image,buffer,length);
}

void SWGF_Canvas::mirror_image(const unsigned char kind)
{
 unsigned long int x,y,index,index2;
 SWGF_Color *mirrored_image;
 mirrored_image=(SWGF_Color*)calloc(width*height,3);
 if (mirrored_image==NULL)
 {
  puts("Can't allocate memory for image buffer");
  exit(EXIT_FAILURE);
 }
 if (kind==0)
 {
  for (x=0;x<width;x++)
  {
   for (y=0;y<height;y++)
   {
    index=x+(y*width);
    index2=(width-x-1)+(y*width);
    mirrored_image[index]=image[index2];
   }

  }

 }
 else
 {
   for (x=0;x<width;x++)
  {
   for (y=0;y<height;y++)
   {
    index=x+(y*width);
    index2=x+(height-y-1)*width;
    mirrored_image[index]=image[index2];
   }

  }

 }
 free(image);
 image=mirrored_image;
}

void SWGF_Canvas::resize_image(const unsigned long int new_width,const unsigned long int new_height)
{
 float x_ratio,y_ratio;
 unsigned long int x,y,index,index2;
 SWGF_Color *scaled_image;
 scaled_image=(SWGF_Color*)calloc(new_width*new_height,3);
 if (scaled_image==NULL)
 {
  puts("Can't allocate memory for image buffer");
  exit(EXIT_FAILURE);
 }
 x_ratio=(float)width/(float)new_width;
 y_ratio=(float)height/(float)new_height;
 for (x=0;x<new_width;x++)
 {
  for (y=0;y<new_height;y++)
  {
   index=x+(y*new_width);
   index2=(unsigned long int)(x_ratio*(float)x)+width*(unsigned long int)(y_ratio*(float)y);
   scaled_image[index]=image[index2];
  }

 }
 free(image);
 image=scaled_image;
 width=new_width;
 height=new_height;
}

void SWGF_Canvas::draw_background()
{
 unsigned long int x,y,offset;
 for (x=0;x<width;x++)
 {
  for (y=0;y<height;y++)
  {
   offset=x+(width*y);
   surface->draw_pixel(x,y,image[offset].red,image[offset].green,image[offset].blue);
  }

 }

}

void SWGF_Canvas::draw_sprite_frame(const unsigned long int x,const unsigned long int y,const unsigned long int frame,const unsigned long int amount)
{
 unsigned long int image_x,image_y,offset,start,frame_width;
 current_x=x;
 current_y=y;
 frame_width=width/amount;
 start=(frame-1)*frame_width;
 for(image_x=0;image_x<frame_width;image_x++)
 {
  for(image_y=0;image_y<height;image_y++)
  {
   offset=start+image_x+(image_y*width);
   if(memcmp(&image[0],&image[offset],3)!=0) surface->draw_pixel(x+image_x,y+image_y,image[offset].red,image[offset].green,image[offset].blue);
  }

 }

}

void SWGF_Canvas::draw_sprite(const unsigned long int x,const unsigned long int y)
{
 current_x=x;
 current_y=y;
 this->draw_sprite_frame(x,y,1,1);
}

unsigned long int SWGF_Canvas::draw_text(const unsigned long int x,const unsigned long int y,const char *text)
{
 unsigned long int index,length,font_width;
 current_x=x;
 current_y=y;
 font_width=width/128;
 length=strlen(text);
 for (index=0;index<length;index++)
 {
  if (text[index]>31)
  {
   this->draw_sprite_frame(current_x,current_y,text[index]+1,128);
   current_x+=font_width;
  }

 }
 return current_x;
}

class SWGF_Collision
{
 public:
 bool check_horizontal_collision(SWGF_Canvas &first,SWGF_Canvas &second);
 bool check_vertical_collision(SWGF_Canvas &first,SWGF_Canvas &second);
 bool check_collision(SWGF_Canvas &first,SWGF_Canvas &second);
};

bool SWGF_Collision::check_horizontal_collision(SWGF_Canvas &first,SWGF_Canvas &second)
{
 bool result;
 result=false;
 if((first.get_x()+first.get_width())>=second.get_x())
 {
  if(first.get_x()<=(second.get_x()+second.get_width())) result=true;
 }
 return result;
}

bool SWGF_Collision::check_vertical_collision(SWGF_Canvas &first,SWGF_Canvas &second)
{
 bool result;
 result=false;
 if((first.get_y()+first.get_height())>=second.get_y())
 {
  if(first.get_y()<=(second.get_y()+second.get_height())) result=true;
 }
 return result;
}

bool SWGF_Collision::check_collision(SWGF_Canvas &first,SWGF_Canvas &second)
{
 bool result;
 result=false;
 if((this->check_horizontal_collision(first,second)==true)&&(this->check_vertical_collision(first,second)==true)) result=true;
 return result;
}

class SWGF_Image
{
 public:
 void load_tga(const char *name,SWGF_Canvas &Canvas);
 void load_pcx(const char *name,SWGF_Canvas &Canvas);
};

void SWGF_Image::load_tga(const char *name,SWGF_Canvas &Canvas)
{
 FILE *target;
 unsigned long int index,position,amount,compressed_length,uncompressed_length;
 unsigned char *compressed;
 unsigned char *uncompressed;
 TGA_head head;
 TGA_map color_map;
 TGA_image image;
 target=fopen(name,"rb");
 if(target==NULL)
 {
  puts("Can't open a image file");
  exit(EXIT_FAILURE);
 }
 fseek(target,0,SEEK_END);
 compressed_length=ftell(target)-18;
 rewind(target);
 fread(&head,3,1,target);
 fread(&color_map,5,1,target);
 fread(&image,10,1,target);
 if((head.color_map!=0)||(image.color!=24))
 {
  puts("Invalid image format");
  exit(EXIT_FAILURE);
 }
 if(head.type!=2)
 {
  if(head.type!=10)
  {
   puts("Invalid image format");
   exit(EXIT_FAILURE);
  }

 }
 index=0;
 position=0;
 uncompressed_length=3*(unsigned long int)image.width*(unsigned long int)image.height;
 uncompressed=(unsigned char*)calloc(uncompressed_length,1);
 if(uncompressed==NULL)
 {
  puts("Can't allocate memory for image buffer");
  exit(EXIT_FAILURE);
 }
 if(head.type==2)
 {
  fread(uncompressed,uncompressed_length,1,target);
  fclose(target);
 }
 if(head.type==10)
 {
  compressed=(unsigned char*)calloc(compressed_length,1);
  if(compressed==NULL)
  {
   puts("Can't allocate memory for image buffer");
   exit(EXIT_FAILURE);
  }
  fread(compressed,compressed_length,1,target);
  fclose(target);
  while(index<uncompressed_length)
  {
   if(compressed[position]<128)
   {
    amount=compressed[position]+1;
    amount*=3;
    memmove(uncompressed+index,compressed+(position+1),amount);
    index+=amount;
    position+=1+amount;
   }
   else
   {
    for(amount=compressed[position]-127;amount>0;amount--)
    {
     memmove(uncompressed+index,compressed+(position+1),3);
     index+=3;
    }
    position+=4;
   }

  }
  free(compressed);
 }
 Canvas.load_image(uncompressed,image.width,image.height);
 free(uncompressed);
}

void SWGF_Image::load_pcx(const char *name,SWGF_Canvas &Canvas)
{
 FILE *target;
 unsigned long int width,heigth,x,y,index,position,line,row,length,uncompressed_length;
 unsigned char repeat;
 unsigned char *original;
 unsigned char *uncompressed;
 PCX_head head;
 target=fopen(name,"rb");
 if(target==NULL)
 {
  puts("Can't open a image file");
  exit(EXIT_FAILURE);
 }
 fseek(target,0,SEEK_END);
 length=ftell(target)-128;
 rewind(target);
 fread(&head,128,1,target);
 if((head.color*head.planes!=24)&&(head.compress!=1))
 {
  puts("Incorrect image format");
  exit(EXIT_FAILURE);
 }
 width=head.max_x-head.min_x+1;
 heigth=head.max_y-head.min_y+1;
 row=3*width;
 line=head.planes*head.plane_length;
 uncompressed_length=row*heigth;
 index=0;
 position=0;
 original=(unsigned char*)calloc(length,1);
 if(original==NULL)
 {
  puts("Can't allocate memory for image buffer");
  exit(EXIT_FAILURE);
 }
 uncompressed=(unsigned char*)calloc(uncompressed_length,1);
 if(uncompressed==NULL)
 {
  puts("Can't allocate memory for image buffer");
  exit(EXIT_FAILURE);
 }
 fread(original,length,1,target);
 fclose(target);
 while (index<length)
 {
  if (original[index]<192)
  {
   uncompressed[position]=original[index];
   position++;
   index++;
  }
  else
  {
   for (repeat=original[index]-192;repeat>0;repeat--)
   {
    uncompressed[position]=original[index+1];
    position++;
   }
   index+=2;
  }

 }
 free(original);
 original=(unsigned char*)calloc(uncompressed_length,1);
 if(original==NULL)
 {
  puts("Can't allocate memory for image buffer");
  exit(EXIT_FAILURE);
 }
 for(x=0;x<width;x++)
 {
  for(y=0;y<heigth;y++)
  {
   index=x*3+y*row;
   position=x+y*line;
   original[index]=uncompressed[position+2*head.plane_length];
   original[index+1]=uncompressed[position+head.plane_length];
   original[index+2]=uncompressed[position];
  }

 }
 free(uncompressed);
 Canvas.load_image(original,width,heigth);
 free(original);
}