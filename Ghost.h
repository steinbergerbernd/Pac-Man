#pragma once

#include "Character.h"

class Ghost : public Character
{
public:
	Ghost(World* world);
	virtual ~Ghost(void) = 0;

	virtual void draw();
	virtual bool update(float elapsed);
	virtual void changeMode();
	virtual bool isPursuitMode() const;
protected:
	void setEyes(CL_StringRef name);
	virtual int getAnimationPos() const;

	virtual void onFieldCenter() = 0;
	virtual void goToTargetField(CL_Point targetField);

	bool pursuitMode;

	CL_Sprite eyes;
};

