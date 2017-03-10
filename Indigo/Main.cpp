#include"Basic.h"
# include <Siv3D.hpp>
# include <HamFramework.hpp>


double soundVolume = 0.1;
int		mouseLPressTime = 0;
int		mouseRPressTime = 0;
Point GUIPos(0, 0);

const String itemAsString[10] =
{
	L"Wood",
	L"Wheat",
	L"Bread",
	L"Meat",
	L"Leather",
	L"Mushroom",
	L"Sake",
	L"Rice",
	L"Kozo",
	L"Washi"
};

class PlaceGUI
{
public:
	PlaceGUI(Place* _p)
	{
		gui = GUI(GUIStyle::Default);
		gui.setTitle(_p->getNameAsString());
		switch (_p->type)
		{
		case Market:
			item = Wheat;
			gui.add(L"t1", GUIText::Create(Format(L"内蔵資源種類:", itemAsString[item])));
			gui.addln(L"tex1", GUITexture::Create(TextureAsset(Format(L"i", item))(0, 0, 16, 16).texture));

			gui.addln(L"mbt1", GUIButton::Create(L"変更"));
			break;
		default:
			break;
		}
		gui.add(L"bt1", GUIButton::Create(L"OK"));
		gui.add(L"bt2", GUIButton::Create(L"キャンセル"));
		gui.setPos(GUIPos);
		GUIPos.moveBy(64, 64);
		place = _p;
	}
	int	item;
	GUI gui;
	Place* place;
};

void Main()
{
	Array<PlaceGUI> placeGUI;
	//Window設定
	Window::SetTitle(L"Platanus");
	Window::SetStyle(WindowStyle::Sizeable);
	Window::SetFullscreen(false, Size(1280, 720));
	//Window::Maximize();

	FontAsset::Register(L"drawPlaceNameFont", 24, Typeface::Medium, FontStyle::Outline);
	FontAsset(L"drawPlaceNameFont").changeOutlineStyle(TextOutlineStyle(Palette::Black, Palette::White, 1.0));
	FontAsset::Register(L"drawUnitItemFont", 8, Typeface::Light);

	//Placeアセット
	TextureAsset::Register(L"house", L"Assets/House.png");
	TextureAsset::Register(L"market", L"Assets/Market.png");
	TextureAsset::Register(L"farm", L"Assets/Farm.png");
	TextureAsset::Register(L"items", L"Assets/Items.png");
	TextureAsset::Register(L"forest", L"Assets/Forest.png");
	TextureAsset::Register(L"farm", L"Assets/Farm.png");
	TextureAsset::Register(L"i0", L"Assets/i0.png");
	TextureAsset::Register(L"i1", L"Assets/i1.png");
	TextureAsset::Register(L"i2", L"Assets/i2.png");
	TextureAsset::Register(L"i3", L"Assets/i3.png");
	TextureAsset::Register(L"i4", L"Assets/i4.png");
	TextureAsset::Register(L"i5", L"Assets/i5.png");
	TextureAsset::Register(L"i6", L"Assets/i6.png");
	TextureAsset::Register(L"i7", L"Assets/i7.png");
	TextureAsset::Register(L"i8", L"Assets/i8.png");
	TextureAsset::Register(L"i9", L"Assets/i9.png");

	//ボタンアセット
	for (int i = 1; i <= 22; i++) TextureAsset::Register(Format(L"b", i), Format(L"Assets/b", i, L".png"));
	TextureAsset::Register(L"button", L"Assets/Button.png");

	SoundAsset::Register(L"music1", L"Assets/Touch_Next.mp3");
	for (int i = 1;; i++)
	{
		const FilePath soundAssetsPath = Format(L"Assets/", i, L".mp3");
		if (!FileSystem::Exists(soundAssetsPath)) break;
		SoundAsset::Register(Format(i), soundAssetsPath);
		SoundAsset(Format(i)).setVolume(soundVolume);
	}

	Graphics::SetBackground(Palette::Blue);

	InitAll();
	Camera2D camera(Vec2(chipX, chipY) / 2 * ChipImageSize, 0.1);

	Font font(24);

	while (System::Update())
	{
		Window::SetBaseSize(Window::Size());

		if (Input::KeyF5.clicked) InitAll();
		Point mousePosAsChipPoint;
		camera.update();
		{
			const auto t1 = camera.createTransformer();

			//update
			int timeSpeed = 1;
			if (Input::KeyEnter.pressed) timeSpeed = 5000;
			else if (Input::KeySpace.pressed) timeSpeed = 500;
			else if (Input::KeyShift.pressed) timeSpeed = 50;
			else timeSpeed = 1;
			for (auto& u : units) u.update(timeSpeed);
			if (getHour() >= 24)
			{
				sec -= 3600 * 24;
				month++;
				if (month == 12) month = 0;
				for (int x = 0; x < chipX; x++)
				{
					for (int y = 0; y < chipY; y++)
					{
						if (getChip(x, y).isFarm)
						{
							if (getChip(x, y).growth % 2 == 1 && getChip(x, y).growth / 2 == month - 3) getChip(x, y).growth++;
							else getChip(x, y).growth = 1;
						}
						getChip(x, y).isUsedByUnit = false;
					}
				}
			}
			for (int i = 0; i < timeSpeed; i++)
			{
				Point pos(Random(0, chipX - 1), Random(0, chipY - 1));
				if (getChip(pos).isForest)
				{
					if (getChip(pos).growth < 3) getChip(pos).growth++;
				}
			}
			sec += timeSpeed;



			//draw
			Vec2 p1 = Vec2(-Graphics2D::GetTransform()._31, -Graphics2D::GetTransform()._32) / Graphics2D::GetTransform()._11;
			Vec2 p2 = (Vec2(-Graphics2D::GetTransform()._31, -Graphics2D::GetTransform()._32) + Window::Size()) / Graphics2D::GetTransform()._11;
			int xMin = int(p1.x / ChipImageSize);
			int yMin = int(p1.y / ChipImageSize);
			int xMax = int(p2.x / ChipImageSize) + 1;
			int yMax = int(p2.y / ChipImageSize) + 2;
			if (xMin < 0) xMin = 0;
			if (yMin < 0) yMin = 0;
			if (xMax >= chipX) xMax = chipX - 1;
			if (yMax >= chipY) yMax = chipY - 1;
			if (Graphics2D::GetTransform()._11 > 0.1)
			{
				//地面の描画
				for (int x = xMin; x < xMax; x++)
					for (int y = yMin; y < yMax; y++)
						chips[x][y].drawGround();

				//placesFrameの描画
				for (auto& p : places) p.drawFrame();

				//roadの描画
				for (int x = xMin; x < xMax; x++)
					for (int y = yMin; y < yMax; y++)
						chips[x][y].drawRoad(Palette::Black, 44);
				for (int x = xMin; x < xMax; x++)
					for (int y = yMin; y < yMax; y++)
						chips[x][y].drawRoad(RoadColor, 36);

				//placeの名前の描画
				for (auto& p : places) p.drawName();

				//unitの描画
				for (auto& u : units) u.drawBody();

				for (int x = xMin; x < xMax; x++)
					for (int y = yMin; y < yMax; y++)
						if (getChip(x, y).isForest) TextureAsset(L"forest")(32 * getChip(x, y).growth, 0, 32, 64).resize(64, 128).drawAt((Point(x, y)*ChipImageSize).movedBy(ChipImageSize / 2, ChipImageSize / 2 - 32));
						else if (getChip(x, y).isFarm) TextureAsset(L"farm")(32 * (getChip(x, y).growth / 2), 0, 32, 64).resize(64, 128).drawAt((Point(x, y)*ChipImageSize).movedBy(ChipImageSize / 2, ChipImageSize / 2 - 32));
			}
			else
			{
				mapTexture.resize(chipX * ChipImageSize, chipY*ChipImageSize).draw();
				//placeの名前の描画
				for (auto& p : places) p.drawName();

				//unitの描画
				for (auto& u : units) u.drawBody();
			}

			Rect selectedArea;
			if (nowSelectedChip != NULL && previousSelectedChip != NULL)
			{
				if (previousSelectedChip->THIS.x < nowSelectedChip->THIS.x)
				{
					selectedArea.x = (previousSelectedChip->THIS.x)*ChipImageSize;
					selectedArea.w = (nowSelectedChip->THIS.x - previousSelectedChip->THIS.x)*ChipImageSize + ChipImageSize;
				}
				else
				{
					selectedArea.x = (nowSelectedChip->THIS.x)*ChipImageSize;
					selectedArea.w = (previousSelectedChip->THIS.x - nowSelectedChip->THIS.x)*ChipImageSize + ChipImageSize;
				}
				if (previousSelectedChip->THIS.y < nowSelectedChip->THIS.y)
				{
					selectedArea.y = (previousSelectedChip->THIS.y)*ChipImageSize;
					selectedArea.h = (nowSelectedChip->THIS.y - previousSelectedChip->THIS.y)*ChipImageSize + ChipImageSize;
				}
				else
				{
					selectedArea.y = (nowSelectedChip->THIS.y)*ChipImageSize;
					selectedArea.h = (previousSelectedChip->THIS.y - nowSelectedChip->THIS.y)*ChipImageSize + ChipImageSize;
				}
			}
			switch (iMode)
			{
			case None:
				break;
			case DestroyPlaceMode:
				if (selectedPlace != NULL) Rect(selectedPlace->pos*ChipImageSize, selectedPlace->getSize()*ChipImageSize).draw(Color(255, 0, 0, 128));
				break;
			case DestroyRoadMode:
				if (nowSelectedChip != NULL && previousSelectedChip != NULL) selectedArea.draw(Color(255, 0, 0, 128));
				break;
			case MakingFarmMode:
				if (nowSelectedChip != NULL && previousSelectedChip != NULL) selectedArea.draw(Color(255, 0, 0, 128));
				break;
			case DestroyFarmMode:
				if (nowSelectedChip != NULL && previousSelectedChip != NULL) selectedArea.draw(Color(255, 0, 0, 128));
				break;
			case MakingHouseMode:
				makingHouseP.drawFrame(1); makingHouseP.drawName(1);
				break;
			case SettingRoadMode:
				if (previousSelectedChip != NULL && nowSelectedChip != NULL)
				{
					Point a = previousSelectedChip->THIS;
					Point b = nowSelectedChip->THIS;
					if (getChip(a).getPlace() != NULL) a = getChip(a).getPlace()->getEntrancePos();
					if (getChip(b).getPlace() != NULL) b = getChip(b).getPlace()->getEntrancePos();
					drawPlannedRoadAToB(a, b);
				}
				break;
			case UnitFallDownMode:
				nowSelectedChip->getDrawRect().draw(Color(255, 255, 0, 128));
				break;
			default:
				break;
			}

			//nowSelectedChip & selectedPlaceの設定
			Mouse::ClearTransform();
			nowMousePosAsGlobalVec2 = Vec2((Vec2(-Graphics2D::GetTransform()._31, -Graphics2D::GetTransform()._32) + Mouse::Pos()) / Graphics2D::GetTransform()._11);
			mousePosAsChipPoint = Point(nowMousePosAsGlobalVec2.x / ChipImageSize, nowMousePosAsGlobalVec2.y / ChipImageSize);
			if (mousePosAsChipPoint.x >= 0 && mousePosAsChipPoint.y >= 0 && mousePosAsChipPoint.x < chipX && mousePosAsChipPoint.y < chipY) nowSelectedChip = &getChip(mousePosAsChipPoint);
			if (nowSelectedChip != NULL) selectedPlace = nowSelectedChip->getPlace();
			else selectedPlace = NULL;

			//ユニットの選択
			if (iMode != None) selectedUnits.clear();
			if (Input::MouseL.clicked)
			{
				if (selectedUnits.size() > 0 && nowSelectedChip != NULL && nowSelectedChip->getPlace() != NULL)
				{
					for (auto& u : selectedUnits)
					{
						if (u->setPlace(nowSelectedChip->getPlace())) SoundAsset(L"9").playMulti(soundVolume);
						else SoundAsset(L"10").playMulti(soundVolume);
					}
				}
				selectedUnits.clear();
				previousMousePosAsGlobalVec2 = nowMousePosAsGlobalVec2;
			}
			if (Input::MouseL.released)
			{
				Vec2 pmp1 = previousMousePosAsGlobalVec2;
				Vec2 pmp2 = nowMousePosAsGlobalVec2;
				if (pmp1.x > pmp2.x)
				{
					pmp1.x = nowMousePosAsGlobalVec2.x;
					pmp2.x = previousMousePosAsGlobalVec2.x;
				}
				if (pmp1.y > pmp2.y)
				{
					pmp1.y = nowMousePosAsGlobalVec2.y;
					pmp2.y = previousMousePosAsGlobalVec2.y;
				}
				for (auto& u : units)
				{
					if (u.enabled && pmp1.x <= u.getRealPos().x && u.getRealPos().x <= pmp2.x && pmp1.y <= u.getRealPos().y &&  u.getRealPos().y <= pmp2.y) { selectedUnits.push_back(&u); }
				}
			}
			const Rect selectedRect(Point(previousMousePosAsGlobalVec2.x, previousMousePosAsGlobalVec2.y), Point(nowMousePosAsGlobalVec2.x - previousMousePosAsGlobalVec2.x, nowMousePosAsGlobalVec2.y - previousMousePosAsGlobalVec2.y));
			if (Input::MouseL.pressed && iMode == None) selectedRect.draw(Color(255, 0, 0, 128));


			for (auto& u : selectedUnits) Line(u->getRealPos(), nowMousePosAsGlobalVec2).draw(4, Palette::Red);


		}


		if (Input::MouseL.clicked) mouseLPressTime = 0;
		if (Input::MouseL.pressed) mouseLPressTime++;
		if (Input::MouseL.released && mouseLPressTime < 30 && iMode == None)
		{
			if (getChip(mousePosAsChipPoint).getPlace() != NULL)
			{
				bool flag = false;
				for (auto& gui : placeGUI) { if (gui.place == getChip(mousePosAsChipPoint).getPlace()) flag = true; }
				if (!flag) placeGUI.push_back(PlaceGUI(getChip(mousePosAsChipPoint).getPlace()));
			}
		}
		for (auto& pGui : placeGUI)
		{
			Line(pGui.gui.getPos(), (pGui.place->getDrawRect().center)*camera.getMat3x2()._11 - Vec2(-camera.getMat3x2()._31, -camera.getMat3x2()._32)).draw(10, Palette::Red);
			if (pGui.gui.button(L"bt1").pushed) SoundAsset(L"1").playMulti(soundVolume);
			if (pGui.gui.button(L"mbt1").pushed)
			{
				pGui.item = (pGui.item + 1) % 10;
				pGui.gui.text(L"t1").text = Format(L"内蔵資源種類:", itemAsString[pGui.item]);
				pGui.gui.texture(L"tex1").texture = TextureAsset(Format(L"i", pGui.item))(0, 0, 16, 16).texture;
			}
		}
		Erase_if(placeGUI, [](PlaceGUI n) { return n.gui.button(L"bt2").pushed; });

		const int alphaMax = 192;
		const int alpha = abs(sec - 43200) * alphaMax / 14400 - alphaMax;
		if (alpha >= alphaMax) Rect(0, 0, Window::Size()).draw(Color(0, 0, 0, alphaMax));
		else if (alpha > 0) Rect(0, 0, Window::Size()).draw(Color(0, 0, 0, alpha));
		camera.draw(Palette::Orange);

		UpdateInterface();



		font.draw(Format(L"Platanus\n",
			month + 1, L"月 ", getHour(), L"時 ", getMinute(), L"分\n",
			L"文化力", culturePoint, L"CP"));
	}
}
