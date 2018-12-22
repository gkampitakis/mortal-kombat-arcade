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

		void	PerformDefaultTransition(void) {
			auto j(table.find(state + ".*"));
			if (j != table.end())
				j->second();
		}

		void	FireTransitions(const std::set<std::string>& keys) {

			// due to lexicographic ordering a fired input '<state-i>.a.b' precedes
			// an extended input '<state-i>.a.b.c', and fires before it as required

			for (auto& key : keys) {
				auto i(table.find(key));
				assert(i != table.end());
				i->second();
			}
		}

		void	InsertByRetainingMaximalEdges(std::set<std::string>& keys, const std::string& fired) {
			for (auto& key : keys) {
				if (key.find(fired) != std::string::npos)	// substring of existing key, ignore fired
					return;
				if (fired.find(key) != std::string::npos)	// superstring of existing key, ignore key
				{
					keys.erase(key); break;
				}
			}
			keys.insert(fired);
		}

	public:
		template <typename Tfunc>
		StateTransitions&	SetTransition(
			const std::string&	from,
			const Input&		input,
			const Tfunc&		f
		) {
			if (!input.empty()) {
				table[from + MakeKey(input)] = f;
				inputs[from].push_back(input);
			}
			else
				table[from + ".*"] = f;
			return *this;
		}

		StateTransitions&	SetState(const std::string& newState)
		{
			state = newState; return*this;
		}

		StateTransitions&	PerformTransitions(const Input& input, bool useMaximalEdges) {

			auto	i(inputs.find(state));
			assert(i != inputs.end());
			auto&	edges(i->second);

			std::set<std::string> fired;

			for (auto& edge : edges)
				if (std::includes(edge.begin(), edge.end(), input.begin(), input.end())) {	// is subset
					auto j(table.find(state + MakeKey(edge)));
					assert(j != table.end());
					auto& key(j->first);
					if (useMaximalEdges)
						InsertByRetainingMaximalEdges(fired, key);
					else
						fired.insert(key);
				}

			if (fired.empty() || input.empty())
				PerformDefaultTransition();
			else
				FireTransitions(fired);
			return *this;
		}

		const std::string&	GetState(void) const { return state; }

		StateTransitions(StateTransitions&&) = delete;
		StateTransitions(const StateTransitions&) = delete;
		StateTransitions(void) = default;
	};

}