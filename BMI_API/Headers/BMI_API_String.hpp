#ifndef BMI_API_String_hpp

#define BMI_API_String_hpp



#include "BMI_API.hpp"



namespace BMI_API
{

	namespace String
	{

		const size_t Length(const wchar_t* _Str);

		const bool TheSame(const wchar_t* _Str1, const wchar_t* _Str2);

		const bool Contains(const wchar_t* _Str, const wchar_t _Chr);

		const wchar_t* TokenizeWhiteSpace(wchar_t* _Str);

	}

}



#endif
