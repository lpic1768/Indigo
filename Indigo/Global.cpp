#include"Basic.h"

Texture mapTexture;
Array<PlaceData> placeData;
PerlinNoise noi(Random(2048));
Vec2 nowMousePosAsGlobalVec2;
Vec2 previousMousePosAsGlobalVec2;
Array<Unit*> selectedUnits;
int sec = 0;
int month = 0;
int year = 0;
int getHour() { return  sec / 3600; } //0～23まで
int getMinute() { return (sec / 60) % 60; }
bool isNight() { return getHour() <= 5 || getHour() >= 17; }
void updateAll()
{
	//unitのupdate
	int timeSpeed = 1;
	if (Input::KeyEnter.pressed) timeSpeed = 5000;
	else if (Input::KeySpace.pressed) timeSpeed = 500;
	else if (Input::KeyShift.pressed) timeSpeed = 50;
	else timeSpeed = 1;
	sec += timeSpeed;
	for (auto& u : units) u.update(timeSpeed);
	if (getHour() >= 24)
	{
		for (auto& u : units) if (u.enabled) u.workStopPenalty = false;
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
		for (auto& u : units) if (u.enabled) u.needFood = true;
	}
	for (int i = 0; i < timeSpeed * 10; i++)
	{
		Point pos(Random(0, chipX - 1), Random(0, chipY - 1));
		if (getChip(pos).isForest)
		{
			if (getChip(pos).growth < 3) getChip(pos).growth++;
		}
	}
}
void drawAll()
{
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
		{
			for (int y = yMin; y < yMax; y++)
			{
				chips[x][y].drawGround();
			}
		}

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
	case MakingVillageMode:
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
	const Point mousePosAsChipPoint(nowMousePosAsGlobalVec2.x / ChipImageSize, nowMousePosAsGlobalVec2.y / ChipImageSize);
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

	Graphics2D::ClearTransform();
	const int alphaMax = 192;
	const int alpha = abs(sec - 43200) * alphaMax / 14400 - alphaMax;
	if (alpha >= alphaMax) Rect(0, 0, Window::Size()).draw(Color(0, 0, 0, alphaMax));
	else if (alpha > 0) Rect(0, 0, Window::Size()).draw(Color(0, 0, 0, alpha));
}
void InitAll()
{
	placeData.clear();
	CSVReader reader(L"Data/PlaceData.csv");
	for (int i = 1; i < reader.rows; i++) placeData.push_back(i);

	for (auto& t : temp) t = NULL;

	PerlinNoise noise(Random(2048));

	for (auto& p : places) p.reset();
	for (int i = 0; i < PlaceMax; i++) places[i].THIS = i;
	for (auto& u : units) u.reset();
	for (int i = 0; i < UnitMax; i++) units[i].THIS = i;

	for (int x = 0; x < chipX; x++)
	{
		for (int y = 0; y < chipY; y++)
		{
			chips[x][y].reset();
			chips[x][y].THIS = Point(x, y);
		}
	}

	setupByPOMS();
	Cursor::SetStyle(CursorStyle::Default);

	for (int x = 0; x < chipX; x++)
	{
		for (int y = 0; y < chipY; y++)
		{
			Color& pos = mapImage[y][x];
			pos = Palette::Blue;
			if (chips[x][y].isLand) pos = Palette::Green;
			if (chips[x][y].isForest) pos = Palette::Darkgreen;
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
bool setupByPOMS()
{

	int fillter = 0;
	int downloadMap = -1;

	if (!Internet::IsConnected()) return false;

	Cursor::SetStyle(CursorStyle::None);

	const Font font(20);
	const Font font2(64, Typeface::Heavy, FontStyle::Outline);
	const Font font3(16);

	font2.changeOutlineStyle(TextOutlineStyle(Palette::Green, Palette::Orange, 4.0));


	HTTPClient client;
	if (!client.download(L"http://lpic.webcrow.jp/Platanus/POMS/list.csv", L"list.csv")) return false;

	//サーバーにあるCSVよりダウンロード先を検索
	const CSVReader csv(L"list.csv");
	if (!csv) return false;
	int row = (int)csv.rows;
	for (int i = 0; i < row; i++)
		if (csv.get<int32>(i, 0) == chipX && csv.get<int32>(i, 1) == chipY) { downloadMap = Random(csv.get<int32>(i, 2), csv.get<int32>(i, 3)); break; }
	if (downloadMap == -1) return false;
	//downloadMap = 1010;

	// マップダウンロード（非同期）
	const FilePath url = Format(L"http://lpic.webcrow.jp/Platanus/POMS/", chipX, L"_", chipY, L"_", downloadMap, L".png");
	const FilePath savePath = L"temp.png";
	if (!client.downloadAsync(url, savePath)) return false;


	// まだダウンロードされていないので、この時点では読み込まない
	Image downloadMapImage;

	const String s = L"Platanus";
	Stopwatch stopwatch(false);

	bool endFlag = false;	//処理終了時
	while (System::Update())
	{

		//if (Input::KeyC.clicked) client.cannelDownload(); // 実行中のダウンロードの中止

		const DownloadProgress progress = client.retreiveProgress();

		const double index = stopwatch.ms() / 80.0;

		const auto animation = [=](KineticTypography& k)
		{
			const double t = index - k.index;

			k.col.a = AlphaF(Saturate(t / 6)).toColor().a;

			k.pos.y -= EaseIn<Easing::Quad>(Saturate(1 - t / 4)) * 20.0;
		};

		font2(s).drawKinetic(Window::Size().movedBy(-420, -256) / 2, animation, Palette::White, 1.2);

		if (progress.status == DownloadStatus::Working)
		{
			const double p = progress.getProgress().value_or(0.5);

			Rect(Point(Window::Size().x / 4, Window::Size().y / 2 + 32).movedBy(2, 2), Point((int32)(p *Window::Size().x / 2), 32)).draw(Color(0, 0, 0, 128));
			Rect(Point(Window::Size().x / 4, Window::Size().y / 2 + 32), Point((int32)(Window::Size().x / 2), 32)).draw(Palette::Gray);
			Rect(Point(Window::Size().x / 4, Window::Size().y / 2 + 32), Point((int32)(p *Window::Size().x / 2), 32)).draw();

			font(L"マップをダウンロードしています", 0_dp, p * 100, L'%').drawCenter((Window::Size().movedBy(4, 4) / 2), Color(0, 0, 0, 128));
			font(L"マップをダウンロードしています", 0_dp, p * 100, L'%').drawCenter((Window::Size() / 2));

			if (stopwatch.ms() > 1000)
			{
				if (stopwatch.ms() > 2000) font3(L"ツイートを拡散して開発をサポートしよう").drawCenter((Window::Size().movedBy(0, 192) / 2));
				else font3(L"ツイートを拡散して開発をサポートしよう").drawCenter((Window::Size().movedBy(0, 192) / 2), Color(255, (stopwatch.ms() - 1000) * 255 / 1000));
			}
		}
		else if (progress.status == DownloadStatus::Succeeded)
		{
			downloadMapImage = Image(savePath);
			FileSystem::Remove(savePath);

			//chipに変換処理
			for (int x = 0; x < chipX; x++)
			{
				for (int y = 0; y < chipY; y++)
				{
					Chip& chip = chips[x][y];
					if (downloadMapImage[x][y].b == 255) downloadMapImage[x][y].b = 0;
					if (downloadMapImage[x][y].b == 0) chip.isLand = false;
					else chip.isLand = true;
					if (downloadMapImage[x][y].b == 2)
					{
						chip.isForest = true;
						chip.growth = 3;
					}
				}
			}

			endFlag = true;
			stopwatch.restart();
			stopwatch.set(MillisecondsF(200));
			SoundAsset(L"5").play();
		}
		if (endFlag)
		{
			if (Input::AnyKeyClicked() || Input::MouseL.clicked || Input::MouseR.clicked) return true;
			if (stopwatch.ms() > 2400)
			{
				RenderTexture tex = Graphics::GetSwapChainTexture();
				fillter = 0;
				for (; fillter < 256; fillter += 8)
				{
					tex.draw();
					RectF(Window::Size()).draw(Color(0, 0, 255, fillter));
					System::Update();
				}
				fillter = 255;
				return true;
			}
			else
			{

				Rect(Point(Window::Size().x / 4, Window::Size().y / 2 + 32).movedBy(2, 2), Point((int32)(Window::Size().x / 2), 32)).draw(Color(0, 0, 0, 128));
				Rect(Point(Window::Size().x / 4, Window::Size().y / 2 + 32), Point((int32)(Window::Size().x / 2), 32)).draw();

				font(L"マップのダウンロードが完了しました。").drawCenter((Window::Size().movedBy(4, 4) / 2), Color(0, 0, 0, 128));
				font(L"マップのダウンロードが完了しました。").drawCenter((Window::Size() / 2));

				if (stopwatch.ms() > 1000)
				{
					if (stopwatch.ms() > 2000) font3(L"ツイートを拡散して開発をサポートしよう").drawCenter((Window::Size().movedBy(0, 192) / 2));
					else font3(L"ツイートを拡散して開発をサポートしよう").drawCenter((Window::Size().movedBy(0, 192) / 2), Color(255, (stopwatch.ms() - 1000) * 255 / 1000));
				}
			}
		}
	}
	return false;
}