#include"Unit.h"
#include "Header.h"

Facility facilities[FACILITY_MAX];
int	numFacilities;

bool	Facility::canGetItem(const int& _id)
{
	if (getNumItem(_id) == 0) return false;
	switch (Type)
	{
	case House:
		break;
	case Farm:
		break;
	case TreeHouse:
		if (_id == Wood) return true;
		break;
	case Market:
		if (_id == Rice) return true;
		break;
	case CarpentersHut:
		break;
	case Brewery:
		if (_id == Sake) return true;
		break;
	default:
		break;
	}
	return false;
}
bool	Facility::canSendItem(const int& _id)
{
	switch (Type)
	{
	case House:
		break;
	case Farm:
		break;
	case TreeHouse:
		break;
	case Market:
		if (_id == Rice && getNumItem(Rice) < 1000) return true;
		break;
	case CarpentersHut:
		break;
	case Brewery:
		break;
	default:
		break;
	}
	return false;

}
Facility* setFacility(const int& _r, Chip* _chip, const FacilityType& _type)
{
	for (auto& facility : facilities)
	{
		if (facility.Enabled) continue;
		facility.build(_r, _chip, _type);
		return &facility;
	}
	return NULL;
}
void Facility::erase() //ˆÀ‘S‚ÈÁ‹Ž
{
	//ƒ†ƒjƒbƒgî•ñ‚ÌƒŠƒZƒbƒg
	for (auto& unit : units)
	{
		if (unit.Enabled && unit.getJoinedRegion() == getJoinedRegion())
		{
			if (unit.getTarFacility() == this) { unit.setTarFacility(NULL); }

			if (unit.getJobFacility() == this && unit.getHomeFacility() == this) {
				unit.setJobFacility(NULL);
				unit.setHomeFacility(NULL);
			}
			else if (unit.getJobFacility() == this)
			{
				unit.setJobFacility(NULL);
			}
			else if (unit.getHomeFacility() == this)
			{
				unit.setHomeFacility(NULL);
				if (unit.hasJobFacility())
				{
					unit.getJobFacility()->NumUnits--;
					unit.setJobFacility(NULL);
				}
			}
		}
	}

	//Region‚©‚ç‚Ìî•ñƒŠƒZƒbƒg
	getJoinedRegion()->NumOwnFaclities--;
	switch (Type)
	{
	case TreeHouse:
	case CarpentersHut:
	case Brewery:
	case House:
		getJoinedRegion()->NumHouseCap -= NumUnitsMax;
	}
	switch (Type)
	{
	case TreeHouse:
	case CarpentersHut:
	case Brewery:
	case Farm:
		getJoinedRegion()->NumJobCap -= NumUnitsMax;
	}


	//Chip‚©‚ç‚Ìî•ñƒŠƒZƒbƒg
	for (int x = getTopLeftChip()->Pos.x; x < getTopLeftChip()->Pos.x + Size.x; x++)
	{
		for (int y = getTopLeftChip()->Pos.y; y < getTopLeftChip()->Pos.y + Size.y; y++)
		{
			if (chips[x][y].getOwnFacility() == this)
			{
				chips[x][y].setOwnFacility(NULL);
				chips[x][y].IsRoad = true;
				chips[x][y].RoadLevel = 3;
			}
		}
	}

	//Enabledî•ñ‚ðŠÜ‚Þ‚·‚×‚Ä‚Ìƒf[ƒ^‚ð
	updateMapImage(getTopLeftChip()->Pos.x, getTopLeftChip()->Pos.y, getTopLeftChip()->Pos.x + Size.x, getTopLeftChip()->Pos.y + Size.y);
	numFacilities--;
	reset();
}
bool Facility::build(const int& _r, Chip* _chip, const FacilityType& _type)
{
	if (Enabled) return false;
	if (_type == Farm) achievements[5]++;
	reset();
	R = _r;
	Type = _type;
	numFacilities++;
	setTopLeftChip(_chip);
	NumUnits = 0;
	Size = getFacilitySize(R, Type);
	setEntranceChip(&_chip->movedBy(getEntrancePos(R, Type)));
	BuildProgress = 20;
	setJoinedRegion(_chip->getJoinedRegion());
	setJoinedZone(_chip->getJoinedZone());
	switch (Type)
	{
	case Market:
		NumUnitsMax = 0;
		break;
	case TreeHouse:
		NumUnitsMax = 1;
		_chip->getJoinedRegion()->NumHouseCap += NumUnitsMax;
		break;
	case Brewery:
		NumUnitsMax = 1;
		break;
	case CarpentersHut:
		NumUnitsMax = 1;
		break;
	case House:
		NumUnitsMax = 1;
		break;
	case Farm:
		NumCropMax = 32;
		NumUnitsMax = 1;
		break;
	}
	if (IsHomeFacility())
		for (int i = 0; i < NumUnitsMax; i++) setUnit(this);
	switch (Type)
	{
	case TreeHouse:
	case Brewery:
	case CarpentersHut:
	case House:
		getJoinedRegion()->NumHouseCap += NumUnitsMax;
	}
	switch (Type)
	{
	case TreeHouse:
	case Brewery:
	case CarpentersHut:
	case Farm:
		getJoinedRegion()->NumJobCap += NumUnitsMax;
	}
	for (int x = getTopLeftChip()->Pos.x; x < getTopLeftChip()->Pos.x + Size.x; x++)
	{
		for (int y = getTopLeftChip()->Pos.y; y < getTopLeftChip()->Pos.y + Size.y; y++)
		{
			chips[x][y].setOwnFacility(this);
			chips[x][y].IsRoad = false;
		}
	}
	getEntranceChip()->IsRoad = true;
	Enabled = true;
	if (Type == TreeHouse && getJoinedZone()->hasForest) setRoadToForest();
	updateMapImage(getTopLeftChip()->Pos.x, getTopLeftChip()->Pos.y, getTopLeftChip()->Pos.x + Size.x, getTopLeftChip()->Pos.y + Size.y);
	return true;
}


void drawFacilities()
{

	for (auto& facility : facilities)
	{
		if (!facility.Enabled) continue;
		facility.draw();
	}
}
void drawFacilitiesB()
{
	for (auto& facility : facilities)
	{
		if (!facility.Enabled) continue;
		facility.drawB();
	}
}

void Facility::draw() const
{
	Point size((CHIP_SIZE*Size).movedBy(-4, -4));
	Point pos((getTopLeftChip()->Pos*CHIP_SIZE).movedBy(2, 2));
	Color color(HSV(Type * 72, 1.0, 1.0), 128);
	switch (Type)
	{
	case Market: color = Color(200, 200, 0); break;
	case House: color = Color(72, 159, 72); break;
	case TreeHouse: color = Color(168, 60, 60); break;
	case Brewery: color = Palette::Purple; break;
	case CarpentersHut: color = Color(0, 132, 151); break;
	case Farm: color = Color(L"#503830"); break;
	}
	if (BuildProgress != 0) color.a = 64;
	Rect(pos, size).draw(color).drawFrame(2, 2, Palette::Black);
	const int len = 8;
	for (int i = 0; i < 4; i++)
	{
		if (getEntranceChip()->getNearChip(i).RoadLevel == 1)
		{
			Point pos2(getEntranceChip()->Pos*CHIP_SIZE);
			Point size2(CHIP_SIZE - len * 2, CHIP_SIZE - len * 2);
			switch (i)
			{
			case 0:
				pos2.moveBy(0, len);
				break;
			case 1:
				pos2.moveBy(len, 0);
				break;
			case 2:
				pos2.moveBy(len * 2, len);
				break;
			case 3:
				pos2.moveBy(len, len * 2);
				break;
			}
			//Rect(pos2, size2).draw(Palette::Darkkhaki);
		}
	}
	//Item‚Ì•`‰æ
	if (NumItems > 0)
	{
		int j = 0;
		for (int i = 0; i < UNIT_ITEMS_MAX; i++)
		{
			if (ItemsAmount[i] == 0) continue;
			j++;
			const Vec2 posI = pos.movedBy(18, j * 18);
			TextureAsset(L"items")(16 * (ItemsID[i] - 1), 0, 16, 16).drawAt(posI);
			FontAsset(L"font8")(L"x", ItemsAmount[i]).draw((posI).movedBy(9, -9), Palette::White);
			if (j == NumItems) break;
		}
	}
	if (mouseOveredChip != NULL && mouseOveredChip->getOwnFacility() == this && destroyerModeEnabled)
		Rect(pos, size).draw(Color(255, 0, 0, 128));
}
void Facility::drawB() const
{
	Point size((CHIP_SIZE*Size).movedBy(-4, -4));
	Point pos((getTopLeftChip()->Pos*CHIP_SIZE).movedBy(2, 2));
	const int len = 2;
	switch (Type)
	{
	case Market:
		FontAsset(L"font32")(L"Žsê").drawCenter((pos + size / 2).movedBy(len, -len), Palette::Black);
		FontAsset(L"font32")(L"Žsê").drawCenter((pos + size / 2), Palette::White);
		break;
	case TreeHouse:
		FontAsset(L"font32")(L"–Ø‚±‚è¬‰®").drawCenter((pos + size / 2).movedBy(len, -len), Palette::Black);
		FontAsset(L"font32")(L"–Ø‚±‚è¬‰®").drawCenter((pos + size / 2), Palette::White);
		break;
	case Brewery:
		FontAsset(L"font32")(L"ø‘¢Š").drawCenter((pos + size / 2).movedBy(len, -len), Palette::Black);
		FontAsset(L"font32")(L"ø‘¢Š").drawCenter((pos + size / 2), Palette::White);
		break;
	case CarpentersHut:
		FontAsset(L"font32")(L"‘åH‚Ì‰Æ").drawCenter((pos + size / 2).movedBy(len, -len), Palette::Black);
		FontAsset(L"font32")(L"‘åH‚Ì‰Æ").drawCenter((pos + size / 2), Palette::White);
		break;
	case House:
		FontAsset(L"font32")(L"Z‹").drawCenter((pos + size / 2).movedBy(len, -len), Palette::Black);
		FontAsset(L"font32")(L"Z‹").drawCenter((pos + size / 2), Palette::White);
		break;
	case Farm:
		FontAsset(L"font32")(L"”_’n").drawCenter((pos + size / 2).movedBy(len, -len), Palette::Black);
		FontAsset(L"font32")(L"”_’n").drawCenter((pos + size / 2), Palette::White);
		break;
	default:
		break;
	}
	FontAsset(L"font32")(NumUnits, L"/", NumUnitsMax).drawCenter((pos + size / 2).movedBy(0, -24).movedBy(len, -len), Palette::Black);
	FontAsset(L"font32")(NumUnits, L"/", NumUnitsMax).drawCenter((pos + size / 2).movedBy(0, -24), Palette::White);
}
bool canBuild(Chip* _chip, const int& _r, const FacilityType& _type)
{
	Point size = getFacilitySize(_r, _type);
	for (int x = _chip->Pos.x - 1; x < _chip->Pos.x + 1 + size.x; x++)
	{
		for (int y = _chip->Pos.y - 1; y < _chip->Pos.y + 1 + size.y; y++)
		{
			const Chip& chip = chips[x][y];
			if (!chip.IsLand ||
				chip.getOwnFacility() != NULL ||
				chip.BiomeType != Plains) return false;
			if (x >= _chip->Pos.x && x < _chip->Pos.x + size.x &&y >= _chip->Pos.y && y < _chip->Pos.y + size.y && chip.RoadLevel == 1) return false;

		}
	}
	return true;
}
Point getFacilitySize(const int& _r, const FacilityType& _type)
{
	Point size(1, 1);
	switch (_type)
	{
	case Market: size = Point(3, 2); break;
	case TreeHouse: size = Point(4, 4); break;
	case Brewery: size = Point(3, 3); break;
	case CarpentersHut: size = Point(3, 3); break;
	case House: size = Point(3, 2); break;
	case Farm: size = Point(3, 4); break;
	}
	if (_r % 2 == 0)
		return size;
	else
		return Point(size.y, size.x);
}
Point getEntrancePos(const int& _r, const FacilityType& _type)
{
	Point size(0, 0);
	switch (_type)
	{
	case Market: size = Point(1, 1); break;
	case TreeHouse: size = Point(3, 2); break;
	case Brewery: size = Point(2, 1); break;
	case CarpentersHut: size = Point(1, 2); break;
	case House: size = Point(1, 1); break;
	case Farm: size = Point(1, 3); break;
	}
	switch (_r)
	{
	case 0: return size;
	case 1: return Point(size.y, getFacilitySize(_r, _type).y - 1 - size.x);
	case 2: return getFacilitySize(_r, _type).movedBy(-1, -1) - size;
	case 3: return Point(getFacilitySize(_r, _type).x - 1 - size.y, size.x);
	}
	return Point(0, 0);
}
bool	Facility::IsHomeFacility()
{
	switch (Type)
	{
	case House:
	case TreeHouse:
	case Brewery:
	case CarpentersHut:
		return true;
	default:
		return false;
	}
}
bool	Facility::IsJobFacility()
{
	switch (Type)
	{
	case TreeHouse:
	case Brewery:
	case CarpentersHut:
	case Farm:
		return true;
	default:
		return false;
	}

}
bool	Facility::IsStockFacility()
{
	switch (Type)
	{
	case Market:
		return true;
	default:
		return false;
	}

}
Chip*	Facility::getTopLeftChip() const
{
	if (TopLeftChipP == Point(0, 0)) return NULL;
	else return &chips[TopLeftChipP.x - 1][TopLeftChipP.y - 1];
}
void	Facility::setTopLeftChip(const Chip* _chip)
{
	if (_chip == NULL) TopLeftChipP = Point(0, 0);
	else TopLeftChipP = _chip->Pos.movedBy(1, 1);
}
Chip*	Facility::getEntranceChip() const
{
	if (EntranceChipP == Point(0, 0)) return NULL;
	else return &chips[EntranceChipP.x - 1][EntranceChipP.y - 1];
}
void	Facility::setEntranceChip(const Chip* _chip)
{
	if (_chip == NULL) EntranceChipP = Point(0, 0);
	else EntranceChipP = _chip->Pos.movedBy(1, 1);
}
Zone*	Facility::getJoinedZone() const
{
	if (JoinedZoneP == NULL) return NULL;
	else return &zones[JoinedZoneP - 1];
}
void	Facility::setJoinedZone(const Zone* _z)
{
	if (_z == NULL) JoinedZoneP = NULL;
	else JoinedZoneP = _z->THIS + 1;
}
void	Facility::setJoinedRegion(const Region* _r)
{
	if (_r == NULL) JoinedRegionP = NULL;
	else JoinedRegionP = _r->THIS + 1;
}
Region* Facility::getJoinedRegion() const
{
	if (JoinedRegionP == NULL) return NULL;
	else return &regions[JoinedRegionP - 1];
}