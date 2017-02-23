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
void	Place::drawFrame(const int& _type) const
{
	const int frameWidth = 4;

	if (!enabled) return;
	const Rect drawRect = getDrawRect();
	if (_type == 1)	//åöê›åvâÊéû
	{
		drawRect.drawFrame(frameWidth, 0, Palette::Black);	//ñ{ëÃÇÕê‘Ç¢ä¥Ç∂Ç≈
	}
	else drawRect.drawFrame(frameWidth, 0, Palette::Black);

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

void	Place::drawName(const int& _type) const
{
	const int frameWidth = 4;
	if (!enabled) return;
	Rect drawRect((pos*ChipImageSize).movedBy(frameWidth, frameWidth), (getSize()*ChipImageSize).movedBy(-frameWidth * 2, -frameWidth * 2));
	if (_type == 1) drawRect.draw(Color(255, 0, 0, 128));
	else drawRect.draw(getDrawColor());
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
	Place p;	//âºëz
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
	if (r % 2 == 1) return Point(placeData[type].size.y, placeData[type].size.x);
	else return placeData[type].size;
}
Color	Place::getDrawColor() const
{
	return placeData[type].color;
}
String	Place::getNameAsString() const
{
	return placeData[type].name;
}
Point	Place::getEntrancePos() const
{
	switch (r)
	{
	case 0: return pos.movedBy(0, placeData[type].entraceY);
	case 1: return pos.movedBy(getSize().x - 1 - placeData[type].entraceY, 0);
	case 2: return pos.movedBy(getSize().x - 1, getSize().y - 1 - placeData[type].entraceY);
	case 3: return pos.movedBy(placeData[type].entraceY, getSize().y - 1);
	}
	return pos;
}