#include"Unit.h"
#include"Header.h"


void	Unit::setStatus(const Pattern& _status)
{
	Status = _status;
	aiProgress = 0;
}
void	Unit::setPenalty(const int& _penalty)
{
	goTo(getHomeFacility());
	Status = DoNothing;
	aiProgress = 0;
	Delay = -_penalty;
}

Chip* rootChips[1000000];
void	Unit::goEntrance()
{
	MovingSpeed = 1.0;
	hasTarPos = true;
	Timer1 = (int)(Moved.length() / MovingSpeed);
	Timer2 = 0;
	Ang = (-Moved).normalized();
}
void	Unit::goRoomRandom()
{
	const Vec2 dr = (getStandingChip().getOwnFacility()->Size*CHIP_SIZE);
	const Vec2 tl = Vec2(getStandingChip().getOwnFacility()->getTopLeftChip()->Pos*CHIP_SIZE).movedBy(Random(10.0, dr.x - 20), Random(10.0, dr.y - 20)) - Pos;
	MovingSpeed = 1.0;
	Timer1 = (int)((tl).distanceFrom(Moved) / MovingSpeed);
	Timer2 = 0;
	hasTarPos = false;
	Ang = (tl - Moved).normalized();

}
void Unit::setTarPos()
{
	const Vec2 _pos = getChip(TarPos).getCenterPos();
	if (&getChip(_pos) == &getStandingChip())
	{
		//Println(L"TarPos Error 1");
		MovingSpeed = 0;
		Timer1 = 0;
		Timer2 = 0;
		hasTarPos = false;
		MovingSleepMode = true;
		Pos = getChip(TarPos).getCenterPos();
		Moved = Vec2(0, 0);
		return;
	}
	//新型ルート検索システム
	rootChips[0] = &getChip(_pos);
	int j = 1;
	for (int i = 0;; i++)
	{
		if (rootChips[i] == NULL) {
			Println(L"TarPos Error 2");
			return;
		}
		if (rootChips[i] == &getChip(Pos)) { break; }
		for (int k = 0; k < 4; k++)
		{
			if (rootChips[i]->getNearChip(k).Flag == false &&
				rootChips[i]->getNearChip(k).RoadLevel == 1)
			{
				rootChips[j] = &rootChips[i]->getNearChip(k);
				rootChips[j]->RoadAng = (k + 2) % 4;
				rootChips[j]->Flag = true;
				j++;
			}
		}
	}
	int rootChipMax = j;

	//ルートを登録
	Chip& goal = getChip(_pos);
	Chip* now = &getStandingChip();
	j = 0;
	Vec2 nowPos = Pos;
	Vec2 nextPos;
	Vec2 realPos = Pos;
	Vec2 ang;
	Vec2 beAng;
	Vec2 bePos;
	for (;;)
	{
		nextPos = getRoadPos(now->getCenterPos(), now->getNearChip(now->RoadAng).getCenterPos());
		ang = (nextPos - nowPos).normalized();
		nextPos = getRoadPos(now->getCenterPos(), now->getNearChip(now->RoadAng).getCenterPos()).movedBy(ang.rotated(HalfPi) * 10);
		ang = (nextPos - nowPos).normalized();
		if (j != 0 && (beAng - ang).length() > 0.01)
		{
			RootTimer[j] = (bePos.distanceFrom(realPos));
			RootAng[j] = beAng;
			j++;
			realPos = bePos;
		}
		now = &now->getNearChip(now->RoadAng);
		beAng = ang;
		bePos = nextPos;
		if (j == 0)
		{
			RootTimer[j] = (nextPos.distanceFrom(realPos));
			RootAng[j] = ang;
			j++;
			realPos = bePos;
		}
		if (now == &goal) {
			RootTimer[j] = (bePos.distanceFrom(realPos));
			RootAng[j] = ang;
			j++;
			nowPos = nextPos;
			realPos = bePos;
			break;
		}
		nowPos = nextPos;
	}
	RootTimer[j] = (_pos.distanceFrom(realPos));
	RootAng[j] = (_pos - nowPos).normalized();
	NumRootPoints = j + 1;	//最後の分
	for (j = 0; j < rootChipMax; j++) { rootChips[j]->Flag = false; }

	MovingProgress = 0;
	Moved = Vec2(0, 0);
	Ang = RootAng[0];
	MovingSpeed = 1.0;
	Timer1 = (int)RootTimer[MovingProgress];
	Timer2 = 0;
	hasTarPos = false;
	MovingSleepMode = false;
}