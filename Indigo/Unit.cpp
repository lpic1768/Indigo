#include"Unit.h"
#include"Header.h"

Unit units[UNIT_MAX];
int	numUnits;

void drawUnits()
{
	if (numUnits == 0) return;
	int count = 0;
	for (int i = 0; i < UNIT_MAX; i++)
	{
		if (!units[i].Enabled) continue;
		units[i].draw();
		count++;
		if (numUnits == count) break;
	}
	count = 0;
	for (int i = 0; i < UNIT_MAX; i++)
	{
		if (!units[i].Enabled) continue;
		units[i].drawB();
		count++;
		if (numUnits == count) return;
	}
}


void Unit::erase()
{
	numUnits--;
	getJoinedRegion()->NumUnits--;
	reset();
}
void updateUnits()
{
	if (numUnits == 0) return;
	int count = 0;
	for (int i = 0; i < UNIT_MAX; i++)
	{
		if (!units[i].Enabled) continue;
		units[i].update();
		count++;
		if (numUnits == count) return;
	}
}

Chip& Unit::getStandingChip()
{
	return chips[(int)(Pos.x / CHIP_SIZE)][(int)(Pos.y / CHIP_SIZE)];
}

Vec2	getRoadPos(const Vec2& _pos1, const Vec2& _pos2)
{
	Vec2 pos = (_pos1 + _pos2) / 2;
	const int len = 0;//4;
	if (_pos1.x < _pos2.x) pos.moveBy(0, len);
	if (_pos1.x > _pos2.x) pos.moveBy(0, -len);
	if (_pos1.y < _pos2.y) pos.moveBy(-len, 0);
	if (_pos1.y > _pos2.y) pos.moveBy(len, 0);
	return pos;
}

void	setUnit(Facility* _f)
{
	for (auto& unit : units)
	{
		if (!unit.Enabled)
		{
			unit.set(_f);
			return;
		}
	}
}

void Unit::set(Facility* _f)
{
	Pos = _f->getEntranceChip()->getCenterPos();
	Enabled = true;
	numUnits++;
	NumEat = 4;
	Age = 0;
	if (_f->IsHomeFacility()) setHomeFacility(_f);
	if (_f->IsJobFacility()) setJobFacility(_f);
	_f->NumUnits++;
	Gender = GenderType(Random(0, 1));

	setJoinedRegion(getStandingChip().getJoinedRegion());
	getJoinedRegion()->NumUnits++;
}


JobType		Unit::getJob()
{
	if (getJobFacility() == NULL) return Labor;
	else
	{
		switch (getJobFacility()->Type)
		{
		case Farm: return Farmer;
		case TreeHouse: return Woodcutter;
		case Market: return Merchant;
		case CarpentersHut: return Builder;
		case Brewery: return Brewer;
		}
	}
	Println(L"Job not Found Error");
	return Labor;
}
/*ポインタ管理システム
*/
Region*	Unit::getJoinedRegion() const {
	if (JoinedRegionP == NULL) return NULL;
	else return &regions[JoinedRegionP - 1];	//-1は[0]を考慮して
}
void	Unit::setJoinedRegion(const Region* _r) {
	if (_r == NULL) JoinedRegionP = NULL;
	else JoinedRegionP = _r->THIS + 1;	//+1は[0]を考慮して
}
Facility*	Unit::getHomeFacility() const
{
	if (HomeFacilityP == NULL) return NULL;
	else return &facilities[HomeFacilityP - 1];
}
void	Unit::setHomeFacility(const Facility* _f)
{
	if (_f == NULL) HomeFacilityP = NULL;
	else HomeFacilityP = _f->THIS + 1;
}
Facility*	Unit::getJobFacility() const
{
	if (JobFacilityP == NULL) return NULL;
	else return &facilities[JobFacilityP - 1];
}
void	Unit::setJobFacility(const Facility* _f)
{
	if (_f == NULL) JobFacilityP = NULL;
	else JobFacilityP = _f->THIS + 1;
}
Facility*	Unit::getTarFacility() const
{
	if (TarFacilityP == NULL) return NULL;
	else return &facilities[TarFacilityP - 1];
}
void	Unit::setTarFacility(const Facility* _f)
{
	if (_f == NULL) TarFacilityP = NULL;
	else TarFacilityP = _f->THIS + 1;
}