#include"Basic.h"

Place places[PlaceMax];
Place* selectedPlace = NULL;
void	Place::reset()
{
	enabled = false;
	type = House;
	item.reset();
	pos = Point(0, 0);
	r = 0;
}
void	Place::drawFloor(const int& _type) const
{
	const int frameWidth = 4;

	if (!enabled) return;
	const Rect drawRect = getDrawRect();
	if (_type == 1)	//建設計画時
	{
		drawRect.draw(Color(255, 0, 0, 128)).drawFrame(frameWidth, 0, Palette::Black);	//本体は赤い感じで
	}
	else drawRect.draw(getDrawColor()).drawFrame(frameWidth, 0, Palette::Black);

	const int inlineRoadWidth = 36;
	switch (r)
	{
	case 0:
		Rect(Point(getChip(getEntrancePos()).THIS*ChipImageSize).movedBy(0, 14), frameWidth, inlineRoadWidth).draw(RoadColor);
		break;
	case 1:
		Rect(Point(getChip(getEntrancePos()).THIS*ChipImageSize).movedBy(14, 0), inlineRoadWidth, frameWidth).draw(RoadColor);
		break;
	case 2:
		Rect(Point(getChip(getEntrancePos()).THIS*ChipImageSize).movedBy(ChipImageSize - frameWidth, 14), frameWidth, inlineRoadWidth).draw(RoadColor);
		break;
	case 3:
		Rect(Point(getChip(getEntrancePos()).THIS*ChipImageSize).movedBy(14, ChipImageSize - frameWidth), inlineRoadWidth, frameWidth).draw(RoadColor);
		break;
	}
}

void	Place::drawName() const
{
	if (!enabled) return;
	FontAsset(L"drawPlaceNameFont").drawCenter(getNameAsString(), (pos + Vec2(getSize()) / 2)*ChipImageSize);
}

void	Place::erase()
{
	enabled = false;
	getChip(getEntrancePos()).isRoad = false;
	for (int x = pos.x; x < pos.x + getSize().x; x++)
		for (int y = pos.y; y < pos.y + getSize().y; y++)
			getChip(x, y).setPlace(NULL);
}
void	Place::set(const int& _r, const Point& _pos, const PType& _t)
{
	type = _t;
	pos = _pos;
	r = _r;
	if (!canSetPlace(_r, _pos, _t)) return;
	enabled = true;
	for (int x = _pos.x; x < _pos.x + getSize().x; x++)
		for (int y = _pos.y; y < _pos.y + getSize().y; y++)
			getChip(x, y).setPlace(this);
	getChip(getEntrancePos()).isRoad = true;
}

bool canSetPlace(const int& _r, const Point& _pos, const PType& _t)
{
	Place p;	//仮想
	p.r = _r;
	p.type = _t;
	for (int x = _pos.x; x < _pos.x + p.getSize().x; x++)
	{
		for (int y = _pos.y; y < _pos.y + p.getSize().y; y++)
		{
			if (getChip(x, y).getPlace() != NULL || getChip(x, y).isRoad || !getChip(x, y).isLand) return false;

		}
	}
	return true;
}
bool setPlace(const int& _r, const Point& _pos, const PType& _t)
{
	for (auto& p : places)
	{
		if (!p.enabled)
		{
			p.set(_r, _pos, _t);
			return true;
		}
	}
	return false;
}
Point	Place::getSize() const
{
	switch (type)
	{
	case House:
		if (r == 0 || r == 2) return Point(2, 3);
		else return Point(3, 2);
		break;
	case Market:
		if (r == 0 || r == 2) return Point(4, 4);
		else return Point(4, 4);
		break;
	case Farm:
		if (r == 0 || r == 2) return Point(5, 5);
		else return Point(5, 5);
		break;
	default:
		break;
	}
	return Point(0, 0);
}
Color	Place::getDrawColor() const
{
	switch (type)
	{
	case House:	return Color(64, 255, 64);
	case Market:return Color(255, 255, 0);
	case Farm:	return Color(0, 0, 128);
	default:	return Color(0);
	}
}
String	Place::getNameAsString() const
{
	switch (type)
	{
	case House:	return  L"長屋";
	case Market:return  L"問屋";
	case Farm:	return  L"水田";
	default:	return  L"No Name";
	}
}
Point	Place::getEntrancePos() const
{
	//r==0の時に、EntrancePosが0方向になるようにする
	int entranceY;	//r==0の場合のEntrancePosのy座標
	switch (type)
	{
	case House:	entranceY = 1; break;
	case Market:entranceY = 1; break;
	case Farm:	entranceY = 2; break;
	default:	entranceY = 0; break;
	}
	switch (r)
	{
	case 0: return pos.movedBy(0, entranceY);
	case 1: return pos.movedBy(getSize().x - 1 - entranceY, 0);
	case 2: return pos.movedBy(getSize().x - 1, getSize().y - 1 - entranceY);
	case 3: return pos.movedBy(entranceY, getSize().y - 1);
	}
	return pos;
}