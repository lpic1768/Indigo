#include"Basic.h"

Texture mapTexture;
PerlinNoise noi(Random(2048));
void drawAll()
{
	Vec2 p1 = Vec2(-Graphics2D::GetTransform()._31, -Graphics2D::GetTransform()._32) / Graphics2D::GetTransform()._11;
	Vec2 p2 = (Vec2(-Graphics2D::GetTransform()._31, -Graphics2D::GetTransform()._32) + Window::Size()) / Graphics2D::GetTransform()._11;
	int xMin = int(p1.x / ChipImageSize);
	int yMin = int(p1.y / ChipImageSize);
	int xMax = int(p2.x / ChipImageSize + 1);
	int yMax = int(p2.y / ChipImageSize + 1);
	if (xMin < 0) xMin = 0;
	if (yMin < 0) yMin = 0;
	if (xMax >= chipX) xMax = chipX - 1;
	if (yMax >= chipY) yMax = chipY - 1;
	if (Graphics2D::GetTransform()._11 > 0.1)
	{
		//’n–Ê‚Ì•`‰æ
		for (int x = xMin; x < xMax; x++)
		{
			for (int y = yMin; y < yMax; y++)
			{
				chips[x][y].drawGround();
			}
		}
		//nowSelectedChip & selectedPlace‚ÌÝ’è
		Mouse::ClearTransform();
		const Vec2 mousePosAsChipVec2((Vec2(-Graphics2D::GetTransform()._31, -Graphics2D::GetTransform()._32) + Mouse::Pos()) / Graphics2D::GetTransform()._11 / ChipImageSize);
		const Point mousePosAsChipPoint(mousePosAsChipVec2.x, mousePosAsChipVec2.y);
		if (mousePosAsChipPoint.x >= 0 && mousePosAsChipPoint.y >= 0 && mousePosAsChipPoint.x < chipX && mousePosAsChipPoint.y < chipY) nowSelectedChip = &getChip(mousePosAsChipPoint);
		if (nowSelectedChip != NULL) selectedPlace = nowSelectedChip->getPlace();
		else selectedPlace = NULL;

		//places‚Ì•`‰æ
		for (auto& p : places) p.drawFloor();

		//road‚Ì•`‰æ
		for (int x = xMin; x < xMax; x++)
			for (int y = yMin; y < yMax; y++)
				if (chips[x][y].isRoad) chips[x][y].getDrawRect().draw(RoadColor);

		//place‚Ì–¼‘O‚Ì•`‰æ
		for (auto& p : places) p.drawName();

		//destroyRoadRenge‚Ì•`‰æ
		if (destroyRoadMode && nowSelectedChip!=NULL && previousSelectedChip !=NULL) Rect(previousSelectedChip->THIS*ChipImageSize, (nowSelectedChip->THIS - previousSelectedChip->THIS)*ChipImageSize).draw(Color(255, 0, 0, 128));

		//destroyPlace‚Ì•`‰æ
		if (destroyPlaceMode && selectedPlace != NULL) Rect(selectedPlace->pos*ChipImageSize, selectedPlace->getSize()*ChipImageSize).draw(Color(255, 0, 0, 128));

		//PlannedRoad‚Ì•`‰æ
		if (settingRoadMode && previousSelectedChip != NULL && nowSelectedChip != NULL) drawPlannedRoadAToB(previousSelectedChip->THIS, nowSelectedChip->THIS);

		//PlannedPlace‚Ì•`‰æ
		if (makingHouseMode) { makingHouseP.drawFloor(1); makingHouseP.drawName(); }
	}
	else
	{
		mapTexture.resize(chipX * ChipImageSize, chipY*ChipImageSize).draw();

		for (auto& p : places) p.drawFloor();
		if (makingHouseMode) makingHouseP.drawFloor(1);

		for (auto& p : places) p.drawName();
		if (makingHouseMode) makingHouseP.drawName();
	}
}
void InitAll()
{
	for (auto& t : temp) t = NULL;

	PerlinNoise noise(Random(2048));

	for (auto& p : places) p.reset();
	for (auto& u : units) u.reset();

	for (int x = 0; x < chipX; x++)
	{
		for (int y = 0; y < chipY; y++)
		{
			chips[x][y].reset();
			chips[x][y].THIS = Point(x, y);
			chips[x][y].isLand = (int)(noise.octaveNoise((double)x*0.005, (double)y*0.005, 0, 5) * 8) > 0;
		}
	}
	for (int i = 0; i < PlaceMax; i++) places[i].THIS = i;


	for (int x = 0; x < chipX; x++)
	{
		for (int y = 0; y < chipY; y++)
		{
			Color& pos = mapImage[y][x];
			if (chips[x][y].isLand) pos = Palette::Green;
			else pos = Palette::Blue;
		}
	}
	mapTexture = Texture(mapImage);

}
bool SetVillage(const Point& _pos)
{
	int r = Random(0, 3);
	if (canSetPlace(r, _pos, Market))
	{
		setPlace(r, _pos, Market);
		return true;
	}
	else return false;
}