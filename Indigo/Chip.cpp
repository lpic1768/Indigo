#include"Header.h"

Chip& getChip(const int& _i)
{
	return chips[_i % CHIP_X_MAX][_i / CHIP_Y_MAX];
}
Chip& getChip(const Vec2& _pos)
{
	return chips[(int)(_pos.x / CHIP_SIZE)][(int)(_pos.y / CHIP_SIZE)];
}
void Chip::debugDraw()
{
	const Point pos(Pos*CHIP_SIZE);
	const Point size(CHIP_SIZE, CHIP_SIZE);
	if (Rect(pos, size).mouseOver) { mouseOveredChip = this; }
	Color landColor;
	switch (BiomeType)
	{
	case Ocean:
		break;
	case Plains:
		landColor = Palette::Green;
		break;
	case Desert:
		landColor = Palette::Yellow;
		break;
	case Forest:
		landColor = Palette::Darkgreen;
		break;
	case Mountain:
		landColor = Palette::Gray;
		break;
	default:
		landColor = Palette::White;
		break;
	}
	//if (JoinedRegion == NULL) landColor = Palette::Red;
	if (!IsLand) return;
	if (IsInLand) Rect(pos, size).draw(landColor);
	else
	{
		Color color(239, 228, 176);
		const Vec2 topLeft(Pos*CHIP_SIZE);
		const Vec2 topRight((Pos*CHIP_SIZE).movedBy(CHIP_SIZE, 0));
		const Vec2 downLeft((Pos*CHIP_SIZE).movedBy(0, CHIP_SIZE));
		const Vec2 downRight((Pos*CHIP_SIZE).movedBy(CHIP_SIZE, CHIP_SIZE));

		bool flag = true;
		for (int i = 0; i < 4; i++) if (!getNearChip(i).IsLand) { flag = false; break; }
		if (flag) Rect(pos, size).draw(landColor);
		color = landColor;
		if (getNearChip(0).IsLand && !getNearChip(0).IsInLand && getNearChip(2).IsLand && !getNearChip(2).IsInLand) { Rect(pos, size).draw(color); }
		if (getNearChip(1).IsLand && !getNearChip(1).IsInLand && getNearChip(3).IsLand && !getNearChip(3).IsInLand) { Rect(pos, size).draw(color); }
		if (getNearChip(0).IsLand && !getNearChip(0).IsInLand && getNearChip(1).IsLand && !getNearChip(1).IsInLand) Triangle(topLeft, topRight, downLeft).draw(color);
		if (getNearChip(1).IsLand && !getNearChip(1).IsInLand && getNearChip(2).IsLand && !getNearChip(2).IsInLand) Triangle(topLeft, topRight, downRight).draw(color);
		if (getNearChip(2).IsLand && !getNearChip(2).IsInLand && getNearChip(3).IsLand && !getNearChip(3).IsInLand) Triangle(downRight, downLeft, topRight).draw(color);
		if (getNearChip(3).IsLand && !getNearChip(3).IsInLand && getNearChip(0).IsLand && !getNearChip(0).IsInLand) Triangle(downRight, downLeft, topLeft).draw(color);
	}

}
void Chip::drawRoad()
{
	if (!IsLand) return;
	const Point pos(Pos*CHIP_SIZE);
	const Point size(CHIP_SIZE, CHIP_SIZE);
	if (RoadLevel == 1)
	{
		Color roadColor = Palette::Darkkhaki;
		const int len = 8;
		bool flag = true;
		for (int i = 0; i < 4; i++)
		{
			if (getNearChip(i).RoadLevel == 1)
			{
				Circle((getCenterPos() + getNearChip(i).getCenterPos()) / 2, CHIP_SIZE / 2 - len).draw(roadColor);
				flag = false;
				for (int j = i + 1; j < 4; j++)
				{
					if (getNearChip(j).RoadLevel == 1)
					{
						Vec2 l1 = (getCenterPos() + getNearChip(i).getCenterPos()) / 2.0;
						Vec2 l2 = (getCenterPos() + getNearChip(j).getCenterPos()) / 2.0;
						Line(l1.movedBy((l2 - l1).normalized()*(CHIP_SIZE - len * 2) / 2), l2.movedBy((l1 - l2).normalized()*(CHIP_SIZE - len * 2) / 2)).draw(CHIP_SIZE - len * 2, roadColor);
					}
				}
			}
		}
		if (flag) Circle(pos + size / 2, CHIP_SIZE / 2 - len).draw(roadColor);
	}
}

/*
0 : 左
1 : 上
2 : 右
3 : 下
*/
Chip& Chip::getNearChip(const int& _value, const bool& _flag)
{
	if (_flag)
	{
		switch (_value)
		{
		case 0: return getChip(Point(Pos.x - 1, Pos.y));
		case 1: return getChip(Point(Pos.x - 1, Pos.y - 1));
		case 2: return getChip(Point(Pos.x, Pos.y - 1));
		case 3: return getChip(Point(Pos.x + 1, Pos.y - 1));
		case 4: return getChip(Point(Pos.x + 1, Pos.y));
		case 5: return getChip(Point(Pos.x + 1, Pos.y + 1));
		case 6: return getChip(Point(Pos.x, Pos.y + 1));
		case 7: return getChip(Point(Pos.x - 1, Pos.y + 1));
		}

	}
	else
	{
		switch (_value)
		{
		case 0: return getChip(Point(Pos.x - 1, Pos.y));
		case 1: return getChip(Point(Pos.x, Pos.y - 1));
		case 2: return getChip(Point(Pos.x + 1, Pos.y));
		case 3: return getChip(Point(Pos.x, Pos.y + 1));
		}
	}
	return *this;
}

bool Chip::canGetNearChip(const int& _value)
{
	switch (_value)
	{
	case 0: return Pos.x != 0;
	case 1: return Pos.y != 0;
	case 2: return Pos.x != CHIP_X_MAX - 1;
	case 3: return Pos.y != CHIP_Y_MAX - 1;
	}
	return false;
}
Vec2	Chip::getCenterPos()
{
	return Vec2((Pos*CHIP_SIZE).movedBy(CHIP_SIZE / 2, CHIP_SIZE / 2));
}
Chip&	Chip::movedBy(const int& _x, const int& _y)
{
	return getChip(Pos.movedBy(_x, _y));
}
Chip&	Chip::movedBy(const Point& _pos)
{
	return getChip(Pos.movedBy(_pos.x, _pos.y));
}
Chip& getChip(const Point& _pos)
{
	return chips[_pos.x][_pos.y];
}
bool	Chip::isFMarket()	//立っている場所がマーケットかどうか
{
	if (getOwnFacility() == NULL || getOwnFacility()->BuildProgress != 0 || getOwnFacility()->Type != Market) return false;
	else return true;
}
void	setRoad(Chip* _pos1, Chip* _pos2, const bool _roadLevelFlag)
{
	if (_pos1 == _pos2)
	{
		_pos1->RoadLevel = 1;
		return;
	}
	if (!_pos1->isConeected(_pos2)) return;
	bool flag3 = false;
	if (_pos1->getJoinedRegion() == _pos2->getJoinedRegion()) flag3 = true;
	//ルート検索プログラム
	Chip& goal = *_pos2;
	int j = 10;
	int c = 0;
	int numRootChips = 0;
	goal.RootNumber = 1;
	goal.Flag = true;
	rootChips[c] = &goal;
	c++;
	numRootChips++;
	for (int i = 0; i < 4; i++)
	{
		if (!goal.getNearChip(i).IsRoad) continue;
		rootChips[c] = &goal.getNearChip(i);
		c++; numRootChips++;
		goal.getNearChip(i).Flag = true;
	}
	int xMin, yMin, xMax, yMax;
	xMin = goal.Pos.x - 2;
	yMin = goal.Pos.y - 2;
	xMax = goal.Pos.x + 2;
	yMax = goal.Pos.y + 2;

	for (;;)
	{
		for (int i = 0; i < numRootChips; i++)
		{
			if (rootChips[i] == NULL) continue;
			for (int k = 0; k < 4; k++)
			{
				if (rootChips[i]->getNearChip(k).RootNumber != 0)
				{
					if (rootChips[i]->getNearChip(k).RoadAng == k)	//距離は10
					{
						if (j >= 3 + 3 * rootChips[i]->getNearChip(k).RoadLevel*_roadLevelFlag + rootChips[i]->getNearChip(k).RootNumber)
						{
							rootChips[i]->RootNumber = j;
							rootChips[i]->RoadAng = k;
							for (int o = 0; o < 4; o++)
							{
								if (rootChips[i]->getNearChip(o).IsRoad &&
									!rootChips[i]->getNearChip(o).Flag &&
									(!flag3 || rootChips[i]->getNearChip(o).getJoinedRegion() == _pos1->getJoinedRegion()))
								{
									rootChips[c] = &(rootChips[i]->getNearChip(o));
									c++;
									numRootChips++;
								}
							}
							for (int o = 0; o < 4; o++) rootChips[i]->getNearChip(o).Flag = true;
							if (xMin == rootChips[i]->Pos.x) xMin--;
							if (yMin == rootChips[i]->Pos.y) yMin--;
							if (xMax == rootChips[i]->Pos.x) xMax++;
							if (yMax == rootChips[i]->Pos.y) yMax++;
							rootChips[i] = NULL;
							break;
						}
					}
					else //距離は7
					{
						if (j >= 2 + 2 * rootChips[i]->getNearChip(k).RoadLevel*_roadLevelFlag + rootChips[i]->getNearChip(k).RootNumber)
						{
							rootChips[i]->RootNumber = j;
							rootChips[i]->RoadAng = k;
							for (int o = 0; o < 4; o++)
							{
								if (rootChips[i]->getNearChip(o).IsRoad &&
									!rootChips[i]->getNearChip(o).Flag &&
									(!flag3 || rootChips[i]->getNearChip(o).getJoinedRegion() == _pos1->getJoinedRegion()))
								{
									rootChips[c] = &(rootChips[i]->getNearChip(o));
									c++;
									numRootChips++;
								}
							}
							for (int o = 0; o < 4; o++) rootChips[i]->getNearChip(o).Flag = true;
							if (xMin == rootChips[i]->Pos.x) xMin--;
							if (yMin == rootChips[i]->Pos.y) yMin--;
							if (xMax == rootChips[i]->Pos.x) xMax++;
							if (yMax == rootChips[i]->Pos.y) yMax++;
							rootChips[i] = NULL;
							break;
						}
					}

				}
			}
		}
		if (_pos1->RootNumber != 0) break;
		j++;
	}
	//ルートを登録
	Chip* now = _pos1;
	for (;;)
	{
		//if(now->IsRoad) 
		now->RoadLevel = 1;
		now = &now->getNearChip(now->RoadAng);
		if (now == &goal) break;
	}
	_pos2->RoadLevel = 1;
	for (int x = xMin - 2; x <= xMax + 2; x++)
	{
		for (int y = yMin - 2; y <= yMax + 2; y++)
		{
			chips[x][y].RootNumber = 0;
			chips[x][y].Flag = false;
		}
	}
	/*
	for (int x = xMin - 2; x <= xMax + 2; x++)
	{
		for (int y = yMin - 2; y <= yMax + 2; y++)
		{
			if (chips[x][y].RoadLevel != 1 || chips[x][y].getOwnFacility() != NULL) continue;
			for (int k = 0; k < 8; k++)
			{
				bool flag1 = false;
				bool flag2 = true;
				int count = 0;
				for (int i = 0; i < 8; i++)
				{
					if (chips[x][y].getNearChip((i + k) % 8, true).RoadLevel == 1)
					{
						count++;
						flag1 = true;
					}
					else
					{
						if (flag1 == true) { flag2 = false; break; }
					}
				}
				if (count >= 3 && flag2) { chips[x][y].RoadLevel = 3; break; }
			}
		}
	}
	*/
	updateMapImage(xMin - 2, yMin - 2, xMax + 2, yMax + 2);
}
void	Facility::setRoadToForest()
{
	//ルート検索プログラム
	Chip& goal = *getEntranceChip();
	Chip* pos = NULL;
	int j = 10;
	int c = 0;
	int numRootChips = 0;
	goal.RootNumber = 1;
	goal.Flag = true;
	rootChips[c] = &goal;
	c++;
	numRootChips++;
	for (int i = 0; i < 4; i++)
	{
		if (!goal.getNearChip(i).IsRoad) continue;
		rootChips[c] = &goal.getNearChip(i);
		c++; numRootChips++;
		goal.getNearChip(i).Flag = true;
	}
	int xMin, yMin, xMax, yMax;
	xMin = goal.Pos.x - 2;
	yMin = goal.Pos.y - 2;
	xMax = goal.Pos.x + 2;
	yMax = goal.Pos.y + 2;

	for (;;)
	{
		for (int i = 0; i < numRootChips; i++)
		{
			if (rootChips[i] == NULL) continue;
			for (int k = 0; k < 4; k++)
			{
				if (rootChips[i]->getNearChip(k).RootNumber != 0)
				{
					if (rootChips[i]->getNearChip(k).RoadAng == k)	//距離は10
					{
						if (j >= 10 * rootChips[i]->getNearChip(k).RoadLevel + rootChips[i]->getNearChip(k).RootNumber)
						{
							rootChips[i]->RootNumber = j;
							rootChips[i]->RoadAng = k;
							if (rootChips[i]->BiomeType == Forest) { pos = rootChips[i]; break; }
							Chip* a = NULL;
							int o = 0;
							for (; o < 4; o++)
								if (rootChips[i]->getNearChip(o).IsRoad && !rootChips[i]->getNearChip(o).Flag) { a = &(rootChips[i]->getNearChip(o)); o++; break; }
							for (; o < 4; o++)
								if (rootChips[i]->getNearChip(o).IsRoad && !rootChips[i]->getNearChip(o).Flag) { rootChips[c] = &(rootChips[i]->getNearChip(o)); c++; numRootChips++; }
							for (o = 0; o < 4; o++) rootChips[i]->getNearChip(o).Flag = true;
							if (xMin == rootChips[i]->Pos.x) xMin--;
							if (yMin == rootChips[i]->Pos.y) yMin--;
							if (xMax == rootChips[i]->Pos.x) xMax++;
							if (yMax == rootChips[i]->Pos.y) yMax++;
							rootChips[i] = a;
							break;
						}
					}
					else //距離は7
					{
						if (j >= 7 * rootChips[i]->getNearChip(k).RoadLevel + rootChips[i]->getNearChip(k).RootNumber)
						{
							rootChips[i]->RootNumber = j;
							rootChips[i]->RoadAng = k;
							if (rootChips[i]->BiomeType == Forest) { pos = rootChips[i]; break; }
							Chip* a = NULL;
							int o = 0;
							for (; o < 4; o++)
								if (rootChips[i]->getNearChip(o).IsRoad && !rootChips[i]->getNearChip(o).Flag) { a = &(rootChips[i]->getNearChip(o)); o++; break; }
							for (o = 0; o < 4; o++)
								if (rootChips[i]->getNearChip(o).IsRoad && !rootChips[i]->getNearChip(o).Flag) { rootChips[c] = &(rootChips[i]->getNearChip(o)); c++; numRootChips++; }
							for (o = 0; o < 4; o++) rootChips[i]->getNearChip(o).Flag = true;
							if (xMin == rootChips[i]->Pos.x) xMin--;
							if (yMin == rootChips[i]->Pos.y) yMin--;
							if (xMax == rootChips[i]->Pos.x) xMax++;
							if (yMax == rootChips[i]->Pos.y) yMax++;
							rootChips[i] = a;
							break;
						}
					}

				}
			}
			if (pos != NULL) break;
		}
		if (pos != NULL) break;
		j++;
	}
	//ルートを登録
	Chip* now = pos;
	for (;;)
	{
		//if(now->IsRoad) 
		now->RoadLevel = 1;
		now = &now->getNearChip(now->RoadAng);
		if (now == &goal) break;
	}
	goal.RoadLevel = 1;
	for (int x = xMin - 2; x <= xMax + 2; x++)
	{
		for (int y = yMin - 2; y <= yMax + 2; y++)
		{
			chips[x][y].RootNumber = 0;
			chips[x][y].Flag = false;
		}
	}

	for (int x = xMin - 2; x <= xMax + 2; x++)
	{
		for (int y = yMin - 2; y <= yMax + 2; y++)
		{
			if (chips[x][y].RoadLevel != 1 || chips[x][y].getOwnFacility() != NULL) continue;
			for (int k = 0; k < 8; k++)
			{
				bool flag1 = false;
				bool flag2 = true;
				int count = 0;
				for (int i = 0; i < 8; i++)
				{
					if (chips[x][y].getNearChip((i + k) % 8, true).RoadLevel == 1)
					{
						count++;
						flag1 = true;
					}
					else
					{
						if (flag1 == true) { flag2 = false; break; }
					}
				}
				if (count >= 3 && flag2) { chips[x][y].RoadLevel = 3; break; }
			}
		}
	}
	updateMapImage(xMin - 2, yMin - 2, xMax + 2, yMax + 2);
}
void updateMapImage(const int& xMin, const int& yMin, const int& xMax, const int& yMax)
{
	for (int x = xMin; x < xMax; x++)
	{
		for (int y = yMin; y < yMax; y++)
		{
			if (chips[x][y].RoadLevel == 1)
			{
				mapImage[y][x] = Palette::Khaki;
			}
			else
			{
				if (chips[x][y].getOwnFacility() != NULL)
				{
					switch (chips[x][y].getOwnFacility()->Type)
					{
					case Market: mapImage[y][x] = Color(200, 200, 0); break;
					case House: mapImage[y][x] = Color(72, 159, 72); break;
					case TreeHouse: mapImage[y][x] = Color(168, 60, 60); break;
					case CarpentersHut: mapImage[y][x] = Color(0, 132, 151); break;
					case Farm: mapImage[y][x] = Color(L"#503830"); break;
					}
				}
				else
				{
					switch (chips[x][y].BiomeType)
					{
					case Ocean:
						mapImage[y][x] = Palette::Blue;
						break;
					case Plains:
						mapImage[y][x] = Palette::Green;
						break;
					case Desert:
						mapImage[y][x] = Palette::Yellow;
						break;
					case Forest:
						mapImage[y][x] = Palette::Darkgreen;
						break;
					case Mountain:
						mapImage[y][x] = Palette::Gray;
						break;
					}
				}
			}
		}
	}
	mapTexture = Texture(mapImage);
}

Facility* Chip::getOwnFacility() const
{
	if (OwnFacilityP == NULL) return NULL;
	else return &facilities[OwnFacilityP - 1];
}
void	Chip::setOwnFacility(const Facility* _f)
{
	if (_f == NULL) OwnFacilityP = NULL;
	else OwnFacilityP = _f->THIS + 1;
}
Zone*	Chip::getJoinedZone() const
{
	if (JoinedZoneP == NULL) return NULL;
	else return &zones[JoinedZoneP - 1];
}
void	Chip::setJoinedZone(const Zone* _z)
{
	if (_z == NULL) JoinedZoneP = NULL;
	else JoinedZoneP = _z->THIS + 1;
}
Region* Chip::getJoinedRegion() const
{
	if (JoinedRegionP == NULL) return NULL;
	else return &regions[JoinedRegionP - 1];
}
void	Chip::setJoinedRegion(const Region* _r)
{
	if (_r == NULL) JoinedRegionP = NULL;
	else JoinedRegionP = _r->THIS + 1;
}