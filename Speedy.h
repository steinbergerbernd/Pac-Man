#pragma once

#include "ghost.h"

class Speedy :
	public Ghost
{
public:
	Speedy(World* world);
	virtual ~Speedy(void);

	virtual void onFieldCenter();
};

