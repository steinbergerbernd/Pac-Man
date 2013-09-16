#pragma once
#include "ghost.h"
class Pokey :
	public Ghost
{
public:
	Pokey(World* world);
	virtual ~Pokey(void);

	virtual void onFieldCenter();
};

