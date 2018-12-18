#include "Fighter.h"
#include "AnimationFilmHolder.h"

Fighter::Fighter(string Name) {
	name = Name;
};

bool Fighter::initialize(const string& path) {
	try {
		json config;
		std::ifstream config_file(path, std::ifstream::binary);
		config_file >> config;
		//cout << config;
		input::key_combination keycodes;
		for (json::iterator it = config["combinations"].begin(); it != config["combinations"].end(); it++) {
			keycodes.clear();
			int i = 0;
			json tmp = *it;
			string action = tmp["action"];
			for (json::iterator i = tmp["key_strokes"].begin(); i != tmp["key_strokes"].end(); i++) {
				keycodes.push_back(*i);
			}
			Fighter::inputController.AddAction(keycodes, action);
		}
		return true;//true or false catch here
	}
	catch (const std::exception& e) {
		cerr << e.what();
		return false;

	}
};

void Fighter::Draw(SDL_Surface& gScreenSurface,string test,Rect t) {
	/*
	* TEMP CODE HERE
	*/
	AnimationFilm* tmp = AnimationFilmHolder::Get()->GetFilm(test);
	//{0 , 0 } coordinates

	//tmp->DisplayFrame(gScreenSurface, { 500,500 }, 4, 200, 450);//100 x100 is the size of the player
	tmp->DisplayFrame(gScreenSurface, { t.x,t.y }, 4, t.w, t.h);//100 x100 is the size of the player


//Not working
//Fighter::stateTransitions.SetTransition(Fighter::stateTransitions.GetState(), Fighter::inputController.GetLogical(), []() {cout << "DAB"; });
}