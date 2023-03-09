#ifndef BMI_API_Debug_hpp

#define BMI_API_Debug_hpp



#include "BMI_API.hpp"



#ifdef UNICODE

#define BMI_API_STRING(X) L ## X

#endif

#ifndef UNICODE

#define BMI_API_STRING(X) X

#endif



#define BMI_API_PRINT_A(X) std::cout << X
#define BMI_API_PRINT_W(X) std::wcout << X

#ifdef UNICODE

#define BMI_API_PRINT(X) BMI_API_PRINT_W(X)

#endif

#ifndef UNICODE

#define BMI_API_PRINT(X) BMI_API_PRINT_A(X)

#endif

#define BMI_API_PRINT_LINE_A(X) std::cout << X << '\n'
#define BMI_API_PRINT_LINE_W(X) std::wcout << X << L'\n'

#ifdef UNICODE

#define BMI_API_PRINT_LINE(X) BMI_API_PRINT_LINE_W(X)

#endif

#ifndef UNICODE

#define BMI_API_PRINT_LINE(X) BMI_API_PRINT_LINE_A(X)

#endif



#ifdef _DEBUG

#define BMI_API_LOG_A(X) std::cout << X
#define BMI_API_LOG_W(X) std::wcout << X

#ifdef UNICODE

#define BMI_API_LOG(X) BMI_API_LOG_W(X)

#endif

#ifndef UNICODE

#define BMI_API_LOG(X) BMI_API_LOG_A(X)

#endif

#define BMI_API_LOG_LINE_A(X) std::cout << X << '\n'
#define BMI_API_LOG_LINE_W(X) std::wcout << X << L'\n'

#ifdef UNICODE

#define BMI_API_LOG_LINE(X) BMI_API_LOG_LINE_W(X)

#endif

#ifndef UNICODE

#define BMI_API_LOG_LINE(X) BMI_API_LOG_LINE_A(X)

#endif

#endif

#ifndef _DEBUG

#define BMI_API_LOG_A(X)
#define BMI_API_LOG_W(X)
#define BMI_API_LOG(X)

#define BMI_API_LOG_LINE_A(X)
#define BMI_API_LOG_LINE_W(X)
#define BMI_API_LOG_LINE(X)

#endif



#ifdef _DEBUG

#define BMI_API_OUTPUT_DEBUG_STR_A(X) OutputDebugStringA(X)
#define BMI_API_OUTPUT_DEBUG_STR_W(X) OutputDebugStringW(X)

#ifdef UNICODE

#define BMI_API_OUTPUT_DEBUG_STR(X) BMI_API_OUTPUT_DEBUG_STR_W(X)

#endif

#ifndef UNICODE

#define BMI_API_OUTPUT_DEBUG_STR(X) BMI_API_OUTPUT_DEBUG_STR_A(X)

#endif

#define BMI_API_OUTPUT_DEBUG_STR_LINE_A(X) OutputDebugStringA(X); OutputDebugStringA("\n")
#define BMI_API_OUTPUT_DEBUG_STR_LINE_W(X) OutputDebugStringW(X); OutputDebugStringW(L"\n")

#ifdef UNICODE

#define BMI_API_OUTPUT_DEBUG_STR_LINE(X) BMI_API_OUTPUT_DEBUG_STR_LINE_W(X)

#endif

#ifndef UNICODE

#define BMI_API_OUTPUT_DEBUG_STR_LINE(X) BMI_API_OUTPUT_DEBUG_STR_LINE_A(X)

#endif

#endif

#ifndef _DEBUG

#define BMI_API_OUTPUT_DEBUG_STR_A(X)
#define BMI_API_OUTPUT_DEBUG_STR_W(X)
#define BMI_API_OUTPUT_DEBUG_STR(X)

#define BMI_API_OUTPUT_DEBUG_STR_LINE_A(X)
#define BMI_API_OUTPUT_DEBUG_STR_LINE_W(X)
#define BMI_API_OUTPUT_DEBUG_STR_LINE(X)

#endif



#ifdef _DEBUG

#define BMI_API_DEBUG_BREAK() BMI_API::Debug::Break()

#define BMI_API_DEBUG_BREAK_MSG_A(Msg) MessageBoxA(NULL, Msg, "Debug break!", MB_OK | MB_ICONERROR); BMI_API::Debug::Break()
#define BMI_API_DEBUG_BREAK_MSG_W(Msg) MessageBoxW(NULL, Msg, L"Debug break!", MB_OK | MB_ICONERROR); BMI_API::Debug::Break()

#ifdef UNICODE

#define BMI_API_DEBUG_BREAK_MSG(Msg) BMI_API_DEBUG_BREAK_MSG_W(Msg)

#endif

#ifndef UNICODE

#define BMI_API_DEBUG_BREAK_MSG(Msg) BMI_API_DEBUG_BREAK_MSG_A(Msg)

#endif

#endif

#ifndef _DEBUG

#define BMI_API_DEBUG_BREAK()
#define BMI_API_DEBUG_BREAK_MSG_A(Msg)
#define BMI_API_DEBUG_BREAK_MSG_W(Msg)
#define BMI_API_DEBUG_BREAK_MSG(Msg)

#endif



#ifdef _DEBUG

#define BMI_API_ASSERT(Condition) if (!(Condition))\
{\
	BMI_API_DEBUG_BREAK();\
}

#define BMI_API_ASSERT_MSG_A(Condition, Msg) if (!(Condition))\
{\
	BMI_API_DEBUG_BREAK_MSG_A(Msg);\
}

#define BMI_API_ASSERT_MSG_W(Condition, Msg) if (!(Condition))\
{\
	BMI_API_DEBUG_BREAK_MSG_W(Msg);\
}

#ifdef UNICODE

#define BMI_API_ASSERT_MSG(Condition, Msg) BMI_API_ASSERT_MSG_W(Condition, Msg)

#endif

#ifndef UNICODE

#define BMI_API_ASSERT_MSG(Condition, Msg) BMI_API_ASSERT_MSG_A(Condition, Msg)

#endif

#endif

#ifndef _DEBUG

#define BMI_API_ASSERT(Condition)
#define BMI_API_ASSERT_MSG_A(Condition, Msg)
#define BMI_API_ASSERT_MSG_W(Condition, Msg)
#define BMI_API_ASSERT_MSG(Condition, Msg)

#endif



namespace BMI_API
{

	namespace Debug
	{

		void Break(const wchar_t* _Msg = nullptr);

	}

}



#endif
