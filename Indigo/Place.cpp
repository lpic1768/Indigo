#include"Basic.h"

Place places[PlaceMax];
Place* selectedPlace = NULL;
void	Place::reset()
{
	enabled = false;
	type = 0;
	items.reset(1000);
	pos = Point(0, 0);
	r = 0;
	capacity = 0;
}
void	Place::drawFrame(const int& _type) const
{
	const int frameWidth = 4;

	if (!enabled) return;

	const Rect drawRect = getDrawRect();
	if (_type == 1)	//建設計画時
	{
		drawRect.drawFrame(frameWidth, 0, Palette::Black);	//本体は赤い感じで
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
	Color color = getDrawColor();

	if (iMode == SettingRoadMode && (nowSelectedChip->getPlace() == this || (previousSelectedChip != NULL && previousSelectedChip->getPlace() == this))) color = Palette::Lightgreen;


	if (_type == 1) drawRect.draw(Color(255, 0, 0, 128));
	else drawRect.draw(color);


	//Itemの表示
	int j = 0;
	for (int i = 0; i < ITEM_MAX; i++)
	{
		if (items.getNumItem(i) > 0)
		{
			TextureAsset(L"items")(16 * i, 0, 16, 16).drawAt((pos*ChipImageSize).movedBy(8, 8).movedBy(frameWidth, j + frameWidth));
			FontAsset(L"drawUnitItemFont")(L"x", items.getNumItem(i)).draw(((pos*ChipImageSize).movedBy(8, 8).movedBy(frameWidth, j + frameWidth)).movedBy(8, -8), Palette::Black);
			j += 16;
		}
	}

	FontAsset(L"drawPlaceNameFont").drawCenter(getNameAsString(), (pos + Vec2(getSize()) / 2)*ChipImageSize);
	FontAsset(L"drawPlaceNameFont").drawCenter(Format(capacity, L"/", getCapacityMax()), ((pos + Vec2(getSize()) / 2)*ChipImageSize).movedBy(0, 32));
}

void	Place::erase()
{
	enabled = false;
	getChip(getEntrancePos()).isRoad = false;
	for (int x = pos.x; x < pos.x + getSize().x; x++)
		for (int y = pos.y; y < pos.y + getSize().y; y++)
			getChip(x, y).setPlace(NULL);
	reset();
}
void	Place::set(const int& _r, const Point& _pos, const int& _t)
{
	type = _t;
	pos = _pos;
	r = _r;
	if (!canSetPlace(_r, _pos, _t)) return;
	enabled = true;
	for (int x = _pos.x; x < _pos.x + getSize().x; x++)
		for (int y = _pos.y; y < _pos.y + getSize().y; y++)
			getChip(x, y).setPlace(this);
	if (_t == Market) items.addItem(Wheat, 1000);
	getChip(getEntrancePos()).isRoad = true;
}
bool canSetPlace(const int& _r, const Point& _pos, const int& _t)
{
	Place p;	//仮想
	p.r = _r;
	p.type = _t;
	for (int x = _pos.x; x < _pos.x + p.getSize().x; x++)
	{
		for (int y = _pos.y; y < _pos.y + p.getSize().y; y++)
		{
			if (getChip(x, y).getPlace() != NULL || getChip(x, y).isForest || getChip(x, y).isFarm || getChip(x, y).isRoad || !getChip(x, y).isLand) return false;

		}
	}
	return true;
}
bool setPlace(const int& _r, const Point& _pos, const int& _t)
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
Color	Place::getDrawColor() const { return placeData[type].color; }
String	Place::getNameAsString() const { return placeData[type].name; }
int		Place::getCapacityMax() const { return placeData[type].capacityMax; }
bool	Place::isHouse() const { return placeData[type].isHouse; }
bool	Place::isWorkPlace() const { return placeData[type].isWorkPlace; }
int		Place::getJobType() const { return placeData[type].jobType; }
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