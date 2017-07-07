/*
Copyright © 2016–2017, Popov Evgeniy Alekseyevich

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
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>
#include <windows.h>
#include <unknwn.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9math.h>
#include <dshow.h>
#include <xinput.h>

#define SWGF_MOUSE_NONE 0
#define SWGF_MOUSE_LEFT 1
#define SWGF_MOUSE_RIGHT 2
#define SWGF_MOUSE_MIDDLE 3

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

struct SWGF_Vertex
{
 float x;
 float y;
 float z;
 float u;
 float v;
};

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

class SWGF_Synchronization
{
 private:
 HANDLE timer;
 protected:
 void set_timer(unsigned long int interval);
 void wait_timer();
 public:
 SWGF_Synchronization();
 ~SWGF_Synchronization();
};

SWGF_Synchronization::SWGF_Synchronization()
{
 timer=CreateWaitableTimer(NULL,FALSE,NULL);
 if (timer==NULL)
 {
  puts("Can't create synchronization timer");
  exit(EXIT_FAILURE);
 }

}

SWGF_Synchronization::~SWGF_Synchronization()
{
 CancelWaitableTimer(timer);
 CloseHandle(timer);
}

void SWGF_Synchronization::set_timer(unsigned long int interval)
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

class SWGF_Engine
{
 protected:
 HWND window;
 WNDCLASS window_class;
 unsigned long int width;
 unsigned long int height;
 void create_window();
 void capture_mouse();
 bool process_message();
 public:
 SWGF_Engine();
 ~SWGF_Engine();
 unsigned long int get_width();
 unsigned long int get_height();
};

SWGF_Engine::SWGF_Engine()
{
 window_class.lpszClassName=TEXT("SWGF");
 window_class.hInstance=GetModuleHandle(NULL);
 window_class.style=CS_HREDRAW|CS_VREDRAW;
 window_class.lpfnWndProc=(WNDPROC)DefWindowProc;
 window_class.hbrBackground=(HBRUSH)GetStockObject(BLACK_BRUSH);
 window_class.hIcon=LoadIcon(NULL,IDI_APPLICATION);
 window_class.hCursor=LoadCursor(NULL,IDC_ARROW);
 window_class.cbClsExtra=0;
 window_class.cbWndExtra=0;
 if (window_class.hCursor==NULL)
 {
  puts("Can't load the standart cursor");
  exit(EXIT_FAILURE);
 }
 if (window_class.hIcon==NULL)
 {
  puts("Can't load the standart program icon");
  exit(EXIT_FAILURE);
 }
 if (RegisterClass(&window_class)==0)
 {
  puts("Can't register window class");
  exit(EXIT_FAILURE);
 }
 width=GetSystemMetrics(SM_CXSCREEN);
 height=GetSystemMetrics(SM_CYSCREEN);
}

SWGF_Engine::~SWGF_Engine()
{
 if(window!=NULL) CloseWindow(window);
 UnregisterClass(window_class.lpszClassName,window_class.hInstance);
}

void SWGF_Engine::create_window()
{
 window=CreateWindowEx(WS_EX_TOPMOST,window_class.lpszClassName,NULL,WS_VISIBLE|WS_POPUP,0,0,width,height,NULL,NULL,window_class.hInstance,NULL);
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

class SWGF_Frame
{
 protected:
 unsigned long int frame_width;
 unsigned long int frame_height;
 unsigned long int length;
 SWGF_Pixel *buffer;
 void create_render_buffer();
 public:
 SWGF_Frame();
 ~SWGF_Frame();
 void draw_pixel(unsigned long int x,unsigned long int y,unsigned char red,unsigned char green,unsigned char blue);
 void clear_screen();
 unsigned long int get_frame_width();
 unsigned long int get_frame_height();
};

SWGF_Frame::SWGF_Frame()
{
 frame_width=512;
 frame_height=512;
 buffer=NULL;
}

SWGF_Frame::~SWGF_Frame()
{
 if(buffer!=NULL)
 {
  buffer=NULL;
  free(buffer);
 }

}

void SWGF_Frame::create_render_buffer()
{
 if(buffer!=NULL)
 {
  buffer=NULL;
  free(buffer);
 }
 length=frame_width*frame_height*sizeof(SWGF_Pixel);
 buffer=(SWGF_Pixel*)calloc(length,1);
 if(buffer==NULL)
 {
  puts("Can't allocate memory for render buffer");
  exit(EXIT_FAILURE);
 }

}

void SWGF_Frame::draw_pixel(unsigned long int x,unsigned long int y,unsigned char red,unsigned char green,unsigned char blue)
{
 unsigned long int offset;
 if((x<frame_width)&&(y<frame_height))
 {
  offset=x+y*frame_width;
  buffer[offset].red=red;
  buffer[offset].green=green;
  buffer[offset].blue=blue;
  buffer[offset].alpha=red;
 }

}

void SWGF_Frame::clear_screen()
{
 memset(buffer,0,length);
}

unsigned long int SWGF_Frame::get_frame_width()
{
 return frame_width;
}

unsigned long int SWGF_Frame::get_frame_height()
{
 return frame_height;
}

class SWGF_Render:public SWGF_Base, public SWGF_Engine, public SWGF_Frame
{
 protected:
 IDirect3D9 *backend;
 IDirect3DDevice9 *device;
 IDirect3DVertexBuffer9 *vertex;
 IDirect3DTexture9 *texture;
 D3DPRESENT_PARAMETERS present;
 D3DDISPLAYMODE display;
 SWGF_Vertex surface[4];
 void initialize_backend();
 void reinitialize_backend();
 void get_display_setting();
 void set_render_setting();
 void configure_video();
 void clear_target();
 void check_videocard();
 void set_render();
 void set_perfomance();
 void set_transform(D3DTRANSFORMSTATETYPE target,D3DXMATRIX *matrix);
 void set_perspective();
 void set_viewport();
 void prepare_surface();
 void create_vertex_buffer();
 void load_vertex_data();
 void set_source();
 void set_format();
 void create_texture();
 void set_texture_setting();
 void update_texture();
 void destroy_resources();
 void create_render();
 void recreate_render();
 void update_surface();
 void refresh();
 public:
 SWGF_Render();
 ~SWGF_Render();
};

SWGF_Render::SWGF_Render()
{
 backend=NULL;
 device=NULL;
 vertex=NULL;
 texture=NULL;
}

SWGF_Render::~SWGF_Render()
{
 vertex->Release();
 texture->Release();
 device->Release();
 backend->Release();
}

void SWGF_Render::initialize_backend()
{
 backend=Direct3DCreate9(D3D_SDK_VERSION);
 if(backend==NULL)
 {
  puts("Can't initialize render back-end");
  exit(EXIT_FAILURE);
 }

}

void SWGF_Render::reinitialize_backend()
{
 if(device->Reset(&present)!=D3D_OK)
 {
  puts("Can't reinitialize back-end");
  exit(EXIT_FAILURE);
 }

}

void SWGF_Render::get_display_setting()
{
 if(backend->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&display)!=D3D_OK)
 {
  puts("Can't get display setting");
  exit(EXIT_FAILURE);
 }

}

void SWGF_Render::set_render_setting()
{
 memset(&present,0,sizeof(D3DPRESENT_PARAMETERS));
 present.BackBufferWidth=display.Width;
 present.BackBufferHeight=display.Height;
 present.BackBufferFormat=display.Format;
 present.FullScreen_RefreshRateInHz=display.RefreshRate;
 present.BackBufferCount=1;
 present.PresentationInterval=D3DPRESENT_INTERVAL_IMMEDIATE;
 present.MultiSampleType=D3DMULTISAMPLE_NONE;
 present.SwapEffect=D3DSWAPEFFECT_COPY;
 present.hDeviceWindow=window;
 present.Windowed=FALSE;
 present.EnableAutoDepthStencil=FALSE;
}

void SWGF_Render::configure_video()
{
 if(backend->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,window,D3DCREATE_HARDWARE_VERTEXPROCESSING,&present,&device)!=D3D_OK)
 {
  puts("Can't configure the video card");
  exit(EXIT_FAILURE);
 }

}

void SWGF_Render::clear_target()
{
 device->Clear(0,NULL,D3DCLEAR_TARGET,D3DCOLOR_ARGB(0,0,0,0),0,0);
}

void SWGF_Render::check_videocard()
{
 D3DCAPS9 configuration;
 if(device->GetDeviceCaps(&configuration)!=D3D_OK)
 {
  puts("Can't get configuration of video card");
  exit(EXIT_FAILURE);
 }
 if((configuration.MaxTextureWidth<frame_width)||(configuration.MaxTextureHeight<frame_height))
 {
  puts("This video card don't support request texture size");
  exit(EXIT_FAILURE);
 }

}

void SWGF_Render::set_render()
{
 this->initialize_backend();
 this->get_display_setting();
 this->set_render_setting();
 this->configure_video();
 this->check_videocard();
}

void SWGF_Render::set_perfomance()
{
 device->SetRenderState(D3DRS_ZENABLE,FALSE);
 device->SetRenderState(D3DRS_ZWRITEENABLE,FALSE);
 device->SetRenderState(D3DRS_LIGHTING,FALSE);
 device->SetRenderState(D3DRS_FOGENABLE,FALSE);
 device->SetRenderState(D3DRS_ALPHATESTENABLE,FALSE);
 device->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
 device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS,FALSE);
 device->SetRenderState(D3DRS_LOCALVIEWER,FALSE);
 device->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
}

void SWGF_Render::set_transform(D3DTRANSFORMSTATETYPE target,D3DXMATRIX *matrix)
{
 if(device->SetTransform(target,matrix)!=D3D_OK)
 {
  puts("Can't transform a matrix");
  exit(EXIT_FAILURE);
 }

}

void SWGF_Render::set_perspective()
{
 D3DXMATRIX identity,projection;
 D3DXMatrixOrthoLH(&projection,width,height,0,1);
 D3DXMatrixIdentity(&identity);
 this->set_transform(D3DTS_WORLD,&identity);
 this->set_transform(D3DTS_VIEW,&identity);
 this->set_transform(D3DTS_PROJECTION,&projection);
}

void SWGF_Render::set_viewport()
{
 D3DVIEWPORT9 port;
 port.X=0;
 port.Y=0;
 port.Width=width;
 port.Height=height;
 port.MinZ=0;
 port.MaxZ=0;
 if(device->SetViewport(&port)!=D3D_OK)
 {
  puts("Can't set viewport");
  exit(EXIT_FAILURE);
 }

}

void SWGF_Render::prepare_surface()
{
 surface[0].x=(float)width/-2.0;
 surface[0].y=(float)height/2.0;
 surface[0].z=0;
 surface[0].u=0;
 surface[0].v=0;
 surface[1].x=(float)width/2.0;
 surface[1].y=(float)height/2.0;
 surface[1].z=0;
 surface[1].u=1;
 surface[1].v=0;
 surface[2].x=(float)width/-2.0;
 surface[2].y=(float)height/-2.0;
 surface[2].z=0;
 surface[2].u=0;
 surface[2].v=1;
 surface[3].x=(float)width/2.0;
 surface[3].y=(float)height/-2.0;
 surface[3].z=0;
 surface[3].u=1;
 surface[3].v=1;
}

void SWGF_Render::create_vertex_buffer()
{
 if(device->CreateVertexBuffer(80,D3DUSAGE_DYNAMIC,D3DFVF_XYZ|D3DFVF_TEX1,D3DPOOL_DEFAULT,&vertex,NULL)!=D3D_OK)
 {
  puts("Can't create vertex buffer");
  exit(EXIT_FAILURE);
 }

}

void SWGF_Render::load_vertex_data()
{
 void *data;
 if(vertex->Lock(0,0,&data,D3DLOCK_DISCARD)!=D3D_OK)
 {
  puts("Can't lock vertex buffer");
  exit(EXIT_FAILURE);
 }
 memmove(data,surface,80);
 vertex->Unlock();
}

void SWGF_Render::set_source()
{
 if(device->SetStreamSource(0,vertex,0,20)!=D3D_OK)
 {
  puts("Can't bind a vertex buffer to a device data stream");
  exit(EXIT_FAILURE);
 }

}

void SWGF_Render::set_format()
{
 if(device->SetFVF(D3DFVF_XYZ|D3DFVF_TEX1)!=D3D_OK)
 {
  puts("Can't set vertex format");
  exit(EXIT_FAILURE);
 }

}

void SWGF_Render::create_texture()
{
 if(device->CreateTexture(frame_width,frame_height,1,D3DUSAGE_DYNAMIC,D3DFMT_A8R8G8B8,D3DPOOL_DEFAULT,&texture,NULL)!=D3D_OK)
 {
  puts("Can't create texture for render surface");
  exit(EXIT_FAILURE);
 }

}

void SWGF_Render::set_texture_setting()
{
 device->SetTexture(0,texture);
 device->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
 device->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);
 device->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
 device->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_DISABLE);
 device->SetSamplerState(0,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
 device->SetSamplerState(0,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
 device->SetSamplerState(0,D3DSAMP_MIPFILTER,D3DTEXF_NONE);
}

void SWGF_Render::update_texture()
{
 D3DLOCKED_RECT lock;
 if(texture->LockRect(0,&lock,NULL,D3DLOCK_DISCARD)!=D3D_OK)
 {
  puts("Can't lock texture");
  exit(EXIT_FAILURE);
 }
 memmove(lock.pBits,buffer,length);
 texture->UnlockRect(0);
}

void SWGF_Render::destroy_resources()
{
 if(vertex!=NULL)
 {
  vertex->Release();
  vertex=NULL;
 }
 if(texture!=NULL)
 {
  texture->Release();
  texture=NULL;
 }

}

void SWGF_Render::create_render()
{
 this->set_perfomance();
 this->set_perspective();
 this->set_viewport();
 this->prepare_surface();
 this->create_vertex_buffer();
 this->load_vertex_data();
 this->set_source();
 this->set_format();
 this->create_texture();
 this->set_texture_setting();
 this->clear_target();
}

void SWGF_Render::recreate_render()
{
 this->destroy_resources();
 this->reinitialize_backend();
 this->create_render();
}

void SWGF_Render::update_surface()
{
 device->BeginScene();
 device->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);
 device->EndScene();
 device->Present(NULL,NULL,NULL,NULL);
}

void SWGF_Render::refresh()
{
 switch (device->TestCooperativeLevel())
 {
  case D3D_OK:
  this->update_texture();
  this->update_surface();
  break;
  case D3DERR_DEVICENOTRESET:
  this->recreate_render();
  break;
  case D3DERR_DRIVERINTERNALERROR:
  puts("Internal driver error");
  exit(EXIT_FAILURE);
  break;
 }


}

class SWGF_Screen:public SWGF_Synchronization, public SWGF_Render
{
 public:
 void initialize();
 bool sync();
 SWGF_Screen* get_handle();
};

void SWGF_Screen::initialize()
{
 this->create_render_buffer();
 this->create_window();
 this->capture_mouse();
 this->set_render();
 this->create_render();
 this->set_timer(17);
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

class SWGF_Keyboard
{
 public:
 bool check_hold(const unsigned int code);
 unsigned int get_virtual_code(const unsigned int code);
 unsigned int get_scan_code(const unsigned int code);
};

bool SWGF_Keyboard::check_hold(const unsigned int code)
{
 bool result;
 result=false;
 if(GetAsyncKeyState(MapVirtualKey(code,MAPVK_VSC_TO_VK))==-32767) result=true;
 return result;
}

unsigned int SWGF_Keyboard::get_virtual_code(const unsigned int code)
{
 return MapVirtualKey(code,MAPVK_VSC_TO_VK);
}

unsigned int SWGF_Keyboard::get_scan_code(const unsigned int code)
{
 return MapVirtualKey(code,MAPVK_VK_TO_VSC);
}

class SWGF_Mouse
{
 private:
 POINT position;
 public:
 SWGF_Mouse();
 ~SWGF_Mouse();
 void show();
 void hide();
 unsigned char get_hold();
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

unsigned char SWGF_Mouse::get_hold()
{
 unsigned char result;
 result=SWGF_MOUSE_NONE;
 if(GetAsyncKeyState(VK_LBUTTON)==-32767) result=SWGF_MOUSE_LEFT;
 if(GetAsyncKeyState(VK_RBUTTON)==-32767) result=SWGF_MOUSE_RIGHT;
 if(GetAsyncKeyState(VK_MBUTTON)==-32767) result=SWGF_MOUSE_MIDDLE;
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
 void stop();
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

void SWGF_Multimedia::stop()
{
 player->Stop();
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
 void draw_rectangle(const unsigned long int x,const unsigned long int y,const unsigned long int width,const unsigned long int height,const unsigned char red,const unsigned char green,const unsigned char blue);
 void draw_filled_rectangle(const unsigned long int x,const unsigned long int y,const unsigned long int width,const unsigned long int height,const unsigned char red,const unsigned char green,const unsigned char blue);
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

void SWGF_Primitive::draw_rectangle(const unsigned long int x,const unsigned long int y,const unsigned long int width,const unsigned long int height,const unsigned char red,const unsigned char green,const unsigned char blue)
{
 unsigned long int stop_x,stop_y;
 stop_x=x+width;
 stop_y=y+height;
 this->draw_line(x,y,stop_x,y,red,green,blue);
 this->draw_line(x,stop_y,stop_x,stop_y,red,green,blue);
 this->draw_line(x,y,x,stop_y,red,green,blue);
 this->draw_line(stop_x,y,stop_x,stop_y,red,green,blue);
}

void SWGF_Primitive::draw_filled_rectangle(const unsigned long int x,const unsigned long int y,const unsigned long int width,const unsigned long int height,const unsigned char red,const unsigned char green,const unsigned char blue)
{
 unsigned long int step_x,step_y,stop_x,stop_y;
 stop_x=x+width;
 stop_y=y+height;
 for(step_x=x;step_x<stop_x;step_x++)
 {
  for(step_y=y;step_y<stop_y;step_y++)
  {
   surface->draw_pixel(step_x,step_y,red,green,blue);
  }

 }

}

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

void SWGF_Image::load_tga(const char *name)
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
 if(data!=NULL)
 {
  free(data);
  data=NULL;
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
 fclose(target);
 width=image.width;
 height=image.height;
 data=uncompressed;
}

void SWGF_Image::load_pcx(const char *name)
{
 FILE *target;
 unsigned long int x,y,index,position,line,row,length,uncompressed_length;
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
 if(data!=NULL)
 {
  free(data);
  data=NULL;
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
 height=head.max_y-head.min_y+1;
 row=3*width;
 line=head.planes*head.plane_length;
 uncompressed_length=row*height;
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
  for(y=0;y<height;y++)
  {
   index=x*3+y*row;
   position=x+y*line;
   original[index]=uncompressed[position+2*head.plane_length];
   original[index+1]=uncompressed[position+head.plane_length];
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

unsigned long int SWGF_Image::get_data_length()
{
 return width*height*3;
}

unsigned char *SWGF_Image::get_data()
{
 return data;
}

void SWGF_Image::destroy_image()
{
 if(data!=NULL)
 {
  width=0;
  height=0;
  free(data);
  data=NULL;
 }

}

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
 unsigned long int get_width();
 unsigned long int get_height();
 void set_frames(const unsigned long int amount);
 unsigned long int get_frames();
 void initialize(SWGF_Screen *Screen);
 void load_image(SWGF_Image &buffer);
 void mirror_image(const unsigned char kind);
 void resize_image(const unsigned long int new_width,const unsigned long int new_height);
};

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
 unsigned long int length;
 width=buffer.get_width();
 height=buffer.get_height();
 length=buffer.get_data_length();
 if(image!=NULL) free(image);
 image=(SWGF_Color*)calloc(length,1);
 if (image==NULL)
 {
  puts("Can't allocate memory for image buffer");
  exit(EXIT_FAILURE);
 }
 memmove(image,buffer.get_data(),length);
 buffer.destroy_image();
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

class SWGF_Background:public SWGF_Canvas
{
 public:
 void draw_background();
 void draw_horizontal_background(const unsigned long int frame);
 void draw_vertical_background(const unsigned long int frame);
};

void SWGF_Background::draw_background()
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

void SWGF_Background::draw_horizontal_background(const unsigned long int frame)
{
 unsigned long int x,y,offset,start,frame_width;
 frame_width=width/frames;
 start=(frame-1)*frame_width;
 for (x=0;x<frame_width;x++)
 {
  for (y=0;y<height;y++)
  {
   offset=start+x+(width*y);
   surface->draw_pixel(x,y,image[offset].red,image[offset].green,image[offset].blue);
  }

 }

}

void SWGF_Background::draw_vertical_background(const unsigned long int frame)
{
 unsigned long int x,y,offset,start,frame_height;
 frame_height=height/frames;
 start=(frame-1)*frame_height;
 for (x=0;x<width;x++)
 {
  for (y=0;y<frame_height;y++)
  {
   offset=start+x+(width*y);
   surface->draw_pixel(x,y,image[offset].red,image[offset].green,image[offset].blue);
  }

 }

}

class SWGF_Sprite:public SWGF_Canvas
{
 private:
 unsigned long int current_x;
 unsigned long int current_y;
 public:
 SWGF_Sprite();
 ~SWGF_Sprite();
 void draw_sprite_frame(const unsigned long int x,const unsigned long int y,const unsigned long int frame);
 void draw_sprite(const unsigned long int x,const unsigned long int y);
 unsigned long int get_x();
 unsigned long int get_y();
 unsigned long int get_sprite_width();
 unsigned long int get_sprite_height();
 SWGF_Sprite* get_handle();
};

SWGF_Sprite::SWGF_Sprite()
{
 current_x=0;
 current_y=0;
}

SWGF_Sprite::~SWGF_Sprite()
{

}

void SWGF_Sprite::draw_sprite_frame(const unsigned long int x,const unsigned long int y,const unsigned long int frame)
{
 unsigned long int sprite_x,sprite_y,offset,start,frame_width;
 current_x=x;
 current_y=y;
 frame_width=width/frames;
 start=(frame-1)*frame_width;
 for(sprite_x=0;sprite_x<frame_width;sprite_x++)
 {
  for(sprite_y=0;sprite_y<height;sprite_y++)
  {
   offset=start+sprite_x+(sprite_y*width);
   if(memcmp(&image[0],&image[offset],3)!=0) surface->draw_pixel(x+sprite_x,y+sprite_y,image[offset].red,image[offset].green,image[offset].blue);
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

SWGF_Text::SWGF_Text()
{
 current_x=0;
 current_y=0;
 sprite=NULL;
}

SWGF_Text::~SWGF_Text()
{
 sprite=NULL;
}

void SWGF_Text::set_position(const unsigned long int x,const unsigned long int y)
{
 current_x=x;
 current_y=y;
}

void SWGF_Text::load_font(SWGF_Sprite *font)
{
 sprite=font;
 sprite->set_frames(128);
}

void SWGF_Text::draw_text(const char *text)
{
 unsigned long int index,length,step_x,step_y;
 length=strlen(text);
 step_x=current_x;
 step_y=current_y;
 for (index=0;index<length;index++)
 {
  if (text[index]>31)
  {
   sprite->draw_sprite_frame(step_x,step_y,text[index]+1);
   step_x+=sprite->get_sprite_width();
  }

 }

}

class SWGF_Collision
{
 public:
 bool check_horizontal_collision(SWGF_Sprite &first,SWGF_Sprite &second);
 bool check_vertical_collision(SWGF_Sprite &first,SWGF_Sprite &second);
 bool check_collision(SWGF_Sprite &first,SWGF_Sprite &second);
};

bool SWGF_Collision::check_horizontal_collision(SWGF_Sprite &first,SWGF_Sprite &second)
{
 bool result;
 result=false;
 if((first.get_x()+first.get_sprite_width())>=second.get_x())
 {
  if(first.get_x()<=(second.get_x()+second.get_sprite_width())) result=true;
 }
 return result;
}

bool SWGF_Collision::check_vertical_collision(SWGF_Sprite &first,SWGF_Sprite &second)
{
 bool result;
 result=false;
 if((first.get_y()+first.get_sprite_height())>=second.get_y())
 {
  if(first.get_y()<=(second.get_y()+second.get_sprite_height())) result=true;
 }
 return result;
}

bool SWGF_Collision::check_collision(SWGF_Sprite &first,SWGF_Sprite &second)
{
 bool result;
 result=false;
 if((this->check_horizontal_collision(first,second)==true)||(this->check_vertical_collision(first,second)==true)) result=true;
 return result;
}