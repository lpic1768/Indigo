#include"Basic.h"
void Unit::reset()
{
	this->items.reset();
	this->timerNow = 0;
	this->timerMax = 0;
	this->pos = Point(0, 0);
	this->addPos = Vec2(0, 0);
}
Unit units[UNIT_MAX];