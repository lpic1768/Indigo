#include"Header.h"
#include"Unit.h"


void setVillage(Chip* _chip)
{
	achievements[4]++;	//ÀÑ

	Facility* f_s = setFacility(0, &_chip->movedBy(-1, -1), Market);
	f_s->addItem(Rice, 100);	//Market‚ÉWheat‚ğ’Ç‰Á
	f_s->BuildProgress = 0;	//Œšİ‚ğŠ®—¹‚³‚¹‚é

	Facility* f = NULL;

	for (int i = 0; i < 4; i++)
	{
		f = setFacilityNearChip(f_s->getEntranceChip(), House);
		if (f == NULL) return;
		f->BuildProgress = 0;
		setRoad(f_s->getEntranceChip(), f->getEntranceChip());
	}

	f = setFacilityNearChip(f_s->getEntranceChip(), CarpentersHut);
	if (f == NULL) return;
	f->BuildProgress = 0;
	setRoad(f_s->getEntranceChip(), f->getEntranceChip());

	for (int i = 0; i < 4; i++)
	{
		f = setFacilityNearChip(f_s->getEntranceChip(), Farm);
		if (f == NULL) return;
		f->BuildProgress = 0;
		setRoad(f_s->getEntranceChip(), f->getEntranceChip());
	}
	f = setFacilityNearChip(f_s->getEntranceChip(), TreeHouse);
	if (f == NULL) return;
	f->BuildProgress = 0;
	f->addItem(Wood, 100);
	setRoad(f_s->getEntranceChip(), f->getEntranceChip());
	if (!_chip->getJoinedZone()->hasForest && achievements[8] == 0) achievements[8] = 1;
}

Facility* setFacilityNearChip(Chip* _chip, const FacilityType& _type)
{
	Zone& zone = *_chip->getJoinedZone();	//Zone‚Ì“o˜^
	int xMin = _chip->Pos.x;
	int yMin = _chip->Pos.y;
	int xMax = _chip->Pos.x;
	int yMax = _chip->Pos.y;
	const int r = Random(3);
	int x;
	int y;
	for (;;)
	{
		for (x = xMin; x <= xMax; x++)
		{
			y = yMin;
			if (chips[x][y].getJoinedRegion() != _chip->getJoinedRegion()) continue;
			if (chips[x][y].getJoinedZone() != &zone) continue;
			if (canBuild(&chips[x][y], r, _type))
			{
				setFacility(r, &chips[x][y], _type);
				setRoad(_chip, chips[x][y].getOwnFacility()->getEntranceChip());
				return chips[x][y].getOwnFacility();
			}
			y = yMax;
			if (chips[x][y].getJoinedRegion() != _chip->getJoinedRegion()) continue;
			if (chips[x][y].getJoinedZone() != &zone) continue;
			if (canBuild(&chips[x][y], r, _type))
			{
				setFacility(r, &chips[x][y], _type);
				setRoad(_chip, chips[x][y].getOwnFacility()->getEntranceChip());
				return chips[x][y].getOwnFacility();
			}
		}
		for (y = yMin; y <= yMax; y++)
		{
			x = xMin;
			if (chips[x][y].getJoinedRegion() != _chip->getJoinedRegion()) continue;
			if (chips[x][y].getJoinedZone() != &zone) continue;
			if (canBuild(&chips[x][y], r, _type))
			{
				setFacility(r, &chips[x][y], _type);
				setRoad(_chip, chips[x][y].getOwnFacility()->getEntranceChip());
				return chips[x][y].getOwnFacility();
			}
			x = xMax;
			if (chips[x][y].getJoinedRegion() != _chip->getJoinedRegion()) continue;
			if (chips[x][y].getJoinedZone() != &zone) continue;
			if (canBuild(&chips[x][y], r, _type))
			{
				setFacility(r, &chips[x][y], _type);
				setRoad(_chip, chips[x][y].getOwnFacility()->getEntranceChip());
				return chips[x][y].getOwnFacility();
			}
		}
		bool flag = true;
		if (_chip->getJoinedRegion()->XMin < xMin) { xMin--; flag = false; }
		if (_chip->getJoinedRegion()->YMin < yMin) { yMin--; flag = false; }
		if (_chip->getJoinedRegion()->XMax > xMax) { xMax++; flag = false; }
		if (_chip->getJoinedRegion()->YMax > yMax) { yMax++; flag = false; }
		if (flag) return NULL;
	}
}