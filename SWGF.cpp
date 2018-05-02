// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

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

#include "SWGF.h"

unsigned char SWGF_Keys[SWGF_KEYBOARD];
unsigned char SWGF_Buttons[SWGF_MOUSE];

LRESULT CALLBACK SWGF_Process_Message(HWND window,UINT Message,WPARAM wParam,LPARAM lParam)
{
 switch (Message)
 {
  case WM_CLOSE:
  DestroyWindow(window);
  break;
  case WM_DESTROY:
  PostQuitMessage(0);
  break;
  case WM_CREATE:
  memset(SWGF_Keys,SWGFKEY_NONE,SWGF_KEYBOARD);
  memset(SWGF_Buttons,SWGFKEY_NONE,SWGF_MOUSE);
  break;
  case WM_LBUTTONDOWN:
  SWGF_Buttons[SWGF_MOUSE_LEFT]=SWGFKEY_PRESS;
  break;
  case WM_LBUTTONUP:
  SWGF_Buttons[SWGF_MOUSE_LEFT]=SWGFKEY_RELEASE;
  break;
  case WM_RBUTTONDOWN:
  SWGF_Buttons[SWGF_MOUSE_RIGHT]=SWGFKEY_PRESS;
  break;
  case WM_RBUTTONUP:
  SWGF_Buttons[SWGF_MOUSE_RIGHT]=SWGFKEY_RELEASE;
  break;
  case WM_MBUTTONDOWN:
  SWGF_Buttons[SWGF_MOUSE_MIDDLE]=SWGFKEY_PRESS;
  break;
  case WM_MBUTTONUP:
  SWGF_Buttons[SWGF_MOUSE_MIDDLE]=SWGFKEY_RELEASE;
  break;
  case WM_KEYDOWN:
  SWGF_Keys[(lParam >> 16)&0x7f]=SWGFKEY_PRESS;
  break;
  case WM_KEYUP:
  SWGF_Keys[(lParam >> 16)&0x7f]=SWGFKEY_RELEASE;
  break;
 }
 return DefWindowProc(window,Message,wParam,lParam);
}

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

SWGF_Synchronization::SWGF_Synchronization()
{
 timer=NULL;
}

SWGF_Synchronization::~SWGF_Synchronization()
{
 if(timer==NULL)
 {
  CancelWaitableTimer(timer);
  CloseHandle(timer);
 }

}

void SWGF_Synchronization::create_timer()
{
 timer=CreateWaitableTimer(NULL,FALSE,NULL);
 if (timer==NULL)
 {
  puts("Can't create synchronization timer");
  exit(EXIT_FAILURE);
 }

}

void SWGF_Synchronization::set_timer(const unsigned long int interval)
{
 LARGE_INTEGER start;
 start.QuadPart=0;
 if(SetWaitableTimer(timer,&start,interval,NULL,NULL,FALSE)==FALSE)
 {
  puts("Can't set timer");
  exit(EXIT_FAILURE);
 }

}

void SWGF_Synchronization::wait_timer()
{
 WaitForSingleObject(timer,INFINITE);
}

SWGF_Engine::SWGF_Engine()
{
 window_class.lpszClassName=TEXT("SWGF");
 window_class.style=CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
 window_class.lpfnWndProc=(WNDPROC)SWGF_Process_Message;
 window_class.hInstance=NULL;
 window_class.hbrBackground=NULL;
 window_class.hIcon=NULL;
 window_class.hCursor=NULL;
 window_class.cbClsExtra=0;
 window_class.cbWndExtra=0;
 window=NULL;
 width=0;
 height=0;
}

SWGF_Engine::~SWGF_Engine()
{
 if(window!=NULL) CloseWindow(window);
 UnregisterClass(window_class.lpszClassName,window_class.hInstance);
}

void SWGF_Engine::prepare_engine()
{
 window_class.hInstance=GetModuleHandle(NULL);
 if(window_class.hInstance==NULL)
 {
  puts("Can't get the application instance");
  exit(EXIT_FAILURE);
 }
 window_class.hbrBackground=(HBRUSH)GetStockObject(BLACK_BRUSH);
 if (window_class.hbrBackground==NULL)
 {
  puts("Can't set background color");
  exit(EXIT_FAILURE);
 }
 window_class.hIcon=LoadIcon(NULL,IDI_APPLICATION);
 if (window_class.hIcon==NULL)
 {
  puts("Can't load the standart program icon");
  exit(EXIT_FAILURE);
 }
 window_class.hCursor=LoadCursor(NULL,IDC_ARROW);
 if (window_class.hCursor==NULL)
 {
  puts("Can't load the standart cursor");
  exit(EXIT_FAILURE);
 }
 if (RegisterClass(&window_class)==0)
 {
  puts("Can't register window class");
  exit(EXIT_FAILURE);
 }

}

void SWGF_Engine::create_window()
{
 window=CreateWindow(window_class.lpszClassName,NULL,WS_VISIBLE|WS_POPUP,0,0,width,height,NULL,NULL,window_class.hInstance,NULL);
 if (window==NULL)
 {
  puts("Can't create window");
  exit(EXIT_FAILURE);
 }
 EnableWindow(window,TRUE);
 SetFocus(window);
}

void SWGF_Engine::destroy_window()
{
 if(window!=NULL) CloseWindow(window);
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
  if(GetMessage(&Message,window,0,0)==TRUE)
  {
   TranslateMessage(&Message);
   DispatchMessage(&Message);
  }
  else
  {
   quit=true;
   break;
  }

 }
 return quit;
}

unsigned long int SWGF_Engine::get_width()
{
 return width;
}

unsigned long int SWGF_Engine::get_height()
{
 return height;
}

SWGF_Frame::SWGF_Frame()
{
 frame_width=512;
 frame_height=512;
 buffer_length=0;
 buffer=NULL;
}

SWGF_Frame::~SWGF_Frame()
{
 if(buffer!=NULL)
 {
  free(buffer);
  buffer=NULL;
 }

}

void SWGF_Frame::create_render_buffer()
{
 buffer_length=(size_t)frame_width*(size_t)frame_height;
 buffer=(unsigned long int*)calloc(buffer_length,sizeof(unsigned long int));
 if(buffer==NULL)
 {
  puts("Can't allocate memory for render buffer");
  exit(EXIT_FAILURE);
 }
 else
 {
  buffer_length*=sizeof(unsigned long int);
 }

}

unsigned long int SWGF_Frame::get_rgb(const unsigned long int red,const unsigned long int green,const unsigned long int blue)
{
 return red+(green<<8)+(blue<<16);
}

void SWGF_Frame::draw_pixel(const unsigned long int x,const unsigned long int y,const unsigned char red,const unsigned char green,const unsigned char blue)
{
 if((x<frame_width)&&(y<frame_height))
 {
  buffer[(size_t)x+(size_t)y*(size_t)frame_width]=this->get_rgb(red,green,blue);
 }

}

void SWGF_Frame::clear_screen()
{
 memset(buffer,0,buffer_length);
}

unsigned long int SWGF_Frame::get_frame_width()
{
 return frame_width;
}

unsigned long int SWGF_Frame::get_frame_height()
{
 return frame_height;
}

SWGF_Display::SWGF_Display()
{
 memset(&display,0,sizeof(DEVMODE));
}

SWGF_Display::~SWGF_Display()
{
 ChangeDisplaySettings(NULL,0);
}

void SWGF_Display::set_video_mode(DEVMODE mode)
{
 if (ChangeDisplaySettings(&mode,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
 {
  puts("Can't change video mode");
  exit(EXIT_FAILURE);
 }
 else
 {
  width=mode.dmPelsWidth;
  height=mode.dmPelsHeight;
 }

}

DEVMODE SWGF_Display::get_video_mode()
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

void SWGF_Display::check_video_mode()
{
 display=this->get_video_mode();
 if(display.dmBitsPerPel<16) display.dmBitsPerPel=16;
 this->set_video_mode(display);
}

void SWGF_Display::reset_display()
{
 ChangeDisplaySettings(NULL,0);
}

void SWGF_Display::set_display_mode(const unsigned long int screen_width,const unsigned long int screen_height)
{
 display=this->get_video_mode();
 display.dmPelsWidth=screen_width;
 display.dmPelsHeight=screen_height;
 if(display.dmBitsPerPel<16) display.dmBitsPerPel=16;
 this->set_video_mode(display);
}

SWGF_Render::SWGF_Render()
{
 context=NULL;
 render=NULL;
 wglSwapIntervalEXT=NULL;
 texture=0;
 surface=0;
}

SWGF_Render::~SWGF_Render()
{
 if(surface!=0) glDeleteLists(surface,1);
 if(render!=NULL)
 {
  wglMakeCurrent(NULL,NULL);
  wglDeleteContext(render);
 }
 if(context!=NULL) ReleaseDC(window,context);
}

bool SWGF_Render::check_common_setting(const PIXELFORMATDESCRIPTOR &setting)
{
 bool result;
 result=false;
 if(setting.cColorBits==display.dmBitsPerPel)
 {
  if((setting.dwFlags&PFD_DRAW_TO_WINDOW)&&(setting.dwFlags&PFD_SUPPORT_OPENGL))
  {
   if(setting.dwFlags&PFD_DOUBLEBUFFER)
   {
    if((setting.iPixelType==PFD_TYPE_RGBA)&&(setting.iLayerType==PFD_MAIN_PLANE)) result=true;
   }

  }

 }
 return result;
}

bool SWGF_Render::check_acceleration(const PIXELFORMATDESCRIPTOR &setting)
{
 bool result;
 result=false;
 if(!(setting.dwFlags&PFD_GENERIC_FORMAT)&&!(setting.dwFlags&PFD_GENERIC_ACCELERATED))
 {
  result=true;
 }
 else
 {
  if((setting.dwFlags&PFD_GENERIC_FORMAT)&&(setting.dwFlags&PFD_GENERIC_ACCELERATED)) result=true;
 }
 return result;
}

int SWGF_Render::get_pixel_format()
{
 int index,result;
 unsigned long int length;
 PIXELFORMATDESCRIPTOR setting;
 result=0;
 length=sizeof(PIXELFORMATDESCRIPTOR);
 memset(&setting,0,length);
 setting.nSize=length;
 setting.nVersion=1;
 for(index=DescribePixelFormat(context,1,length,&setting);index>0;--index)
 {
  DescribePixelFormat(context,index,length,&setting);
  if(this->check_common_setting(setting)==true)
  {
   if(this->check_acceleration(setting)==true)
   {
    result=index;
    break;
   }

  }

 }
 return result;
}

void SWGF_Render::set_pixel_format(const int format)
{
 PIXELFORMATDESCRIPTOR setting;
 memset(&setting,0,sizeof(PIXELFORMATDESCRIPTOR));
 setting.nSize=sizeof(PIXELFORMATDESCRIPTOR);
 setting.nVersion=1;
 if(format==0)
 {
  puts("Invalid pixel format");
  exit(EXIT_FAILURE);
 }
 DescribePixelFormat(context,format,sizeof(PIXELFORMATDESCRIPTOR),&setting);
 if(SetPixelFormat(context,format,&setting)==FALSE)
 {
  puts("Can't set pixel format");
  exit(EXIT_FAILURE);
 }

}

void SWGF_Render::create_render_context()
{
 render=wglCreateContext(context);
 if(render==NULL)
 {
  puts("Can't create render context");
  exit(EXIT_FAILURE);
 }
 wglMakeCurrent(context,render);
}

void SWGF_Render::set_render()
{
 int format;
 context=GetDC(window);
 if(context==NULL)
 {
  puts("Can't get the window context");
  exit(EXIT_FAILURE);
 }
 format=this->get_pixel_format();
 this->set_pixel_format(format);
 this->create_render_context();
}

void SWGF_Render::set_perfomance_setting()
{
 glDisable(GL_ALPHA_TEST);
 glDisable(GL_BLEND);
 glDisable(GL_DEPTH_TEST);
 glDisable(GL_DITHER);
 glDisable(GL_FOG);
 glDisable(GL_LIGHTING);
 glDisable(GL_TEXTURE_1D);
 glEnable(GL_TEXTURE_2D);
}

void SWGF_Render::set_perspective()
{
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glOrtho(0,width,height,0,0,1);
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 glEnable(GL_CULL_FACE);
 glCullFace(GL_BACK);
 glMatrixMode(GL_TEXTURE);
 glLoadIdentity();
 glViewport(0,0,width,height);
}

void SWGF_Render::clear_stage()
{
 glClearColor(0,0,0,0);
 glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

void SWGF_Render::check_videocard()
{
 int control;
 control=0;
 glGetIntegerv(GL_MAX_TEXTURE_SIZE,&control);
 if((control<(int)frame_width)||(control<(int)frame_height))
 {
  puts("This video card don't support request texture size");
  exit(EXIT_FAILURE);
 }

}

void SWGF_Render::prepare_surface()
{
 surface=glGenLists(1);
 if(surface==0)
 {
  puts("Can't create the display list");
  exit(EXIT_FAILURE);
 }
 glNewList(surface,GL_COMPILE);
 glBegin(GL_QUADS);
 glTexCoord2f(0,1);
 glVertex2i(0,height);
 glTexCoord2f(1,1);
 glVertex2i(width,height);
 glTexCoord2f(1,0);
 glVertex2i(width,0);
 glTexCoord2f(0,0);
 glVertex2i(0,0);
 glEnd();
 glEndList();
}

void SWGF_Render::create_texture()
{
 glPixelStorei(GL_UNPACK_ALIGNMENT,1);
 glGenTextures(1,&texture);
 glBindTexture(GL_TEXTURE_2D,texture);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
}

void SWGF_Render::disable_vsync()
{
 wglSwapIntervalEXT=(PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
 if(wglSwapIntervalEXT==NULL)
 {
  puts("Can't load OPENGL extension");
  exit(EXIT_FAILURE);
 }
 wglSwapIntervalEXT(0);
}

void SWGF_Render::create_render()
{
 this->set_render();
 this->set_perfomance_setting();
 this->set_perspective();
 this->clear_stage();
 this->check_videocard();
 this->create_texture();
 this->prepare_surface();
 this->disable_vsync();
}

void SWGF_Render::start_render()
{
 this->create_window();
 this->capture_mouse();
 this->create_render();
}

void SWGF_Render::destroy_render()
{
 if(surface!=0) glDeleteLists(surface,1);
 if(render!=NULL)
 {
  wglMakeCurrent(NULL,NULL);
  wglDeleteContext(render);
 }
 if(context!=NULL) ReleaseDC(window,context);
}

void SWGF_Render::refresh()
{
 glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
 glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,frame_width,frame_height,0,GL_RGBA,GL_UNSIGNED_BYTE,buffer);
 glCallList(surface);
 SwapBuffers(context);
}

void SWGF_Screen::initialize()
{
 this->check_video_mode();
 this->prepare_engine();
 this->start_render();
 this->create_render_buffer();
 this->create_timer();
 this->set_timer(17);
}

void SWGF_Screen::set_mode(const unsigned long int screen_width,const unsigned long int screen_height)
{
 this->destroy_render();
 this->destroy_window();
 this->set_display_mode(screen_width,screen_height);
 this->start_render();
}

bool SWGF_Screen::sync()
{
 bool quit;
 this->refresh();
 quit=this->process_message();
 this->wait_timer();
 return quit;
}

SWGF_Screen* SWGF_Screen::get_handle()
{
 return this;
}

SWGF_Keyboard::SWGF_Keyboard()
{
 preversion=NULL;
}

SWGF_Keyboard::~SWGF_Keyboard()
{
 if(preversion!=NULL) free(preversion);
}

void SWGF_Keyboard::initialize()
{
 preversion=(unsigned char*)calloc(SWGF_KEYBOARD,1);
 if(preversion==NULL)
 {
  puts("Can't allocate memory for keyboard state buffer");
  exit(EXIT_FAILURE);
 }

}

bool SWGF_Keyboard::check_hold(const unsigned char code)
{
 bool result;
 result=false;
 if(SWGF_Keys[code]==SWGFKEY_PRESS) result=true;
 return result;
}

bool SWGF_Keyboard::check_press(const unsigned char code)
{
 bool result;
 result=false;
 if(SWGF_Keys[code]==SWGFKEY_PRESS)
 {
  if(preversion[code]!=SWGFKEY_PRESS) result=true;
 }
 preversion[code]=SWGF_Keys[code];
 return result;
}

bool SWGF_Keyboard::check_release(const unsigned char code)
{
 bool result;
 result=false;
 if(SWGF_Keys[code]==SWGFKEY_RELEASE)
 {
  result=true;
  SWGF_Keys[code]=SWGFKEY_NONE;
 }
 return result;
}

SWGF_Mouse::SWGF_Mouse()
{
 memset(preversion,SWGFKEY_NONE,SWGF_MOUSE);
}

SWGF_Mouse::~SWGF_Mouse()
{
 while(ShowCursor(TRUE)<1) ;
}

void SWGF_Mouse::show()
{
 while(ShowCursor(TRUE)<1) ;
}

void SWGF_Mouse::hide()
{
 while(ShowCursor(FALSE)>-2) ;
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
 POINT position;
 if(GetCursorPos(&position)==FALSE)
 {
  puts("Can't get the mouse cursor position");
  exit(EXIT_FAILURE);
 }
 return position.x;
}

unsigned long int SWGF_Mouse::get_y()
{
 POINT position;
 if(GetCursorPos(&position)==FALSE)
 {
  puts("Can't get the mouse cursor position");
  exit(EXIT_FAILURE);
 }
 return position.y;
}

bool SWGF_Mouse::check_hold(const unsigned char button)
{
 bool result;
 result=false;
 if(button<=SWGF_MOUSE_MIDDLE)
 {
  if(SWGF_Buttons[button]==SWGFKEY_PRESS) result=true;
 }
 return result;
}

bool SWGF_Mouse::check_press(const unsigned char button)
{
 bool result;
 result=false;
 if(button<=SWGF_MOUSE_MIDDLE)
 {
  if(SWGF_Buttons[button]==SWGFKEY_PRESS)
  {
   if(preversion[button]!=SWGFKEY_PRESS) result=true;
  }

 }
 preversion[button]=SWGF_Buttons[button];
 return result;
}

bool SWGF_Mouse::check_release(const unsigned char button)
{
 bool result;
 result=false;
 if(button<=SWGF_MOUSE_MIDDLE)
 {
  if(SWGF_Buttons[button]==SWGFKEY_RELEASE)
  {
   result=true;
   SWGF_Buttons[button]=SWGFKEY_NONE;
  }

 }
 return result;
}

SWGF_Gamepad::SWGF_Gamepad()
{
 active=SWGF_GAMEPAD1;
 length[0]=(unsigned long int)sizeof(JOYCAPS);
 length[1]=(unsigned long int)sizeof(JOYINFOEX);
 memset(&configuration,0,sizeof(JOYCAPS));
 memset(&current,0,sizeof(JOYINFOEX));
 memset(&preversion,0,sizeof(JOYINFOEX));
 current.dwSize=length[1];
 preversion.dwSize=length[1];
 current.dwFlags=JOY_RETURNALL;
 preversion.dwFlags=JOY_RETURNALL;
 current.dwPOV=JOY_POVCENTERED;
 preversion.dwPOV=JOY_POVCENTERED;
}

SWGF_Gamepad::~SWGF_Gamepad()
{

}

bool SWGF_Gamepad::read_configuration()
{
 bool result;
 result=false;
 if(joyGetDevCaps(active,&configuration,length[0])==JOYERR_NOERROR) result=true;
 return result;
}

bool SWGF_Gamepad::read_state()
{
 bool result;
 result=false;
 if(joyGetPosEx(active,&current)==JOYERR_NOERROR) result=true;
 return result;
}

void SWGF_Gamepad::clear_state()
{
 memset(&configuration,0,sizeof(JOYCAPS));
 memset(&current,0,sizeof(JOYINFOEX));
 memset(&preversion,0,sizeof(JOYINFOEX));
 current.dwSize=length[1];
 preversion.dwSize=length[1];
 current.dwFlags=JOY_RETURNALL;
 preversion.dwFlags=JOY_RETURNALL;
 current.dwPOV=JOY_POVCENTERED;
 preversion.dwPOV=JOY_POVCENTERED;
}

bool SWGF_Gamepad::check_button(const unsigned long int button,const JOYINFOEX &target)
{
 bool result;
 result=false;
 if(target.dwButtons&button) result=true;
 return result;
}

void SWGF_Gamepad::set_active(const unsigned int gamepad)
{
 if(active<=SWGF_GAMEPAD15)
 {
  this->clear_state();
  active=gamepad;
 }

}

unsigned int SWGF_Gamepad::get_active()
{
 return active;
}

unsigned int SWGF_Gamepad::get_amount()
{
 return joyGetNumDevs();
}

unsigned int SWGF_Gamepad::get_button_amount()
{
 unsigned int result;
 result=0;
 if(this->read_configuration()==true) result=configuration.wNumButtons;
 return result;
}

bool SWGF_Gamepad::check_connection()
{
 return this->read_state();
}

void SWGF_Gamepad::update()
{
 preversion=current;
 if(this->read_state()==false) this->clear_state();
}

unsigned char SWGF_Gamepad::get_dpad()
{
 unsigned char result;
 result=SWGF_GAMEPAD_NONE;
 switch (current.dwPOV)
 {
  case JOY_POVFORWARD:
  result=SWGF_GAMEPAD_UP;
  break;
  case JOY_POVBACKWARD:
  result=SWGF_GAMEPAD_DOWN;
  break;
  case JOY_POVLEFT:
  result=SWGF_GAMEPAD_LEFT;
  break;
  case JOY_POVRIGHT:
  result=SWGF_GAMEPAD_RIGHT;
  break;
  case JOYSTICK_UPLEFT:
  result=SWGF_GAMEPAD_UPLEFT;
  break;
  case JOYSTICK_UPRIGHT:
  result=SWGF_GAMEPAD_UPRIGHT;
  break;
  case JOYSTICK_DOWNLEFT:
  result=SWGF_GAMEPAD_DOWNLEFT;
  break;
  case JOYSTICK_DOWNRIGHT:
  result=SWGF_GAMEPAD_DOWNRIGHT;
  break;
 }
 return result;
}

unsigned long int SWGF_Gamepad::get_sticks_amount()
{
 unsigned long int result;
 result=0;
 if(this->read_configuration()==true)
 {
  switch (configuration.wNumAxes)
  {
   case 2:
   result=1;
   break;
   case 4:
   result=2;
   break;
   default:
   result=0;
   break;
  }

 }
 return result;
}

char SWGF_Gamepad::get_stick_x(const unsigned char stick)
{
 char result;
 unsigned long int control;
 result=0;
 if(stick==SWGF_GAMEPAD_LEFT_STICK)
 {
  if(this->read_configuration()==true)
  {
   if((configuration.wNumAxes==2)||(configuration.wNumAxes==4))
   {
    control=(configuration.wXmax-configuration.wXmin)/2;
    if(current.dwXpos<control) result=-1;
    if(current.dwXpos>control) result=1;
   }

  }

 }
 if(stick==SWGF_GAMEPAD_RIGHT_STICK)
 {
  if(this->read_configuration()==true)
  {
   if(configuration.wNumAxes==4)
   {
    control=(configuration.wZmax-configuration.wZmin)/2;
    if(current.dwZpos<control) result=-1;
    if(current.dwZpos>control) result=1;
   }

  }

 }
 return result;
}

char SWGF_Gamepad::get_stick_y(const unsigned char stick)
{
 char result;
 unsigned long int control;
 result=0;
 if(stick==SWGF_GAMEPAD_LEFT_STICK)
 {
  if(this->read_configuration()==true)
  {
   if((configuration.wNumAxes==2)||(configuration.wNumAxes==4))
   {
    control=(configuration.wYmax-configuration.wYmin)/2;
    if(current.dwYpos<control) result=-1;
    if(current.dwYpos>control) result=1;
   }

  }

 }
 if(stick==SWGF_GAMEPAD_RIGHT_STICK)
 {
  if(this->read_configuration()==true)
  {
   if(configuration.wNumAxes==4)
   {
    control=(configuration.wRmax-configuration.wRmin)/2;
    if(current.dwRpos<control) result=-1;
    if(current.dwRpos>control) result=1;
   }

  }

 }
 return result;
}

bool SWGF_Gamepad::check_hold(const unsigned long int button)
{
 return this->check_button(button,current);
}

bool SWGF_Gamepad::check_press(const unsigned long int button)
{
 bool result;
 result=false;
 if(this->check_button(button,current)==true)
 {
  if(this->check_button(button,preversion)==false) result=true;
 }
 return result;
}

bool SWGF_Gamepad::check_release(const unsigned long int button)
{
 bool result;
 result=false;
 if(this->check_button(button,current)==false)
 {
  if(this->check_button(button,preversion)==true) result=true;
 }
 return result;
}

SWGF_Multimedia::SWGF_Multimedia()
{
 loader=NULL;
 player=NULL;
 controler=NULL;
 video=NULL;
}

SWGF_Multimedia::~SWGF_Multimedia()
{
 if(player!=NULL) player->StopWhenReady();
 if(video!=NULL) video->Release();
 if(controler!=NULL) controler->Release();
 if(player!=NULL) player->Release();
 if(loader!=NULL) loader->Release();
}

wchar_t *SWGF_Multimedia::convert_file_name(const char *target)
{
 wchar_t *name;
 size_t index,length;
 length=strlen(target);
 name=(wchar_t*)calloc(length+1,sizeof(wchar_t));
 if(name==NULL)
 {
  puts("Can't allocate memory");
  exit(EXIT_FAILURE);
 }
 for(index=0;index<length;index++) name[index]=btowc(target[index]);
 return name;
}

void SWGF_Multimedia::open(const wchar_t *target)
{
 player->StopWhenReady();
 if(loader->RenderFile(target,NULL)!=S_OK)
 {
  puts("Can't load a multimedia file");
  exit(EXIT_FAILURE);
 }
 video->put_FullScreenMode(OATRUE);
}

bool SWGF_Multimedia::is_end()
{
 bool result;
 long long current,stop;
 result=false;
 if(controler->GetPositions(&current,&stop)==S_OK)
 {
  if(current>=stop) result=true;
 }
 else
 {
  puts("Can't get the current and the end position");
  exit(EXIT_FAILURE);
 }
 return result;
}

void SWGF_Multimedia::rewind()
{
 long long position;
 position=0;
 if(controler->SetPositions(&position,AM_SEEKING_AbsolutePositioning,NULL,AM_SEEKING_NoPositioning)!=S_OK)
 {
  puts("Can't set start position");
  exit(EXIT_FAILURE);
 }

}

void SWGF_Multimedia::initialize()
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

void SWGF_Multimedia::load(const char *target)
{
 wchar_t *name;
 name=this->convert_file_name(target);
 this->open(name);
 free(name);
}

bool SWGF_Multimedia::check_playing()
{
 OAFilterState state;
 bool result;
 result=false;
 if(player->GetState(INFINITE,&state)==E_FAIL)
 {
  puts("Can't get the multimedia state");
  exit(EXIT_FAILURE);
 }
 else
 {
  if(state==State_Running)
  {
   if(this->is_end()==false) result=true;
  }

 }
 return result;
}

void SWGF_Multimedia::stop()
{
 player->StopWhenReady();
}

void SWGF_Multimedia::play()
{
 this->stop();
 this->rewind();
 player->Run();
}

SWGF_Memory::SWGF_Memory()
{
 memset(&memory,0,sizeof(MEMORYSTATUSEX));
 memory.dwLength=sizeof(MEMORYSTATUSEX);
}

SWGF_Memory::~SWGF_Memory()
{

}

void SWGF_Memory::get_status()
{
 if(GlobalMemoryStatusEx(&memory)==FALSE)
 {
  puts("Can't get the memory status");
  exit(EXIT_FAILURE);
 }

}

unsigned long long int SWGF_Memory::get_total_memory()
{
 this->get_status();
 return memory.ullTotalPhys;
}

unsigned long long int SWGF_Memory::get_free_memory()
{
 this->get_status();
 return memory.ullAvailPhys;
}

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

void SWGF_System::enable_logging(const char *name)
{
 if(freopen(name,"wt",stdout)==NULL)
 {
  puts("Can't create log file");
  exit(EXIT_FAILURE);
 }

}

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

SWGF_Primitive::SWGF_Primitive()
{
 color.red=0;
 color.green=0;
 color.blue=0;
 surface=NULL;
}

SWGF_Primitive::~SWGF_Primitive()
{
 color.red=0;
 color.green=0;
 color.blue=0;
 surface=NULL;
}

void SWGF_Primitive::initialize(SWGF_Screen *Screen)
{
 surface=Screen;
}

void SWGF_Primitive::set_color(const unsigned char red,const unsigned char green,const unsigned char blue)
{
 color.red=red;
 color.green=green;
 color.blue=blue;
}

void SWGF_Primitive::draw_line(const unsigned long int x1,const unsigned long int y1,const unsigned long int x2,const unsigned long int y2)
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
 for (index=steps;index>0;--index)
 {
  x+=shift_x;
  y+=shift_y;
  surface->draw_pixel(x,y,color.red,color.green,color.blue);
 }

}

void SWGF_Primitive::draw_rectangle(const unsigned long int x,const unsigned long int y,const unsigned long int width,const unsigned long int height)
{
 unsigned long int stop_x,stop_y;
 stop_x=x+width;
 stop_y=y+height;
 this->draw_line(x,y,stop_x,y);
 this->draw_line(x,stop_y,stop_x,stop_y);
 this->draw_line(x,y,x,stop_y);
 this->draw_line(stop_x,y,stop_x,stop_y);
}

void SWGF_Primitive::draw_filled_rectangle(const unsigned long int x,const unsigned long int y,const unsigned long int width,const unsigned long int height)
{
 unsigned long int step_x,step_y,stop_x,stop_y;
 stop_x=x+width;
 stop_y=y+height;
 for(step_x=x;step_x<stop_x;++step_x)
 {
  for(step_y=y;step_y<stop_y;++step_y)
  {
   surface->draw_pixel(step_x,step_y,color.red,color.green,color.blue);
  }

 }

}

SWGF_Image::SWGF_Image()
{
 width=0;
 height=0;
 data=NULL;
}

SWGF_Image::~SWGF_Image()
{
 if(data!=NULL) free(data);
}

unsigned char *SWGF_Image::create_buffer(const size_t length)
{
 unsigned char *result;
 result=(unsigned char*)calloc(length,sizeof(unsigned char));
 if(result==NULL)
 {
  puts("Can't allocate memory for image buffer");
  exit(EXIT_FAILURE);
 }
 return result;
}

void SWGF_Image::clear_buffer()
{
 if(data!=NULL)
 {
  free(data);
  data=NULL;
 }

}

FILE *SWGF_Image::open_image(const char *name)
{
 FILE *target;
 target=fopen(name,"rb");
 if(target==NULL)
 {
  puts("Can't open a image file");
  exit(EXIT_FAILURE);
 }
 return target;
}

unsigned long int SWGF_Image::get_file_size(FILE *target)
{
 unsigned long int length;
 fseek(target,0,SEEK_END);
 length=ftell(target);
 rewind(target);
 return length;
}

void SWGF_Image::load_tga(const char *name)
{
 FILE *target;
 size_t index,position,amount,compressed_length,uncompressed_length;
 unsigned char *compressed;
 unsigned char *uncompressed;
 TGA_head head;
 TGA_map color_map;
 TGA_image image;
 this->clear_buffer();
 target=this->open_image(name);
 compressed_length=(size_t)this->get_file_size(target)-18;
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
 width=image.width;
 height=image.height;
 uncompressed_length=this->get_data_length();
 uncompressed=this->create_buffer(uncompressed_length);
 if(head.type==2)
 {
  fread(uncompressed,uncompressed_length,1,target);
 }
 if(head.type==10)
 {
  compressed=this->create_buffer(compressed_length);
  fread(compressed,compressed_length,1,target);
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
    for(amount=compressed[position]-127;amount>0;--amount)
    {
     memmove(uncompressed+index,compressed+(position+1),3);
     index+=3;
    }
    position+=4;
   }

  }
  free(compressed);
 }
 fclose(target);
 data=uncompressed;
}

void SWGF_Image::load_pcx(const char *name)
{
 FILE *target;
 unsigned long int x,y;
 size_t index,position,line,row,length,uncompressed_length;
 unsigned char repeat;
 unsigned char *original;
 unsigned char *uncompressed;
 PCX_head head;
 this->clear_buffer();
 target=this->open_image(name);
 length=(size_t)this->get_file_size(target)-128;
 fread(&head,128,1,target);
 if((head.color*head.planes!=24)&&(head.compress!=1))
 {
  puts("Incorrect image format");
  exit(EXIT_FAILURE);
 }
 width=head.max_x-head.min_x+1;
 height=head.max_y-head.min_y+1;
 row=3*(size_t)width;
 line=(size_t)head.planes*(size_t)head.plane_length;
 uncompressed_length=row*height;
 index=0;
 position=0;
 original=this->create_buffer(length);
 uncompressed=this->create_buffer(uncompressed_length);
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
   for (repeat=original[index]-192;repeat>0;--repeat)
   {
    uncompressed[position]=original[index+1];
    position++;
   }
   index+=2;
  }

 }
 free(original);
 original=this->create_buffer(uncompressed_length);
 for(x=0;x<width;++x)
 {
  for(y=0;y<height;++y)
  {
   index=(size_t)x*3+(size_t)y*row;
   position=(size_t)x+(size_t)y*line;
   original[index]=uncompressed[position+2*(size_t)head.plane_length];
   original[index+1]=uncompressed[position+(size_t)head.plane_length];
   original[index+2]=uncompressed[position];
  }

 }
 free(uncompressed);
 data=original;
}

unsigned long int SWGF_Image::get_width()
{
 return width;
}

unsigned long int SWGF_Image::get_height()
{
 return height;
}

size_t SWGF_Image::get_data_length()
{
 return (size_t)width*(size_t)height*3;
}

unsigned char *SWGF_Image::get_data()
{
 return data;
}

void SWGF_Image::destroy_image()
{
 width=0;
 height=0;
 this->clear_buffer();
}

SWGF_Canvas::SWGF_Canvas()
{
 image=NULL;
 surface=NULL;
 width=0;
 height=0;
 frames=1;
}

SWGF_Canvas::~SWGF_Canvas()
{
 surface=NULL;
 if(image!=NULL) free(image);
}

void SWGF_Canvas::clear_buffer()
{
 if(image!=NULL) free(image);
}

SWGF_Color *SWGF_Canvas::create_buffer(const unsigned long int image_width,const unsigned long int image_height)
{
 SWGF_Color *result;
 size_t length;
 length=(size_t)image_width*(size_t)image_height;
 result=(SWGF_Color*)calloc(length,3);
 if(result==NULL)
 {
  puts("Can't allocate memory for image buffer");
  exit(EXIT_FAILURE);
 }
 return result;
}

void SWGF_Canvas::draw_image_pixel(const size_t offset,const unsigned long int x,const unsigned long int y)
{
 surface->draw_pixel(x,y,image[offset].red,image[offset].green,image[offset].blue);
}

size_t SWGF_Canvas::get_offset(const unsigned long int start,const unsigned long int x,const unsigned long int y)
{
 return (size_t)start+(size_t)x+(size_t)y*(size_t)width;
}

SWGF_Color *SWGF_Canvas::get_image()
{
 return image;
}

unsigned long int SWGF_Canvas::get_width()
{
 return width;
}

unsigned long int SWGF_Canvas::get_height()
{
 return height;
}

void SWGF_Canvas::set_frames(const unsigned long int amount)
{
 if(amount>1) frames=amount;
}

unsigned long int SWGF_Canvas::get_frames()
{
 return frames;
}

void SWGF_Canvas::initialize(SWGF_Screen *Screen)
{
 surface=Screen;
}

void SWGF_Canvas::load_image(SWGF_Image &buffer)
{
 width=buffer.get_width();
 height=buffer.get_height();
 this->clear_buffer();
 image=this->create_buffer(width,height);
 memmove(image,buffer.get_data(),buffer.get_data_length());
 buffer.destroy_image();
}

void SWGF_Canvas::mirror_image(const unsigned char kind)
{
 unsigned long int x,y;
 size_t index,index2;
 SWGF_Color *mirrored_image;
 mirrored_image=this->create_buffer(width,height);
 if (kind==0)
 {
  for (x=0;x<width;++x)
  {
   for (y=0;y<height;++y)
   {
    index=this->get_offset(0,x,y);
    index2=this->get_offset(0,(width-x-1),y);
    mirrored_image[index]=image[index2];
   }

  }

 }
 else
 {
   for (x=0;x<width;++x)
  {
   for (y=0;y<height;++y)
   {
    index=this->get_offset(0,x,y);
    index2=this->get_offset(0,x,(height-y-1));
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
 unsigned long int x,y;
 size_t index,index2;
 SWGF_Color *scaled_image;
 scaled_image=this->create_buffer(new_width,new_height);
 x_ratio=(float)width/(float)new_width;
 y_ratio=(float)height/(float)new_height;
 for (x=0;x<new_width;++x)
 {
  for (y=0;y<new_height;++y)
  {
   index=(size_t)x+(size_t)y*(size_t)new_width;
   index2=(size_t)(x_ratio*(float)x)+(size_t)width*(size_t)(y_ratio*(float)y);
   scaled_image[index]=image[index2];
  }

 }
 free(image);
 image=scaled_image;
 width=new_width;
 height=new_height;
}

void SWGF_Background::draw_background_image(const unsigned long int start,const unsigned long int frame_width,const unsigned long int frame_height)
{
 unsigned long int x,y;
 size_t offset;
 for(x=0;x<frame_width;++x)
 {
  for(y=0;y<frame_height;++y)
  {
   offset=this->get_offset(start,x,y);
   this->draw_image_pixel(offset,x,y);
  }

 }

}

void SWGF_Background::draw_horizontal_background(const unsigned long int frame)
{
 unsigned long int start,frame_width;
 frame_width=width/frames;
 start=(frame-1)*frame_width;
 this->draw_background_image(start,frame_width,height);
}

void SWGF_Background::draw_vertical_background(const unsigned long int frame)
{
 unsigned long int start,frame_height;
 frame_height=height/frames;
 start=(frame-1)*frame_height*width;
 this->draw_background_image(start,width,frame_height);
}

void SWGF_Background::draw_background()
{
 this->draw_horizontal_background(1);
}

SWGF_Sprite::SWGF_Sprite()
{
 current_x=0;
 current_y=0;
}

SWGF_Sprite::~SWGF_Sprite()
{

}

bool SWGF_Sprite::compare_pixels(const SWGF_Color &first,const SWGF_Color &second)
{
 bool result;
 result=false;
 if ((first.red!=second.red)||(first.green!=second.green))
 {
  result=true;
 }
 else
 {
  if(first.blue!=second.blue) result=true;
 }
 return result;
}

void SWGF_Sprite::draw_sprite_pixel(const size_t offset,const unsigned long int x,const unsigned long int y)
{
 if(this->compare_pixels(image[0],image[offset])==true) this->draw_image_pixel(offset,x,y);
}

void SWGF_Sprite::clone(SWGF_Sprite &target)
{
 size_t length;
 frames=target.get_frames();
 width=target.get_sprite_width();
 height=target.get_sprite_height();
 length=(size_t)width*(size_t)height*3;
 image=this->create_buffer(width,height);
 memmove(image,target.get_image(),length);
}

void SWGF_Sprite::draw_sprite_frame(const unsigned long int x,const unsigned long int y,const unsigned long int frame)
{
 unsigned long int sprite_x,sprite_y,start,frame_width;
 size_t offset;
 current_x=x;
 current_y=y;
 frame_width=width/frames;
 start=(frame-1)*frame_width;
 for(sprite_x=0;sprite_x<frame_width;++sprite_x)
 {
  for(sprite_y=0;sprite_y<height;++sprite_y)
  {
   offset=this->get_offset(start,sprite_x,sprite_y);
   this->draw_sprite_pixel(offset,x+sprite_x,y+sprite_y);
  }

 }

}

void SWGF_Sprite::draw_sprite(const unsigned long int x,const unsigned long int y)
{
 current_x=x;
 current_y=y;
 this->draw_sprite_frame(x,y,1);
}

unsigned long int SWGF_Sprite::get_x()
{
 return current_x;
}

unsigned long int SWGF_Sprite::get_y()
{
 return current_y;
}

unsigned long int SWGF_Sprite::get_sprite_width()
{
 return width/frames;
}

unsigned long int SWGF_Sprite::get_sprite_height()
{
 return height;
}

SWGF_Sprite* SWGF_Sprite::get_handle()
{
 return this;
}

SWGF_Box SWGF_Sprite::get_box()
{
 SWGF_Box target;
 target.x=current_x;
 target.y=current_y;
 target.width=width/frames;
 target.height=height;
 return target;
}

SWGF_Text::SWGF_Text()
{
 current_x=0;
 current_y=0;
 step_x=0;
 sprite=NULL;
}

SWGF_Text::~SWGF_Text()
{
 sprite=NULL;
}

void SWGF_Text::draw_character(const char target)
{
 if((target>31)||(target<0))
 {
  sprite->draw_sprite_frame(step_x,current_y,(unsigned long int)target+1);
  step_x+=sprite->get_sprite_width();
 }

}

void SWGF_Text::set_position(const unsigned long int x,const unsigned long int y)
{
 current_x=x;
 current_y=y;
}

void SWGF_Text::load_font(SWGF_Sprite *font)
{
 sprite=font;
 sprite->set_frames(256);
}

void SWGF_Text::draw_text(const char *text)
{
 size_t index,length;
 length=strlen(text);
 step_x=current_x;
 for (index=0;index<length;++index)
 {
  this->draw_character(text[index]);
 }

}

bool SWGF_Collision::check_horizontal_collision(const SWGF_Box &first,const SWGF_Box &second)
{
 bool result;
 result=false;
 if((first.x+first.width)>=second.x)
 {
  if(first.x<=(second.x+second.width)) result=true;
 }
 return result;
}

bool SWGF_Collision::check_vertical_collision(const SWGF_Box &first,const SWGF_Box &second)
{
 bool result;
 result=false;
 if((first.y+first.height)>=second.y)
 {
  if(first.y<=(second.y+second.height)) result=true;
 }
 return result;
}

bool SWGF_Collision::check_collision(const SWGF_Box &first,const SWGF_Box &second)
{
 return this->check_horizontal_collision(first,second) || this->check_vertical_collision(first,second);
}