#include "window.h"
#include "bmputil.h"

class window::implementation
{
	public:
								implementation		() : hInstance(0), hwnd(0), bmpu(0), double_buffer(true) { }
		BOOL					init_instance		(HINSTANCE hInstance, int nCmdShow);

		static LRESULT CALLBACK	WndProc				(HWND, UINT, WPARAM, LPARAM);

		HINSTANCE				hInstance;
		HWND					hwnd;
		bmputil*				bmpu;
		RECT					rect;
		bool					double_buffer;
		window*					win;
		HBRUSH					background_brush;
};

window::window()
{
	m_imp = new implementation();
	m_imp->bmpu = new bmputil();
	m_imp->win = this;

	m_width = 800;
	m_height = 600;
}

window::~window()
{
	destroy();
	delete m_imp->bmpu;
	delete m_imp;
}

bool window::create	()
{
	m_imp->hInstance = GetModuleHandle(NULL);

	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)window::implementation::WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= m_imp->hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= L"window_class";
	wcex.hIconSm		= NULL;

	if (!RegisterClassEx(&wcex)) return false;

	if (!m_imp->init_instance (m_imp->hInstance, TRUE)) 
	{
		return false;
	}

	return true;
}

void window::destroy ()
{
	DeleteObject(m_imp->background_brush);
	DestroyWindow(m_imp->hwnd);
}

void window::refresh ()
{
	UpdateWindow(m_imp->hwnd);
}

void window::on_init()
{

}

void window::on_draw()
{

}

void window::on_close(bool& _cancel)
{

}

void window::update ()
{
	MSG msg;

	while (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

BOOL window::implementation::init_instance(HINSTANCE hInstance, int nCmdShow)
{
	hwnd = CreateWindow(L"window_class", L"window title", WS_OVERLAPPEDWINDOW,
	  200, 200, win->m_width, win->m_height, NULL, NULL, hInstance, NULL);

	if (!hwnd)
	{
	  return FALSE;
	}

	win->resize	(win->m_width, win->m_height);

	bmpu->initialize_buffers(hwnd,1);
	bmpu->create_buffer(0);//(B)

	SetWindowLongPtr( hwnd, GWL_USERDATA, (LONG)this);

	background_brush = CreateSolidBrush(RGB(255,255,255));

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	win->on_init();

	return TRUE;
}

LRESULT CALLBACK window::implementation::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	window::implementation* m_imp = (window::implementation*)GetWindowLongPtr(hWnd, GWL_USERDATA);
	if (m_imp == NULL) return DefWindowProc(hWnd, message, wParam, lParam);

	switch (message) 
	{
	case WM_CREATE:
		{
 

		}
		break;
	//will be called periodically, if SetTimer function is invoked.
    //In our case the WM_TIMER event occurs every 15 seconds see (A)
	case WM_PAINT:
		{		
			m_imp->win->on_draw();
			
			hdc = BeginPaint(hWnd, &ps);
			m_imp->bmpu->copy_to_screen(0);
			EndPaint(hWnd, &ps);
		}
		break;
	case WM_DESTROY:
		{
			bool cancel = false;
			m_imp->win->on_close(cancel);
			if (cancel) break;
			PostQuitMessage(0);
			 m_imp->bmpu->free_buffer(0);
		}
		break;
	case WM_SIZE:
		{
			m_imp->win->m_width = LOWORD(lParam);
			m_imp->win->m_height = HIWORD(lParam);
			m_imp->bmpu->resize(hWnd);
		}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void window::draw (image* _img, const int& _x, const int& _y, const int& _w, const int& _h)
{
    HDC dc = m_imp->bmpu->get_dc_buffer(0);

    BITMAPINFO info;
    ZeroMemory(&info, sizeof(BITMAPINFO));
    info.bmiHeader.biBitCount = 24;
    info.bmiHeader.biWidth =  _img->width();
    info.bmiHeader.biHeight = _img->height();
    info.bmiHeader.biPlanes = 1;
    info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    info.bmiHeader.biSizeImage = _img->size();
    info.bmiHeader.biCompression = BI_RGB;

	int dw = (_w==0)?m_width:_w;
	int dh = (_h==0)?m_height:_h;

    StretchDIBits(dc, 0, 0, dw, dh, 0, 0, _img->width(), _img->height(), _img->bytes(), &info, DIB_RGB_COLORS, SRCCOPY);

	RECT rect;
	rect.left = _x;
	rect.top = _y;
	rect.right = _x+_img->width();
	rect.bottom = _y+_img->height();
	InvalidateRect(m_imp->hwnd, &rect, false);
}

void window::resize	(const int& _w, const int& _h)
{
	m_width = _w;
	m_height = _h;
	
	if (!m_imp->hwnd) return;

	DWORD dwStyle = GetWindowLongPtr( m_imp->hwnd, GWL_STYLE ) ;
    DWORD dwExStyle = GetWindowLongPtr( m_imp->hwnd, GWL_EXSTYLE ) ;
    HMENU menu = GetMenu( m_imp->hwnd ) ;
	RECT rc = { 0, 0, m_width, m_height } ; 
    AdjustWindowRectEx( &rc, dwStyle, menu ? TRUE : FALSE, dwExStyle );
	SetWindowPos(m_imp->hwnd,0,0,0,rc.right-rc.left, rc.bottom-rc.top,SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
}