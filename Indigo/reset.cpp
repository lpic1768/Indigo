#include"Unit.h"
#include"Header.h"
#include <Siv3D.hpp>

void Facility::reset()
{
	setJoinedZone(NULL);
	setJoinedRegion(NULL);
	R = 0;
	Size = Point(0, 0);
	NumUnitsMax = 0;
	NumUnits = 0;
	Enabled = false;
	setTopLeftChip(NULL);
	setEntranceChip(NULL);
	BuildProgress = 0;
	Type = House;
	for (auto& amount : ItemsAmount)
		amount = 0;
	for (auto& id : ItemsID)
		id = 0;
	NumItems = 0;
}
void Unit::reset()
{
	Gender = Male;
	textDraw = 0;
	Delay = 0;
	hasTarPos = false;
	Status = DoNothing;
	Progress = 0;
	setJoinedRegion(NULL);
	MovingSleepMode = true;
	MovingSpeed = 0.0;
	Timer1 = 0;
	Timer2 = 0;
	MovingProgress = 0;
	NumRootPoints = 0;
	Moved = Vec2(0, 0);
	Ang = Vec2(1, 0);
	setTarFacility(NULL);
	setHomeFacility(NULL);
	setJobFacility(NULL);
	Enabled = false;
	Pos = Vec2(0, 0);
	IsSleep = false;
	NumEat = 0;
	for (auto& amount : ItemsAmount)
		amount = 0;
	for (auto& id : ItemsID)
		id = 0;
	NumItems = 0;
}
void Zone::reset()
{
	Enabled = false;
	XMin = CHIP_X_MAX;
	YMin = CHIP_Y_MAX;
	XMax = 0;
	YMax = 0;
	hasForest = false;
}
void Chip::reset(const int& _x, const int& _y)
{
	BiomeType = Ocean;
	RootNumber = 0;
	RoadLevel = 0;
	RoadAng = 0;
	Number = 0;
	Flag = false;
	IsRoad = false;
	IsLand = false;
	IsInLand = false;
	RoadLevel = 3;
	setJoinedZone(NULL);
	setJoinedRegion(NULL);
	setOwnFacility(NULL);
	Pos = Point(_x, _y);
}
void Region::reset()
{
	for (auto& amount : ItemsAmount)
		amount = 0;
	for (auto& id : ItemsID)
		id = 0;
	NumItems = 0;
	NumBuilder = 0;
	NumOwnFaclities = 0;
	NumHouseCap = 0;
	NumJobCap = 0;
	NumUnits = 0;
	Enabled =false;
	BiomeType = Ocean;
	XMin = CHIP_X_MAX;
	XMax = 0;
	YMin = CHIP_Y_MAX;
	YMax = 0;
	SumPos = Point(0,0);
	NumChips=0;
}
void allReset()
{
	twitterText1 = L"";
	twitterText2 = L"";
	cameraTarUnit = NULL;
	sec = 43200;
	season = 1;
	year = 1;
		
	numLandChips = 0;
	for (int x = 0; x < CHIP_X_MAX; x++)
		for (int y = 0; y < CHIP_Y_MAX; y++)
			chips[x][y].reset(x, y);

	numZones = 0;
	for (auto& zone : zones) zone.reset();

	numUnits = 0;
	for (auto& unit : units) unit.reset();

	numFacilities = 0;
	for (auto& facility : facilities) facility.reset();

	numRegions = 0;
	for (auto& region : regions) region.reset();

	for (auto& t : temp) t = NULL;

	for (auto& a : achievements) a = 0;
}