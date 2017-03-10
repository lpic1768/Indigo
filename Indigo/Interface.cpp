#include"Basic.h"
IMode iMode = None;
bool mouseOverInterface = false;
Chip* previousSelectedChip = NULL;
Place makingHouseP;

bool setButton(const int& _imageNumber, const Point _pos, const IMode& _mode, const Size _size = Point(64, 64))
{
	TextureAsset(L"button")((_mode == iMode) * 64 + 1, _imageNumber * 64 + 1, 62, 62).resize(_size).draw(_pos);
	if (Rect(_pos, _size).mouseOver)
	{
		mouseOverInterface = true;
		if (Input::MouseL.clicked)	//マウスチェック
		{
			if (iMode == _mode) iMode = None;
			else iMode = _mode;
			return true;
		}
	}
	return false;
}
void destroyFarm()
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
					if (getChip(x, y).isFarm)
					{
						getChip(x, y).isFarm = false;
						mapImage[y][x] = PlainsColor;
					}
				}
			}
			mapTexture = Texture(mapImage);	//Textureに反映
			SoundAsset(L"4").playMulti(soundVolume);
		}
		previousSelectedChip = NULL;
	}
	if (nowSelectedChip == NULL) return;
	if (Input::MouseL.clicked) previousSelectedChip = nowSelectedChip;
}
void makeFarm()
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
					if (getChip(x, y).isLand && !getChip(x, y).isForest && !getChip(x, y).isFarm && getChip(x, y).getPlace() == NULL && !getChip(x, y).isRoad)
					{
						getChip(x, y).isFarm = true;
						mapImage[y][x] = FarmColor;
					}
				}
			}
			mapTexture = Texture(mapImage);	//Textureに反映
			SoundAsset(L"1").playMulti(soundVolume);
		}
		previousSelectedChip = NULL;
	}
	if (nowSelectedChip == NULL) return;
	if (Input::MouseL.clicked) previousSelectedChip = nowSelectedChip;
}
void unitFallDown()
{
	if (nowSelectedChip == NULL) return;
	if (Input::MouseL.clicked && nowSelectedChip->isLand && (nowSelectedChip->isRoad || nowSelectedChip->getPlace() != NULL))
	{
		for (auto& u : units) if (u.set(nowMousePosAsGlobalVec2)) {
			if (nowSelectedChip->getPlace() != NULL) u.setPlace(nowSelectedChip->getPlace());
			SoundAsset(L"3").playMulti(soundVolume);
			break;
		}
	}
}
void makeHouse()
{
	if (nowSelectedChip == NULL) return;
	makingHouseP.enabled = true;
	if (Input::KeyR.clicked) makingHouseP.r = (makingHouseP.r + 1) % 4;
	if (Input::KeyE.clicked)
	{
		makingHouseP.type++;
		if (makingHouseP.type == placeData.size()) makingHouseP.type = 0;
	}
	makingHouseP.pos = nowSelectedChip->THIS;
	if (Input::MouseL.clicked && canSetPlace(makingHouseP.r, nowSelectedChip->THIS, makingHouseP.type))
	{
		SoundAsset(L"3").playMulti(soundVolume);
		setPlace(makingHouseP.r, nowSelectedChip->THIS, makingHouseP.type);
		for (auto& u : units) if (u.enabled) u.workStopPenalty = false;
	}
}
void setRoad()
{
	if (!Input::MouseL.pressed)
	{
		if (previousSelectedChip != NULL)
		{
			Point a = previousSelectedChip->THIS;
			Point b = nowSelectedChip->THIS;
			if (getChip(a).getPlace() != NULL) a = getChip(a).getPlace()->getEntrancePos();
			if (getChip(b).getPlace() != NULL) b = getChip(b).getPlace()->getEntrancePos();
			if (!isConnectedByLand(a, b)) SoundAsset(L"8").playMulti(soundVolume);
			else
			{
				SoundAsset(L"3").playMulti(soundVolume);
				setRoadAToB(a, b);
				for (auto& u : units) if (u.enabled) u.workStopPenalty = false;
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
		SoundAsset(L"4").playMulti(soundVolume);
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
			SoundAsset(L"4").playMulti(soundVolume);
		}
		previousSelectedChip = NULL;
	}
	if (nowSelectedChip == NULL) return;
	if (Input::MouseL.clicked) previousSelectedChip = nowSelectedChip;

}
void UpdateInterface()
{
	const Rect interfaceRect(Point(0, Window::Size().y - 64), Point(640, 64));
	interfaceRect.draw(Palette::Gray).drawFrame(8, 0, Palette::Darkgray);
	if (interfaceRect.mouseOver) mouseOverInterface = true;
	else mouseOverInterface = false;

	if (setButton(9, Point(8, Window::Size().y - 56), DestroyPlaceMode, Point(48, 48)))	SoundAsset(L"11").playMulti(soundVolume);
	if (setButton(3, Point(8 + 48 * 1, Window::Size().y - 56), MakingHouseMode, Point(48, 48)))	SoundAsset(L"11").playMulti(soundVolume);
	if (setButton(7, Point(8 + 48 * 2, Window::Size().y - 56), SettingRoadMode, Point(48, 48)))	SoundAsset(L"11").playMulti(soundVolume);
	if (setButton(8, Point(8 + 48 * 3, Window::Size().y - 56), DestroyRoadMode, Point(48, 48)))	SoundAsset(L"11").playMulti(soundVolume);
	if (setButton(10, Point(8 + 48 * 4, Window::Size().y - 56), UnitFallDownMode, Point(48, 48)))	SoundAsset(L"11").playMulti(soundVolume);
	if (setButton(11, Point(8 + 48 * 5, Window::Size().y - 56), MakingFarmMode, Point(48, 48)))	SoundAsset(L"11").playMulti(soundVolume);
	if (setButton(12, Point(8 + 48 * 6, Window::Size().y - 56), DestroyFarmMode, Point(48, 48)))	SoundAsset(L"11").playMulti(soundVolume);
	if (Input::Key1.clicked) iMode = None;
	if (Input::Key2.clicked) iMode = DestroyPlaceMode;
	if (Input::Key4.clicked) iMode = MakingHouseMode;
	if (Input::Key5.clicked) iMode = SettingRoadMode;
	if (Input::Key6.clicked) iMode = DestroyRoadMode;
	if (Input::Key7.clicked) iMode = UnitFallDownMode;
	if (Input::Key8.clicked) iMode = MakingFarmMode;
	if (Input::Key9.clicked) iMode = DestroyFarmMode;
	if (!mouseOverInterface)
	{
		switch (iMode)
		{
		case None:
			break;
		case DestroyPlaceMode: destroyPlace(); break;
		case DestroyRoadMode:destroyRoad(); break;
		case MakingHouseMode:makeHouse(); break;
		case SettingRoadMode: setRoad(); break;
		case UnitFallDownMode:unitFallDown(); break;
		case MakingFarmMode: makeFarm(); break;
		case DestroyFarmMode: destroyFarm(); break;
		default:
			break;
		}
	}

}
