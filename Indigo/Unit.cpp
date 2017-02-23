#include"Basic.h"
Unit units[UnitMax];
void Unit::reset()
{
	this->enabled = false;
	this->items.reset();
	this->timerNow = 0;
	this->timerMax = 0;
	this->pos = Point(0, 0);
	this->addPos = Vec2(0, 0);
	this->message = L"";
	isInPlace = false;
	isMoving = false;
	movingSpeed = 0;
	movingAng = Vec2(0, 0);
	home.setPlace(NULL);
}
bool	Unit::goTo(const Point& _pos)
{
	//addPosを0に
	timerMax = int(addPos.length());
	movingAng = -addPos.normalized();
	movingSpeed = 1.0;
	rootAng[0] = -addPos.normalized();
	rootTimer[0] = addPos.length();
	isMoving = true;
	tarPos = _pos;

	if (_pos == pos) return true;
	//新型ルート検索システム
	temp[0] = &getChip(_pos);
	int j = 1;
	for (int i = 0;; i++)
	{
		if (temp[i] == NULL) { resetTemp(); return false; }
		if (temp[i] == &getChip(pos)) { break; }
		for (int k = 0; k < 4; k++)
		{
			if (temp[i]->getNearChip(k).flag == false &&
				temp[i]->getNearChip(k).isRoad)
			{
				temp[j] = &temp[i]->getNearChip(k);
				temp[j]->ang = (k + 2) % 4;
				temp[j]->flag = true;
				j++;
			}
		}
	}
	resetTemp();

	//ルートを登録

	Chip& goal = getChip(_pos);
	Chip* now = &getChip(pos);
	j = 1;
	Vec2 nowPos = getChip(pos).getGlobalPos();
	Vec2 nextPos;
	Vec2 realPos = getChip(pos).getGlobalPos();
	Vec2 ang;
	Vec2 beAng;
	Vec2 bePos;
	for (;;)
	{
		nextPos = Vec2((now->getGlobalPos() + now->getNearChip(now->ang).getGlobalPos()) / 2.0);
		ang = (nextPos - nowPos).normalized();
		nextPos = ((now->getGlobalPos() + now->getNearChip(now->ang).getGlobalPos()) / 2.0).movedBy(ang.rotated(HalfPi) * 10);
		ang = (nextPos - nowPos).normalized();
		if (j != 1 && (beAng - ang).length() > 0.01)
		{
			rootTimer[j] = (bePos.distanceFrom(realPos));
			rootAng[j] = beAng;
			if (rootTimer[j] != 0) j++;
			realPos = bePos;
		}
		now = &now->getNearChip(now->ang);
		beAng = ang;
		bePos = nextPos;
		if (j == 1)
		{
			rootTimer[j] = (nextPos.distanceFrom(realPos));
			rootAng[j] = ang;
			if (rootTimer[j] != 0) j++;
			realPos = bePos;
		}
		if (now == &goal) {
			rootTimer[j] = (bePos.distanceFrom(realPos));
			rootAng[j] = ang;
			if (rootTimer[j] != 0) j++;
			nowPos = nextPos;
			realPos = bePos;
			break;
		}
		nowPos = nextPos;
	}
	rootTimer[j] = (getChip(_pos).getGlobalPos().distanceFrom(realPos));
	rootAng[j] = (getChip(_pos).getGlobalPos() - nowPos).normalized();
	rootMax = j;
	rootNow = 0;
	return true;
}
bool	Unit::searchAndSetHome()
{
	temp[0] = &getChip(pos);
	int c = 1;
	for (int j = 0;; j++)
	{
		if (temp[j] == NULL) { resetTemp();  return false; }
		if (temp[j]->getPlace() != NULL && temp[j]->getPlace()->type == House) { home.setPlace(temp[j]->getPlace()); resetTemp(); return true; }
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
bool	Unit::set(const Vec2& _globalPos)
{
	if (enabled) return false;
	enabled = true;
	pos = Point((int)(_globalPos.x / ChipImageSize), (int)(_globalPos.y / ChipImageSize));
	if (getChip(pos).getPlace() != NULL)
	{
		isInPlace = true;
		pos = getChip(pos).getPlace()->getEntrancePos();
	}
	addPos = _globalPos - getChip(pos).getGlobalPos();
	return true;
}
void	Unit::drawBody() const
{
	if (!enabled) return;


	//Rootの表示
	Vec2 drawPos = getChip(pos).getGlobalPos();
	for (int i = 1; i <= rootMax; i++)
	{
		Line(drawPos, drawPos + rootTimer[i] * rootAng[i]).draw(3, Color(255,0,0));
		drawPos += rootTimer[i] * rootAng[i];
	}

	const double unitRadius = 8.0;
	const Color unitColor(255, 0, 0);
	Circle(getChip(pos).getGlobalPos().movedBy(addPos).movedBy(movingAng*movingSpeed*timerNow), unitRadius).draw(unitColor);
}
void	Unit::update(const int& _actionPoint)
{
	if (!enabled) return;
	int actionPoint = _actionPoint;	//値の移動

	for (;;)
	{
		if (timerMax != 0)
		{
			if (timerMax - timerNow > actionPoint)
			{
				timerNow += actionPoint;
				actionPoint = 0;
				break;
			}
			else
			{
				actionPoint -= (timerMax - timerNow);
				if (isMoving)
				{
					if (rootNow == rootMax)
					{
						rootNow = 0;
						rootMax = 0;
						isMoving = false;
						addPos = Point(0, 0);
						pos = tarPos;
						timerMax = 0;
						timerNow = 0;
					}
					else
					{
						addPos += rootAng[rootNow] * movingSpeed*rootTimer[rootNow];
						rootNow++;
						movingAng = rootAng[rootNow];
						timerMax = int(rootTimer[rootNow]);
						timerNow = 0;
					}
				}
				else
				{
					timerMax = 0;
					timerNow = 0;
				}
			}
		}
		else
		{
			if (getChip(pos).getPlace() == NULL || getChip(pos).getPlace()->type != House)
			{
				if (searchAndSetHome())
				{
					goTo(home.getPlace()->getEntrancePos());
				}
				else timerMax = 60;
			}
			else
			{
				actionPoint = 0;
				break;
			}

		}
	}

}