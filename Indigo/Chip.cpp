#include"Basic.h"

Chip chips[ChipXMax][ChipYMax];
int chipX = ChipXMax;
int chipY = ChipYMax;
double sec = 0.0;
Image mapImage(ChipXMax, ChipYMax);

Chip* nowSelectedChip = NULL;
Chip* newnowSelectedChip = NULL;

Chip* temp[1000000];
void	Chip::reset()
{
	this->number = 0;
	this->isRoad = false;
	this->isLand = false;
	setPlace(NULL);
	this->flag = false;
}
PerlinNoise noise(Random(2048));
void	Chip::drawGround()
{
	const Rect drawRect = getDrawRect();
	if (flag) drawRect.draw(Palette::Red);
	else if (isLand) drawRect.draw(Palette::Green);
	else drawRect.draw(Palette::Blue);
}
void resetTemp()
{
	for (auto& t : temp)
	{
		if (t == NULL)  return;
		t->flag = false;
		t = NULL;
	}
}
bool isConnectedByRoad(const Point& _posA, const Point& _posB)
{
	if (!getChip(_posA).isRoad || !getChip(_posB).isRoad) return false;
	temp[0] = &getChip(_posA);
	int c = 1;
	for (int j = 0;; j++)
	{
		if (temp[j] == NULL) { resetTemp();  return false; }
		if (temp[j] == &getChip(_posB)) { resetTemp(); return true; }
		for (int i = 0; i < 4; i++)
		{
			if (temp[j]->getNearChip(i).isRoad && !temp[j]->getNearChip(i).flag) {
				temp[c] = &temp[j]->getNearChip(i);
				temp[c]->flag = true;
				c++;
			}
		}
	}
}
bool isConnectedByLand(const Point& _posA, const Point& _posB)
{
	if (!getChip(_posA).isLand || !getChip(_posB).isLand) return false;
	temp[0] = &getChip(_posA);
	int c = 1;
	for (int j = 0;; j++)
	{
		if (temp[j] == NULL) { resetTemp();  return false; }
		if (temp[j] == &getChip(_posB)) { resetTemp(); return true; }
		for (int i = 0; i < 4; i++)
		{
			if (temp[j]->getNearChip(i).isLand && temp[j]->getNearChip(i).getPlace() == NULL && !temp[j]->getNearChip(i).flag) {
				temp[c] = &temp[j]->getNearChip(i);
				temp[c]->flag = true;
				c++;
			}
		}
	}
}
void setRoadAToB(const Point& _posA, const Point& _posB)
{
	if (!isConnectedByLand(_posA, _posB)) return;
	Chip& goal = getChip(_posB);
	int j = 3, c = 0;
	goal.number = 1;
	goal.flag = true;
	temp[c] = &goal;
	c++;
	for (int i = 0; i < 4; i++)
	{
		if (!goal.getNearChip(i).canSetRoad()) continue;
		temp[c] = &goal.getNearChip(i);
		temp[c]->ang = (i + 2) % 4;
		c++;
		goal.getNearChip(i).flag = true;
	}

	for (;;)
	{
		for (int i = 0; i < c; i++)
		{
			if (temp[i] == NULL) break;
			if (temp[i]->number != 0) continue;
			for (int k = 0; k < 4; k++)
			{
				if (temp[i]->getNearChip(k).number != 0)
				{
					if ((temp[i]->getNearChip(k).ang == k && j >= 3 + temp[i]->getNearChip(k).number) ||
						(temp[i]->getNearChip(k).ang != k && j >= 2 + temp[i]->getNearChip(k).number))
					{
						temp[i]->number = j;
						temp[i]->ang = k;
						for (int o = 0; o < 4; o++)
						{
							if (temp[i]->getNearChip(o).canSetRoad() && !temp[i]->getNearChip(o).flag)
							{
								temp[c] = &(temp[i]->getNearChip(o));
								temp[c]->flag = true;
								c++;
							}
						}
						break;
					}
				}
			}
		}
		if (getChip(_posA).number != 0) break;
		j++;
	}
	//ƒ‹[ƒg‚ð“o˜^
	Chip* now = &getChip(_posA);
	for (;;)
	{
		now->isRoad = true;
		mapImage[now->THIS.y][now->THIS.x] = RoadColor;
		if (now == &goal) break;
		now = &now->getNearChip(now->ang);
	}
	mapTexture = Texture(mapImage);	//Texture‚É”½‰f
	//number‚Æflag‚Ì‰Šú‰»
	for (auto& t : temp)
	{
		if (t == NULL) break;
		t->number = 0;
		t->flag = false;
		t = NULL;
	}
}
void drawPlannedRoadAToB(const Point& _posA, const Point& _posB)
{
	if (!isConnectedByLand(_posA, _posB)) return;
	Chip& goal = getChip(_posB);
	int j = 3, c = 0;
	goal.number = 1;
	goal.flag = true;
	temp[c] = &goal;
	c++;
	for (int i = 0; i < 4; i++)
	{
		if (!goal.getNearChip(i).canSetRoad()) continue;
		temp[c] = &goal.getNearChip(i);
		temp[c]->ang = (i + 2) % 4;
		c++;
		goal.getNearChip(i).flag = true;
	}

	for (;;)
	{
		for (int i = 0; i < c; i++)
		{
			if (temp[i] == NULL) break;
			if (temp[i]->number != 0) continue;
			for (int k = 0; k < 4; k++)
			{
				if (temp[i]->getNearChip(k).number != 0)
				{
					if ((temp[i]->getNearChip(k).ang == k && j >= 3 + temp[i]->getNearChip(k).number) ||
						(temp[i]->getNearChip(k).ang != k && j >= 2 + temp[i]->getNearChip(k).number))
					{
						temp[i]->number = j;
						temp[i]->ang = k;
						for (int o = 0; o < 4; o++)
						{
							if (temp[i]->getNearChip(o).canSetRoad() && !temp[i]->getNearChip(o).flag)
							{
								temp[c] = &(temp[i]->getNearChip(o));
								temp[c]->flag = true;
								c++;
							}
						}
						break;
					}
				}
			}
		}
		if (getChip(_posA).number != 0) break;
		j++;
	}
	//ƒ‹[ƒg‚ð•`‰æ
	Chip* now = &getChip(_posA);
	for (;;)
	{
		now->getDrawRect().draw(Color(255, 0, 0, 128));
		if (now == &goal) break;
		now = &now->getNearChip(now->ang);
	}
	//number‚Æflag‚Ì‰Šú‰»
	for (auto& t : temp)
	{
		if (t == NULL) break;
		t->number = 0;
		t->flag = false;
		t = NULL;
	}
}
Chip& getChip(const Point& _pos) { return chips[_pos.x][_pos.y]; }
Chip& getChip(const int& _x, const int& _y) { return chips[_x][_y]; }
Chip&	Chip::getNearChip(const int& _r)
{
	switch (_r % 4)
	{
	case 0: return getChip(THIS.movedBy(-1, 0));
	case 1: return getChip(THIS.movedBy(0, -1));
	case 2: return getChip(THIS.movedBy(1, 0));
	case 3: return getChip(THIS.movedBy(0, 1));
	}
	return chips[0][0];
}