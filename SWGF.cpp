/*
Simple windows game framework was create by Popov Evgeniy Alekseyevich
Some code was taken from wglext.h(https://www.khronos.org/registry/OpenGL/api/GL/wglext.h) by The Khronos Group Inc

Simple windows game framework license

Copyright (C) 2016-2020 Popov Evgeniy Alekseyevich

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

unsigned char Keys[KEYBOARD];
unsigned char Buttons[MOUSE];

LRESULT CALLBACK Process_Message(HWND window,UINT Message,WPARAM wParam,LPARAM lParam)
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
  memset(Keys,KEY_RELEASE,KEYBOARD);
  memset(Buttons,KEY_RELEASE,MOUSE);
  break;
  case WM_LBUTTONDOWN:
  Buttons[MOUSE_LEFT]=KEY_PRESS;
  break;
  case WM_LBUTTONUP:
  Buttons[MOUSE_LEFT]=KEY_RELEASE;
  break;
  case WM_RBUTTONDOWN:
  Buttons[MOUSE_RIGHT]=KEY_PRESS;
  break;
  case WM_RBUTTONUP:
  Buttons[MOUSE_RIGHT]=KEY_RELEASE;
  break;
  case WM_MBUTTONDOWN:
  Buttons[MOUSE_MIDDLE]=KEY_PRESS;
  break;
  case WM_MBUTTONUP:
  Buttons[MOUSE_MIDDLE]=KEY_RELEASE;
  break;
  case WM_KEYDOWN:
  Keys[GETSCANCODE(lParam)]=KEY_PRESS;
  break;
  case WM_KEYUP:
  Keys[GETSCANCODE(lParam)]=KEY_RELEASE;
  break;
 }
 return DefWindowProc(window,Message,wParam,lParam);
}

namespace SWGF
{

void Halt(const char *message)
{
 puts(message);
 exit(EXIT_FAILURE);
}

 COM_Base::COM_Base()
{
 HRESULT status;
 status=CoInitializeEx(NULL,COINIT_APARTMENTTHREADED);
 if(status!=S_OK)
 {
  if(status!=S_FALSE)
  {
   Halt("Can't initialize COM");
  }

 }

}

COM_Base::~COM_Base()
{
 CoUninitialize();
}

Synchronization::Synchronization()
{
 timer=NULL;
}

Synchronization::~Synchronization()
{
 if(timer==NULL)
 {
  CancelWaitableTimer(timer);
  CloseHandle(timer);
 }

}

void Synchronization::create_timer()
{
 timer=CreateWaitableTimer(NULL,FALSE,NULL);
 if (timer==NULL)
 {
  Halt("Can't create synchronization timer");
 }

}

void Synchronization::set_timer(const unsigned long int interval)
{
 LARGE_INTEGER start;
 start.QuadPart=0;
 if(SetWaitableTimer(timer,&start,interval,NULL,NULL,FALSE)==FALSE)
 {
  Halt("Can't set timer");
 }

}

void Synchronization::wait_timer()
{
 WaitForSingleObjectEx(timer,INFINITE,TRUE);
}

Engine::Engine()
{
 window_class.lpszClassName=TEXT("SWGF");
 window_class.style=CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
 window_class.cbSize=sizeof(WNDCLASSEX);
 window_class.lpfnWndProc=Process_Message;
 window_class.hInstance=NULL;
 window_class.hbrBackground=NULL;
 window_class.hIcon=NULL;
 window_class.hCursor=NULL;
 window_class.hIconSm=NULL;
 window_class.cbClsExtra=0;
 window_class.cbWndExtra=0;
 window=NULL;
}

Engine::~Engine()
{
 if(window!=NULL) CloseWindow(window);
 if(window_class.hbrBackground!=NULL) DeleteObject(window_class.hbrBackground);
 UnregisterClass(window_class.lpszClassName,window_class.hInstance);
}

void Engine::get_instance()
{
 if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,NULL,&window_class.hInstance)==FALSE)
 {
  Halt("Can't get the application instance");
 }

}

void Engine::set_backgrond_color()
{
 window_class.hbrBackground=CreateSolidBrush(RGB(0,0,0));
 if (window_class.hbrBackground==NULL)
 {
  Halt("Can't set background color");
 }

}

void Engine::load_icon()
{
 window_class.hIcon=LoadIcon(NULL,IDI_APPLICATION);
 if (window_class.hIcon==NULL)
 {
  Halt("Can't load the standart program icon");
 }

}

void Engine::load_cursor()
{
 window_class.hCursor=LoadCursor(NULL,IDC_ARROW);
 if (window_class.hCursor==NULL)
 {
  Halt("Can't load the standart cursor");
 }

}

void Engine::register_window_class()
{
 if (!RegisterClassEx(&window_class))
 {
  Halt("Can't register window class");
 }

}

HWND Engine::get_window()
{
 return window;
}

HDC Engine::get_context()
{
 return GetWindowDC(window);
}

void Engine::prepare_engine()
{
 this->get_instance();
 this->set_backgrond_color();
 this->load_icon();
 this->load_cursor();
 this->register_window_class();
}

void Engine::destroy_window()
{
 if(window!=NULL)
 {
  CloseWindow(window);
  window=NULL;
 }

}

void Engine::create_window()
{
 window=CreateWindowEx(WS_EX_APPWINDOW,window_class.lpszClassName,NULL,WS_VISIBLE|WS_POPUP,0,0,GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN),NULL,NULL,window_class.hInstance,NULL);
 if (window==NULL)
 {
  Halt("Can't create window");
 }
 SetFocus(window);
}

void Engine::capture_mouse()
{
 RECT border;
 if(GetClientRect(window,&border)==FALSE)
 {
  Halt("Can't capture window");
 }
 if(ClipCursor(&border)==FALSE)
 {
  Halt("Can't capture cursor");
 }

}

bool Engine::process_message()
{
 bool run;
 MSG Message;
 run=true;
 while(PeekMessage(&Message,window,0,0,PM_NOREMOVE)==TRUE)
 {
  if(GetMessage(&Message,window,0,0)==TRUE)
  {
   TranslateMessage(&Message);
   DispatchMessage(&Message);
  }
  else
  {
   run=false;
   break;
  }

 }
 return run;
}

Frame::Frame()
{
 frame_width=512;
 frame_height=512;
 pixels=0;
 buffer=NULL;
 shadow=NULL;
}

Frame::~Frame()
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

unsigned int *Frame::create_buffer(const char *error)
{
 unsigned int *target;
 pixels=static_cast<size_t>(frame_width)*static_cast<size_t>(frame_height);
 target=static_cast<unsigned int*>(calloc(pixels,sizeof(unsigned int)));
 if(target==NULL)
 {
  Halt(error);
 }
 return target;
}

void Frame::put_pixel(const size_t offset,const unsigned int red,const unsigned int green,const unsigned int blue)
{
 if (offset<pixels)
 {
  buffer[offset]=red+(green<<8)+(blue<<16);
 }

}

size_t Frame::get_offset(const unsigned long int x,const unsigned long int y,const unsigned long int target_width)
{
 return static_cast<size_t>(x)+static_cast<size_t>(y)*static_cast<size_t>(target_width);
}

size_t Frame::get_offset(const unsigned long int x,const unsigned long int y)
{
 return this->get_offset(x,y,frame_width);
}

void Frame::set_size(const unsigned long int surface_width,const unsigned long int surface_height)
{
 frame_width=surface_width;
 frame_height=surface_height;
}

void Frame::set_size(const SURFACE surface)
{
 if (surface==SURFACE_SMALL) this->set_size(256,256);
 if (surface==SURFACE_LARGE) this->set_size(512,512);
}

void Frame::create_buffers()
{
 buffer=this->create_buffer("Can't allocate memory for render buffer");
 shadow=this->create_buffer("Can't allocate memory for shadow buffer");
}

unsigned int *Frame::get_buffer()
{
 return buffer;
}

size_t Frame::get_pixels() const
{
 return pixels;
}

void Frame::draw_pixel(const unsigned long int x,const unsigned long int y,const unsigned char red,const unsigned char green,const unsigned char blue)
{
 this->put_pixel(this->get_offset(x,y),red,green,blue);
}

void Frame::clear_screen()
{
 size_t index;
 for (index=0;index<pixels;++index)
 {
  buffer[index]=0;
 }

}

void Frame::save()
{
 size_t index;
 for (index=0;index<pixels;++index)
 {
  shadow[index]=buffer[index];
 }

}

void Frame::restore()
{
 size_t index;
 for (index=0;index<pixels;++index)
 {
  buffer[index]=shadow[index];
 }

}

void Frame::restore(const unsigned long int x,const unsigned long int y,const unsigned long int width,const unsigned long int height)
{
 unsigned long int target_x,target_y,stop_x,stop_y;
 size_t position;
 stop_x=x+width;
 stop_y=y+height;
 if ((x<frame_width)&&(y<frame_height))
 {
  if ((stop_x<=frame_width)&&(stop_y<=frame_height))
  {
   for (target_x=x;target_x<stop_x;++target_x)
   {
    for (target_y=y;target_y<stop_y;++target_y)
    {
     position=this->get_offset(target_x,target_y);
     buffer[position]=shadow[position];
    }

   }

  }

 }

}

unsigned long int Frame::get_frame_width() const
{
 return frame_width;
}

unsigned long int Frame::get_frame_height() const
{
 return frame_height;
}

Plane::Plane()
{
 target=NULL;
 plane=NULL;
 target_width=0;
 target_height=0;
 x_ratio=0;
 y_ratio=0;
}

Plane::~Plane()
{

}

void Plane::create_plane(const unsigned long int width,const unsigned long int height,const unsigned long int surface_width,const unsigned long int surface_height,unsigned int *surface_buffer)
{
 this->set_size(width,height);
 this->create_buffers();
 plane=this->get_buffer();
 target=surface_buffer;
 target_width=surface_width;
 target_height=surface_height;
 x_ratio=static_cast<float>(width)/static_cast<float>(surface_width);
 y_ratio=static_cast<float>(height)/static_cast<float>(surface_height);
}

void Plane::transfer()
{
 unsigned long int x,y,width;
 size_t index,position;
 width=this->get_frame_width();
 for (x=0;x<target_width;++x)
 {
  for (y=0;y<target_height;++y)
  {
   index=this->get_offset(x,y,target_width);
   position=this->get_offset((x_ratio*static_cast<float>(x)),(y_ratio*static_cast<float>(y)),width);
   target[index]=plane[position];
  }

 }

}

Plane* Plane::get_handle()
{
 return this;
}

Timer::Timer()
{
 interval=0;
 start=time(NULL);
}

Timer::~Timer()
{

}

void Timer::set_timer(const unsigned long int seconds)
{
 interval=seconds;
 start=time(NULL);
}

bool Timer::check_timer()
{
 bool result;
 result=false;
 if (difftime(time(NULL),start)>=interval)
 {
  result=true;
  start=time(NULL);
 }
 return result;
}

FPS::FPS()
{
 timer.set_timer(1);
 current=0;
 fps=0;
}

FPS::~FPS()
{

}

void FPS::update_counter()
{
 ++current;
 if (timer.check_timer()==true)
 {
  fps=current;
  current=0;
 }

}

unsigned long int FPS::get_fps() const
{
 return fps;
}

Display::Display()
{
 memset(&display,0,sizeof(DEVMODE));
 display.dmSize=sizeof(DEVMODE);
}

Display::~Display()
{
 ChangeDisplaySettingsEx(NULL,NULL,NULL,0,NULL);
}

void Display::set_video_mode()
{
 if (ChangeDisplaySettingsEx(NULL,&display,NULL,CDS_FULLSCREEN,NULL)!=DISP_CHANGE_SUCCESSFUL)
 {
  Halt("Can't change video mode");
 }

}

void Display::get_video_mode()
{
 if (EnumDisplaySettingsEx(NULL,ENUM_CURRENT_SETTINGS,&display,EDS_RAWMODE)==FALSE)
 {
  Halt("Can't get display setting");
 }

}

void Display::check_video_mode()
{
 this->get_video_mode();
 if(display.dmBitsPerPel<16)
 {
  display.dmBitsPerPel=16;
  this->set_video_mode();
 }

}

void Display::set_display_mode(const unsigned long int screen_width,const unsigned long int screen_height)
{
 this->get_video_mode();
 if((display.dmPelsWidth!=screen_width)||(display.dmPelsHeight!=screen_height))
 {
  display.dmPelsWidth=screen_width;
  display.dmPelsHeight=screen_height;
  this->set_video_mode();
 }

}

unsigned long int Display::get_color() const
{
 return display.dmBitsPerPel;
}

unsigned long int Display::get_width() const
{
 return display.dmPelsWidth;
}

unsigned long int Display::get_height() const
{
 return display.dmPelsHeight;
}

WINGL::WINGL()
{
 context=NULL;
 render=NULL;
 wglSwapIntervalEXT=NULL;
 memset(&setting,0,sizeof(PIXELFORMATDESCRIPTOR));
 setting.nSize=sizeof(PIXELFORMATDESCRIPTOR);
 setting.nVersion=1;
}

WINGL::~WINGL()
{
 if(render!=NULL)
 {
  wglMakeCurrent(NULL,NULL);
  wglDeleteContext(render);
 }
 if(context!=NULL) ReleaseDC(this->get_window(),context);
}

bool WINGL::check_base_setting()
{
 bool result;
 result=false;
 if (setting.cColorBits==this->get_color())
 {
  if ((setting.dwFlags&PFD_DRAW_TO_WINDOW)&&(setting.dwFlags&PFD_SUPPORT_OPENGL)) result=true;
 }
 return result;
}

bool WINGL::check_advanced_setting() const
{
 bool result;
 result=false;
 if (setting.dwFlags&PFD_DOUBLEBUFFER)
 {
  if ((setting.iPixelType==PFD_TYPE_RGBA)&&(setting.iLayerType==PFD_MAIN_PLANE)) result=true;
 }
 return result;
}

bool WINGL::check_common_setting()
{
 bool result;
 result=false;
 if(this->check_base_setting()==true)
 {
  result=this->check_advanced_setting();
 }
 return result;
}

bool WINGL::check_acceleration() const
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

int WINGL::get_pixel_format()
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

void WINGL::set_pixel_format(const int format)
{
 if(format==0)
 {
  Halt("Invalid pixel format");
 }
 DescribePixelFormat(context,format,setting.nSize,&setting);
 if(SetPixelFormat(context,format,&setting)==FALSE)
 {
  Halt("Can't set pixel format");
 }

}

void WINGL::create_render_context()
{
 render=wglCreateContext(context);
 if(render==NULL)
 {
  Halt("Can't create render context");
 }
 wglMakeCurrent(context,render);
}

void WINGL::destroy_render_context()
{
 if(render!=NULL)
 {
  wglMakeCurrent(NULL,NULL);
  wglDeleteContext(render);
  render=NULL;
 }
 if(context!=NULL)
 {
  ReleaseDC(this->get_window(),context);
  context=NULL;
 }

}

void WINGL::set_render()
{
 int format;
 context=this->get_context();
 if(context==NULL)
 {
  Halt("Can't get the window context");
 }
 format=this->get_pixel_format();
 this->set_pixel_format(format);
 this->create_render_context();
}

void WINGL::disable_vsync()
{
 wglSwapIntervalEXT=reinterpret_cast<PFNWGLSWAPINTERVALEXTPROC>(wglGetProcAddress("wglSwapIntervalEXT"));
 if(wglSwapIntervalEXT==NULL)
 {
  Halt("Can't load OPENGL extension");
 }
 wglSwapIntervalEXT(0);
}

void WINGL::Swap()
{
 SwapBuffers(context);
}

Render::Render()
{
 vertex[0].x=0;
 vertex[0].y=0;
 vertex[1].x=0;
 vertex[1].y=0;
 vertex[2].x=0;
 vertex[2].y=0;
 vertex[3].x=0;
 vertex[3].y=0;
 point[0].u=0;
 point[0].v=0;
 point[1].u=0;
 point[1].v=0;
 point[2].u=0;
 point[2].v=0;
 point[3].u=0;
 point[3].v=0;
 texture=0;
}

Render::~Render()
{

}

void Render::set_perfomance_setting()
{
 glDisable(GL_ALPHA_TEST);
 glDisable(GL_BLEND);
 glDisable(GL_DEPTH_TEST);
 glDisable(GL_DITHER);
 glDisable(GL_FOG);
 glDisable(GL_CULL_FACE);
 glDisable(GL_LIGHTING);
 glDisable(GL_NORMALIZE);
 glDisable(GL_TEXTURE_1D);
 glEnable(GL_TEXTURE_2D);
 glEnableClientState(GL_VERTEX_ARRAY);
 glEnableClientState(GL_TEXTURE_COORD_ARRAY);
 glDisableClientState(GL_COLOR_ARRAY);
 glDisableClientState(GL_EDGE_FLAG_ARRAY);
 glDisableClientState(GL_INDEX_ARRAY);
 glDisableClientState(GL_NORMAL_ARRAY);
}

void Render::set_perspective()
{
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glOrtho(0,this->get_width(),this->get_height(),0,0,1);
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 glMatrixMode(GL_TEXTURE);
 glLoadIdentity();
 glViewport(0,0,this->get_width(),this->get_height());
}

void Render::clear_stage()
{
 glClearColor(0,0,0,0);
 glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

void Render::check_videocard()
{
 int control;
 control=0;
 glGetIntegerv(GL_MAX_TEXTURE_SIZE,&control);
 if (control<static_cast<int>(this->get_frame_width()))
 {
  Halt("This video card don't support request texture size");
 }
 if (control<static_cast<int>(this->get_frame_height()))
 {
  Halt("This video card don't support request texture size");
 }

}

void Render::set_vertex_coordinates()
{
 vertex[0].x=0;
 vertex[0].y=this->get_height();
 vertex[1].x=this->get_width();
 vertex[1].y=this->get_height();
 vertex[2].x=this->get_width();
 vertex[2].y=0;
 vertex[3].x=0;
 vertex[3].y=0;
}

void Render::set_texture_coordinates()
{
 point[0].u=0;
 point[0].v=1;
 point[1].u=1;
 point[1].v=1;
 point[2].u=1;
 point[2].v=0;
 point[3].u=0;
 point[3].v=0;
}

void Render::prepare_surface()
{
 this->set_vertex_coordinates();
 this->set_texture_coordinates();
}

void Render::create_texture()
{
 glPixelStorei(GL_UNPACK_ALIGNMENT,1);
 glGenTextures(1,&texture);
 glBindTexture(GL_TEXTURE_2D,texture);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
}

void Render::load_surface_data()
{
 glVertexPointer(2,GL_INT,0,vertex);
 glTexCoordPointer(2,GL_FLOAT,0,point);
}

void Render::create_render()
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

void Render::draw()
{
 glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,this->get_frame_width(),this->get_frame_height(),0,GL_RGBA,GL_UNSIGNED_BYTE,this->get_buffer());
 glDrawArrays(GL_TRIANGLE_FAN,0,4);
}

void Render::destroy_render()
{
 this->destroy_render_context();
 this->destroy_window();
}

void Render::start_render()
{
 this->create_window();
 this->capture_mouse();
 this->create_render();
}

void Render::refresh()
{
 this->draw();
 this->Swap();
}

void Screen::initialize()
{
 this->check_video_mode();
 this->prepare_engine();
 this->start_render();
 this->create_buffers();
 this->create_timer();
 this->set_timer(17);
}

void Screen::initialize(const SURFACE surface)
{
 this->set_size(surface);
 this->initialize();
}

void Screen::set_mode(const unsigned long int screen_width,const unsigned long int screen_height)
{
 this->destroy_render();
 this->set_display_mode(screen_width,screen_height);
 this->start_render();
}

bool Screen::update()
{
 this->refresh();
 this->update_counter();
 return this->process_message();
}

bool Screen::sync()
{
 bool run;
 run=this->update();
 this->wait_timer();
 return run;
}

Screen* Screen::get_handle()
{
 return this;
}

Keyboard::Keyboard()
{
 preversion=NULL;
}

Keyboard::~Keyboard()
{
 if(preversion!=NULL) free(preversion);
}

unsigned char *Keyboard::create_buffer(const char *error)
{
 unsigned char *buffer;
 buffer=static_cast<unsigned char*>(calloc(KEYBOARD,sizeof(unsigned char)));
 if (buffer==NULL)
 {
  Halt(error);
 }
 return buffer;
}

bool Keyboard::check_state(const unsigned char code,const unsigned char state)
{
 bool result;
 result=false;
 if(Keys[code]==state)
 {
  if(preversion[code]!=state) result=true;
 }
 preversion[code]=Keys[code];
 return result;
}

void Keyboard::initialize()
{
 preversion=this->create_buffer("Can't allocate memory for keyboard state buffer");
}

bool Keyboard::check_hold(const unsigned char code)
{
 bool result;
 result=false;
 if(Keys[code]==KEY_PRESS) result=true;
 preversion[code]=Keys[code];
 return result;
}

bool Keyboard::check_press(const unsigned char code)
{
 return this->check_state(code,KEY_PRESS);
}

bool Keyboard::check_release(const unsigned char code)
{
 return this->check_state(code,KEY_RELEASE);
}

Mouse::Mouse()
{
 memset(preversion,KEY_RELEASE,MOUSE);
 position.x=0;
 position.y=0;
}

Mouse::~Mouse()
{
 while(ShowCursor(TRUE)<1) ;
}

void Mouse::get_position()
{
 if(GetCursorPos(&position)==FALSE)
 {
  Halt("Can't get the mouse cursor position");
 }

}

bool Mouse::check_state(const unsigned char button,const unsigned char state)
{
 bool result;
 result=false;
 if(button<=MOUSE_MIDDLE)
 {
  if(Buttons[button]==state)
  {
   if(preversion[button]!=state) result=true;
  }
  preversion[button]=Buttons[button];
 }
 return result;
}

void Mouse::show()
{
 while(ShowCursor(TRUE)<1) ;
}

void Mouse::hide()
{
 while(ShowCursor(FALSE)>-2) ;
}

void Mouse::set_position(const unsigned long int x,const unsigned long int y)
{
 if(SetCursorPos(x,y)==FALSE)
 {
  Halt("Can't set the mouse cursor position");
 }

}

unsigned long int Mouse::get_x()
{
 this->get_position();
 return position.x;
}

unsigned long int Mouse::get_y()
{
 this->get_position();
 return position.y;
}

bool Mouse::check_hold(const unsigned char button)
{
 bool result;
 result=false;
 if(button<=MOUSE_MIDDLE)
 {
  if(Buttons[button]==KEY_PRESS) result=true;
  preversion[button]=Buttons[button];
 }
 return result;
}

bool Mouse::check_press(const unsigned char button)
{
 return this->check_state(button,KEY_PRESS);
}

bool Mouse::check_release(const unsigned char button)
{
 return this->check_state(button,KEY_RELEASE);
}

Gamepad::Gamepad()
{
 active=0;
 max_amount=16;
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

Gamepad::~Gamepad()
{

}

bool Gamepad::read_configuration()
{
 bool result;
 result=false;
 if (joyGetDevCaps(static_cast<size_t>(active),&configuration,sizeof(JOYCAPS))==JOYERR_NOERROR)
 {
  result=true;
 }
 return result;
}

bool Gamepad::read_state()
{
 bool result;
 result=false;
 if(joyGetPosEx(active,&current)==JOYERR_NOERROR) result=true;
 return result;
}

void Gamepad::clear_state()
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

bool Gamepad::check_button(const GAMEPAD_BUTTONS button,const JOYINFOEX &target)
{
 bool result;
 result=false;
 if(target.dwButtons&button) result=true;
 return result;
}

unsigned int Gamepad::get_amount()
{
 return joyGetNumDevs();
}

unsigned int Gamepad::get_button_amount()
{
 unsigned int result;
 result=0;
 if(this->read_configuration()==true) result=configuration.wNumButtons;
 return result;
}

unsigned int Gamepad::get_last_index()
{
 unsigned int last_index;
 last_index=this->get_amount();
 if (last_index>0) --last_index;
 return last_index;
}

bool Gamepad::check_connection()
{
 return this->read_state();
}

void Gamepad::update()
{
 preversion=current;
 if(this->read_state()==false) this->clear_state();
}

unsigned long int Gamepad::get_sticks_amount()
{
 unsigned long int result;
 result=0;
 if (this->read_configuration()==true)
 {
  switch (configuration.wNumAxes)
  {
   case 2:
   result=1;
   break;
   case 4:
   result=2;
   break;
  }

 }
 return result;
}

void Gamepad::set_active(const unsigned int gamepad)
{
 if(active<max_amount)
 {
  this->clear_state();
  active=gamepad;
 }

}

unsigned int Gamepad::get_max_amount() const
{
 return max_amount;
}

unsigned int Gamepad::get_active() const
{
 return active;
}

GAMEPAD_DPAD Gamepad::get_dpad() const
{
 GAMEPAD_DPAD result;
 result=GAMEPAD_NONE;
 switch (current.dwPOV)
 {
  case JOY_POVFORWARD:
  result=GAMEPAD_UP;
  break;
  case JOY_POVBACKWARD:
  result=GAMEPAD_DOWN;
  break;
  case JOY_POVLEFT:
  result=GAMEPAD_LEFT;
  break;
  case JOY_POVRIGHT:
  result=GAMEPAD_RIGHT;
  break;
  case JOYSTICK_UPLEFT:
  result=GAMEPAD_UPLEFT;
  break;
  case JOYSTICK_UPRIGHT:
  result=GAMEPAD_UPRIGHT;
  break;
  case JOYSTICK_DOWNLEFT:
  result=GAMEPAD_DOWNLEFT;
  break;
  case JOYSTICK_DOWNRIGHT:
  result=GAMEPAD_DOWNRIGHT;
  break;
 }
 return result;
}

GAMEPAD_DIRECTION Gamepad::get_stick_x(const GAMEPAD_STICKS stick)
{
 GAMEPAD_DIRECTION result;
 unsigned long int control;
 result=GAMEPAD_NEUTRAL_DIRECTION;
 if(stick==GAMEPAD_LEFT_STICK)
 {
  if(this->get_sticks_amount()>0)
  {
   control=(configuration.wXmax-configuration.wXmin)/2;
   if(current.dwXpos<control) result=GAMEPAD_NEGATIVE_DIRECTION;
   if(current.dwXpos>control) result=GAMEPAD_POSITIVE_DIRECTION;
  }

 }
 if(stick==GAMEPAD_RIGHT_STICK)
 {
  if(this->get_sticks_amount()>1)
  {
   control=(configuration.wZmax-configuration.wZmin)/2;
   if(current.dwZpos<control) result=GAMEPAD_NEGATIVE_DIRECTION;
   if(current.dwZpos>control) result=GAMEPAD_POSITIVE_DIRECTION;
  }

 }
 return result;
}

GAMEPAD_DIRECTION Gamepad::get_stick_y(const GAMEPAD_STICKS stick)
{
 GAMEPAD_DIRECTION result;
 unsigned long int control;
 result=GAMEPAD_NEUTRAL_DIRECTION;
 if(stick==GAMEPAD_LEFT_STICK)
 {
  if(this->get_sticks_amount()>0)
  {
   control=(configuration.wYmax-configuration.wYmin)/2;
   if(current.dwYpos<control) result=GAMEPAD_NEGATIVE_DIRECTION;
   if(current.dwYpos>control) result=GAMEPAD_POSITIVE_DIRECTION;
  }

 }
 if(stick==GAMEPAD_RIGHT_STICK)
 {
  if(this->get_sticks_amount()>1)
  {
   control=(configuration.wRmax-configuration.wRmin)/2;
   if(current.dwRpos<control) result=GAMEPAD_NEGATIVE_DIRECTION;
   if(current.dwRpos>control) result=GAMEPAD_POSITIVE_DIRECTION;
  }

 }
 return result;
}

bool Gamepad::check_hold(const GAMEPAD_BUTTONS button)
{
 return this->check_button(button,current);
}

bool Gamepad::check_press(const GAMEPAD_BUTTONS button)
{
 bool result;
 result=false;
 if(this->check_button(button,current)==true)
 {
  if(this->check_button(button,preversion)==false) result=true;
 }
 return result;
}

bool Gamepad::check_release(const GAMEPAD_BUTTONS button)
{
 bool result;
 result=false;
 if(this->check_button(button,current)==false)
 {
  if(this->check_button(button,preversion)==true) result=true;
 }
 return result;
}

Multimedia::Multimedia()
{
 loader=NULL;
 player=NULL;
 controler=NULL;
 video=NULL;
}

Multimedia::~Multimedia()
{
 if(player!=NULL) player->StopWhenReady();
 if(video!=NULL) video->Release();
 if(controler!=NULL) controler->Release();
 if(player!=NULL) player->Release();
 if(loader!=NULL) loader->Release();
}

wchar_t *Multimedia::create_buffer(const size_t length)
{
 wchar_t *target;
 target=NULL;
 target=static_cast<wchar_t*>(calloc(length+1,sizeof(wchar_t)));
 if(target==NULL)
 {
  Halt("Can't allocate memory");
 }
 return target;
}

void Multimedia::convert_string(const char *source,wchar_t *target)
{
 size_t index,length;
 length=strlen(source);
 for (index=0;index<length;++index)
 {
  target[index]=btowc(source[index]);
 }

}

wchar_t *Multimedia::convert_file_name(const char *target)
{
 wchar_t *name;
 name=NULL;
 name=this->create_buffer(strlen(target));
 this->convert_string(target,name);
 return name;
}

void Multimedia::open(const wchar_t *target)
{
 player->StopWhenReady();
 if(loader->RenderFile(target,NULL)!=S_OK)
 {
  Halt("Can't load a multimedia file");
 }
 video->put_FullScreenMode(OATRUE);
}

bool Multimedia::is_end()
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
  Halt("Can't get the current and the end position");
 }
 return result;
}

void Multimedia::rewind()
{
 long long position;
 position=0;
 if(controler->SetPositions(&position,AM_SEEKING_AbsolutePositioning,NULL,AM_SEEKING_NoPositioning)!=S_OK)
 {
  Halt("Can't set start position");
 }

}

void Multimedia::create_loader()
{
 if(CoCreateInstance(CLSID_FilterGraph,NULL,CLSCTX_INPROC_SERVER,IID_IGraphBuilder,reinterpret_cast<void**>(&loader))!=S_OK)
 {
  Halt("Can't create a multimedia loader");
 }

}

void Multimedia::create_player()
{
 if(loader->QueryInterface(IID_IMediaControl,reinterpret_cast<void**>(&player))!=S_OK)
 {
  Halt("Can't create a multimedia player");
 }

}

void Multimedia::create_controler()
{
 if(loader->QueryInterface(IID_IMediaSeeking,reinterpret_cast<void**>(&controler))!=S_OK)
 {
  Halt("Can't create a player controler");
 }

}

void Multimedia::create_video_player()
{
 if(loader->QueryInterface(IID_IVideoWindow,reinterpret_cast<void**>(&video))!=S_OK)
 {
  Halt("Can't create a video player");
 }

}

void Multimedia::initialize()
{
 this->create_loader();
 this->create_player();
 this->create_controler();
 this->create_video_player();
}

void Multimedia::load(const char *target)
{
 wchar_t *name;
 name=this->convert_file_name(target);
 this->open(name);
 free(name);
}

bool Multimedia::check_playing()
{
 OAFilterState state;
 bool result;
 result=false;
 if(player->GetState(INFINITE,&state)==E_FAIL)
 {
  Halt("Can't get the multimedia state");
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

void Multimedia::stop()
{
 player->StopWhenReady();
}

void Multimedia::play()
{
 this->stop();
 this->rewind();
 player->Run();
}

Memory::Memory()
{
 memset(&memory,0,sizeof(MEMORYSTATUSEX));
 memory.dwLength=sizeof(MEMORYSTATUSEX);
}

Memory::~Memory()
{

}

void Memory::get_status()
{
 if(GlobalMemoryStatusEx(&memory)==FALSE)
 {
  Halt("Can't get the memory status");
 }

}

unsigned long long int Memory::get_total_physical()
{
 this->get_status();
 return memory.ullTotalPhys;
}

unsigned long long int Memory::get_free_physical()
{
 this->get_status();
 return memory.ullAvailPhys;
}

unsigned long long int Memory::get_total_virtual()
{
 this->get_status();
 return memory.ullTotalVirtual;
}

unsigned long long int Memory::get_free_virtual()
{
 this->get_status();
 return memory.ullAvailVirtual;
}

unsigned long int Memory::get_usage()
{
 this->get_status();
 return memory.dwMemoryLoad;
}

System::System()
{
 srand(UINT_MAX);
}

System::~System()
{

}

unsigned long int System::get_random(const unsigned long int number)
{
 return rand()%number;
}

void System::quit()
{
 exit(EXIT_SUCCESS);
}

void System::run(const char *command)
{
 system(command);
}

char* System::read_environment(const char *variable)
{
 return getenv(variable);
}

bool System::file_exist(const char *name)
{
 FILE *target;
 bool result;
 result=false;
 target=fopen(name,"rb");
 if (target!=NULL)
 {
  result=true;
  fclose(target);
 }
 return result;
}

bool System::delete_file(const char *name)
{
 bool result;
 result=false;
 if (remove(name)==0)
 {
  result=true;
 }
 return result;
}

void System::enable_logging(const char *name)
{
 if(freopen(name,"wt",stdout)==NULL)
 {
  Halt("Can't create log file");
 }

}

Binary_File::Binary_File()
{
 target=NULL;
}

Binary_File::~Binary_File()
{
 if(target!=NULL)
 {
  fclose(target);
  target=NULL;
 }

}

void Binary_File::open(const char *name,const char *mode)
{
 target=fopen(name,mode);
 if(target==NULL)
 {
  Halt("Can't open the binary file");
 }

}

void Binary_File::create_temp()
{
 target=tmpfile();
 if(target==NULL)
 {
  Halt("Can't create a temporary file");
 }

}

void Binary_File::open_read(const char *name)
{
 this->open(name,"rb");
}

void Binary_File::open_write(const char *name)
{
 this->open(name,"w+b");
}

void Binary_File::close()
{
 if(target!=NULL)
 {
  fclose(target);
  target=NULL;
 }

}

void Binary_File::set_position(const long int offset)
{
 fseek(target,offset,SEEK_SET);
}

long int Binary_File::get_position()
{
 return ftell(target);
}

long int Binary_File::get_length()
{
 long int result;
 fseek(target,0,SEEK_END);
 result=ftell(target);
 rewind(target);
 return result;
}

void Binary_File::read(void *buffer,const size_t length)
{
 fread(buffer,sizeof(char),length,target);
}

void Binary_File::write(void *buffer,const size_t length)
{
 fwrite(buffer,sizeof(char),length,target);
}

bool Binary_File::check_error()
{
 bool result;
 result=false;
 if(ferror(target)!=0) result=true;
 return result;
}

Primitive::Primitive()
{
 color.red=0;
 color.green=0;
 color.blue=0;
 surface=NULL;
}

Primitive::~Primitive()
{
 color.red=0;
 color.green=0;
 color.blue=0;
 surface=NULL;
}

void Primitive::initialize(Screen *Screen)
{
 surface=Screen;
}

void Primitive::set_color(const unsigned char red,const unsigned char green,const unsigned char blue)
{
 color.red=red;
 color.green=green;
 color.blue=blue;
}

void Primitive::draw_line(const unsigned long int x1,const unsigned long int y1,const unsigned long int x2,const unsigned long int y2)
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
 shift_x=static_cast<float>(delta_x)/static_cast<float>(steps);
 shift_y=static_cast<float>(delta_y)/static_cast<float>(steps);
 for (index=steps;index>0;--index)
 {
  x+=shift_x;
  y+=shift_y;
  surface->draw_pixel(x,y,color.red,color.green,color.blue);
 }

}

void Primitive::draw_rectangle(const unsigned long int x,const unsigned long int y,const unsigned long int width,const unsigned long int height)
{
 unsigned long int stop_x,stop_y;
 stop_x=x+width;
 stop_y=y+height;
 this->draw_line(x,y,stop_x,y);
 this->draw_line(x,stop_y,stop_x,stop_y);
 this->draw_line(x,y,x,stop_y);
 this->draw_line(stop_x,y,stop_x,stop_y);
}

void Primitive::draw_filled_rectangle(const unsigned long int x,const unsigned long int y,const unsigned long int width,const unsigned long int height)
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

Image::Image()
{
 width=0;
 height=0;
 data=NULL;
}

Image::~Image()
{
 if(data!=NULL) free(data);
}

unsigned char *Image::create_buffer(const size_t length)
{
 unsigned char *result;
 result=static_cast<unsigned char*>(calloc(length,sizeof(unsigned char)));
 if(result==NULL)
 {
  Halt("Can't allocate memory for image buffer");
 }
 return result;
}

void Image::clear_buffer()
{
 if(data!=NULL)
 {
  free(data);
  data=NULL;
 }

}

void Image::load_tga(const char *name)
{
 Binary_File target;
 size_t index,position,amount,compressed_length,uncompressed_length;
 unsigned char *compressed;
 unsigned char *uncompressed;
 TGA_head head;
 TGA_map color_map;
 TGA_image image;
 this->clear_buffer();
 target.open_read(name);
 compressed_length=static_cast<size_t>(target.get_length()-18);
 target.read(&head,3);
 target.read(&color_map,5);
 target.read(&image,10);
 if((head.color_map!=0)||(image.color!=24))
 {
  Halt("Invalid image format");
 }
 if(head.type!=2)
 {
  if(head.type!=10)
  {
   Halt("Invalid image format");
  }

 }
 index=0;
 position=0;
 width=image.width;
 height=image.height;
 uncompressed_length=this->get_length();
 uncompressed=this->create_buffer(uncompressed_length);
 if(head.type==2)
 {
  target.read(uncompressed,uncompressed_length);
 }
 if(head.type==10)
 {
  compressed=this->create_buffer(compressed_length);
  target.read(compressed,compressed_length);
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
 target.close();
 data=uncompressed;
}

void Image::load_pcx(const char *name)
{
 Binary_File target;
 unsigned long int x,y;
 size_t index,position,line,row,length,uncompressed_length;
 unsigned char repeat;
 unsigned char *original;
 unsigned char *uncompressed;
 PCX_head head;
 this->clear_buffer();
 target.open_read(name);
 length=static_cast<size_t>(target.get_length()-128);
 target.read(&head,128);
 if((head.color*head.planes!=24)&&(head.compress!=1))
 {
  Halt("Incorrect image format");
 }
 width=head.max_x-head.min_x+1;
 height=head.max_y-head.min_y+1;
 row=static_cast<size_t>(width)*3;
 line=static_cast<size_t>(head.planes)*static_cast<size_t>(head.plane_length);
 uncompressed_length=row*height;
 index=0;
 position=0;
 original=this->create_buffer(length);
 uncompressed=this->create_buffer(uncompressed_length);
 target.read(original,length);
 target.close();
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
   index=static_cast<size_t>(x)*3+static_cast<size_t>(y)*row;
   position=static_cast<size_t>(x)+static_cast<size_t>(y)*line;
   original[index]=uncompressed[position+2*static_cast<size_t>(head.plane_length)];
   original[index+1]=uncompressed[position+static_cast<size_t>(head.plane_length)];
   original[index+2]=uncompressed[position];
  }

 }
 free(uncompressed);
 data=original;
}

unsigned long int Image::get_width() const
{
 return width;
}

unsigned long int Image::get_height() const
{
 return height;
}

size_t Image::get_length() const
{
 return static_cast<size_t>(width)*static_cast<size_t>(height)*3;
}

unsigned char *Image::get_data()
{
 return data;
}

void Image::destroy_image()
{
 width=0;
 height=0;
 this->clear_buffer();
}

Surface::Surface()
{
 width=0;
 height=0;
 image=NULL;
 surface=NULL;
}

Surface::~Surface()
{
 surface=NULL;
 if(image!=NULL) free(image);
}

IMG_Pixel *Surface::create_buffer(const unsigned long int image_width,const unsigned long int image_height)
{
 IMG_Pixel *result;
 size_t length;
 length=static_cast<size_t>(image_width)*static_cast<size_t>(image_height);
 result=reinterpret_cast<IMG_Pixel*>(calloc(length,3));
 if(result==NULL)
 {
  Halt("Can't allocate memory for image buffer");
 }
 return result;
}

void Surface::save()
{
 surface->save();
}

void Surface::restore()
{
 surface->restore();
}

void Surface::clear_buffer()
{
 if(image!=NULL)
 {
  free(image);
  image=NULL;
 }

}

void Surface::load_from_buffer(Image &buffer)
{
 width=buffer.get_width();
 height=buffer.get_height();
 this->clear_buffer();
 image=this->create_buffer(width,height);
 memmove(image,buffer.get_data(),buffer.get_length());
}

void Surface::set_width(const unsigned long int image_width)
{
 width=image_width;
}

void Surface::set_height(const unsigned long int image_height)
{
 height=image_height;
}

void Surface::set_buffer(IMG_Pixel *buffer)
{
 image=buffer;
}

size_t Surface::get_offset(const unsigned long int start,const unsigned long int x,const unsigned long int y,const unsigned long int target_width)
{
 return static_cast<size_t>(start)+static_cast<size_t>(x)+static_cast<size_t>(y)*static_cast<size_t>(target_width);
}

size_t Surface::get_offset(const unsigned long int start,const unsigned long int x,const unsigned long int y)
{
 return this->get_offset(start,x,y,width);
}

void Surface::draw_image_pixel(const size_t offset,const unsigned long int x,const unsigned long int y)
{
 surface->draw_pixel(x,y,image[offset].red,image[offset].green,image[offset].blue);
}

bool Surface::compare_pixels(const size_t first,const size_t second) const
{
 bool result;
 result=false;
 if ((image[first].red!=image[second].red)||(image[first].green!=image[second].green))
 {
  result=true;
 }
 else
 {
  if(image[first].blue!=image[second].blue) result=true;
 }
 return result;
}

void Surface::initialize(Screen *screen)
{
 surface=screen;
}

size_t Surface::get_length() const
{
 return static_cast<size_t>(width)*static_cast<size_t>(height)*3;
}

IMG_Pixel *Surface::get_image()
{
 return image;
}

unsigned long int Surface::get_image_width() const
{
 return width;
}

unsigned long int Surface::get_image_height() const
{
 return height;
}

void Surface::mirror_image(const MIRROR_TYPE kind)
{
 unsigned long int x,y;
 size_t index,index2;
 IMG_Pixel *mirrored_image;
 mirrored_image=this->create_buffer(width,height);
 if (kind==MIRROR_HORIZONTAL)
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
 if(kind==MIRROR_VERTICAL)
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

void Surface::resize_image(const unsigned long int new_width,const unsigned long int new_height)
{
 float x_ratio,y_ratio;
 unsigned long int x,y;
 size_t index,position;
 IMG_Pixel *scaled_image;
 scaled_image=this->create_buffer(new_width,new_height);
 x_ratio=static_cast<float>(width)/static_cast<float>(new_width);
 y_ratio=static_cast<float>(height)/static_cast<float>(new_height);
 for (x=0;x<new_width;++x)
 {
  for (y=0;y<new_height;++y)
  {
   index=this->get_offset(0,x,y,new_width);
   position=this->get_offset(0,(x_ratio*static_cast<float>(x)),(y_ratio*static_cast<float>(y)),width);
   scaled_image[index]=image[position];
  }

 }
 free(image);
 image=scaled_image;
 width=new_width;
 height=new_height;
}

Canvas::Canvas()
{
 start=0;
 frame=1;
 frames=1;
}

Canvas::~Canvas()
{

}

void Canvas::set_frame(const unsigned long int target)
{
 if (target>0)
 {
  if (target<=frames) frame=target;
 }

}

void Canvas::increase_frame()
{
 ++frame;
 if (frame>frames)
 {
  frame=1;
 }

}

void Canvas::set_frames(const unsigned long int amount)
{
 if(amount>1) frames=amount;
}

unsigned long int Canvas::get_frames() const
{
 return frames;
}

unsigned long int Canvas::get_frame() const
{
 return frame;
}

void Canvas::load_image(Image &buffer)
{
 this->load_from_buffer(buffer);
}

Background::Background()
{
 background_width=0;
 background_height=0;
 current=0;
 current_kind=NORMAL_BACKGROUND;
}

Background::~Background()
{

}

void Background::slow_draw_background()
{
 unsigned long int x,y;
 for(x=0;x<background_width;++x)
 {
  for(y=0;y<background_height;++y)
  {
   this->draw_image_pixel(this->get_offset(start,x,y),x,y);
  }

 }

}

unsigned long int Background::get_width() const
{
 return background_width;
}

unsigned long int Background::get_height() const
{
 return background_height;
}

void Background::set_kind(const BACKGROUND_TYPE kind)
{
 switch(kind)
 {
  case NORMAL_BACKGROUND:
  background_width=this->get_image_width();
  background_height=this->get_image_height();
  start=0;
  break;
  case HORIZONTAL_BACKGROUND:
  background_width=this->get_image_width()/this->get_frames();
  background_height=this->get_image_height();
  start=(this->get_frame()-1)*background_width;
  break;
  case VERTICAL_BACKGROUND:
  background_width=this->get_image_width();
  background_height=this->get_image_height()/this->get_frames();
  start=(this->get_frame()-1)*background_width*background_height;
  break;
 }
 current_kind=kind;
}

void Background::set_target(const unsigned long int target)
{
 this->set_frame(target);
 this->set_kind(current_kind);
}

void Background::step()
{
 this->increase_frame();
 this->set_kind(current_kind);
}

void Background::draw_background()
{
 if (current!=this->get_frame())
 {
  this->slow_draw_background();
  this->save();
  current=this->get_frame();
 }
 else
 {
  this->restore();
 }

}

Sprite::Sprite()
{
 transparent=true;
 current_x=0;
 current_y=0;
 sprite_width=0;
 sprite_height=0;
 current_kind=SINGLE_SPRITE;
}

Sprite::~Sprite()
{

}

void Sprite::draw_sprite_pixel(const size_t offset,const unsigned long int x,const unsigned long int y)
{
 if (this->compare_pixels(0,offset)==true)
 {
  this->draw_image_pixel(offset,x,y);
 }

}

void Sprite::draw_transparent_sprite()
{
 size_t offset;
 unsigned long int sprite_x,sprite_y;
 for (sprite_x=0;sprite_x<sprite_width;++sprite_x)
 {
  for (sprite_y=0;sprite_y<sprite_height;++sprite_y)
  {
   offset=this->get_offset(start,sprite_x,sprite_y);
   this->draw_sprite_pixel(offset,current_x+sprite_x,current_y+sprite_y);
  }

 }

}

void Sprite::draw_normal_sprite()
{
 size_t offset;
 unsigned long int sprite_x,sprite_y;
 for (sprite_x=0;sprite_x<sprite_width;++sprite_x)
 {
  for (sprite_y=0;sprite_y<sprite_height;++sprite_y)
  {
   offset=this->get_offset(start,sprite_x,sprite_y);
   this->draw_image_pixel(offset,current_x+sprite_x,current_y+sprite_y);
  }

 }

}

void Sprite::load_sprite(Image &buffer,const SPRITE_TYPE kind,const unsigned long int frames)
{
 this->load_image(buffer);
 if (kind!=SINGLE_SPRITE) this->set_frames(frames);
 this->set_kind(kind);
}

void Sprite::set_transparent(const bool enabled)
{
 transparent=enabled;
}

bool Sprite::get_transparent() const
{
 return transparent;
}

void Sprite::set_x(const unsigned long int x)
{
 current_x=x;
}

void Sprite::set_y(const unsigned long int y)
{
 current_y=y;
}

unsigned long int Sprite::get_x() const
{
 return current_x;
}

unsigned long int Sprite::get_y() const
{
 return current_y;
}

unsigned long int Sprite::get_width() const
{
 return sprite_width;
}

unsigned long int Sprite::get_height() const
{
 return sprite_height;
}

Sprite* Sprite::get_handle()
{
 return this;
}

Collision_Box Sprite::get_box() const
{
 Collision_Box target;
 target.x=current_x;
 target.y=current_y;
 target.width=sprite_width;
 target.height=sprite_height;
 return target;
}

void Sprite::set_kind(const SPRITE_TYPE kind)
{
 switch(kind)
 {
  case SINGLE_SPRITE:
  sprite_width=this->get_image_width();
  sprite_height=this->get_image_height();
  start=0;
  break;
  case HORIZONTAL_STRIP:
  sprite_width=this->get_image_width()/this->get_frames();
  sprite_height=this->get_image_height();
  start=(this->get_frame()-1)*sprite_width;
  break;
  case VERTICAL_STRIP:
  sprite_width=this->get_image_width();
  sprite_height=this->get_image_height()/this->get_frames();
  start=(this->get_frame()-1)*sprite_width*sprite_height;
  break;
 }
 current_kind=kind;
}

SPRITE_TYPE Sprite::get_kind() const
{
 return current_kind;
}

void Sprite::set_target(const unsigned long int target)
{
 this->set_frame(target);
 this->set_kind(current_kind);
}

void Sprite::step()
{
 this->increase_frame();
 this->set_kind(current_kind);
}

void Sprite::set_position(const unsigned long int x,const unsigned long int y)
{
 current_x=x;
 current_y=y;
}

void Sprite::clone(Sprite &target)
{
 this->set_width(target.get_image_width());
 this->set_height(target.get_image_height());
 this->set_frames(target.get_frames());
 this->set_kind(target.get_kind());
 this->set_transparent(target.get_transparent());
 this->set_buffer(this->create_buffer(target.get_image_width(),target.get_image_width()));
 memmove(this->get_image(),target.get_image(),target.get_length());
}

void Sprite::draw_sprite()
{
 if (transparent==true)
 {
  this->draw_transparent_sprite();
 }
 else
 {
  this->draw_normal_sprite();
 }

}

void Sprite::draw_sprite(const unsigned long int x,const unsigned long int y)
{
 this->set_position(x,y);
 this->draw_sprite();
}

Tileset::Tileset()
{
 offset=0;
 rows=0;
 columns=0;
 tile_width=0;
 tile_height=0;
}

Tileset::~Tileset()
{

}

unsigned long int Tileset::get_tile_width() const
{
 return tile_width;
}

unsigned long int Tileset::get_tile_height() const
{
 return tile_height;
}

unsigned long int Tileset::get_rows() const
{
 return rows;
}

unsigned long int Tileset::get_columns() const
{
 return columns;
}

void Tileset::select_tile(const unsigned long int row,const unsigned long int column)
{
 if ((row<rows)&&(column<columns))
 {
  offset=this->get_offset(0,row*tile_width,column*tile_height);
 }

}

void Tileset::draw_tile(const unsigned long int x,const unsigned long int y)
{
 size_t tile_offset;
 unsigned long int tile_x,tile_y;
 for(tile_x=0;tile_x<tile_width;++tile_x)
 {
  for(tile_y=0;tile_y<tile_height;++tile_y)
  {
   tile_offset=offset+this->get_offset(0,tile_x,tile_y);
   this->draw_image_pixel(tile_offset,x+tile_x,y+tile_y);
  }

 }

}

void Tileset::load_tileset(Image &buffer,const unsigned long int row_amount,const unsigned long int column_amount)
{
 if ((row_amount>0)&&(column_amount>0))
 {
  this->load_from_buffer(buffer);
  rows=row_amount;
  columns=column_amount;
  tile_width=this->get_image_width()/rows;
  tile_height=this->get_image_height()/columns;
 }

}

Text::Text()
{
 current_x=0;
 current_y=0;
 font=NULL;
}

Text::~Text()
{
 font=NULL;
}

void Text::increase_position()
{
 unsigned long int x;
 x=font->get_x()+font->get_width();
 font->set_x(x);
}

void Text::set_position(const unsigned long int x,const unsigned long int y)
{
 font->set_position(x,y);
 current_x=x;
 current_y=y;
}

void Text::load_font(Sprite *target)
{
 font=target;
 font->set_frames(256);
 font->set_kind(HORIZONTAL_STRIP);
}

void Text::draw_character(const char target)
{
 font->set_target(static_cast<unsigned char>(target)+1);
 font->draw_sprite();
}

void Text::draw_text(const char *text)
{
 size_t index,length;
 length=strlen(text);
 font->set_position(current_x,current_y);
 for (index=0;index<length;++index)
 {
  this->draw_character(text[index]);
  this->increase_position();
 }
 font->set_position(current_x,current_y);
}

void Text::draw_character(const unsigned long int x,const unsigned long int y,const char target)
{
 this->set_position(x,y);
 this->draw_character(target);
}

void Text::draw_text(const unsigned long int x,const unsigned long int y,const char *text)
{
 this->set_position(x,y);
 this->draw_text(text);
}

Transformation::Transformation()
{
 screen_x_factor=0;
 screen_y_factor=0;
 surface_x_factor=0;
 surface_y_factor=0;
}

Transformation::~Transformation()
{

}

void Transformation::initialize(const float screen_width,const float screen_height,const float surface_width,const float surface_height)
{
 screen_x_factor=screen_width/surface_width;
 screen_y_factor=screen_height/surface_height;
 surface_x_factor=surface_width/screen_width;
 surface_y_factor=surface_height/screen_height;
}

float Transformation::get_screen_x(const float surface_x) const
{
 return screen_x_factor*surface_x;
}

float Transformation::get_screen_y(const float surface_y) const
{
 return screen_y_factor*surface_y;
}

float Transformation::get_surface_x(const float screen_x) const
{
 return surface_x_factor*screen_x;
}

float Transformation::get_surface_y(const float screen_y) const
{
 return surface_y_factor*screen_y;
}

Collision::Collision()
{
 memset(&first,0,sizeof(Collision_Box));
 memset(&second,0,sizeof(Collision_Box));
}

Collision::~Collision()
{

}

void Collision::set_target(const Collision_Box &first_target,const Collision_Box &second_target)
{
 first=first_target;
 second=second_target;
}

bool Collision::check_horizontal_collision() const
{
 bool result;
 result=false;
 if ((first.x+first.width)>=second.x)
 {
  if (first.x<=(second.x+second.width)) result=true;
 }
 return result;
}

bool Collision::check_vertical_collision() const
{
 bool result;
 result=false;
 if ((first.y+first.height)>=second.y)
 {
  if (first.y<=(second.y+second.height)) result=true;
 }
 return result;
}

bool Collision::check_collision() const
{
 return this->check_horizontal_collision() || this->check_vertical_collision();
}

Collision_Box Collision::generate_box(const unsigned long int x,const unsigned long int y,const unsigned long int width,const unsigned long int height) const
{
 Collision_Box result;
 result.x=x;
 result.y=y;
 result.width=width;
 result.height=height;
 return result;
}

}