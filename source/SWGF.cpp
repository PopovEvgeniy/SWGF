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

#include "SWGF.h"

const size_t KEYBOARD=256;
const unsigned char KEY_RELEASE=0;
const unsigned char KEY_PRESS=1;
const unsigned long int JOYSTICK_UPLEFT=31500;
const unsigned long int JOYSTICK_UPRIGHT=4500;
const unsigned long int JOYSTICK_DOWNLEFT=22500;
const unsigned long int JOYSTICK_DOWNRIGHT=13500;

unsigned char Keys[KEYBOARD]={KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE};
unsigned char Buttons[MOUSE]={KEY_RELEASE,KEY_RELEASE,KEY_RELEASE};

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
 if (status!=S_OK)
 {
  if (status!=S_FALSE)
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
 if (timer!=NULL)
 {
  CancelWaitableTimer(timer);
  CloseHandle(timer);
  timer=NULL;
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
 if (SetWaitableTimer(timer,&start,interval,NULL,NULL,FALSE)==FALSE)
 {
  Halt("Can't set timer");
 }

}

void Synchronization::wait_timer()
{
 if (timer!=NULL)
 {
  WaitForSingleObjectEx(timer,INFINITE,TRUE);
 }

}

Engine::Engine()
{
 window_class.lpszClassName=TEXT("SWGF");
 window_class.style=CS_OWNDC;
 window_class.cbSize=sizeof(WNDCLASSEX);
 window_class.lpfnWndProc=Process_Message;
 window_class.hInstance=NULL;
 window_class.hbrBackground=NULL;
 window_class.hIcon=NULL;
 window_class.hIconSm=NULL;
 window_class.hCursor=NULL;
 window_class.cbClsExtra=0;
 window_class.cbWndExtra=0;
 window=NULL;
 context=NULL;
}

Engine::~Engine()
{
 if (context!=NULL)
 {
  ReleaseDC(window,context);
  context=NULL;
 }
 if (window!=NULL)
 {
  CloseWindow(window);
  window=NULL;
 }
 if (window_class.hbrBackground!=NULL)
 {
  DeleteObject(window_class.hbrBackground);
  window_class.hbrBackground=NULL;
 }
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

HDC Engine::get_context()
{
 return context;
}

void Engine::prepare_engine()
{
 this->get_instance();
 this->set_backgrond_color();
 this->load_icon();
 this->load_cursor();
 this->register_window_class();
}

void Engine::take_context()
{
 context=GetWindowDC(window);
 if (context==NULL)
 {
  Halt("Can't take render context");
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

bool Engine::process_message()
{
 bool run;
 MSG Message;
 run=true;
 while(PeekMessage(&Message,window,0,0,PM_NOREMOVE)==TRUE)
 {
  if (GetMessage(&Message,window,0,0)==TRUE)
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

unsigned int Engine::get_width()
{
 return GetSystemMetrics(SM_CXSCREEN);
}

unsigned int Engine::get_height()
{
 return GetSystemMetrics(SM_CYSCREEN);
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
 if (buffer!=NULL)
 {
  delete[] buffer;
  buffer=NULL;
 }
 if (shadow!=NULL)
 {
  delete[] shadow;
  shadow=NULL;
 }

}

void Frame::calculate_buffer_length()
{
 pixels=static_cast<size_t>(frame_width)*static_cast<size_t>(frame_height);
}

unsigned int *Frame::get_memory(const char *error)
{
 unsigned int *target;
 target=NULL;
 try
 {
  target=new unsigned int[pixels];
 }
 catch (...)
 {
  Halt(error);
 }
 return target;
}

void Frame::clear_buffer(unsigned int *target)
{
 size_t index;
 for (index=0;index<pixels;++index)
 {
  target[index]=0;
 }

}

unsigned int *Frame::create_buffer(const char *error)
{
 unsigned int *target;
 target=NULL;
 this->calculate_buffer_length();
 target=this->get_memory(error);
 this->clear_buffer(target);
 return target;
}

size_t Frame::get_offset(const unsigned int x,const unsigned int y,const unsigned int target_width)
{
 return static_cast<size_t>(x)+static_cast<size_t>(y)*static_cast<size_t>(target_width);
}

size_t Frame::get_offset(const unsigned int x,const unsigned int y) const
{
 return static_cast<size_t>(x)+static_cast<size_t>(y)*static_cast<size_t>(frame_width);
}

void Frame::set_size(const unsigned int surface_width,const unsigned int surface_height)
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

void Frame::draw_pixel(const unsigned int x,const unsigned int y,const unsigned int red,const unsigned int green,const unsigned int blue)
{
 size_t offset;
 offset=static_cast<size_t>(x)+static_cast<size_t>(y)*static_cast<size_t>(frame_width);
 if (offset<pixels)
 {
  if (buffer!=NULL)
  {
   buffer[offset]=blue+(green<<8)+(red<<16);
  }

 }

}

void Frame::clear_screen()
{
 this->clear_buffer(buffer);
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

unsigned int Frame::get_frame_width() const
{
 return frame_width;
}

unsigned int Frame::get_frame_height() const
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

void Plane::create_plane(const unsigned int width,const unsigned int height,const unsigned int surface_width,const unsigned int surface_height,unsigned int *surface_buffer)
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
 unsigned int x,y,width,steps;
 size_t index,location,position;
 width=this->get_frame_width();
 x=0;
 y=0;
 steps=target_width*target_height;
 for (index=0;index<steps;++index)
 {
  location=this->get_offset(x,y,target_width);
  position=this->get_offset((x_ratio*static_cast<float>(x)),(y_ratio*static_cast<float>(y)),width);
  target[location]=plane[position];
  ++x;
  if (x==target_width)
  {
   x=0;
   ++y;
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

void Timer::set_timer(const double seconds)
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
 start=time(NULL);
 current=0;
 fps=0;
}

FPS::~FPS()
{

}

void FPS::update_counter()
{
 ++current;
 if (difftime(time(NULL),start)>=1)
 {
  fps=current;
  current=0;
  start=time(NULL);
 }

}

unsigned int FPS::get_fps() const
{
 return fps;
}

Unicode_Convertor::Unicode_Convertor()
{
 target=NULL;
}

Unicode_Convertor::~Unicode_Convertor()
{
 if (target!=NULL)
 {
  delete[] target;
  target=NULL;
 }

}

void Unicode_Convertor::get_memory(const size_t length)
{
 try
 {
  target=new wchar_t[length+1];
 }
 catch (...)
 {
  Halt("Can't allocate memory");
 }

}

void Unicode_Convertor::clear_buffer(const size_t length)
{
 size_t index,stop;
 stop=length+1;
 for (index=0;index<stop;++index)
 {
  target[index]=0;
 }

}

void Unicode_Convertor::create_buffer(const size_t length)
{
 this->get_memory(length);
 this->clear_buffer(length);
}

void Unicode_Convertor::convert_string(const char *source)
{
 size_t index,length;
 length=strlen(source);
 for (index=0;index<length;++index)
 {
  target[index]=btowc(source[index]);
 }

}

wchar_t *Unicode_Convertor::convert(const char *source)
{
 this->create_buffer(strlen(source));
 this->convert_string(source);
 return target;
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
 if (display.dmBitsPerPel<16)
 {
  display.dmBitsPerPel=16;
  this->set_video_mode();
 }

}

unsigned long int Display::get_color() const
{
 return display.dmBitsPerPel;
}

WINGL::WINGL()
{
 memset(&setting,0,sizeof(PIXELFORMATDESCRIPTOR));
 render=NULL;
 wglSwapIntervalEXT=NULL;
 setting.nSize=sizeof(PIXELFORMATDESCRIPTOR);
 setting.nVersion=1;
 setting.dwFlags=PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER;
 setting.iPixelType=PFD_TYPE_RGBA;
 setting.iLayerType=PFD_MAIN_PLANE;
 setting.cAlphaBits=CHAR_BIT;
 setting.cDepthBits=16;
}

WINGL::~WINGL()
{
 if (render!=NULL)
 {
  wglMakeCurrent(NULL,NULL);
  wglDeleteContext(render);
 }

}

int WINGL::get_pixel_format()
{
 setting.cColorBits=this->get_color();
 return ChoosePixelFormat(this->get_context(),&setting);
}

void WINGL::set_pixel_format(const int format)
{
 if (format==0)
 {
  Halt("Invalid pixel format");
 }
 DescribePixelFormat(this->get_context(),format,setting.nSize,&setting);
 if (SetPixelFormat(this->get_context(),format,&setting)==FALSE)
 {
  Halt("Can't set pixel format");
 }

}

void WINGL::create_render_context()
{
 render=wglCreateContext(this->get_context());
 if (render==NULL)
 {
  Halt("Can't create render context");
 }
 wglMakeCurrent(this->get_context(),render);
}

void WINGL::set_render()
{
 this->set_pixel_format(this->get_pixel_format());
 this->create_render_context();
}

void WINGL::disable_vsync()
{
 wglSwapIntervalEXT=reinterpret_cast<PFNWGLSWAPINTERVALEXTPROC>(wglGetProcAddress("wglSwapIntervalEXT"));
 if (wglSwapIntervalEXT!=NULL)
 {
  wglSwapIntervalEXT(0);
 }

}

void WINGL::Swap()
{
 SwapBuffers(this->get_context());
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
 point[0].v=1;
 point[1].u=1;
 point[1].v=1;
 point[2].u=1;
 point[2].v=0;
 point[3].u=0;
 point[3].v=0;
 texture=0;
}

Render::~Render()
{
 if (texture!=0)
 {
  glBindTexture(GL_TEXTURE_2D,0);
  glDeleteTextures(1,&texture);
  texture=0;
 }

}

void Render::set_perfomance_setting()
{
 glDisable(GL_ALPHA_TEST);
 glDisable(GL_BLEND);
 glDisable(GL_POINT_SMOOTH);
 glDisable(GL_LINE_SMOOTH);
 glDisable(GL_POLYGON_SMOOTH);
 glDisable(GL_DITHER);
 glDisable(GL_LOGIC_OP);
 glDisable(GL_FOG);
 glDisable(GL_STENCIL_TEST);
 glDisable(GL_SCISSOR_TEST);
 glDisable(GL_LIGHTING);
 glDisable(GL_NORMALIZE);
 glDisable(GL_AUTO_NORMAL);
 glDisable(GL_COLOR_MATERIAL);
 glDisable(GL_TEXTURE_GEN_Q);
 glDisable(GL_TEXTURE_GEN_R);
 glDisable(GL_TEXTURE_GEN_S);
 glDisable(GL_TEXTURE_GEN_T);
 glDisable(GL_TEXTURE_1D);
 glEnable(GL_DEPTH_TEST);
 glEnable(GL_CULL_FACE);
 glEnable(GL_TEXTURE_2D);
 glEnableClientState(GL_VERTEX_ARRAY);
 glEnableClientState(GL_TEXTURE_COORD_ARRAY);
 glDisableClientState(GL_COLOR_ARRAY);
 glDisableClientState(GL_EDGE_FLAG_ARRAY);
 glDisableClientState(GL_INDEX_ARRAY);
 glDisableClientState(GL_NORMAL_ARRAY);
}

void Render::set_common_setting()
{
 glDepthFunc(GL_ALWAYS);
 glDepthMask(GL_TRUE);
 glFrontFace(GL_CCW);
 glCullFace(GL_BACK);
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

void Render::create_texture()
{
 glPixelStorei(GL_UNPACK_ALIGNMENT,1);
 glGenTextures(1,&texture);
 glBindTexture(GL_TEXTURE_2D,texture);
 glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
}

void Render::check_texture()
{
 if (glGetError()!=GL_NO_ERROR)
 {
  Halt("Can't create the target texture");
 }

}

void Render::load_surface_data()
{
 glVertexPointer(2,GL_INT,0,vertex);
 glTexCoordPointer(2,GL_FLOAT,0,point);
}

void Render::draw()
{
 glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,this->get_frame_width(),this->get_frame_height(),0,GL_BGRA_EXT,GL_UNSIGNED_BYTE,this->get_buffer());
 glDrawArrays(GL_TRIANGLE_FAN,0,4);
}

void Render::create_render()
{
 this->set_render();
 this->set_perfomance_setting();
 this->set_common_setting();
 this->set_perspective();
 this->clear_stage();
 this->check_videocard();
 this->create_texture();
 this->check_texture();
 this->set_vertex_coordinates();
 this->load_surface_data();
 this->disable_vsync();
}

void Render::start_render()
{
 this->create_window();
 this->take_context();
 this->create_render();
}

void Render::refresh()
{
 this->draw();
 this->Swap();
}

Screen::Screen()
{
 ready=false;
}

Screen::~Screen()
{

}

void Screen::initialize()
{
 if (ready==false)
 {
  this->check_video_mode();
  this->prepare_engine();
  this->start_render();
  this->create_buffers();
  this->create_timer();
  this->set_timer(17);
  ready=true;
 }

}

void Screen::initialize(const SURFACE surface)
{
 if (ready==false)
 {
  this->set_size(surface);
  this->check_video_mode();
  this->prepare_engine();
  this->start_render();
  this->create_buffers();
  this->create_timer();
  this->set_timer(17);
  ready=true;
 }

}

bool Screen::update()
{
 if (ready==true)
 {
  this->refresh();
  this->clear_screen();
  this->update_counter();
 }
 return this->process_message();
}

bool Screen::sync()
{
 this->wait_timer();
 return this->update();
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
 if (preversion!=NULL)
 {
  delete[] preversion;
  preversion=NULL;
 }

}

void Keyboard::create_buffer()
{
 try
 {
  preversion=new unsigned char[KEYBOARD];
 }
 catch (...)
 {
  Halt("Can't allocate memory for keyboard state buffer");
 }

}

void Keyboard::clear_buffer()
{
 size_t index;
 for (index=0;index<KEYBOARD;++index)
 {
  preversion[index]=KEY_RELEASE;
 }

}

bool Keyboard::check_state(const unsigned char code,const unsigned char state)
{
 bool result;
 result=false;
 if (Keys[code]==state)
 {
  if (preversion[code]!=state) result=true;
 }
 preversion[code]=Keys[code];
 return result;
}

void Keyboard::initialize()
{
 if (preversion==NULL)
 {
  this->create_buffer();
 }
 if (preversion!=NULL)
 {
  this->clear_buffer();
 }

}

bool Keyboard::check_hold(const unsigned char code)
{
 preversion[code]=Keys[code];
 return Keys[code]==KEY_PRESS;
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
 preversion[MOUSE_LEFT]=KEY_RELEASE;
 preversion[MOUSE_RIGHT]=KEY_RELEASE;
 preversion[MOUSE_MIDDLE]=KEY_RELEASE;
 position.x=0;
 position.y=0;
}

Mouse::~Mouse()
{
 while(ShowCursor(TRUE)<1) ;
}

void Mouse::get_position()
{
 if (GetCursorPos(&position)==FALSE)
 {
  position.x=0;
  position.y=0;
 }

}

bool Mouse::check_state(const MOUSE_BUTTON button,const unsigned char state)
{
 bool result;
 result=false;
 if (Buttons[button]==state)
 {
  if (preversion[button]!=state) result=true;
 }
 preversion[button]=Buttons[button];
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

void Mouse::set_position(const unsigned int x,const unsigned int y)
{
 if (SetCursorPos(x,y)==FALSE)
 {
  position.x=0;
  position.y=0;
 }

}

unsigned int Mouse::get_x()
{
 this->get_position();
 return position.x;
}

unsigned int Mouse::get_y()
{
 this->get_position();
 return position.y;
}

bool Mouse::check_hold(const MOUSE_BUTTON button)
{
 preversion[button]=Buttons[button];
 return Buttons[button]==KEY_PRESS;
}

bool Mouse::check_press(const MOUSE_BUTTON button)
{
 return this->check_state(button,KEY_PRESS);
}

bool Mouse::check_release(const MOUSE_BUTTON button)
{
 return this->check_state(button,KEY_RELEASE);
}

Gamepad::Gamepad()
{
 active=0;
 memset(&configuration,0,sizeof(JOYCAPS));
 memset(&current,0,sizeof(JOYINFOEX));
 current.dwSize=sizeof(JOYINFOEX);
 current.dwFlags=JOY_RETURNALL;
 current.dwPOV=JOY_POVCENTERED;
 preversion=current;
}

Gamepad::~Gamepad()
{

}

bool Gamepad::read_configuration()
{
 return joyGetDevCaps(static_cast<size_t>(active),&configuration,sizeof(JOYCAPS))==JOYERR_NOERROR;
}

bool Gamepad::read_state()
{
 return joyGetPosEx(active,&current)==JOYERR_NOERROR;
}

void Gamepad::clear_state()
{
 memset(&configuration,0,sizeof(JOYCAPS));
 memset(&current,0,sizeof(JOYINFOEX));
 current.dwSize=sizeof(JOYINFOEX);
 current.dwFlags=JOY_RETURNALL;
 current.dwPOV=JOY_POVCENTERED;
 preversion=current;
}

bool Gamepad::check_button(const GAMEPAD_BUTTONS button,const JOYINFOEX &target)
{
 bool result;
 result=false;
 if (target.dwButtons&button) result=true;
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
 if (this->read_configuration()==true) result=configuration.wNumButtons;
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
 if (this->read_state()==false) this->clear_state();
}

unsigned long int Gamepad::get_sticks_amount()
{
 unsigned long int result;
 result=0;
 if (this->read_configuration()==true)
 {
  if (configuration.wNumAxes>1) result=configuration.wNumAxes/2;
 }
 return result;
}

void Gamepad::set_active(const unsigned int gamepad)
{
 if (gamepad<this->get_amount())
 {
  this->clear_state();
  active=gamepad;
 }

}

unsigned int Gamepad::get_max_amount() const
{
 return 16;
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
 if (stick==GAMEPAD_LEFT_STICK)
 {
  if (this->get_sticks_amount()>0)
  {
   control=(configuration.wXmax-configuration.wXmin)/2;
   if (current.dwXpos<control) result=GAMEPAD_NEGATIVE_DIRECTION;
   if (current.dwXpos>control) result=GAMEPAD_POSITIVE_DIRECTION;
  }

 }
 if (stick==GAMEPAD_RIGHT_STICK)
 {
  if (this->get_sticks_amount()>1)
  {
   control=(configuration.wZmax-configuration.wZmin)/2;
   if (current.dwZpos<control) result=GAMEPAD_NEGATIVE_DIRECTION;
   if (current.dwZpos>control) result=GAMEPAD_POSITIVE_DIRECTION;
  }

 }
 return result;
}

GAMEPAD_DIRECTION Gamepad::get_stick_y(const GAMEPAD_STICKS stick)
{
 GAMEPAD_DIRECTION result;
 unsigned long int control;
 result=GAMEPAD_NEUTRAL_DIRECTION;
 if (stick==GAMEPAD_LEFT_STICK)
 {
  if (this->get_sticks_amount()>0)
  {
   control=(configuration.wYmax-configuration.wYmin)/2;
   if (current.dwYpos<control) result=GAMEPAD_NEGATIVE_DIRECTION;
   if (current.dwYpos>control) result=GAMEPAD_POSITIVE_DIRECTION;
  }

 }
 if (stick==GAMEPAD_RIGHT_STICK)
 {
  if (this->get_sticks_amount()>1)
  {
   control=(configuration.wRmax-configuration.wRmin)/2;
   if (current.dwRpos<control) result=GAMEPAD_NEGATIVE_DIRECTION;
   if (current.dwRpos>control) result=GAMEPAD_POSITIVE_DIRECTION;
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
 return (this->check_button(button,current)==true) && (this->check_button(button,preversion)==false);
}

bool Gamepad::check_release(const GAMEPAD_BUTTONS button)
{
 return (this->check_button(button,current)==false) && (this->check_button(button,preversion)==true);
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
 if (player!=NULL)
 {
  player->StopWhenReady();
  player->Release();
  player=NULL;
 }
 if (video!=NULL)
 {
  video->Release();
  video=NULL;
 }
 if (controler!=NULL)
 {
  controler->Release();
  controler=NULL;
 }
 if (loader!=NULL)
 {
  loader->Release();
  loader=NULL;
 }

}

void Multimedia::set_screen_mode()
{
 if (video!=NULL)
 {
  video->put_FullScreenMode(OATRUE);
 }

}

void Multimedia::load_content(const wchar_t *target)
{
 if (loader!=NULL)
 {
  loader->RenderFile(target,NULL);
 }

}

void Multimedia::open(const wchar_t *target)
{
 this->load_content(target);
 this->set_screen_mode();
}

bool Multimedia::is_play()
{
 long long current,total;
 current=0;
 total=0;
 if (controler!=NULL)
 {
  if (controler->GetPositions(&current,&total)!=S_OK)
  {
   current=0;
   total=0;
  }

 }
 return current<total;
}

void Multimedia::rewind()
{
 long long position;
 position=0;
 if (controler!=NULL)
 {
  controler->SetPositions(&position,AM_SEEKING_AbsolutePositioning,NULL,AM_SEEKING_NoPositioning);
 }

}

void Multimedia::play_content()
{
 if (player!=NULL)
 {
  player->Run();
 }

}

void Multimedia::create_loader()
{
 if (loader==NULL)
 {
  if (CoCreateInstance(CLSID_FilterGraph,NULL,CLSCTX_INPROC_SERVER,IID_IGraphBuilder,reinterpret_cast<void**>(&loader))!=S_OK)
  {
   loader=NULL;
   Halt("Can't create a multimedia loader");
  }

 }

}

void Multimedia::create_player()
{
 if (player==NULL)
 {
  if (loader->QueryInterface(IID_IMediaControl,reinterpret_cast<void**>(&player))!=S_OK)
  {
   player=NULL;
   Halt("Can't create a multimedia player");
  }

 }

}

void Multimedia::create_controler()
{
 if (controler==NULL)
 {
  if (loader->QueryInterface(IID_IMediaSeeking,reinterpret_cast<void**>(&controler))!=S_OK)
  {
   controler=NULL;
   Halt("Can't create a player controler");
  }

 }

}

void Multimedia::create_video_player()
{
 if (video==NULL)
 {
  if (loader->QueryInterface(IID_IVideoWindow,reinterpret_cast<void**>(&video))!=S_OK)
  {
   video=NULL;
   Halt("Can't create a video player");
  }

 }

}

void Multimedia::initialize()
{
 this->create_loader();
 this->create_player();
 this->create_controler();
 this->create_video_player();
}

bool Multimedia::check_playing()
{
 OAFilterState state;
 bool result;
 result=false;
 if (player!=NULL)
 {
  if (player->GetState(INFINITE,&state)!=E_FAIL)
  {
   if (state==State_Running) result=this->is_play();
  }

 }
 return result;
}

void Multimedia::stop()
{
 if (player!=NULL)
 {
  player->StopWhenReady();
 }

}

void Multimedia::play()
{
 this->stop();
 this->rewind();
 this->play_content();
}

void Multimedia::load(const char *target)
{
 Unicode_Convertor convertor;
 this->stop();
 this->open(convertor.convert(target));
}

void Multimedia::initialize(const char *target)
{
 this->initialize();
 this->load(target);
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
 if (GlobalMemoryStatusEx(&memory)==FALSE)
 {
  memset(&memory,0,sizeof(MEMORYSTATUSEX));
  memory.dwLength=sizeof(MEMORYSTATUSEX);
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

unsigned int System::get_random(const unsigned int number)
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

void System::enable_logging(const char *name)
{
 if (freopen(name,"wt",stdout)==NULL)
 {
  Halt("Can't create log file");
 }

}

Filesystem::Filesystem()
{

}

Filesystem::~Filesystem()
{

}

bool Filesystem::file_exist(const char *name)
{
 FILE *target;
 bool exist;
 exist=false;
 target=fopen(name,"rb");
 if (target!=NULL)
 {
  exist=true;
  fclose(target);
 }
 return exist;
}

bool Filesystem::delete_file(const char *name)
{
 return remove(name)==0;
}

Binary_File::Binary_File()
{
 target=NULL;
}

Binary_File::~Binary_File()
{
 if (target!=NULL)
 {
  fclose(target);
  target=NULL;
 }

}

void Binary_File::close()
{
 if (target!=NULL)
 {
  fclose(target);
  target=NULL;
 }

}

void Binary_File::set_position(const long int offset)
{
 if (target!=NULL)
 {
  fseek(target,offset,SEEK_SET);
 }

}

long int Binary_File::get_position()
{
 long int position;
 position=0;
 if (target!=NULL)
 {
  position=ftell(target);
 }
 return position;
}

long int Binary_File::get_length()
{
 long int length;
 length=0;
 if (target!=NULL)
 {
  fseek(target,0,SEEK_END);
  length=ftell(target);
  rewind(target);
 }
 return length;
}

bool Binary_File::check_error()
{
 int error;
 error=-1;
 if (target!=NULL)
 {
  error=ferror(target);
 }
 return error!=0;
}

bool Binary_File::is_open() const
{
 return target!=NULL;
}

Input_File::Input_File()
{

}

Input_File::~Input_File()
{

}

void Input_File::open(const char *name)
{
 this->close();
 target=fopen(name,"rb");
}

void Input_File::read(void *buffer,const size_t length)
{
 if (target!=NULL)
 {
  if (buffer!=NULL)
  {
   fread(buffer,sizeof(char),length,target);
  }

 }

}

Output_File::Output_File()
{

}

Output_File::~Output_File()
{

}

void Output_File::open(const char *name)
{
 this->close();
 target=fopen(name,"wb");
}

void Output_File::create_temp()
{
 this->close();
 target=tmpfile();
}

void Output_File::write(void *buffer,const size_t length)
{
 if (target!=NULL)
 {
  if (buffer!=NULL)
  {
   fwrite(buffer,sizeof(char),length,target);
  }

 }

}

void Output_File::flush()
{
 if (target!=NULL)
 {
  fflush(target);
 }

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

}

void Primitive::initialize(Screen *screen)
{
 surface=screen;
}

void Primitive::set_color(const unsigned char red,const unsigned char green,const unsigned char blue)
{
 color.red=red;
 color.green=green;
 color.blue=blue;
}

void Primitive::draw_line(const unsigned int x1,const unsigned int y1,const unsigned int x2,const unsigned int y2)
{
 unsigned int delta_x,delta_y,index,steps;
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
  if (surface!=NULL)
  {
   surface->draw_pixel(x,y,color.red,color.green,color.blue);
  }

 }

}

void Primitive::draw_rectangle(const unsigned int x,const unsigned int y,const unsigned int width,const unsigned int height)
{
 unsigned int stop_x,stop_y;
 stop_x=x+width;
 stop_y=y+height;
 this->draw_line(x,y,stop_x,y);
 this->draw_line(x,stop_y,stop_x,stop_y);
 this->draw_line(x,y,x,stop_y);
 this->draw_line(stop_x,y,stop_x,stop_y);
}

void Primitive::draw_filled_rectangle(const unsigned int x,const unsigned int y,const unsigned int width,const unsigned int height)
{
 unsigned int step_x,step_y,stop_x,stop_y;
 stop_x=x+width;
 stop_y=y+height;
 for (step_x=x;step_x<stop_x;++step_x)
 {
  for (step_y=y;step_y<stop_y;++step_y)
  {
   if (surface!=NULL)
   {
    surface->draw_pixel(step_x,step_y,color.red,color.green,color.blue);
   }

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
 if (data!=NULL)
 {
  delete[] data;
  data=NULL;
 }

}

unsigned char *Image::create_buffer(const size_t length)
{
 unsigned char *result;
 result=NULL;
 try
 {
  result=new unsigned char[length];
 }
 catch (...)
 {
  Halt("Can't allocate memory for image buffer");
 }
 return result;
}

void Image::load_tga(Input_File &target)
{
 size_t index,position,amount,compressed_length,uncompressed_length;
 unsigned char *compressed;
 unsigned char *uncompressed;
 TGA_head head;
 TGA_map color_map;
 TGA_image image;
 compressed_length=static_cast<size_t>(target.get_length()-18);
 target.read(&head,3);
 target.read(&color_map,5);
 target.read(&image,10);
 if ((head.color_map!=0)||(image.color!=24))
 {
  Halt("Invalid image format");
 }
 if (head.type!=2)
 {
  if (head.type!=10)
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
 if (head.type==2)
 {
  target.read(uncompressed,uncompressed_length);
 }
 if (head.type==10)
 {
  compressed=this->create_buffer(compressed_length);
  target.read(compressed,compressed_length);
  while(index<uncompressed_length)
  {
   if (compressed[position]<128)
   {
    amount=compressed[position]+1;
    amount*=3;
    memmove(uncompressed+index,compressed+(position+1),amount);
    index+=amount;
    position+=1+amount;
   }
   else
   {
    for (amount=compressed[position]-127;amount>0;--amount)
    {
     memmove(uncompressed+index,compressed+(position+1),3);
     index+=3;
    }
    position+=4;
   }

  }
  delete[] compressed;
 }
 data=uncompressed;
}

void Image::load_pcx(Input_File &target)
{
 unsigned int x,y;
 size_t index,position,line,row,length,uncompressed_length;
 unsigned char repeat;
 unsigned char *original;
 unsigned char *uncompressed;
 PCX_head head;
 length=static_cast<size_t>(target.get_length()-128);
 target.read(&head,128);
 if ((head.color*head.planes!=24)&&(head.compress!=1))
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
 while (index<length)
 {
  if (original[index]<192)
  {
   uncompressed[position]=original[index];
   ++position;
   ++index;
  }
  else
  {
   for (repeat=original[index]-192;repeat>0;--repeat)
   {
    uncompressed[position]=original[index+1];
    ++position;
   }
   index+=2;
  }

 }
 delete[] original;
 original=this->create_buffer(uncompressed_length);
 for (x=0;x<width;++x)
 {
  for (y=0;y<height;++y)
  {
   index=static_cast<size_t>(x)*3+static_cast<size_t>(y)*row;
   position=static_cast<size_t>(x)+static_cast<size_t>(y)*line;
   original[index]=uncompressed[position+2*static_cast<size_t>(head.plane_length)];
   original[index+1]=uncompressed[position+static_cast<size_t>(head.plane_length)];
   original[index+2]=uncompressed[position];
  }

 }
 delete[] uncompressed;
 data=original;
}

unsigned int Image::get_width() const
{
 return width;
}

unsigned int Image::get_height() const
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
 if (data!=NULL)
 {
  delete[] data;
  data=NULL;
  width=0;
  height=0;
 }

}

void Image::load_tga(const char *name)
{
 Input_File target;
 target.open(name);
 this->destroy_image();
 if (target.is_open()==true)
 {
  this->load_tga(target);
  target.close();
 }

}

void Image::load_pcx(const char *name)
{
 Input_File target;
 target.open(name);
 this->destroy_image();
 if (target.is_open()==true)
 {
  this->load_pcx(target);
  target.close();
 }

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
 if (image!=NULL)
 {
  free(image);
  image=NULL;
  width=0;
  height=0;
 }

}

IMG_Pixel *Surface::create_buffer(const unsigned int image_width,const unsigned int image_height)
{
 IMG_Pixel *result;
 size_t length;
 length=static_cast<size_t>(image_width)*static_cast<size_t>(image_height);
 result=reinterpret_cast<IMG_Pixel*>(calloc(length,3));
 if (result==NULL)
 {
  Halt("Can't allocate memory for image buffer");
 }
 return result;
}

void Surface::save()
{
 if (surface!=NULL)
 {
  surface->save();
 }

}

void Surface::restore()
{
 if (surface!=NULL)
 {
  surface->restore();
 }

}

void Surface::clear_buffer()
{
 if (image!=NULL)
 {
  free(image);
  image=NULL;
  width=0;
  height=0;
 }

}

void Surface::set_size(const unsigned int image_width,const unsigned int image_height)
{
 width=image_width;
 height=image_height;
}

void Surface::set_buffer(IMG_Pixel *buffer)
{
 image=buffer;
}

size_t Surface::get_offset(const unsigned int start,const unsigned int x,const unsigned int y,const unsigned int target_width)
{
 return static_cast<size_t>(start)+static_cast<size_t>(x)+static_cast<size_t>(y)*static_cast<size_t>(target_width);
}

size_t Surface::get_offset(const unsigned int start,const unsigned int x,const unsigned int y) const
{
 return static_cast<size_t>(start)+static_cast<size_t>(x)+static_cast<size_t>(y)*static_cast<size_t>(width);
}

void Surface::draw_image_pixel(const size_t offset,const unsigned int x,const unsigned int y)
{
 if (surface!=NULL)
 {
  surface->draw_pixel(x,y,image[offset].red,image[offset].green,image[offset].blue);
 }

}

bool Surface::compare_pixels(const size_t first,const size_t second) const
{
 bool result;
 result=false;
 if (image[first].red!=image[second].red)
 {
  result=true;
  goto finish;
 }
 if (image[first].green!=image[second].green)
 {
  result=true;
  goto finish;
 }
 if (image[first].blue!=image[second].blue)
 {
  result=true;
  goto finish;
 }
 finish: ;
 return result;
}

unsigned int Surface::get_surface_width() const
{
 return surface->get_frame_width();
}

unsigned int Surface::get_surface_height() const
{
 return surface->get_frame_width();
}

void Surface::do_mirror_image(const MIRROR_TYPE kind)
{
 unsigned int x,y,index;
 IMG_Pixel *mirrored_image;
 x=0;
 y=0;
 mirrored_image=this->create_buffer(width,height);
 if (kind==MIRROR_HORIZONTAL)
 {
  for (index=width*height;index>0;--index)
  {
   mirrored_image[this->get_offset(0,x,y)]=image[this->get_offset(0,(width-x-1),y)];
   ++x;
   if (x==width)
   {
    x=0;
    ++y;
   }

  }

 }
 if (kind==MIRROR_VERTICAL)
 {
  for (index=width*height;index>0;--index )
  {
   mirrored_image[this->get_offset(0,x,y)]=image[this->get_offset(0,x,(height-y-1))];
   ++x;
   if (x==width)
   {
    x=0;
    ++y;
   }

  }

 }
 free(image);
 image=mirrored_image;
}

void Surface::do_resize_image(const unsigned int new_width,const unsigned int new_height)
{
 float x_ratio,y_ratio;
 unsigned int x,y,steps;
 size_t index,location,position;
 IMG_Pixel *scaled_image;
 x=0;
 y=0;
 steps=new_width*new_height;
 scaled_image=this->create_buffer(new_width,new_height);
 x_ratio=static_cast<float>(width)/static_cast<float>(new_width);
 y_ratio=static_cast<float>(height)/static_cast<float>(new_height);
 for (index=0;index<steps;++index)
 {
  location=this->get_offset(0,x,y,new_width);
  position=this->get_offset(0,(x_ratio*static_cast<float>(x)),(y_ratio*static_cast<float>(y)),width);
  scaled_image[location]=image[position];
  ++x;
  if (x==new_width)
  {
   x=0;
   ++y;
  }

 }
 free(image);
 image=scaled_image;
 width=new_width;
 height=new_height;
}

void Surface::initialize(Screen *screen)
{
 surface=screen;
}

size_t Surface::get_length() const
{
 return static_cast<size_t>(width)*static_cast<size_t>(height)*3;
}

bool Surface::is_surface_empty() const
{
 return image==NULL;
}

IMG_Pixel *Surface::get_image()
{
 return image;
}

void Surface::load_image(Image &buffer)
{
 this->clear_buffer();
 if (buffer.get_length()>0)
 {
  width=buffer.get_width();
  height=buffer.get_height();
  this->clear_buffer();
  image=this->create_buffer(width,height);
  memmove(image,buffer.get_data(),buffer.get_length());
 }

}

unsigned int Surface::get_image_width() const
{
 return width;
}

unsigned int Surface::get_image_height() const
{
 return height;
}

void Surface::mirror_image(const MIRROR_TYPE kind)
{
 if (this->is_surface_empty()==false)
 {
  this->do_mirror_image(kind);
 }

}

void Surface::resize_image(const unsigned int new_width,const unsigned int new_height)
{
 if (this->is_surface_empty()==false)
 {
  this->do_resize_image(new_width,new_height);
 }

}

void Surface::horizontal_mirror()
{
 this->mirror_image(MIRROR_HORIZONTAL);
}

void Surface::vertical_mirror()
{
 this->mirror_image(MIRROR_VERTICAL);
}

Animation::Animation()
{
 start=0;
 frame=1;
 frames=1;
}

Animation::~Animation()
{

}

void Animation::set_frame(const unsigned int target)
{
 if (target>0)
 {
  if (target<=frames) frame=target;
 }

}

void Animation::increase_frame()
{
 ++frame;
 if (frame>frames)
 {
  frame=1;
 }

}

void Animation::set_frames(const unsigned int amount)
{
 if (amount>1) frames=amount;
}

unsigned int Animation::get_frames() const
{
 return frames;
}

unsigned int Animation::get_frame() const
{
 return frame;
}

Background::Background()
{
 background_width=0;
 background_height=0;
 maximum_width=0;
 maximum_height=0;
 current=0;
 current_kind=NORMAL_BACKGROUND;
}

Background::~Background()
{

}

void Background::get_maximum_width()
{
 maximum_width=background_width;
 if (maximum_width>this->get_surface_width())
 {
  maximum_width=this->get_surface_width();
 }

}

void Background::get_maximum_height()
{
 maximum_height=background_height;
 if (maximum_height>this->get_surface_height())
 {
  maximum_height=this->get_surface_height();
 }

}

void Background::slow_draw_background()
{
 unsigned int x,y,index;
 x=0;
 y=0;
 for (index=maximum_width*maximum_height;index>0;--index)
 {
  this->draw_image_pixel(this->get_offset(start,x,y),x,y);
  ++x;
  if (x==maximum_width)
  {
   x=0;
   ++y;
  }

 }

}

void Background::background_setup()
{
 switch(current_kind)
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

}

void Background::configure_background()
{
 if (this->is_surface_empty()==false)
 {
  this->background_setup();
 }

}

unsigned int Background::get_width() const
{
 return background_width;
}

unsigned int Background::get_height() const
{
 return background_height;
}

void Background::set_kind(const BACKGROUND_TYPE kind)
{
 current_kind=kind;
 this->configure_background();
 this->get_maximum_width();
 this->get_maximum_height();
}

void Background::set_setting(const BACKGROUND_TYPE kind,const unsigned int frames)
{
 if (kind!=NORMAL_BACKGROUND) this->set_frames(frames);
 this->set_kind(kind);
}

void Background::set_target(const unsigned int target)
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

void Sprite::configure_sprite()
{
 switch(current_kind)
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

}

void Sprite::draw_transparent_sprite()
{
 unsigned int x,y,index;
 x=0;
 y=0;
 for (index=sprite_width*sprite_height;index>0;--index)
 {
  if (this->compare_pixels(0,this->get_offset(start,x,y))==true)
  {
   this->draw_image_pixel(this->get_offset(start,x,y),x+current_x,y+current_y);
  }
  ++x;
  if (x==sprite_width)
  {
   x=0;
   ++y;
  }

 }

}

void Sprite::draw_normal_sprite()
{
 unsigned int x,y,index;
 x=0;
 y=0;
 for (index=sprite_width*sprite_height;index>0;--index)
 {
  this->draw_image_pixel(this->get_offset(start,x,y),x+current_x,y+current_y);
  ++x;
  if (x==sprite_width)
  {
   x=0;
   ++y;
  }

 }

}

void Sprite::set_transparent(const bool enabled)
{
 transparent=enabled;
}

bool Sprite::get_transparent() const
{
 return transparent;
}

void Sprite::set_x(const unsigned int x)
{
 current_x=x;
}

void Sprite::set_y(const unsigned int y)
{
 current_y=y;
}

void Sprite::increase_x()
{
 ++current_x;
}

void Sprite::decrease_x()
{
 --current_x;
}

void Sprite::increase_y()
{
 ++current_y;
}

void Sprite::decrease_y()
{
 --current_y;
}

void Sprite::increase_x(const unsigned int increment)
{
 current_x+=increment;
}

void Sprite::decrease_x(const unsigned int decrement)
{
 current_x-=decrement;
}

void Sprite::increase_y(const unsigned int increment)
{
 current_y+=increment;
}

void Sprite::decrease_y(const unsigned int decrement)
{
 current_y-=decrement;
}

unsigned int Sprite::get_x() const
{
 return current_x;
}

unsigned int Sprite::get_y() const
{
 return current_y;
}

unsigned int Sprite::get_width() const
{
 return sprite_width;
}

unsigned int Sprite::get_height() const
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
 if (this->is_surface_empty()==false)
 {
  current_kind=kind;
  this->configure_sprite();
 }

}

void Sprite::set_setting(const SPRITE_TYPE kind,const unsigned int frames)
{
 if (kind!=SINGLE_SPRITE)
 {
  this->set_frames(frames);
 }
 this->set_kind(kind);
}

SPRITE_TYPE Sprite::get_kind() const
{
 return current_kind;
}

void Sprite::set_target(const unsigned int target)
{
 this->set_frame(target);
 this->set_kind(current_kind);
}

void Sprite::step()
{
 this->increase_frame();
 this->set_kind(current_kind);
}

void Sprite::set_position(const unsigned int x,const unsigned int y)
{
 current_x=x;
 current_y=y;
}

void Sprite::clone(Sprite &target)
{
 if (this->is_surface_empty()==false)
 {
  this->set_size(target.get_image_width(),target.get_image_height());
  this->set_frames(target.get_frames());
  this->set_kind(target.get_kind());
  this->set_transparent(target.get_transparent());
  this->set_buffer(this->create_buffer(target.get_image_width(),target.get_image_width()));
  memmove(this->get_image(),target.get_image(),target.get_length());
 }

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

void Sprite::draw_sprite(const unsigned int x,const unsigned int y)
{
 this->set_position(x,y);
 this->draw_sprite();
}

void Sprite::draw_sprite(const bool transparency)
{
 this->set_transparent(transparency);
 this->draw_sprite();
}

void Sprite::draw_sprite(const bool transparency,const unsigned int x,const unsigned int y)
{
 this->set_transparent(transparency);
 this->draw_sprite(x,y);
}

void Sprite::load_sprite(Image &buffer,const SPRITE_TYPE kind,const unsigned int frames)
{
 this->load_image(buffer);
 if (this->is_surface_empty()==false)
 {
  this->set_setting(kind,frames);
 }

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

void Tileset::set_tileset_setting(const unsigned int row_amount,const unsigned int column_amount)
{
 if (row_amount>0)
 {
  if (column_amount>0)
  {
   rows=row_amount;
   columns=column_amount;
   tile_width=this->get_image_width()/rows;
   tile_height=this->get_image_height()/columns;
  }

 }

}

unsigned int Tileset::get_tile_width() const
{
 return tile_width;
}

unsigned int Tileset::get_tile_height() const
{
 return tile_height;
}

unsigned int Tileset::get_rows() const
{
 return rows;
}

unsigned int Tileset::get_columns() const
{
 return columns;
}

void Tileset::select_tile(const unsigned int row,const unsigned int column)
{
 if (row<rows)
 {
  if (column<columns)
  {
   offset=this->get_offset(0,row*tile_width,column*tile_height);
  }

 }

}

void Tileset::draw_tile(const unsigned int x,const unsigned int y)
{
 unsigned int tile_x,tile_y,index;
 tile_x=0;
 tile_y=0;
 for (index=tile_width*tile_height;index>0;--index)
 {
  this->draw_image_pixel(offset+this->get_offset(0,tile_x,tile_y),x+tile_x,y+tile_y);
  ++tile_x;
  if (tile_x==tile_width)
  {
   tile_x=0;
   ++tile_y;
  }

 }

}

void Tileset::draw_tile(const unsigned int row,const unsigned int column,const unsigned int x,const unsigned int y)
{
 this->select_tile(row,column);
 this->draw_tile(x,y);
}

void Tileset::load_tileset(Image &buffer,const unsigned int row_amount,const unsigned int column_amount)
{
 this->load_image(buffer);
 if (this->is_surface_empty()==false)
 {
  this->set_tileset_setting(row_amount,column_amount);
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

}

void Text::increase_position()
{
 font->increase_x(font->get_width());
}

void Text::restore_position()
{
 font->set_position(current_x,current_y);
}

void Text::print_character(const char target)
{
 font->set_target(static_cast<unsigned char>(target)+1);
 font->draw_sprite();
}

void Text::print_text(const char *text)
{
 size_t index,length;
 length=strlen(text);
 this->restore_position();
 for (index=0;index<length;++index)
 {
  this->print_character(text[index]);
  this->increase_position();
 }

}

void Text::set_position(const unsigned int x,const unsigned int y)
{
 font->set_position(x,y);
 current_x=x;
 current_y=y;
}

void Text::load_font(Sprite *target)
{
 if (target!=NULL)
 {
  font=target;
  font->set_setting(HORIZONTAL_STRIP,256);
 }

}

void Text::load_font(Sprite &target)
{
 this->load_font(target.get_handle());
}

void Text::draw_character(const char target)
{
 if (font!=NULL)
 {
  this->print_character(target);
 }

}

void Text::draw_text(const char *text)
{
 if (font!=NULL)
 {
  this->print_text(text);
 }

}

void Text::draw_character(const unsigned int x,const unsigned int y,const char target)
{
 this->set_position(x,y);
 this->draw_character(target);
}

void Text::draw_text(const unsigned int x,const unsigned int y,const char *text)
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
 first.x=0;
 first.y=0;
 first.width=0;
 first.height=0;
 second=first;
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

bool Collision::check_horizontal_collision(const Collision_Box &first_target,const Collision_Box &second_target)
{
 this->set_target(first_target,second_target);
 return this->check_horizontal_collision();
}

bool Collision::check_vertical_collision(const Collision_Box &first_target,const Collision_Box &second_target)
{
 this->set_target(first_target,second_target);
 return this->check_vertical_collision();
}

bool Collision::check_collision(const Collision_Box &first_target,const Collision_Box &second_target)
{
 this->set_target(first_target,second_target);
 return this->check_collision();
}

Collision_Box Collision::generate_box(const unsigned int x,const unsigned int y,const unsigned int width,const unsigned int height) const
{
 Collision_Box result;
 result.x=x;
 result.y=y;
 result.width=width;
 result.height=height;
 return result;
}

}
