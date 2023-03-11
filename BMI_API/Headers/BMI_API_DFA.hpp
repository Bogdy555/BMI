#ifndef BMI_API_DFA_hpp

#define BMI_API_DFA_hpp



#include "BMI_API.hpp"



namespace BMI_API
{

	namespace DFA
	{

		enum ChunkTypes : uint8_t
		{
			_NullChunk = 0,
			_AlphabetChunk = 1,
			_StatesChunk = 2,
			_TransitionsChunk = 3
		};

		class Symbol
		{

		public:

			Symbol();
			Symbol(const Symbol& _Other);
			Symbol(Symbol&& _Other) noexcept;
			~Symbol();

			void Init(const std::wstring& _Name);
			void UnInit();

			const std::wstring& GetName() const;

			const Symbol& operator= (const Symbol& _Other);
			const Symbol& operator= (Symbol&& _Other) noexcept;

		private:

			std::wstring Name;

		};

		class State
		{

		public:

			State();
			State(const State& _Other);
			State(State&& _Other) noexcept;
			~State();

			void Init(const std::wstring& _Name, const bool _Final);
			void UnInit();

			const std::wstring& GetName() const;
			const bool IsFinal() const;

			const State& operator= (const State& _Other);
			const State& operator= (State&& _Other) noexcept;

		private:

			std::wstring Name;
			bool Final;

		};

		class Machine
		{

		public:

			Machine();
			Machine(const Machine& _Other) = delete;
			Machine(Machine&& _Other) noexcept;
			~Machine();

			const bool ComputeString(const std::wstring& _Text, std::vector<size_t>& _String) const;
			bool Run(const std::vector<size_t>& _String, std::vector<size_t>* _History = nullptr);

			bool LoadFromFile(std::wifstream& _File);
			void CleanUp();

			const bool IsInitialized() const;
			const size_t GetSymbolId(const std::wstring& _Name) const;
			const size_t GetStateId(const std::wstring& _Name) const;
			const size_t GetSymbolId(const Symbol* _Symbol) const;
			const size_t GetStateId(const State* _State) const;
			const State& GetInitialState() const;
			const size_t GetNextStateId(const size_t _CurrentStateId, const size_t _CurrentSymbolId) const;
			const std::vector<Symbol>& GetAlphabet() const;
			const std::vector<State>& GetStates() const;
			const size_t GetInitialStateId() const;

			void operator= (const Machine& _Other) = delete;
			void operator= (Machine&& _Other) noexcept;

		private:

			std::vector<Symbol> Alphabet;
			std::vector<State> States;
			size_t InitialStateId;
			size_t* TransitionTable;

		};

	}

}



#endif
