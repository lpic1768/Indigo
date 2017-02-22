#include"Basic.h"

Texture mapTexture;
Array<PlaceData> placeData;
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


	}
	else
	{
		mapTexture.resize(chipX * ChipImageSize, chipY*ChipImageSize).draw();
	}

	//placeの名前の描画
	for (auto& p : places) p.drawName();

	//destroyRoadRengeの描画
	if (destroyRoadMode && nowSelectedChip != NULL && previousSelectedChip != NULL) Rect(previousSelectedChip->THIS*ChipImageSize, (nowSelectedChip->THIS - previousSelectedChip->THIS)*ChipImageSize).draw(Color(255, 0, 0, 128));

	//destroyPlaceの描画
	if (destroyPlaceMode && selectedPlace != NULL) Rect(selectedPlace->pos*ChipImageSize, selectedPlace->getSize()*ChipImageSize).draw(Color(255, 0, 0, 128));

	//PlannedRoadの描画
	if (settingRoadMode && previousSelectedChip != NULL && nowSelectedChip != NULL) drawPlannedRoadAToB(previousSelectedChip->THIS, nowSelectedChip->THIS);

	//PlannedPlaceの描画
	if (makingHouseMode) { makingHouseP.drawFrame(1); makingHouseP.drawName(1); }

	//nowSelectedChip & selectedPlaceの設定
	Mouse::ClearTransform();
	const Vec2 mousePosAsChipVec2((Vec2(-Graphics2D::GetTransform()._31, -Graphics2D::GetTransform()._32) + Mouse::Pos()) / Graphics2D::GetTransform()._11 / ChipImageSize);
	const Point mousePosAsChipPoint(mousePosAsChipVec2.x, mousePosAsChipVec2.y);
	if (mousePosAsChipPoint.x >= 0 && mousePosAsChipPoint.y >= 0 && mousePosAsChipPoint.x < chipX && mousePosAsChipPoint.y < chipY) nowSelectedChip = &getChip(mousePosAsChipPoint);
	if (nowSelectedChip != NULL) selectedPlace = nowSelectedChip->getPlace();
	else selectedPlace = NULL;
}
void InitAll()
{
	placeData.clear();
	CSVReader reader(L"Data/PlaceData.csv");
	for (int i = 0; i < reader.rows; i++) placeData.push_back(i);

	for (auto& t : temp) t = NULL;

	PerlinNoise noise(Random(2048));

	for (auto& p : places) p.reset();
	for (int i = 0; i < PlaceMax; i++) places[i].THIS = i;
	for (auto& u : units) u.reset();

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
				}
			}

			endFlag = true;
			stopwatch.restart();
			stopwatch.set(MillisecondsF(200));
			SoundAsset(L"5").play();
		}
		if (endFlag)
		{
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