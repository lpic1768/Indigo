#include"Basic.h"

bool destroyPlaceMode = false;
bool destroyRoadMode = false;
bool makingVillageMode = false;
bool makingHouseMode = false;
bool settingRoadMode = false;
bool mouseOverInterface = false;
bool unitFallDownMode = false;
Chip* previousSelectedChip = NULL;
Place makingHouseP;

void closeAllInterface()
{
	destroyPlaceMode = false;
	destroyRoadMode = false;
	makingVillageMode = false;
	makingHouseMode = false;
	settingRoadMode = false;
	unitFallDownMode = false;
}

bool setButton(const int& _imageNumber, const Point _pos, bool& _flag, const Size _size = Point(64, 64))
{
	TextureAsset(Format(L"b", _imageNumber + _flag)).draw(_pos);
	if (Rect(_pos, _size).mouseOver)
	{
		mouseOverInterface = true;
		if (Input::MouseL.clicked)	//マウスチェック
		{
			bool f = !_flag;
			closeAllInterface();
			_flag = f;
			return true;
		}
	}
	return false;
}
void unitFallDown()
{
	if (nowSelectedChip == NULL) return;
	if (Input::MouseL.clicked && nowSelectedChip->isLand && (nowSelectedChip->isRoad || nowSelectedChip->getPlace() != NULL))
	{
		for (auto& u : units) if (u.set(mousePosAsGlobalVec2)) { SoundAsset(L"3").playMulti(); break; }
	}
}
void makeVillage()
{
	if (nowSelectedChip == NULL || !nowSelectedChip->isLand) return;
	if (Input::MouseL.clicked)
	{
		SoundAsset(L"6").playMulti();
		SetVillage(nowSelectedChip->THIS);
	}
}
void makeHouse()
{
	if (nowSelectedChip == NULL) return;
	makingHouseP.enabled = true;
	if (Input::KeyR.clicked) makingHouseP.r = (makingHouseP.r + 1) % 4;
	if (Input::KeyE.clicked)
	{
		switch (makingHouseP.type)
		{
		case House: makingHouseP.type = Market; break;
		case Market: makingHouseP.type = Farm; break;
		case Farm: makingHouseP.type = House; break;
		}
	}
	makingHouseP.pos = nowSelectedChip->THIS;
	if (Input::MouseL.clicked && canSetPlace(makingHouseP.r, nowSelectedChip->THIS, makingHouseP.type))
	{
		SoundAsset(L"3").playMulti();
		setPlace(makingHouseP.r, nowSelectedChip->THIS, makingHouseP.type);
	}
}
void setRoad()
{
	if (!Input::MouseL.pressed)
	{
		if (previousSelectedChip != NULL)
		{
			if (!isConnectedByLand(previousSelectedChip->THIS, nowSelectedChip->THIS)) SoundAsset(L"8").playMulti();
			else
			{
				SoundAsset(L"3").playMulti();
				setRoadAToB(previousSelectedChip->THIS, nowSelectedChip->THIS);
			}
		}
		previousSelectedChip = NULL;
	}
	if (nowSelectedChip == NULL) return;
	if (Input::MouseL.clicked) previousSelectedChip = nowSelectedChip;
}
void destroyPlace()
{
	if (selectedPlace == NULL) return;
	if (Input::MouseL.clicked)
	{
		selectedPlace->erase();
		SoundAsset(L"4").playMulti();
	}
}
void destroyRoad()
{
	if (!Input::MouseL.pressed)
	{
		if (previousSelectedChip != NULL)
		{
			int xMin, yMin, xMax, yMax;
			if (previousSelectedChip->THIS.x >= nowSelectedChip->THIS.x) { xMin = nowSelectedChip->THIS.x; xMax = previousSelectedChip->THIS.x; }
			else { xMin = previousSelectedChip->THIS.x; xMax = nowSelectedChip->THIS.x; }
			if (previousSelectedChip->THIS.y >= nowSelectedChip->THIS.y) { yMin = nowSelectedChip->THIS.y; yMax = previousSelectedChip->THIS.y; }
			else { yMin = previousSelectedChip->THIS.y; yMax = nowSelectedChip->THIS.y; }
			for (int x = xMin; x <= xMax; x++)
			{
				for (int y = yMin; y <= yMax; y++)
				{
					if (getChip(x, y).isRoad)
					{
						getChip(x, y).isRoad = false;
						mapImage[y][x] = PlainsColor;
					}
				}
			}
			mapTexture = Texture(mapImage);	//Textureに反映
			SoundAsset(L"4").playMulti();
		}
		previousSelectedChip = NULL;
	}
	if (nowSelectedChip == NULL) return;
	if (Input::MouseL.clicked) previousSelectedChip = nowSelectedChip;

}
void UpdateInterface()
{
	const Rect interfaceRect(Point(0, Window::Size().y - 96), Point(640, 96));
	interfaceRect.draw(Palette::Gray).drawFrame(8, 0, Palette::Darkgray);
	if (interfaceRect.mouseOver) mouseOverInterface = true;
	else mouseOverInterface = false;

	if (setButton(19, Point(16, Window::Size().y - 80), destroyPlaceMode))	SoundAsset(L"11").playMulti();
	if (setButton(11, Point(80, Window::Size().y - 80), makingVillageMode)) SoundAsset(L"11").playMulti();
	if (setButton(7, Point(144, Window::Size().y - 80), makingHouseMode))	SoundAsset(L"11").playMulti();
	if (setButton(15, Point(208, Window::Size().y - 80), settingRoadMode))	SoundAsset(L"11").playMulti();
	if (setButton(17, Point(272, Window::Size().y - 80), destroyRoadMode))	SoundAsset(L"11").playMulti();
	if (setButton(21, Point(336, Window::Size().y - 80), unitFallDownMode))	SoundAsset(L"11").playMulti();

	if (!mouseOverInterface)
	{
		if (makingVillageMode) makeVillage();
		if (makingHouseMode) makeHouse();
		if (settingRoadMode) setRoad();
		if (destroyPlaceMode) destroyPlace();
		if (destroyRoadMode) destroyRoad();
		if (unitFallDownMode) unitFallDown();
	}
}
