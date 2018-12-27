// This is an open source non-commercial project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

/*
Simple windows game framework was create by Popov Evgeniy Alekseyevich
Some code was taken from wglext.h(https://www.khronos.org/registry/OpenGL/api/GL/wglext.h) by The Khronos Group Inc

Simple windows game framework license

Copyright (C) 2016-2018 Popov Evgeniy Alekseyevich

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
  memset(SWGF_Keys,SWGFKEY_RELEASE,SWGF_KEYBOARD);
  memset(SWGF_Buttons,SWGFKEY_RELEASE,SWGF_MOUSE);
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
  SWGF_Keys[SWGF_GETSCANCODE(lParam)]=SWGFKEY_PRESS;
  break;
  case WM_KEYUP:
  SWGF_Keys[SWGF_GETSCANCODE(lParam)]=SWGFKEY_RELEASE;
  break;
 }
 return DefWindowProc(window,Message,wParam,lParam);
}

void SWGF_Show_Error(const char *message)
{
 puts(message);
 exit(EXIT_FAILURE);
}

SWGF_Base::SWGF_Base()
{
 HRESULT status;
 status=CoInitialize(NULL);
 if(status!=S_OK)
 {
  if(status!=S_FALSE)
  {
   SWGF_Show_Error("Can't initialize COM");
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
  SWGF_Show_Error("Can't create synchronization timer");
 }

}

void SWGF_Synchronization::set_timer(const unsigned long int interval)
{
 LARGE_INTEGER start;
 start.QuadPart=0;
 if(SetWaitableTimer(timer,&start,interval,NULL,NULL,FALSE)==FALSE)
 {
  SWGF_Show_Error("Can't set timer");
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
  SWGF_Show_Error("Can't get the application instance");
 }
 window_class.hbrBackground=(HBRUSH)GetStockObject(BLACK_BRUSH);
 if (window_class.hbrBackground==NULL)
 {
  SWGF_Show_Error("Can't set background color");
 }
 window_class.hIcon=LoadIcon(NULL,IDI_APPLICATION);
 if (window_class.hIcon==NULL)
 {
  SWGF_Show_Error("Can't load the standart program icon");
 }
 window_class.hCursor=LoadCursor(NULL,IDC_ARROW);
 if (window_class.hCursor==NULL)
 {
  SWGF_Show_Error("Can't load the standart cursor");
 }
 if (!RegisterClass(&window_class))
 {
  SWGF_Show_Error("Can't register window class");
 }

}


HWND SWGF_Engine::get_window()
{
 return window;
}

void SWGF_Engine::create_window()
{
 width=GetSystemMetrics(SM_CXSCREEN);
 height=GetSystemMetrics(SM_CYSCREEN);
 window=CreateWindow(window_class.lpszClassName,NULL,WS_VISIBLE|WS_POPUP,0,0,width,height,NULL,NULL,window_class.hInstance,NULL);
 if (window==NULL)
 {
  SWGF_Show_Error("Can't create window");
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
  SWGF_Show_Error("Can't capture window");
 }
 if(ClipCursor(&border)==FALSE)
 {
  SWGF_Show_Error("Can't capture cursor");
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
 pixels=0;
 buffer=NULL;
 shadow=NULL;
}

SWGF_Frame::~SWGF_Frame()
{
 if(buffer!=NULL)
 {
  free(buffer);
  buffer=NULL;
 }
 if(shadow!=NULL)
 {
  free(shadow);
  shadow=NULL;
 }

}

unsigned int SWGF_Frame::get_rgb(const unsigned int red,const unsigned int green,const unsigned int blue)
{
 return red+(green<<8)+(blue<<16);
}

size_t SWGF_Frame::get_offset(const unsigned long int x,const unsigned long int y)
{
 return (size_t)x+(size_t)y*(size_t)frame_width;
}

void SWGF_Frame::set_size(const SWGF_SURFACE surface)
{
 if(surface==SWGF_SURFACE_SMALL)
 {
  frame_width=256;
  frame_height=256;
 }
 if(surface==SWGF_SURFACE_LARGE)
 {
  frame_width=512;
  frame_height=512;
 }

}

unsigned int *SWGF_Frame::create_buffer(const char *error)
{
 unsigned int *target;
 pixels=(size_t)frame_width*(size_t)frame_height;
 target=(unsigned int*)calloc(pixels,sizeof(unsigned int));
 if(target==NULL)
 {
  SWGF_Show_Error(error);
 }
 return target;
}

void SWGF_Frame::create_buffers()
{
 buffer=this->create_buffer("Can't allocate memory for render buffer");
 shadow=this->create_buffer("Can't allocate memory for shadow buffer");
}

unsigned int *SWGF_Frame::get_buffer()
{
 return buffer;
}

void SWGF_Frame::draw_pixel(const unsigned long int x,const unsigned long int y,const unsigned char red,const unsigned char green,const unsigned char blue)
{
 if((x<frame_width)&&(y<frame_height))
 {
  buffer[this->get_offset(x,y)]=this->get_rgb(red,green,blue);
 }

}

void SWGF_Frame::clear_screen()
{
 size_t index;
 for (index=0;index<pixels;++index)
 {
  buffer[index]=0;
 }

}

void SWGF_Frame::save()
{
 size_t index;
 for (index=0;index<pixels;++index)
 {
  shadow[index]=buffer[index];
 }

}

void SWGF_Frame::restore()
{
 size_t index;
 for (index=0;index<pixels;++index)
 {
  buffer[index]=shadow[index];
 }

}

unsigned long int SWGF_Frame::get_frame_width()
{
 return frame_width;
}

unsigned long int SWGF_Frame::get_frame_height()
{
 return frame_height;
}

SWGF_FPS::SWGF_FPS()
{
 start=time(NULL);
 current=0;
 fps=0;
}

SWGF_FPS::~SWGF_FPS()
{

}

void SWGF_FPS::update_counter()
{
 time_t stop;
 if(current==0) start=time(NULL);
 ++current;
 stop=time(NULL);
 if(difftime(stop,start)>=1)
 {
  fps=current;
  current=0;
 }

}

unsigned long int SWGF_FPS::get_fps()
{
 return fps;
}

SWGF_Display::SWGF_Display()
{
 memset(&display,0,sizeof(DEVMODE));
 display.dmSize=sizeof(DEVMODE);
}

SWGF_Display::~SWGF_Display()
{
 ChangeDisplaySettings(NULL,0);
}

void SWGF_Display::set_video_mode()
{
 if (ChangeDisplaySettings(&display,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
 {
  SWGF_Show_Error("Can't change video mode");
  ;
 }

}

void SWGF_Display::get_video_mode()
{
 if (EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS,&display)==FALSE)
 {
  SWGF_Show_Error("Can't get display setting");
 }

}

void SWGF_Display::check_video_mode()
{
 this->get_video_mode();
 if(display.dmBitsPerPel<16)
 {
  display.dmBitsPerPel=16;
  this->set_video_mode();
 }

}

void SWGF_Display::set_display_mode(const unsigned long int screen_width,const unsigned long int screen_height)
{
 this->get_video_mode();
 if((display.dmPelsWidth!=screen_width)||(display.dmPelsHeight!=screen_height))
 {
  display.dmPelsWidth=screen_width;
  display.dmPelsHeight=screen_height;
  this->set_video_mode();
 }

}

unsigned long int SWGF_Display::get_color()
{
 return display.dmBitsPerPel;
}

SWGF_WINGL::SWGF_WINGL()
{
 context=NULL;
 render=NULL;
 wglSwapIntervalEXT=NULL;
 memset(&setting,0,sizeof(PIXELFORMATDESCRIPTOR));
 setting.nSize=sizeof(PIXELFORMATDESCRIPTOR);
 setting.nVersion=1;
}

SWGF_WINGL::~SWGF_WINGL()
{
 if(render!=NULL)
 {
  wglMakeCurrent(NULL,NULL);
  wglDeleteContext(render);
 }
 if(context!=NULL) ReleaseDC(this->get_window(),context);
}

bool SWGF_WINGL::check_common_setting()
{
 bool result;
 result=false;
 if(setting.cColorBits==this->get_color())
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

bool SWGF_WINGL::check_acceleration()
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

int SWGF_WINGL::get_pixel_format()
{
 int index,result;
 result=0;
 for(index=DescribePixelFormat(context,1,setting.nSize,&setting);index>0;--index)
 {
  DescribePixelFormat(context,index,setting.nSize,&setting);
  if(this->check_common_setting()==true)
  {
   if(this->check_acceleration()==true)
   {
    result=index;
    break;
   }

  }

 }
 return result;
}

void SWGF_WINGL::set_pixel_format(const int format)
{
 if(format==0)
 {
  SWGF_Show_Error("Invalid pixel format");
 }
 DescribePixelFormat(context,format,setting.nSize,&setting);
 if(SetPixelFormat(context,format,&setting)==FALSE)
 {
  SWGF_Show_Error("Can't set pixel format");
 }

}

void SWGF_WINGL::create_render_context()
{
 render=wglCreateContext(context);
 if(render==NULL)
 {
  SWGF_Show_Error("Can't create render context");
 }
 wglMakeCurrent(context,render);
}

void SWGF_WINGL::set_render()
{
 int format;
 context=GetDC(this->get_window());
 if(context==NULL)
 {
  SWGF_Show_Error("Can't get the window context");
 }
 format=this->get_pixel_format();
 this->set_pixel_format(format);
 this->create_render_context();
}

void SWGF_WINGL::destroy_render()
{
 if(render!=NULL)
 {
  wglMakeCurrent(NULL,NULL);
  wglDeleteContext(render);
 }
 if(context!=NULL) ReleaseDC(this->get_window(),context);
}

void SWGF_WINGL::disable_vsync()
{
 wglSwapIntervalEXT=(PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
 if(wglSwapIntervalEXT==NULL)
 {
  SWGF_Show_Error("Can't load OPENGL extension");
 }
 wglSwapIntervalEXT(0);
}

void SWGF_WINGL::Swap()
{
 SwapBuffers(context);
}

SWGF_Render::SWGF_Render()
{
 memset(vertex,0,4*sizeof(SWGF_Vertex));
 memset(point,0,4*sizeof(SWGF_Point));
 texture=0;
}

SWGF_Render::~SWGF_Render()
{

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
 glEnableClientState(GL_VERTEX_ARRAY);
 glEnableClientState(GL_TEXTURE_COORD_ARRAY);
 glDisableClientState(GL_COLOR_ARRAY);
 glDisableClientState(GL_EDGE_FLAG_ARRAY);
 glDisableClientState(GL_INDEX_ARRAY);
 glDisableClientState(GL_NORMAL_ARRAY);
}

void SWGF_Render::set_perspective()
{
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glOrtho(0,this->get_width(),this->get_height(),0,0,1);
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 glEnable(GL_CULL_FACE);
 glCullFace(GL_BACK);
 glMatrixMode(GL_TEXTURE);
 glLoadIdentity();
 glViewport(0,0,this->get_width(),this->get_height());
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
 if((control<(int)this->get_frame_width())||(control<(int)this->get_frame_height()))
 {
  SWGF_Show_Error("This video card don't support request texture size");
 }

}

void SWGF_Render::prepare_surface()
{
 vertex[0].x=0;
 vertex[0].y=this->get_height();
 vertex[1].x=this->get_width();
 vertex[1].y=this->get_height();
 vertex[2].x=this->get_width();
 vertex[2].y=0;
 vertex[3].x=0;
 vertex[3].y=0;
 point[0].u=0;
 point[0].v=1;
 point[1].u=1;
 point[1].v=1;
 point[2].u=1;
 point[2].v=0;
 point[3].u=0;
 point[3].v=0;
}

void SWGF_Render::create_texture()
{
 glPixelStorei(GL_UNPACK_ALIGNMENT,1);
 glGenTextures(1,&texture);
 glBindTexture(GL_TEXTURE_2D,texture);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
}

void SWGF_Render::load_surface_data()
{
 glVertexPointer(2,GL_INT,0,vertex);
 glTexCoordPointer(2,GL_FLOAT,0,point);
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
 this->load_surface_data();
 this->disable_vsync();
}

void SWGF_Render::draw()
{
 glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,this->get_frame_width(),this->get_frame_height(),0,GL_RGBA,GL_UNSIGNED_BYTE,this->get_buffer());
 glDrawArrays(GL_TRIANGLE_FAN,0,4);
}

void SWGF_Render::start_render()
{
 this->create_window();
 this->capture_mouse();
 this->create_render();
}

void SWGF_Render::refresh()
{
 this->draw();
 this->Swap();
}

void SWGF_Screen::initialize()
{
 this->check_video_mode();
 this->prepare_engine();
 this->start_render();
 this->create_buffers();
 this->create_timer();
 this->set_timer(17);
}

void SWGF_Screen::initialize(const SWGF_SURFACE surface)
{
 this->set_size(surface);
 this->initialize();
}

void SWGF_Screen::set_mode(const unsigned long int screen_width,const unsigned long int screen_height)
{
 this->destroy_render();
 this->destroy_window();
 this->set_display_mode(screen_width,screen_height);
 this->start_render();
}

bool SWGF_Screen::update()
{
 this->refresh();
 this->update_counter();
 return this->process_message();
}

bool SWGF_Screen::sync()
{
 bool quit;
 quit=this->update();
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
 preversion=(unsigned char*)calloc(SWGF_KEYBOARD,sizeof(unsigned char));
 if(preversion==NULL)
 {
  SWGF_Show_Error("Can't allocate memory for keyboard state buffer");
 }

}

bool SWGF_Keyboard::check_hold(const unsigned char code)
{
 bool result;
 result=false;
 if(SWGF_Keys[code]==SWGFKEY_PRESS) result=true;
 preversion[code]=SWGF_Keys[code];
 return result;
}

bool SWGF_Keyboard::check_press(const unsigned char code)
{
 bool result;
 result=false;
 if(SWGF_Keys[code]==SWGFKEY_PRESS)
 {
  if(preversion[code]==SWGFKEY_RELEASE) result=true;
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
  if(preversion[code]==SWGFKEY_PRESS) result=true;
 }
 preversion[code]=SWGF_Keys[code];
 return result;
}

SWGF_Mouse::SWGF_Mouse()
{
 memset(preversion,SWGFKEY_RELEASE,SWGF_MOUSE);
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
  SWGF_Show_Error("Can't set the mouse cursor position");
 }

}

unsigned long int SWGF_Mouse::get_x()
{
 POINT position;
 if(GetCursorPos(&position)==FALSE)
 {
  SWGF_Show_Error("Can't get the mouse cursor position");
 }
 return position.x;
}

unsigned long int SWGF_Mouse::get_y()
{
 POINT position;
 if(GetCursorPos(&position)==FALSE)
 {
  SWGF_Show_Error("Can't get the mouse cursor position");
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
  preversion[button]=SWGF_Buttons[button];
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
   if(preversion[button]==SWGFKEY_RELEASE) result=true;
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
   if(preversion[button]==SWGFKEY_PRESS) result=true;
  }

 }
 preversion[button]=SWGF_Buttons[button];
 return result;
}

SWGF_Gamepad::SWGF_Gamepad()
{
 active=0;
 memset(&configuration,0,sizeof(JOYCAPS));
 memset(&current,0,sizeof(JOYINFOEX));
 memset(&preversion,0,sizeof(JOYINFOEX));
 current.dwSize=sizeof(JOYINFOEX);
 preversion.dwSize=sizeof(JOYINFOEX);
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
 if(joyGetDevCaps((UINT_PTR)active,&configuration,sizeof(JOYCAPS))==JOYERR_NOERROR) result=true;
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
 current.dwSize=sizeof(JOYINFOEX);
 preversion.dwSize=sizeof(JOYINFOEX);
 current.dwFlags=JOY_RETURNALL;
 preversion.dwFlags=JOY_RETURNALL;
 current.dwPOV=JOY_POVCENTERED;
 preversion.dwPOV=JOY_POVCENTERED;
}

bool SWGF_Gamepad::check_button(const SWGF_GAMEPAD_BUTTONS button,const JOYINFOEX &target)
{
 bool result;
 result=false;
 if(target.dwButtons&button) result=true;
 return result;
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

void SWGF_Gamepad::set_active(const unsigned int gamepad)
{
 if(active<16)
 {
  this->clear_state();
  active=gamepad;
 }

}

unsigned int SWGF_Gamepad::get_active()
{
 return active;
}

SWGF_GAMEPAD_DPAD SWGF_Gamepad::get_dpad()
{
 SWGF_GAMEPAD_DPAD result;
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

SWGF_GAMEPAD_DIRECTION SWGF_Gamepad::get_stick_x(const SWGF_GAMEPAD_STICKS stick)
{
 SWGF_GAMEPAD_DIRECTION result;
 unsigned long int control;
 result=SWGF_NEUTRAL_DIRECTION;
 if(stick==SWGF_GAMEPAD_LEFT_STICK)
 {
  if(this->get_sticks_amount()>0)
  {
   control=(configuration.wXmax-configuration.wXmin)/2;
   if(current.dwXpos<control) result=SWGF_NEGATIVE_DIRECTION;
   if(current.dwXpos>control) result=SWGF_POSITIVE_DIRECTION;
  }

 }
 if(stick==SWGF_GAMEPAD_RIGHT_STICK)
 {
  if(this->get_sticks_amount()>1)
  {
   control=(configuration.wZmax-configuration.wZmin)/2;
   if(current.dwZpos<control) result=SWGF_NEGATIVE_DIRECTION;
   if(current.dwZpos>control) result=SWGF_POSITIVE_DIRECTION;
  }

 }
 return result;
}

SWGF_GAMEPAD_DIRECTION SWGF_Gamepad::get_stick_y(const SWGF_GAMEPAD_STICKS stick)
{
 SWGF_GAMEPAD_DIRECTION result;
 unsigned long int control;
 result=SWGF_NEUTRAL_DIRECTION;
 if(stick==SWGF_GAMEPAD_LEFT_STICK)
 {
  if(this->get_sticks_amount()>0)
  {
   control=(configuration.wYmax-configuration.wYmin)/2;
   if(current.dwYpos<control) result=SWGF_NEGATIVE_DIRECTION;
   if(current.dwYpos>control) result=SWGF_POSITIVE_DIRECTION;
  }

 }
 if(stick==SWGF_GAMEPAD_RIGHT_STICK)
 {
  if(this->get_sticks_amount()>1)
  {
   control=(configuration.wRmax-configuration.wRmin)/2;
   if(current.dwRpos<control) result=SWGF_NEGATIVE_DIRECTION;
   if(current.dwRpos>control) result=SWGF_POSITIVE_DIRECTION;
  }

 }
 return result;
}

bool SWGF_Gamepad::check_hold(const SWGF_GAMEPAD_BUTTONS button)
{
 return this->check_button(button,current);
}

bool SWGF_Gamepad::check_press(const SWGF_GAMEPAD_BUTTONS button)
{
 bool result;
 result=false;
 if(this->check_button(button,current)==true)
 {
  if(this->check_button(button,preversion)==false) result=true;
 }
 return result;
}

bool SWGF_Gamepad::check_release(const SWGF_GAMEPAD_BUTTONS button)
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
  SWGF_Show_Error("Can't allocate memory");
 }
 for(index=0;index<length;++index) name[index]=btowc(target[index]);
 return name;
}

void SWGF_Multimedia::open(const wchar_t *target)
{
 player->StopWhenReady();
 if(loader->RenderFile(target,NULL)!=S_OK)
 {
  SWGF_Show_Error("Can't load a multimedia file");
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
  SWGF_Show_Error("Can't get the current and the end position");
 }
 return result;
}

void SWGF_Multimedia::rewind()
{
 long long position;
 position=0;
 if(controler->SetPositions(&position,AM_SEEKING_AbsolutePositioning,NULL,AM_SEEKING_NoPositioning)!=S_OK)
 {
  SWGF_Show_Error("Can't set start position");
 }

}

void SWGF_Multimedia::initialize()
{
 if(CoCreateInstance(CLSID_FilterGraph,NULL,CLSCTX_INPROC_SERVER,IID_IGraphBuilder,(void**)&loader)!=S_OK)
 {
  SWGF_Show_Error("Can't create a multimedia loader");
 }
 if(loader->QueryInterface(IID_IMediaControl,(void**)&player)!=S_OK)
 {
  SWGF_Show_Error("Can't create a multimedia player");
  ;
 }
 if(loader->QueryInterface(IID_IMediaSeeking,(void**)&controler)!=S_OK)
 {
  SWGF_Show_Error("Can't create a player controler");
 }
 if(loader->QueryInterface(IID_IVideoWindow,(void**)&video)!=S_OK)
 {
  SWGF_Show_Error("Can't create a video player");
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
  SWGF_Show_Error("Can't get the multimedia state");
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
  SWGF_Show_Error("Can't get the memory status");
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
  SWGF_Show_Error("Can't create log file");
 }

}

SWGF_File::SWGF_File()
{
 target=NULL;
}

SWGF_File::~SWGF_File()
{
 if(target!=NULL) fclose(target);
}

void SWGF_File::open(const char *name)
{
 target=fopen(name,"w+b");
 if(target==NULL)
 {
  SWGF_Show_Error("Can't open the binary file");
 }

}

void SWGF_File::close()
{
 if(target!=NULL) fclose(target);
}

void SWGF_File::set_position(const off_t offset)
{
 fseek(target,offset,SEEK_SET);
}

long int SWGF_File::get_position()
{
 return ftell(target);
}

long int SWGF_File::get_length()
{
 long int result;
 fseek(target,0,SEEK_END);
 result=ftell(target);
 rewind(target);
 return result;
}

void SWGF_File::read(void *buffer,const size_t length)
{
 fread(buffer,length,1,target);
}

void SWGF_File::write(void *buffer,const size_t length)
{
 fwrite(buffer,length,1,target);
}

bool SWGF_File::check_error()
{
 bool result;
 result=false;
 if(ferror(target)!=0) result=true;
 return result;
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
 result=static_cast<unsigned char*>(calloc(length,sizeof(unsigned char)));
 if(result==NULL)
 {
  SWGF_Show_Error("Can't allocate memory for image buffer");
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
  SWGF_Show_Error("Can't open a image file");
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
  SWGF_Show_Error("Invalid image format");
 }
 if(head.type!=2)
 {
  if(head.type!=10)
  {
   SWGF_Show_Error("Invalid image format");
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
  SWGF_Show_Error("Incorrect image format");
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

void SWGF_Canvas::save()
{
 surface->save();
}

void SWGF_Canvas::restore()
{
 surface->restore();
}

void SWGF_Canvas::set_width(const unsigned long int image_width)
{
 width=image_width;
}

void SWGF_Canvas::set_height(const unsigned long int image_height)
{
 height=image_height;
}

SWGF_Color *SWGF_Canvas::create_buffer(const unsigned long int image_width,const unsigned long int image_height)
{
 SWGF_Color *result;
 size_t length;
 length=(size_t)image_width*(size_t)image_height;
 result=(SWGF_Color*)calloc(length,3);
 if(result==NULL)
 {
  SWGF_Show_Error("Can't allocate memory for image buffer");
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

size_t SWGF_Canvas::get_length()
{
 return (size_t)width*(size_t)height;
}

unsigned long int SWGF_Canvas::get_image_width()
{
 return width;
}

unsigned long int SWGF_Canvas::get_image_height()
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

void SWGF_Canvas::mirror_image(const SWGF_MIRROR_TYPE kind)
{
 unsigned long int x,y;
 size_t index,index2;
 SWGF_Color *mirrored_image;
 mirrored_image=this->create_buffer(width,height);
 if (kind==SWGF_MIRROR_HORIZONTAL)
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
 if(kind==SWGF_MIRROR_VERTICAL)
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

SWGF_Background::SWGF_Background()
{
 start=0;
 background_width=0;
 background_height=0;
 current=0;
 frame=1;
 current_kind=SWGF_NORMAL_BACKGROUND;
}

SWGF_Background::~SWGF_Background()
{

}

void SWGF_Background::slow_draw_background()
{
 unsigned long int x,y;
 size_t offset;
 for(x=0;x<background_width;++x)
 {
  for(y=0;y<background_height;++y)
  {
   offset=this->get_offset(start,x,y);
   this->draw_image_pixel(offset,x,y);
  }

 }

}

void SWGF_Background::set_kind(SWGF_BACKGROUND_TYPE kind)
{
 switch(kind)
 {
  case SWGF_NORMAL_BACKGROUND:
  background_width=this->get_image_width();
  background_height=this->get_image_height();
  start=0;
  break;
  case SWGF_HORIZONTAL_BACKGROUND:
  background_width=this->get_image_width()/this->get_frames();
  background_height=this->get_image_height();
  start=(frame-1)*background_width;
  break;
  case SWGF_VERTICAL_BACKGROUND:
  background_width=this->get_image_width();
  background_height=this->get_image_height()/this->get_frames();
  start=(frame-1)*background_width*background_height;
  break;
 }
 current_kind=kind;
}

void SWGF_Background::set_target(const unsigned long int target)
{
 if((target>0)&&(target<=this->get_frames()))
 {
  frame=target;
  this->set_kind(current_kind);
 }

}

void SWGF_Background::draw_background()
{
 if (current!=frame)
 {
  this->slow_draw_background();
  this->save();
  current=frame;
 }
 else
 {
  this->restore();
 }

}

SWGF_Sprite::SWGF_Sprite()
{
 current_x=0;
 current_y=0;
 sprite_width=0;
 sprite_height=0;
 frame=0;
 start=0;
 current_kind=SWGF_SINGE_SPRITE;
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

unsigned long int SWGF_Sprite::get_x()
{
 return current_x;
}

unsigned long int SWGF_Sprite::get_y()
{
 return current_y;
}

unsigned long int SWGF_Sprite::get_width()
{
 return sprite_width;
}

unsigned long int SWGF_Sprite::get_height()
{
 return sprite_height;
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
 target.width=sprite_width;
 target.height=sprite_height;
 return target;
}

void SWGF_Sprite::set_kind(const SWGF_SPRITE_TYPE kind)
{
 switch(kind)
 {
  case SWGF_SINGE_SPRITE:
  sprite_width=this->get_image_width();
  sprite_height=this->get_image_height();
  start=0;
  break;
  case SWGF_HORIZONTAL_STRIP:
  sprite_width=this->get_image_width()/this->get_frames();
  sprite_height=this->get_image_height();
  start=(frame-1)*sprite_width;
  break;
  case SWGF_VERTICAL_STRIP:
  sprite_width=this->get_image_width();
  sprite_height=this->get_image_height()/this->get_frames();
  start=(frame-1)*sprite_width*sprite_height;
  break;
 }
 current_kind=kind;
}

SWGF_SPRITE_TYPE SWGF_Sprite::get_kind()
{
 return current_kind;
}

void SWGF_Sprite::set_target(const unsigned long int target)
{
 if((target>0)&&(target<=this->get_frames()))
 {
  frame=target;
  this->set_kind(current_kind);
 }

}

void SWGF_Sprite::set_position(const unsigned long int x,const unsigned long int y)
{
 current_x=x;
 current_y=y;
}

void SWGF_Sprite::clone(SWGF_Sprite &target)
{
 this->set_width(target.get_image_width());
 this->set_height(target.get_image_height());
 this->set_frames(target.get_frames());
 this->set_kind(target.get_kind());
 image=this->create_buffer(target.get_image_width(),target.get_image_width());
 memmove(image,target.get_image(),target.get_length());
}

void SWGF_Sprite::draw_sprite()
{
 size_t offset;
 unsigned long int sprite_x,sprite_y;
 for(sprite_x=0;sprite_x<sprite_width;++sprite_x)
 {
  for(sprite_y=0;sprite_y<sprite_height;++sprite_y)
  {
   offset=this->get_offset(start,sprite_x,sprite_y);
   this->draw_sprite_pixel(offset,current_x+sprite_x,current_y+sprite_y);
  }

 }

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
  sprite->set_target((unsigned long int)target+1);
  sprite->set_position(step_x,current_y);
  sprite->draw_sprite();
  step_x+=sprite->get_width();
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
 sprite->set_kind(SWGF_HORIZONTAL_STRIP);
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