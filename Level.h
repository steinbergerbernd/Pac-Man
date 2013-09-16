#pragma once

#include <string>

#include "GameObject.h"

using std::string;
using std::map;

class Level : public GameObject
{
public:
	Level(World* world, string filename, CL_Rect viewPort);
	~Level(void);

	void draw();
	bool update(float elapsed);

	void setViewPort(int width, int height);

	CL_Sizef getFieldSize() const;
	CL_Point getStartPosition() const;

	bool isFreeField(CL_Point field) const;
	bool takeDotOnField(CL_Point field);
private:
	void initLevel(string filename);

	map<char, CL_Texture> textures;
	string levelFields;
	
	CL_Size levelSize;
	CL_Point startPosition;

	CL_Rect viewPort;
	CL_Sizef fieldSize;
};

