#include "GuiEngine.h"

//D2D
ID2D1Factory* pFactory;
ID2D1HwndRenderTarget* pRenderTarget;
ID2D1SolidColorBrush* ColorBrush;

IDWriteFactory1* pDWriteFactory;
IDWriteTextFormat* TextFormat;
IDWriteTextFormat* BigTextFormat;

D2D1_COLOR_F clear_color_D2D = { 0.0f, 0.0f, 0.0f, 0.0f }; //очистка ID2D1RenderTarget* pRenderTarget 
D2D1_COLOR_F color_brush = { 0.0f,0.0f, 0.0f, 1.0f }; //цвет кисти

//Esp var
WNDCLASSEX wcEsp; //класс окна есп


bool GuiEngine::Esp::init_window_Esp(const char* windowsName,const char* className)
{
	strcpy(gameName, windowsName);
	gameHWND = FindWindow(className, gameName);
	if (gameHWND <= NULL)
	{
		MessageBox(0, "[ GuiEngine Menu ] Not find Game", "ERROR", MB_OK | MB_ICONERROR);
		return 0;
	}

	wcEsp = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, GuiEngine::WndProc, 0L, 0L, GetModuleHandle(NULL), NULL, NULL, (HBRUSH)CreateSolidBrush(RGB(0, 0, 0)), NULL,randomStrGen(10), NULL };
	RegisterClassEx(&wcEsp);



	RECT rc;
	GetClientRect(gameHWND, &rc);
	cheatEspHWND = CreateWindowExA(WS_EX_TOPMOST | WS_EX_LAYERED  | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW , wcEsp.lpszClassName, wcEsp.lpszMenuName, WS_POPUP, 0, 0, rc.right - rc.left, rc.bottom - rc.top, 0, 0, 0, 0);  // |  WS_EX_CLIENTEDGE| WS_EX_TRANSPARENT
	
	SetLayeredWindowAttributes(cheatEspHWND, RGB(0, 0, 0), 255, /*ULW_COLORKEY | */LWA_ALPHA);


	MARGINS Margin = { -1};
	DwmExtendFrameIntoClientArea(cheatEspHWND, &Margin);



	if (!GuiEngine::Esp::init_render())
	{
		MessageBox(0, "[ GuiEngine Menu ] init_render error", "ERROR", MB_OK | MB_ICONERROR);
		cleanup_canvas();
		UnregisterClass(wcEsp.lpszClassName, wcEsp.hInstance);
		return 0;
	}

	
	ShowWindow(cheatEspHWND, SW_SHOWDEFAULT);
	UpdateWindow(cheatEspHWND);

	return 1;
}
bool GuiEngine::Esp::init_render()
{
	D2D1_FACTORY_OPTIONS CreateOpt = { D2D1_DEBUG_LEVEL_NONE };
	
	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(pDWriteFactory), (IUnknown**)&pDWriteFactory);
	pDWriteFactory->CreateTextFormat(L"Arial Unicode MS", NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_CONDENSED, 12.f, L"", &TextFormat);
	pDWriteFactory->CreateTextFormat(L"Arial Unicode MS", NULL, DWRITE_FONT_WEIGHT_THIN, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_CONDENSED, 20.f, L"", &BigTextFormat);

	//if (S_OK != D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory), &CreateOpt, (void**)&pFactory))
	if (S_OK != D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory))
	{
		MessageBox(0, "D2D1CreateFactory", "ERROR", MB_OK | MB_ICONERROR);
		return 0;
	}
		

	GuiEngine::Esp::create_canvas();

	return 1;
}


void GuiEngine::Esp::cleanup_canvas() {
	if (pRenderTarget) {
		pRenderTarget->Release();
		pRenderTarget = nullptr;
	}
}

void GuiEngine::Esp::create_canvas()
{
	RECT rc;
	GetClientRect(cheatEspHWND, &rc);

	pFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_HARDWARE, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED)),//D2D1_RENDER_TARGET_TYPE_DEFAULT
		D2D1::HwndRenderTargetProperties(cheatEspHWND, D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)),//D2D1_PRESENT_OPTIONS_IMMEDIATELY
		&pRenderTarget);
	if (!pRenderTarget)	MessageBox(0, "[ GuiEngine create_canvas ] !pRenderTarget", "ERROR", MB_OK | MB_ICONERROR);
	pRenderTarget->SetDpi(96, 96);


	pRenderTarget->CreateSolidColorBrush(color_brush, &ColorBrush);


}

void GuiEngine::Esp::begin_draw_esp()
{

	pRenderTarget->BeginDraw();
	pRenderTarget->Clear(clear_color_D2D);

}

void GuiEngine::Esp::clear_window()
{
	GuiEngine::Esp::begin_draw_esp();
	pRenderTarget->Clear(clear_color_D2D);
	GuiEngine::Esp::end_draw_esp();
}

void GuiEngine::Esp::end_draw_esp() {
	HRESULT state = pRenderTarget->EndDraw();

	if (state == D2DERR_RECREATE_TARGET)
	{
		//std::cout << "D2DERR_RECREATE_TARGET" << std::endl;
		//	cleanup_canvas();
		//	create_canvas();
	}
}

void GuiEngine::Esp::shutdown()
{

	if (pRenderTarget) {
		pRenderTarget->Release();
		pRenderTarget = nullptr;
	}
	if (pFactory) {
		pFactory->Release();
		pFactory = nullptr;
	}
	if (ColorBrush) {
		ColorBrush->Release();
		ColorBrush = nullptr;
	}
	if (pDWriteFactory) {
		pDWriteFactory->Release();
		pDWriteFactory = nullptr;
	}
	if (TextFormat) {
		TextFormat->Release();
		TextFormat = nullptr;
	}
	if (BigTextFormat) {
		BigTextFormat->Release();
		BigTextFormat = nullptr;
	}

	DestroyWindow(cheatEspHWND);
	UnregisterClass(wcEsp.lpszClassName, wcEsp.hInstance);
}




//primitives
/*
void GuiEngine::Esp::rect(float x,float y,float h,float w) {
    //D2D1_SIZE_F rtSize= pRenderTarget->GetSize();
	//int width = static_cast<int>(rtSize.width);
    //int height = static_cast<int>(rtSize.height);
	//std::cout <<"Render target: "<< width<<" " << height << std::endl;

	ColorBrush->SetColor({0.0f,1.0f,0.0f,1.0f});
	pRenderTarget->DrawRectangle(D2D1::RectF(x, y, x+h, y+w), ColorBrush,1.0f);
}
*/
void GuiEngine::Esp::rect(float x, float y, float h, float w, const D2D1::ColorF& Clr,float Thick)
{
	ColorBrush->SetColor(Clr);
	pRenderTarget->DrawRectangle(D2D1::RectF(x, y, x + h, y + w), ColorBrush, Thick);
}

void GuiEngine::Esp::fill_rect(float x, float y, float w, float h,const D2D1::ColorF& Clr)
{
	ColorBrush->SetColor(Clr);
	pRenderTarget->FillRectangle(D2D1::RectF(x, y, x + w, y + h), ColorBrush);
}


void GuiEngine::Esp::Line(const Vector2& Start, const Vector2& End, const D2D1::ColorF& Clr, float Thick )
{
	ColorBrush->SetColor(Clr);
	pRenderTarget->DrawLine({ Start.x, Start.y }, { End.x, End.y }, ColorBrush, Thick);
}


void  GuiEngine::Esp::Crosshair(const int &screenWidth,const int &screenHeight, const D2D1::ColorF& Clr,const float &thick)
{
	ColorBrush->SetColor(Clr);
	//ColorBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
	float xCenter = screenWidth / 2, yCenter= screenHeight / 2;
	pRenderTarget->DrawLine({ xCenter-5, yCenter-5 }, { xCenter+5,yCenter + 5 }, ColorBrush, thick);
	pRenderTarget->DrawLine({ xCenter + 5, yCenter - 5 }, { xCenter - 5,yCenter + 5 }, ColorBrush, thick);

	//pRenderTarget->DrawLine({ xCenter - 6, yCenter - 6 }, { xCenter ,yCenter -12 }, ColorBrush, 2.f);
	//pRenderTarget->DrawLine({ xCenter + 6, yCenter - 6 }, { xCenter +12 ,yCenter  }, ColorBrush, 2.f);
	//pRenderTarget->DrawLine({ xCenter + 6,yCenter + 6 }, { xCenter ,yCenter + 12 }, ColorBrush, 2.f);
	//pRenderTarget->DrawLine({ xCenter - 6,yCenter + 6 }, { xCenter - 12,yCenter  }, ColorBrush, 2.f);

	ColorBrush->SetColor(D2D1::ColorF(D2D1::ColorF::BlueViolet));//BlueViolet

	pRenderTarget->DrawRectangle(D2D1::RectF(xCenter, yCenter, xCenter, yCenter), ColorBrush, thick+1.0f);

}

void GuiEngine::Esp::Сircle(const Vector2& Start, const D2D1::ColorF& Clr, float Rad, float Thick) {
	ColorBrush->SetColor(Clr);
	pRenderTarget->DrawEllipse({ { Start.x, Start.y }, Rad, Rad }, ColorBrush, Thick);
}

void GuiEngine::Esp::CenterString(const Vector2& pos, const wchar_t* Str, const D2D1::ColorF& Clr, bool big)
{
	ColorBrush->SetColor(Clr);
	if (!big)pRenderTarget->DrawTextA(Str, wcslen(Str), TextFormat, { pos.x - wcslen(Str) * 5.8f / 2, pos.y - 7,FLT_MAX,FLT_MAX }, ColorBrush);
	else pRenderTarget->DrawTextA(Str, wcslen(Str), BigTextFormat, { pos.x, pos.y, FLT_MAX, FLT_MAX }, ColorBrush);
}

void GuiEngine::Esp::String(const Vector2& pos, const wchar_t* Str, const D2D1::ColorF& Clr, bool big)
{
	ColorBrush->SetColor(Clr);
	if (!big)pRenderTarget->DrawTextA(Str, wcslen(Str), TextFormat, { pos.x , pos.y,FLT_MAX,FLT_MAX }, ColorBrush);
	else pRenderTarget->DrawTextA(Str, wcslen(Str), BigTextFormat, { pos.x, pos.y, FLT_MAX, FLT_MAX }, ColorBrush);
}
