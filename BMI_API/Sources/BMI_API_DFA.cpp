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

	return true;
}

bool BMI_API::DFA::Machine::Run(const std::vector<size_t>& _String, std::vector<size_t>* _History)
{
	if (!IsInitialized())
	{
		BMI_API_DEBUG_BREAK_MSG(BMI_API_STRING("Can't run the Machine, it is not initialized."));

		return false;
	}

	size_t _CurrentStateId = InitialStateId;

	if (_History)
	{
		_History->push_back(_CurrentStateId);
	}

	for (size_t _Index = 0; _Index < _String.size(); _Index++)
	{
		_CurrentStateId = GetNextStateId(_CurrentStateId, _String[_Index]);

		if (_CurrentStateId == States.size())
		{
			return false;
		}

		if (_History)
		{
			_History->push_back(_CurrentStateId);
		}
	}

	return States[_CurrentStateId].IsFinal();
}

bool BMI_API::DFA::Machine::LoadFromFile(std::wifstream& _File)
{
	CleanUp();

	bool _FoundInitialState = false;
	uint8_t _CurrentChunk = _NullChunk;
	std::vector<uint8_t> _FoundChunks;
	std::vector<std::vector<std::wstring>> _Transitions;

	wchar_t _Line[BMI_API_MAX_LINE_SIZE + 1];

	while (!_File.eof())
	{
		_File.getline(_Line, BMI_API_MAX_LINE_SIZE);

		std::vector<std::wstring> _TokensVec;

		for (const wchar_t* _Token = String::TokenizeWhiteSpace(_Line); _Token != nullptr; _Token = String::TokenizeWhiteSpace(nullptr))
		{
			_TokensVec.push_back(_Token);
		}

		if (!_TokensVec.size())
		{
			continue;
		}

		if (_TokensVec[0][0] == L'#')
		{
			continue;
		}

		if (_TokensVec[0] == L"->")
		{
			if (_TokensVec.size() != 2)
			{
				CleanUp();

				return false;
			}

			if (_TokensVec[1] == L"Alphabet")
			{
				for (size_t _Index = 0; _Index < _FoundChunks.size(); _Index++)
				{
					if (_FoundChunks[_Index] == _AlphabetChunk)
					{
						CleanUp();
						return false;
					}
				}

				_FoundChunks.push_back(_AlphabetChunk);
				_CurrentChunk = _AlphabetChunk;

				continue;
			}

			if (_TokensVec[1] == L"States")
			{
				for (size_t _Index = 0; _Index < _FoundChunks.size(); _Index++)
				{
					if (_FoundChunks[_Index] == _StatesChunk)
					{
						CleanUp();
						return false;
					}
				}

				_FoundChunks.push_back(_StatesChunk);
				_CurrentChunk = _StatesChunk;

				continue;
			}

			if (_TokensVec[1] == L"Transitions")
			{
				for (size_t _Index = 0; _Index < _FoundChunks.size(); _Index++)
				{
					if (_FoundChunks[_Index] == _TransitionsChunk)
					{
						CleanUp();
						return false;
					}
				}

				_FoundChunks.push_back(_TransitionsChunk);
				_CurrentChunk = _TransitionsChunk;

				continue;
			}

			CleanUp();

			return false;
		}

		switch (_CurrentChunk)
		{
		case _NullChunk:
		{
			CleanUp();
			return false;
		}
		case _AlphabetChunk:
		{
			if (_TokensVec.size() != 1)
			{
				CleanUp();
				return false;
			}

			for (size_t _Index = 0; _Index < Alphabet.size(); _Index++)
			{
				if (Alphabet[_Index].GetName() == _TokensVec[0])
				{
					CleanUp();
					return false;
				}
			}

			Alphabet.push_back(Symbol());

			Alphabet[Alphabet.size() - 1].Init(_TokensVec[0]);

			continue;
		}
		case _StatesChunk:
		{
			if (_TokensVec.size() != 2 && _TokensVec.size() != 3)
			{
				CleanUp();
				return false;
			}

			for (size_t _Index = 0; _Index < States.size(); _Index++)
			{
				if (States[_Index].GetName() == _TokensVec[0])
				{
					CleanUp();
					return false;
				}
			}

			bool _Final = false;
			bool _Initial = false;

			if (_TokensVec.size() == 2)
			{
				if (_TokensVec[1] == L"F")
				{
					_Final = true;
				}
				else
				{
					if (_TokensVec[1] == L"S")
					{
						_Initial = true;
					}
					else
					{
						CleanUp();
						return false;
					}
				}
			}

			if (_TokensVec.size() == 3)
			{
				if (_TokensVec[1] == _TokensVec[2])
				{
					CleanUp();
					return false;
				}

				if (_TokensVec[1] != L"F" && _TokensVec[1] != L"S")
				{
					CleanUp();
					return false;
				}

				if (_TokensVec[2] != L"F" && _TokensVec[2] != L"S")
				{
					CleanUp();
					return false;
				}

				_Initial = true;
				_Final = true;
			}

			if (_Initial)
			{
				if (_FoundInitialState)
				{
					CleanUp();
					return false;
				}

				_FoundInitialState = true;
				InitialStateId = States.size();
			}

			States.push_back(State());

			States[States.size() - 1].Init(_TokensVec[0], _Final);

			continue;
		}
		case _TransitionsChunk:
		{
			if (_TokensVec.size() != 3)
			{
				CleanUp();
				return false;
			}

			for (size_t _Index = 0; _Index < _Transitions.size(); _Index++)
			{
				if (_Transitions[_Index][0] == _TokensVec[0] && _Transitions[_Index][1] == _TokensVec[1])
				{
					CleanUp();
					return false;
				}
			}

			_Transitions.push_back(std::vector<std::wstring>());

			_Transitions[_Transitions.size() - 1].push_back(_TokensVec[0]);
			_Transitions[_Transitions.size() - 1].push_back(_TokensVec[1]);
			_Transitions[_Transitions.size() - 1].push_back(_TokensVec[2]);

			continue;
		}
		default:
		{
			CleanUp();
			return false;
		}
		}

		CleanUp();

		return false;
	}

	if (!Alphabet.size() || !States.size() || !_FoundInitialState || _FoundChunks.size() != 3)
	{
		CleanUp();
		return false;
	}

	for (size_t _Index = 0; _Index < _Transitions.size(); _Index++)
	{
		if (_Transitions[_Index].size() != 3)
		{
			CleanUp();
			return false;
		}

		bool _FoundInStates1 = false;
		bool _FoundInAlphabet = false;
		bool _FoundInStates2 = false;

		for (size_t _StatesIndex = 0; _StatesIndex < States.size(); _StatesIndex++)
		{
			if (States[_StatesIndex].GetName() == _Transitions[_Index][0])
			{
				_FoundInStates1 = true;
				break;
			}
		}

		for (size_t _AlphabetIndex = 0; _AlphabetIndex < Alphabet.size(); _AlphabetIndex++)
		{
			if (Alphabet[_AlphabetIndex].GetName() == _Transitions[_Index][1])
			{
				_FoundInAlphabet = true;
				break;
			}
		}

		for (size_t _StatesIndex = 0; _StatesIndex < States.size(); _StatesIndex++)
		{
			if (States[_StatesIndex].GetName() == _Transitions[_Index][2])
			{
				_FoundInStates2 = true;
				break;
			}
		}

		if (!_FoundInStates1 || !_FoundInAlphabet || !_FoundInStates2)
		{
			CleanUp();
			return false;
		}
	}

	size_t _TransitionTableSize = Alphabet.size() * States.size();

	TransitionTable = new size_t[_TransitionTableSize];

	if (!TransitionTable)
	{
		CleanUp();
		return false;
	}

	for (size_t _Index = 0; _Index < _TransitionTableSize; _Index++)
	{
		TransitionTable[_Index] = States.size();
	}

	for (size_t _Index = 0; _Index < _Transitions.size(); _Index++)
	{
		TransitionTable[GetSymbolId(_Transitions[_Index][1]) + GetStateId(_Transitions[_Index][0]) * Alphabet.size()] = GetStateId(_Transitions[_Index][2]);
	}

	return true;
}

void BMI_API::DFA::Machine::CleanUp()
{
	if (Alphabet.size())
	{
		Alphabet.clear();
	}
	if (States.size())
	{
		States.clear();
	}
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
