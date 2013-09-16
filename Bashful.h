#pragma once
#include "ghost.h"
class Bashful : public Ghost
{
public:
	Bashful(World* world);
	virtual ~Bashful(void);

	virtual void onFieldCenter();
};

