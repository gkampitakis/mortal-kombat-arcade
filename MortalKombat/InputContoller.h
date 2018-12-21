#include "MyLibraries.h"

namespace input {

	using key_combination = std::list<int>;

	extern bool test_key(const int keyCode);

	inline bool test_keys(const key_combination& keys) {
		for (auto& key : keys)
			if (!test_key(key))
				return false;
		return true;
	}

	class InputController final {
	public:
		using Logical = std::set<std::string>;
	private:
		using Actions = std::list<std::pair<input::key_combination, std::string>>;
		using All = std::list<InputController*>;
		Actions actions;
		Logical logical;
		static All all;		// TODO: define it in a cpp file!	
		void SetLogical(const std::string& id) { logical.insert(id); }
	public:
		void AddAction(const input::key_combination& keys, const std::string& logical)
		{
			actions.push_back(std::make_pair(keys, logical));
		}
		void Handle(void) {
			logical.clear();
			for (auto& i : actions)
				if (input::test_keys(i.first)) {
					SetLogical(i.second);//How to handle ambiguity
					//cout << i.second <<"\n";//for debugging the input controller
				}	
		}
		const Logical& GetLogical(void) const
		{
			return logical;
		}
		static void HandleAll(void) {
			for (auto* handler : all)
				handler->Handle();
		}

		InputController(void) { all.push_back(this); }
		~InputController() { all.remove(this); }
	};

}