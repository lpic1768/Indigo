#include"Unit.h"
#include "Header.h"

FacilityType fType = Farm;
int	fR = 0;
int selectType = 0;
void drawInterface()
{
	Mouse::ClearTransform();
	if (Rect(0, 192, 64, 64 * 8).mouseOver) return;
	if (mouseOveredChip == NULL) return;
	if (destroyerModeEnabled) destroyerMode();
	if (makingRoadModeEnabled) makingRoadMode();
	if (!mouseOveredChip->IsLand) return;
	if (constructingModeEnabled) constructingMode();
	if (fallingDownModeEnabled) fallingDownMode();
}
void constructingMode()
{

	if (Input::KeyR.clicked) { fR = (fR + 1) % 4; SoundAsset(L"2").play(); }
	if (Input::KeyE.clicked)
	{
		SoundAsset(L"2").play();
		fType = FacilityType(selectType);
		selectType = (selectType + 1) % 8;
	}
	Point fSize = getFacilitySize(fR, fType);
	mouseOveredChip = &getChip(mouseOveredChip->Pos.movedBy((-fSize.movedBy(-1, -1) / 2)));


	Point size((CHIP_SIZE*fSize).movedBy(-4, -4));
	Point pos((mouseOveredChip->Pos*CHIP_SIZE).movedBy(2, 2));
	Color color(HSV(fType * 72, 1.0, 1.0), 128);
	switch (fType)
	{
	case Market: color = Color(200, 200, 0); break;
	case House: color = Color(72, 159, 72); break;
	case TreeHouse: color = Color(168, 60, 60); break;
	case CarpentersHut: color = Color(0, 132, 151); break;
	case Farm: color = Color(L"#503830"); break;
	}
	if (!canBuild(mouseOveredChip, fR, fType)) color = Color(255, 0, 0, 128);
	else {
		if (Input::MouseL.pressed) {
			setFacility(fR, mouseOveredChip, fType);
			SoundAsset(L"3").play();
			bool flag = false;
			for (auto& facility : facilities)
			{
				if (!facility.Enabled || facility.getJoinedRegion() != mouseOveredChip->getJoinedRegion()) continue;
				if (flag && facility.getEntranceChip()->getCenterPos().distanceFrom(mouseOveredChip->getOwnFacility()->getEntranceChip()->getCenterPos()) > 640) continue;
				setRoad(facility.getEntranceChip(), mouseOveredChip->getOwnFacility()->getEntranceChip());
				flag = true;
			}
		}
		color.a = 128;
	}
	Rect(pos, size).draw(color).drawFrame(2, 2, Palette::Black);
	Color roadColor = Palette::Khaki;
	roadColor.a = 128;
	Rect(Point(mouseOveredChip->Pos + getEntrancePos(fR, fType))*CHIP_SIZE, Point(CHIP_SIZE, CHIP_SIZE)).draw(roadColor);
	const int len = 2;
	switch (fType)
	{
	case Market:
		FontAsset(L"font32")(L"Žsê").drawCenter((pos + size / 2).movedBy(len, -len), Palette::Black);
		FontAsset(L"font32")(L"Žsê").drawCenter((pos + size / 2), Palette::White);
		break;
	case TreeHouse:
		FontAsset(L"font32")(L"–Ø‚±‚è¬‰®").drawCenter((pos + size / 2).movedBy(len, -len), Palette::Black);
		FontAsset(L"font32")(L"–Ø‚±‚è¬‰®").drawCenter((pos + size / 2), Palette::White);
		break;
	case Brewery:
		FontAsset(L"font32")(L"ø‘¢Š").drawCenter((pos + size / 2).movedBy(len, -len), Palette::Black);
		FontAsset(L"font32")(L"ø‘¢Š").drawCenter((pos + size / 2), Palette::White);
		break;
	case CarpentersHut:
		FontAsset(L"font32")(L"‘åH‚Ì‰Æ").drawCenter((pos + size / 2).movedBy(len, -len), Palette::Black);
		FontAsset(L"font32")(L"‘åH‚Ì‰Æ").drawCenter((pos + size / 2), Palette::White);
		break;
	case House:
		FontAsset(L"font32")(L"Z‹").drawCenter((pos + size / 2).movedBy(len, -len), Palette::Black);
		FontAsset(L"font32")(L"Z‹").drawCenter((pos + size / 2), Palette::White);
		break;
	case Farm:
		FontAsset(L"font32")(L"”_’n").drawCenter((pos + size / 2).movedBy(len, -len), Palette::Black);
		FontAsset(L"font32")(L"”_’n").drawCenter((pos + size / 2), Palette::White);
		break;
	default:
		break;
	}
}
void destroyerMode()
{
	if (roadDestroyerModeEnabled)
	{
		if (roadStartChip == NULL)
		{
			if (Input::MouseL.clicked)
			{
				roadStartChip = mouseOveredChip;
			}
		}
		else
		{
			Rect(roadStartChip->Pos*CHIP_SIZE, (mouseOveredChip->Pos.movedBy(1, 1) - roadStartChip->Pos)*CHIP_SIZE).draw(Color(255, 0, 0, 128));
			if (Input::MouseL.released)
			{
				int xMin;
				int yMin;
				int xMax;
				int yMax;
				if (roadStartChip->Pos.x < mouseOveredChip->Pos.x) { xMin = roadStartChip->Pos.x; xMax = mouseOveredChip->Pos.x; }
				else { xMin = mouseOveredChip->Pos.x; xMax = roadStartChip->Pos.x; }
				if (roadStartChip->Pos.y < mouseOveredChip->Pos.y) { yMin = roadStartChip->Pos.y; yMax = mouseOveredChip->Pos.y; }
				else { yMin = mouseOveredChip->Pos.y; yMax = roadStartChip->Pos.y; }

				for (int x = xMin; x <= xMax; x++)
					for (int y = yMin; y <= yMax; y++)
						if (chips[x][y].RoadLevel == 1 && chips[x][y].getOwnFacility() == NULL) chips[x][y].RoadLevel = 3;

				updateMapImage(xMin, yMin, xMax, yMax);
				SoundAsset(L"4").play();
				roadStartChip = NULL;
			}
		}
	}
	if (facilityDestroyerModeEnabled)
	{

		if (mouseOveredChip->getOwnFacility() != NULL)
		{
			if (Input::MouseL.pressed)
			{
				mouseOveredChip->getOwnFacility()->erase();
				SoundAsset(L"4").play();
			}
		}
	}
}
void fallingDownMode()
{
	if (mouseOveredChip->IsLand && mouseOveredChip->getJoinedRegion()->NumUnits == 0 && mouseOveredChip->IsInLand &&
		canBuild(&mouseOveredChip->movedBy(-1, -1), 0, Market))
	{
		const Point pos = mouseOveredChip->Pos.movedBy(-1, -1)*CHIP_SIZE;
		const Point size = Point(CHIP_SIZE, CHIP_SIZE) * 3;
		Rect(pos, size).draw(Color(255, 0, 0, 128));
		if (Input::MouseL.clicked)
		{
			SoundAsset(L"6").play();
			setVillage(mouseOveredChip);
		}
	}

}
Chip* roadStartChip;
void makingRoadMode()
{
	if (roadStartChip == NULL)
	{
		if (Input::MouseL.clicked)
		{
			if (mouseOveredChip->IsLand && mouseOveredChip->getOwnFacility() == NULL) roadStartChip = mouseOveredChip;
			else SoundAsset(L"8").play();

		}
	}
	else
	{
		Line(roadStartChip->getCenterPos(), mouseOveredChip->getCenterPos()).draw(CHIP_SIZE, Color(255, 0, 0, 128));
		if (Input::MouseL.released)
		{
			if (roadStartChip->getJoinedZone() == mouseOveredChip->getJoinedZone() && mouseOveredChip->IsLand && mouseOveredChip->getOwnFacility() == NULL)
			{
				setRoad(roadStartChip, mouseOveredChip, false);
				SoundAsset(L"3").play();
				achievements[9]++;
			}
			else SoundAsset(L"8").play();
			roadStartChip = NULL;
		}
	}
}