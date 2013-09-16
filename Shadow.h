#pragma once

#include "Ghost.h"

class Shadow : public Ghost
{
public:
	Shadow(World* world);
	virtual ~Shadow(void);

	virtual void onFieldCenter();
};

