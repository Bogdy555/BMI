#include "..\Headers\BMI.hpp"



int WINAPI wWinMain(_In_ HINSTANCE _hInstance, _In_opt_ HINSTANCE _hPrevInstance, _In_ LPWSTR _CmdLine, _In_ int _ShowCmd)
{
	if (__argc != 2)
	{
		BMI_API_PRINT(BMI_API_STRING("Usage: BMI.exe \".\\File.dfa\"\n"));

		return -1;
	}

	std::wifstream _FileIn;

	_FileIn.open(__wargv[1]);

	if (!_FileIn.is_open())
	{
		BMI_API_PRINT(BMI_API_STRING("Could not open the file!\n"));

		return -1;
	}

	BMI_API::DFA::Machine _Machine;

	if (!_Machine.LoadFromFile(_FileIn))
	{
		BMI_API_PRINT(BMI_API_STRING("Could not parse file!\n"));

		_FileIn.close();

		return -1;
	}

	wchar_t _InputString[BMI_API_MAX_STRING_SIZE + 1];

	std::wcin.getline(_InputString, BMI_API_MAX_STRING_SIZE);

	std::vector<size_t> _IndexesString;

	if (!_Machine.ComputeString(_InputString, _IndexesString))
	{
		BMI_API_PRINT(BMI_API_STRING("An error occured while trying to process the input string!\n"));

		_Machine.CleanUp();

		_FileIn.close();

		return -1;
	}

	BMI_API_PRINT(BMI_API_STRING("The result is: "));
	BMI_API_PRINT(_Machine.Run(_IndexesString));
	BMI_API_PRINT(BMI_API_STRING('\n'));

	_Machine.CleanUp();

	_FileIn.close();

	return 0;
}
