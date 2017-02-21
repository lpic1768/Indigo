#include"Unit.h"
#include"Header.h"

//このシステムに使う一時保管変数
Chip* temp[1000000];

bool	Unit::pickUpItem(const int& _id)
{
	temp[0] = &getChip(Pos);
	int j = 1;
	for (int i = 0;; i++)
	{
		if (temp[i] == NULL) {
			chipFlagReset();
			return false;
		}
		if (temp[i]->getOwnFacility() != NULL && temp[i]->getOwnFacility()->BuildProgress == 0 && temp[i]->getOwnFacility()->getNumItem(_id) > 0) {
			TarPos = temp[i]->getOwnFacility()->getEntranceChip()->Pos;
			goEntrance();
			chipFlagReset();
			return true;
		}
		for (int k = 0; k < 4; k++)
		{
			if (temp[i]->getNearChip(k).Flag == false &&
				temp[i]->getNearChip(k).RoadLevel == 1)
			{
				temp[j] = &temp[i]->getNearChip(k);
				temp[j]->Flag = true;
				j++;
			}
		}
	}
}
bool	Unit::goTo(Facility* _f)
{
	if (_f == NULL) return false;
	return goTo(_f->getEntranceChip());
}
bool	Unit::goTo(Chip* _chip)
{
	if (&getStandingChip() == _chip) return true;

	if (isConeected(_chip))
	{
		goEntrance();
		TarPos = _chip->Pos;
		return true;
	}
	else
	{
		return false;
	}
}
bool Unit::goNearMarket()
{
	temp[0] = &getChip(Pos);
	int j = 1;
	for (int i = 0;; i++)
	{
		if (temp[i] == NULL) { chipFlagReset(); return false; }
		if (temp[i]->getOwnFacility() != NULL && temp[i]->isFMarket()) {
			goEntrance();
			TarPos = temp[i]->Pos;
			chipFlagReset();
			return true;
		}
		for (int k = 0; k < 4; k++)
		{
			if (temp[i]->getNearChip(k).Flag == false &&
				temp[i]->getNearChip(k).RoadLevel == 1)
			{
				temp[j] = &temp[i]->getNearChip(k);
				temp[j]->Flag = true;
				j++;
			}
		}
	}
}

bool	Unit::goToNearForest()
{
	if (!getChip(Pos).getJoinedZone()->hasForest) return false;
	temp[0] = &getChip(Pos);
	int j = 1;
	for (int i = 0;; i++)
	{
		if (temp[i] == NULL) { chipFlagReset(); return false; }
		if (temp[i]->BiomeType == Forest) {
			goTo(temp[i]);
			chipFlagReset();
			return true;
		}
		for (int k = 0; k < 4; k++) if (temp[i]->getNearChip(k).Flag == false && temp[i]->getNearChip(k).RoadLevel == 1) { temp[j] = &temp[i]->getNearChip(k); temp[j]->Flag = true; j++; }
	}
}
bool	Unit::isConeected(const Facility* _f)
{
	return isConeected(_f->getEntranceChip());
}

bool	Unit::isConeected(const Chip* _chip)
{
	if (getChip(Pos).RoadLevel != 1) return false;
	if (&getChip(Pos) == _chip) return false;
	temp[0] = &getChip(Pos);
	int j = 1;
	for (int i = 0;; i++)
	{
		if (temp[i] == NULL) { chipFlagReset(); return false; }
		if (temp[i] == _chip) { chipFlagReset(); return true; }
		for (int k = 0; k < 4; k++) {
			if (temp[i]->getNearChip(k).Flag == false &&
				temp[i]->getNearChip(k).RoadLevel == 1)
			{
				temp[j] = &temp[i]->getNearChip(k);
				temp[j]->Flag = true;
				j++;
			}
		}
	}
}
bool	Chip::isConeected(const Chip* _chip)
{
	if (!IsRoad || !_chip->IsRoad) return false;	//もし、道じゃない場合
	if (!IsLand || !_chip->IsLand) return false;
	if (getJoinedZone() != _chip->getJoinedZone()) return false;
	if (this == _chip) return true;
	temp[0] = this;
	int j = 1;
	for (int i = 0;; i++)
	{
		if (temp[i] == NULL) { chipFlagReset(); return false; }
		if (temp[i] == _chip) { chipFlagReset(); return true; }
		for (int k = 0; k < 4; k++) {
			if (temp[i]->getNearChip(k).Flag == false &&
				temp[i]->getNearChip(k).IsRoad)
			{
				temp[j] = &temp[i]->getNearChip(k);
				temp[j]->Flag = true;
				j++;
			}
		}
	}
}

Facility*	Unit::searchPlannedSite()
{
	temp[0] = &getChip(Pos);
	int j = 1;
	for (int i = 0;; i++)
	{
		if (temp[i] == NULL) { chipFlagReset(); return NULL; }
		if (temp[i]->getOwnFacility() != NULL && temp[i]->getOwnFacility()->BuildProgress != 0) { Chip* chip = temp[i]; chipFlagReset(); return chip->getOwnFacility(); }
		for (int k = 0; k < 4; k++) if (temp[i]->getNearChip(k).Flag == false && temp[i]->getNearChip(k).RoadLevel == 1) { temp[j] = &temp[i]->getNearChip(k); temp[j]->Flag = true; j++; }
	}
}
Facility*	Unit::searchItem(const int& _id)
{
	temp[0] = &getChip(Pos);
	int j = 1;
	for (int i = 0;; i++)
	{
		if (temp[i] == NULL) { chipFlagReset(); return NULL; }
		if (temp[i]->getOwnFacility() != NULL &&
			temp[i]->getOwnFacility()->canGetItem(_id)) { Chip* chip = temp[i]; chipFlagReset(); return chip->getOwnFacility(); }
		for (int k = 0; k < 4; k++) if (temp[i]->getNearChip(k).Flag == false && temp[i]->getNearChip(k).RoadLevel == 1) { temp[j] = &temp[i]->getNearChip(k); temp[j]->Flag = true; j++; }
	}
}

void chipFlagReset()
{
	for (int i = 0;; i++)
	{
		if (temp[i] == NULL) return;
		temp[i]->Flag = false;
		temp[i] = NULL;
	}
}