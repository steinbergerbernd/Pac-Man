#pragma once

#include "Character.h"

class PacMan : public Character
{
public:
	PacMan(World *world);
	~PacMan(void);

	void changeMode();
	bool isAIMode() const;
protected:
	virtual int getAnimationPos() const;

	void onFieldCenter();

private:
	bool aiMode;
};

