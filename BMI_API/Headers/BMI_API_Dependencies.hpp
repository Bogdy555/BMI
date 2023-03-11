#ifndef BMI_API_Dependencies_hpp

#define BMI_API_Dependencies_hpp



#pragma pack(pop)



#include "BMI_API_WinTargetVer.hpp"
#include <Windows.h>



#include <iostream>
#include <fstream>

#include <vector>
#include <string>



#pragma pack(push, 1)



#include "BMI_API_Macros.hpp"



namespace BMI_API
{

	namespace Debug
	{



	}

	namespace String
	{



	}

	namespace DFA
	{

		enum ChunkTypes : uint8_t;

		class Symbol;

		class State;

		class Machine;

	}

}



#endif
