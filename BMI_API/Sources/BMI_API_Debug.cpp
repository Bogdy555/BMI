#include "..\Headers\BMI_API.hpp"



void BMI_API::Debug::Break(const wchar_t* _Msg)
{
	if (_Msg)
	{
		MessageBox(NULL, _Msg, L"Debug break!", MB_OK | MB_ICONERROR);
	}

	__debugbreak();
}
