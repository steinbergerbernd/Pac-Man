#ifndef _WORLD_H_
#define _WORLD_H_

#include <list>

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "PacMan.h"
#include "Level.h"
#include "Shadow.h"
#include "Speedy.h"
#include "Bashful.h"
#include "Pokey.h"

class World
{
public:
	World(CL_DisplayWindow &display_window);
	~World(void);

//Attribute
public:
	CL_ResourceManager resources;
	CL_GraphicContext get_gc() { return graphicContext; }

//Operationen
public:
	void run();

	void addObject(GameObject *object);

	Level* getLevel();
	PacMan* getPacMan();
	Shadow* getShadow();
	Speedy* getSpeedy();
	Bashful* getBashful();
	Pokey* getPokey();

//Implementierung
private:
	void draw();
	void update();

	float getElapsedTime();

	void onWindowClose();
	void onWindowResize(int width, int height);
	void onMouseDown(const CL_InputEvent &key, const CL_InputState &state);
	void onMouseMove(const CL_InputEvent &key, const CL_InputState &state);
	void onKeyDown(const CL_InputEvent &key, const CL_InputState &state);

	CL_DisplayWindow window;
	CL_GraphicContext graphicContext;

	std::list<GameObject*> objects;

	PacMan* pacMan;
	Level* level;

	Shadow* shadow;
	Speedy* speedy;
	Bashful* bashful;
	Pokey* pokey;

	float durationPursuitMode;
	float durationPatrolMode;
	float currentDuration;

	bool quit;
};

#endif

