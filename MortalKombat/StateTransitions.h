#include <assert.h>

namespace logic {

	class StateTransitions final {
	public:
		using Input = std::set<std::string>;
	private:
		using Inputs = std::map<std::string, std::list<Input>>;
		using Table = std::map<std::string, std::function<void(void)>>;
		Table			table;
		Inputs			inputs;
		std::string		state;

		const std::string	MakeKey(const Input& input) const {
			std::string result;
			for (auto& s : input)
				result += "." + s;
			return result;
		}

		void				PerformDefaultTransition(void) {
			auto j(table.find(state + ".*"));
			if (j != table.end())
				j->second();
		}

	public:
		template <typename Tfunc>
		StateTransitions&	SetTransition(
			const std::string&	from,
			const Input&		input,
			const Tfunc&		f
		) {
			table[from + MakeKey(input)] = std::make_pair(input, f);
			inputs[from].push_back(input);
		}

		void				SetState(const std::string& newState)
		{
			state = newState;
		}

		void				PerformTransition(const Input& input) {
			auto  i(inputs.find(state));
			auto  found(false);
			for (auto& edge : i->second)
				if (edge <= input) {
					auto j(table.find(state + MakeKey(edge)));
					assert(j != table.end());
					j->second();
					found = true;
				}
			if (!found)
				PerformDefaultTransition();
		}

		const std::string&	GetState(void) const { return state; }

		StateTransitions(StateTransitions&&) = delete;
		StateTransitions(const StateTransitions&) = delete;
		StateTransitions(void) = default;
	};

}