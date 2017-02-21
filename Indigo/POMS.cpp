#include"Header.h"

int fillter = 0;
int downloadMap = -1;

bool setupByPOMS()
{
	if (!Internet::IsConnected()) return false;

	Cursor::SetStyle(CursorStyle::None);

	allReset();	//初期化

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
		if (csv.get<int32>(i, 0) == CHIP_X_MAX && csv.get<int32>(i, 1) == CHIP_Y_MAX) { downloadMap = Random(csv.get<int32>(i, 2), csv.get<int32>(i, 3)); break; }
	if (downloadMap == -1) return false;
	//downloadMap = 1010;

	// マップダウンロード（非同期）
	const FilePath url = Format(L"http://lpic.webcrow.jp/Platanus/POMS/", CHIP_X_MAX, L"_", CHIP_Y_MAX, L"_", downloadMap, L".png");
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
			for (int x = 0; x < CHIP_X_MAX; x++)
			{
				for (int y = 0; y < CHIP_Y_MAX; y++)
				{
					Zone& zone = zones[downloadMapImage[x][y].r];
					if (numZones < (int)downloadMapImage[x][y].r + 1) numZones = (int)downloadMapImage[x][y].r + 1;	//zone[0]対策に+1する
					Region& region = regions[downloadMapImage[x][y].r * 256 + downloadMapImage[x][y].g];
					Chip& chip = chips[x][y];
					if (downloadMapImage[x][y].b == 255) downloadMapImage[x][y].b = 0;
					chip.BiomeType = Biome(downloadMapImage[x][y].b);
					region.BiomeType = Biome(downloadMapImage[x][y].b);
					if (Biome(downloadMapImage[x][y].b) == Forest) zone.hasForest = true;
					chip.setJoinedZone(&zone);
					chip.setJoinedRegion(&region);
					region.Enabled = true;
					zone.Enabled = true;
					if (chip.BiomeType != Ocean) {
						chip.IsRoad = true;
						chip.IsLand = true;
						chip.IsInLand = true;
					}
					zone.Enabled;
					if (x <= zone.XMin) zone.XMin = x;
					if (y <= zone.YMin) zone.YMin = y;
					if (x >= zone.XMax) zone.XMax = x;
					if (y >= zone.YMax) zone.YMax = y;
					if (x <= region.XMin) region.XMin = x;
					if (y <= region.YMin) region.YMin = y;
					if (x >= region.XMax) region.XMax = x;
					if (y >= region.YMax) region.YMax = y;
				}
			}
			for (int x = 0; x < CHIP_X_MAX; x++)
				for (int y = 0; y < CHIP_Y_MAX; y++)
					for (int i = 0; i < 8; i++)
						if (chips[x][y].getNearChip(i, true).IsLand == Ocean) { chips[x][y].IsInLand = false; break; }

			endFlag = true;
			updateMapImage();
			stopwatch.restart();
			stopwatch.set(MillisecondsF(200));
			SoundAsset(L"5").play();
			return true;	//デバッグ用高速化
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
