#include"Basic.h"
Unit units[UnitMax];
void Unit::erase()
{
	if (home.getPlace() != NULL) for (auto& p : places) if (p.enabled && home.getPlace() == &p) p.capacity--;
	reset();
}
void Unit::reset()
{
	tarPos = Point(0, 0);
	home.setPlace(NULL);
	rootNow = 0;
	needFood = false;
	rootMax = 0;
	for (auto& r : rootTimer) r = 0.0;
	for (auto& r : rootAng) r = Vec2(1, 0);
	enabled = false;
	message = L"";
	items.reset(100);
	isInPlace = false;
	isMoving = false;
	movingSpeed = 0;
	movingAng = Vec2(1, 0);
	timerNow = 0;	//汎用タイマ                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
	timerMax = 0;	//汎用タイマ
	pos = Point(0, 0);		//内部座標	Chipに基づく
	addPos = Vec2(0, 0);		//追加座標。建物内部や、移動中を表す
	jobType = Laborer;
	workStopPenalty = false;
}
bool Unit::goTo(const Point& _pos)
{
	resetTemp();
	//addPosを0に
	timerMax = int(addPos.length());
	movingSpeed = 1.0;
	rootNow = 0;
	rootMax = 0;
	if (timerMax > 0)
	{
		movingAng = -addPos.normalized();
		rootAng[0] = -addPos.normalized();
	}
	else
	{
		movingAng = Vec2(0, 0);
		rootAng[0] = Vec2(0, 0);
	}
	rootTimer[0] = addPos.length();
	isMoving = true;
	tarPos = _pos;

	if (_pos == pos) { return true; }

	int cost = 3, c = 0;
	{
		Chip& goal = getChip(_pos);
		goal.flag = true;
		goal.number = 1;
		temp[c] = &goal;
		c++;
		for (int i = 0; i < 4; i++)
		{
			if (!canMove(goal.getNearChip(i).THIS)) continue;
			temp[c] = &goal.getNearChip(i);
			temp[c]->ang = (i + 2) % 4;
			c++;
			goal.getNearChip(i).flag = true;
		}
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
					if ((temp[i]->getNearChip(k).ang == k && cost >= 3 * temp[i]->getNearChip(k).getRoadLevel() + temp[i]->getNearChip(k).number) ||
						(temp[i]->getNearChip(k).ang != k && cost >= 2 * temp[i]->getNearChip(k).getRoadLevel() + temp[i]->getNearChip(k).number))
					{
						temp[i]->number = cost;
						temp[i]->ang = k;
						for (int o = 0; o < 4; o++)
						{
							if (canMove(temp[i]->getNearChip(o).THIS) && !temp[i]->getNearChip(o).flag)
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
		if (getChip(pos).number != 0) break;
		cost++;
	}
	resetTemp();


	//ルートを登録

	Chip& goal = getChip(_pos);
	Chip* now = &getChip(pos);
	int j = 1;
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
bool	Unit::goTo(const int& _type)
{
	temp[0] = &getChip(pos);
	int c = 1;
	for (int j = 0;; j++)
	{
		if (temp[j] == NULL) { resetTemp(); return false; }

		switch (_type)
		{
		case GoToHome:
			if (temp[j]->getPlace() != NULL && temp[j]->getPlace() == home.getPlace()) {
				return goTo(temp[j]->THIS);
			}
			break;
		case GoToForest:
			if (temp[j]->isForest && temp[j]->growth == 3 && !temp[j]->isUsedByUnit)
			{
				temp[j]->isUsedByUnit = true;
				return goTo(temp[j]->THIS);
			}
			break;
		case GoToFarm:
			if (temp[j]->isFarm && temp[j]->growth % 2 == 0 && !temp[j]->isUsedByUnit)
			{
				temp[j]->isUsedByUnit = true;
				return goTo(temp[j]->THIS);
			}
			break;

		default:
			Println(L"goTo() Error 0x01");
			resetTemp();
			return false;
		}


		for (int i = 0; i < 4; i++)
		{
			if (canMove(temp[j]->getNearChip(i).THIS) && temp[j]->getNearChip(i).flag == false) {
				temp[c] = &temp[j]->getNearChip(i);
				temp[c]->flag = true;
				c++;
			}
		}
	}
}
bool	Unit::goToTakeItem(const int& _itemID)
{
	temp[0] = &getChip(pos);
	int c = 1;
	for (int j = 0;; j++)
	{
		if (temp[j] == NULL) { resetTemp(); return false; }

		if (temp[j]->getPlace() != NULL && temp[j]->getPlace()->items.getNumItem(_itemID) > 0) {
			return goTo(temp[j]->THIS);
		}


		for (int i = 0; i < 4; i++)
		{
			if (canMove(temp[j]->getNearChip(i).THIS) && temp[j]->getNearChip(i).flag == false) {
				temp[c] = &temp[j]->getNearChip(i);
				temp[c]->flag = true;
				c++;
			}
		}
	}
}
bool Unit::set(const Vec2& _globalPos)
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

	const double unitRadius = 8.0;
	const Color unitColor = HSV(jobType * 45);

	//Rootの表示
	if (isMoving)
	{
		Vec2 drawPos = getChip(pos).getGlobalPos().movedBy(addPos).movedBy(rootTimer[rootNow] * rootAng[rootNow]);
		Line(getChip(pos).getGlobalPos().movedBy(addPos).movedBy(movingAng*movingSpeed*timerNow), drawPos).draw(3, Color(255, 0, 0));
		for (int i = rootNow + 1; i <= rootMax; i++)
		{
			Line(drawPos, drawPos + rootTimer[i] * rootAng[i]).draw(3, Color(255, 0, 0));
			drawPos += rootTimer[i] * rootAng[i];
		}
	}

	Circle(getRealPos(), unitRadius).draw(unitColor).drawFrame(0, 2, Palette::Black);
	if (needFood) Circle(getRealPos(), unitRadius).draw(Palette::White);

	//Itemの表示
	int j = 12;
	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (items.getNumItem(i) > 0)
		{
			TextureAsset(L"items")(16 * i, 0, 16, 16).drawAt(getRealPos().movedBy(0, -j));
			FontAsset(L"drawUnitItemFont")(L"x", items.getNumItem(i)).draw((getRealPos().movedBy(0, -j)).movedBy(8, -8), Palette::White);
			j += 12;
		}
	}
}
void	Unit::update(const int& _actionPoint)
{
	if (!enabled) return;
	int actionPoint = _actionPoint;	//値の移動
	if ((sec < 3600 * 6 && sec + actionPoint >= 3600 * 6) || (sec < 3600 * 18 && sec + actionPoint >= 3600 * 18))
	{
		if (needFood) erase();
		else needFood = true;
	}
	if (sec < 3600 * 6 && sec + actionPoint >= 3600 * 6) workStopPenalty = false;
	if (sec < 3600 * 18 && sec + actionPoint >= 3600 * 18) workStopPenalty = true;

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
				timerMax = 0;
				timerNow = 0;
			}
		}
		else
		{
			if (isMoving)
			{
				if (rootNow == rootMax)
				{
					addPos = Vec2(0, 0);
					rootNow = 0;
					rootMax = 0;
					isMoving = false;
					movingSpeed = 0;
					pos = tarPos;
				}
				else
				{
					addPos += rootAng[rootNow] * movingSpeed*rootTimer[rootNow];
					rootNow++;
					movingAng = rootAng[rootNow];
					timerMax = int(rootTimer[rootNow]);
				}
			}
			else
			{
				if (needFood)
				{
					if (getPlace() != NULL && getPlace()->items.getNumItem(Sake) != 0)
					{
						getPlace()->items.pullItem(Sake, 1);
						needFood = false;
					}
					else if (getPlace() != NULL && getPlace()->items.getNumItem(Wheat) != 0)
					{
						getPlace()->items.pullItem(Wheat, 1);
						needFood = false;
					}
					else if (!goToTakeItem(Sake) && !goToTakeItem(Wheat)) timerMax = 3600;
				}
				else if (!workStopPenalty)
				{
					switch (jobType)
					{
					case Laborer: workStopPenalty = true;
						break;
					case Farmer:
						if (getChip(pos).getPlace() == home.getPlace() && home.getPlace() != NULL)
						{
							getChip(pos).getPlace()->items.addItem(Wheat, items.getNumItem(Wheat));
							items.pullItem(Wheat, items.getNumItem(Wheat));

							if (!goTo(GoToFarm) && (getChip(pos).getPlace() == home.getPlace() || !goTo(GoToHome)))  workStopPenalty = true;
						}
						else if (items.getNumAllItem() >= 10)
						{
							if (!goTo(GoToHome)) workStopPenalty = true;
						}
						else if (!getChip(pos).isFarm || getChip(pos).growth % 2 == 1)
						{
							if (!goTo(GoToFarm) && (getChip(pos).getPlace() == home.getPlace() || !goTo(GoToHome))) workStopPenalty = true;
						}
						else
						{
							if (getChip(pos).growth == 12)
							{
								getChip(pos).growth = 1;
								items.addItem(Wheat, 5);
							}
							else
							{
								getChip(pos).growth++;
							}
							timerMax = 1800;
							getChip(pos).isUsedByUnit = false;
						}
						break;
					case Lumberjack:
						if (getChip(pos).getPlace() == home.getPlace() && home.getPlace() != NULL)
						{
							getChip(pos).getPlace()->items.addItem(Wood, items.getNumItem(Wood));
							items.pullItem(Wood, items.getNumItem(Wood));
							if (!goTo(GoToForest) && (getChip(pos).getPlace() == home.getPlace() || !goTo(GoToHome))) workStopPenalty = true;
						}
						else if (items.getNumAllItem() >= 10)
						{
							if (!goTo(GoToHome)) workStopPenalty = true;
						}
						else if (!getChip(pos).isForest || getChip(pos).growth != 3)
						{
							if (!goTo(GoToForest) && (getChip(pos).getPlace() == home.getPlace() || !goTo(GoToHome))) workStopPenalty = true;
						}
						else
						{
							getChip(pos).isUsedByUnit = false;
							getChip(pos).growth = 0;
							items.addItem(Wood, 5);
							timerMax = 1800;
						}
						break;
					case Merchant:
						timerMax = 3600;
						break;
					case Brewer:
						if (items.getNumItem(Wheat) == 0)
						{
							if (getPlace() == NULL || getPlace()->items.getNumItem(Wheat) == 0) { if (!goToTakeItem(Wheat)) workStopPenalty = true; }
							else
							{
								getPlace()->items.pullItem(Wheat, 1);
								items.addItem(Wheat, 1);
							}
						}
						else if (items.getNumItem(Wood) == 0)
						{
							if (getPlace() == NULL || getPlace()->items.getNumItem(Wood) == 0) { if (!goToTakeItem(Wood)) workStopPenalty = true; }
							else
							{
								getPlace()->items.pullItem(Wood, 1);
								items.addItem(Wood, 1);
							}
						}
						else
						{
							if (getPlace() != home.getPlace()) { if (!goTo(GoToHome)) workStopPenalty = true; }
							else
							{
								items.pullItem(Wheat, 1);
								items.pullItem(Wood, 1);
								getPlace()->items.addItem(Sake, 1);
								timerMax = 3600;
							}
						}
						break;
					default: workStopPenalty = true; break;
					}
				}
				else return;
			}
		}
	}

}
Vec2	Unit::getRealPos() const
{
	return getChip(pos).getGlobalPos().movedBy(addPos).movedBy(movingAng*movingSpeed*timerNow);
}
bool	Unit::setPlace(Place* _p)
{
	if (_p->getCapacityMax() > _p->capacity)
	{
		if (home.getPlace() != NULL) home.getPlace()->capacity--;
		home.setPlace(_p);
		_p->capacity++;
		jobType = _p->getJobType();
		return true;

	}
	else return false;
}