#include "..\Headers\BMI_API.hpp"



BMI_API::DFA::Symbol::Symbol() : Name()
{

}

BMI_API::DFA::Symbol::Symbol(const Symbol& _Other) : Name(_Other.Name)
{

}

BMI_API::DFA::Symbol::Symbol(Symbol&& _Other) noexcept : Name(std::move(_Other.Name))
{

}

BMI_API::DFA::Symbol::~Symbol()
{

}

void BMI_API::DFA::Symbol::Init(const std::wstring& _Name)
{
	Name = _Name;
}

void BMI_API::DFA::Symbol::UnInit()
{
	Name.clear();
}

const std::wstring& BMI_API::DFA::Symbol::GetName() const
{
	return Name;
}

const BMI_API::DFA::Symbol& BMI_API::DFA::Symbol::operator= (const Symbol& _Other)
{
	Name = _Other.Name;

	return *this;
}

const BMI_API::DFA::Symbol& BMI_API::DFA::Symbol::operator= (Symbol&& _Other) noexcept
{
	Name = std::move(_Other.Name);

	return *this;
}



BMI_API::DFA::State::State() : Name(), Final(false)
{

}

BMI_API::DFA::State::State(const State& _Other) : Name(_Other.Name), Final(_Other.Final)
{

}

BMI_API::DFA::State::State(State&& _Other) noexcept : Name(std::move(_Other.Name)), Final(_Other.Final)
{
	_Other.Final = false;
}

BMI_API::DFA::State::~State()
{

}

void BMI_API::DFA::State::Init(const std::wstring& _Name, const bool _Final)
{
	Name = _Name;
	Final = _Final;
}

void BMI_API::DFA::State::UnInit()
{
	Name.clear();
	Final = false;
}

const std::wstring& BMI_API::DFA::State::GetName() const
{
	return Name;
}

const bool BMI_API::DFA::State::IsFinal() const
{
	return Final;
}

const BMI_API::DFA::State& BMI_API::DFA::State::operator= (const State& _Other)
{
	Name = _Other.Name;
	Final = _Other.Final;

	return *this;
}

const BMI_API::DFA::State& BMI_API::DFA::State::operator= (State&& _Other) noexcept
{
	Name = std::move(_Other.Name);
	Final = _Other.Final;

	_Other.Final = false;

	return *this;
}



BMI_API::DFA::Machine::Machine() : Alphabet(), States(), InitialStateId(0), TransitionTable(nullptr)
{

}

BMI_API::DFA::Machine::Machine(Machine&& _Other) noexcept : Alphabet(std::move(_Other.Alphabet)), States(std::move(_Other.States)), InitialStateId(_Other.InitialStateId), TransitionTable(_Other.TransitionTable)
{
	_Other.InitialStateId = 0;
	_Other.TransitionTable = nullptr;
}

BMI_API::DFA::Machine::~Machine()
{

}

const bool BMI_API::DFA::Machine::ComputeString(const std::wstring& _Text, std::vector<size_t>& _String) const
{
	wchar_t* _TextRaw = new wchar_t[_Text.size() + 1];

	if (!_TextRaw)
	{
		return false;
	}

	_TextRaw[_Text.size()] = '\0';

	for (size_t _Index = 0; _Index < _Text.size(); _Index++)
	{
		_TextRaw[_Index] = _Text[_Index];
	}

	for (const wchar_t* _CurrentToken = String::TokenizeWhiteSpace(_TextRaw); _CurrentToken != nullptr; _CurrentToken = String::TokenizeWhiteSpace(nullptr))
	{
		bool _Found = false;

		for (size_t _Index = 0; _Index < Alphabet.size(); _Index++)
		{
			if (Alphabet[_Index].GetName() == _CurrentToken)
			{
				_String.push_back(_Index);
				_Found = true;
				break;
			}
		}

		if (!_Found)
		{
			delete[] _TextRaw;

			return false;
		}
	}

	delete[] _TextRaw;

	return _String.size() != 0;
}

bool BMI_API::DFA::Machine::Run(const std::vector<size_t>& _String)
{
	if (!IsInitialized())
	{
		BMI_API_DEBUG_BREAK_MSG(BMI_API_STRING("Can't run the Machine, it is not initialized."));

		return false;
	}

	if (!_String.size())
	{
		BMI_API_DEBUG_BREAK_MSG(BMI_API_STRING("Can't run an empty string through the Machine."));

		return false;
	}

	size_t _CurrentStateId = InitialStateId;

	for (size_t _Index = 0; _Index < _String.size(); _Index++)
	{
		_CurrentStateId = GetNextStateId(_CurrentStateId, _String[_Index]);

		if (_CurrentStateId == States.size())
		{
			return false;
		}
	}

	return States[_CurrentStateId].IsFinal();
}

bool BMI_API::DFA::Machine::LoadFromFile(std::wifstream& _File)
{
	Alphabet.push_back(Symbol());
	Alphabet.push_back(Symbol());

	Alphabet[0].Init(L"a");
	Alphabet[1].Init(L"b");

	States.push_back(State());
	States.push_back(State());

	States[0].Init(L"q0", 0);
	States[1].Init(L"q1", 1);

	InitialStateId = 0;

	TransitionTable = new size_t[4];

	if (!TransitionTable)
	{
		Alphabet.clear();
		States.clear();
		return false;
	}

	TransitionTable[0] = 0;
	TransitionTable[1] = 1;
	TransitionTable[2] = 0;
	TransitionTable[3] = 1;

	return true;
}

void BMI_API::DFA::Machine::CleanUp()
{
	Alphabet.clear();
	States.clear();
	InitialStateId = 0;
	if (TransitionTable)
	{
		delete[] TransitionTable;
		TransitionTable = nullptr;
	}
}

const bool BMI_API::DFA::Machine::IsInitialized() const
{
	return TransitionTable != nullptr;
}

const size_t BMI_API::DFA::Machine::GetSymbolId(const std::wstring& _Name) const
{
	for (size_t _Index = 0; _Index < Alphabet.size(); _Index++)
	{
		if (_Name == Alphabet[_Index].GetName())
		{
			return _Index;
		}
	}

	BMI_API_DEBUG_BREAK_MSG(BMI_API_STRING("No Symbol with this name found."));

	return 0;
}

const size_t BMI_API::DFA::Machine::GetStateId(const std::wstring& _Name) const
{
	for (size_t _Index = 0; _Index < States.size(); _Index++)
	{
		if (_Name == States[_Index].GetName())
		{
			return _Index;
		}
	}

	BMI_API_DEBUG_BREAK_MSG(BMI_API_STRING("No State with this name found."));

	return 0;
}

const size_t BMI_API::DFA::Machine::GetSymbolId(const Symbol* _Symbol) const
{
	for (size_t _Index = 0; _Index < Alphabet.size(); _Index++)
	{
		if (_Symbol == &Alphabet[_Index])
		{
			return _Index;
		}
	}

	BMI_API_DEBUG_BREAK_MSG(BMI_API_STRING("No Symbol found."));

	return 0;
}

const size_t BMI_API::DFA::Machine::GetStateId(const State* _State) const
{
	for (size_t _Index = 0; _Index < States.size(); _Index++)
	{
		if (_State == &States[_Index])
		{
			return _Index;
		}
	}

	BMI_API_DEBUG_BREAK_MSG(BMI_API_STRING("No State found."));

	return 0;
}

const BMI_API::DFA::State& BMI_API::DFA::Machine::GetInitialState() const
{
	return States[InitialStateId];
}

const size_t BMI_API::DFA::Machine::GetNextStateId(const size_t _CurrentStateId, const size_t _CurrentSymbolId) const
{
	return TransitionTable[_CurrentSymbolId + _CurrentStateId * Alphabet.size()];
}

const std::vector<BMI_API::DFA::Symbol>& BMI_API::DFA::Machine::GetAlphabet() const
{
	return Alphabet;
}

const std::vector<BMI_API::DFA::State>& BMI_API::DFA::Machine::GetStates() const
{
	return States;
}

const size_t BMI_API::DFA::Machine::GetInitialStateId() const
{
	return InitialStateId;
}

void BMI_API::DFA::Machine::operator= (Machine&& _Other) noexcept
{
	Alphabet = std::move(_Other.Alphabet);
	States = std::move(_Other.States);
	InitialStateId = _Other.InitialStateId;
	TransitionTable = _Other.TransitionTable;

	_Other.InitialStateId = 0;
	_Other.TransitionTable = nullptr;
}
