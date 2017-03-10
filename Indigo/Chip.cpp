swd#include"Basic.h"

Chip chips[ChipXMax][ChipYMax];
int chipX = ChipXMax;
int chipY = ChipYMax;
Image mapImage(ChipXMax, ChipYMax);

Chip* nowSelectedChip = NULL;
Chip* newnowSelectedChip = NULL;

Chip* temp[1000000];
void	Chip::reset()
{
	number = 0;
	isRoad = false;
	isLand = false;
	setPlace(NULL);
	flag = false;
	isForest = false;
	isFarm = false;
	growth = 0;
	isUsedByUnit = false;
	numItem = 0;
}

PerlinNoise noise(Random(2048));
void	Chip::drawGround()
{
	const Rect drawRect = getDrawRect();

	if (flag)
	{
		drawRect.draw(Palette::Red);
	}
	else if (isLand)
	{
		Color groundColor = PlainsColor;
		if (isForest) groundColor = ForestColor;
		if (isFarm) groundColor = FarmColor;
		drawRect.draw(groundColor);
	}
	else drawRect.draw(OceanColor);
}

void	Chip::drawRoad(const Color& _color, const double& _width)
{
	if (!isRoad) return;
	for (int i = 0; i < 4; i++) if (getNearChip(i).isRoad || getNearChip(i).isForest) Circle((THIS + getNearChip(i).THIS).movedBy(1, 1) / 2.0 * ChipImageSize, _width / 2.0).draw(_color);
	for (int i = 0; i < 4; i++)
	{
		if (getNearChip(i).isRoad || getNearChip(i).isForest)
		{
			for (int j = i + 1; j < 4; j++)
			{
				if (getNearChip(j).isRoad || getNearChip(j).isForest)
				{
					const Vec2 direction = Vec2(getNearChip(j).THIS - getNearChip(i).THIS).normalized()*_width / 2.0;
					Line(Vec2((THIS + getNearChip(i).THIS).movedBy(1, 1) * ChipImageSize / 2.0) + direction,
						Vec2((THIS + getNearChip(j).THIS).movedBy(1, 1) * ChipImageSize / 2.0) - direction).draw(_width, _color);
				}
			}
		}
	}
}
void resetTemp()
{
	for (auto& t : temp)
	{
		if (t == NULL)  return;
		t->flag = false;
		t->number = 0;
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
	if (!getChip(_posA).canSetRoad() || !getChip(_posB).canSetRoad()) return false;
	temp[0] = &getChip(_posA);
	int c = 1;
	for (int j = 0;; j++)
	{
		if (temp[j] == NULL) { resetTemp();  return false; }
		if (temp[j] == &getChip(_posB)) { resetTemp(); return true; }
		for (int i = 0; i < 4; i++)
		{
			if (temp[j]->getNearChip(i).canSetRoad() && !temp[j]->getNearChip(i).flag) {
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