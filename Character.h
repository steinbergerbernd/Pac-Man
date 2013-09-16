#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include <ClanLib/core.h>
#include <ClanLib/display.h>

#include "GameObject.h"

class Character : public GameObject
{
public:
	Character(World *world);
	virtual ~Character(void) = 0;

	virtual void draw();
	virtual bool update(float elapsed);

	void changeDirection(int direction);
	CL_Point getCurrentField() const;
	int getMoveDirection() const;
protected:
	void setBody(CL_StringRef name, int animationGroups = 1);
	CL_Point getNextField(int moveDirection) const;
	CL_Pointf getCoordinates(const CL_Point& field) const;
	CL_Point getIndices(const CL_Pointf& field) const;
	virtual int getAnimationPos() const;
	virtual void onFieldCenter() = 0;

	CL_Sizef fieldSize;
	CL_Pointf position;
	CL_Point currentField;
	CL_Sprite body;
	int speed;
	int animationLength;
	float animationPos;
	float animationSpeed;
	int moveDirection;
	int newDirection;
private:
	void move(float distance);
};

#endif

