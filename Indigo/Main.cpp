
# include <Siv3D.hpp>
# include <HamFramework.hpp>
#include"Unit.h"
# include "Header.h"
double soundEffectsVolume = 1.0;
double musicVolume = 1.0;
Chip* mouseOveredChip = NULL;
Camera2D* camera;
Zone zones[ZONE_MAX];
int numZones;
Chip chips[CHIP_X_MAX][CHIP_Y_MAX];
Unit* cameraTarUnit = NULL;
int sec = 0;
int season = 1;
int year = 1;
int	actionPoint = 0;

bool constructingModeEnabled = false;
bool destroyerModeEnabled = false;
bool fallingDownModeEnabled = false;
bool makingRoadModeEnabled = false;
bool roadDestroyerModeEnabled = false;
bool facilityDestroyerModeEnabled = false;


bool nightEnabled = true;
bool lineEnabled = false;
bool twitterAuthEnabled = false;
bool twitterGuiEnabled = false;
int achievements[ACHIEVEMENTS_MAX];

Image mapImage(CHIP_X_MAX, CHIP_Y_MAX);
Texture mapTexture;


String twitterText1 = L"";
String twitterText2 = L"";
GUI* guiAuthP;
GUI* guiTweetP;
TwitterClient* twitterP;
WidgetStyle* widgetStyleP;
int pbv = 0;
int pbw = 0;
int pbb = 0;
int pbc = 0;
void Main()
{
	Image image;
	scrCapImageP = &image;
	GUIStyle guiStyle = GUIStyle::Default;
	WidgetStyle widgetStyle;
	Font guiFont(12);
	widgetStyle.font = guiFont;
	widgetStyleP = &widgetStyle;
	guiStyle.background.color = Palette::Darkgray;
	guiStyle.font = guiFont;
	guiStyle.borderRadius = 0;

	//ツイートシステム
	const String API_key = L"oCjcQKHrQlRkf9XMOzJoAli21";
	const String API_secret = L"3znqLUS4eLVnkAe4Vd4yb854ezX07cB780STyvaamfTByRHOX0";
	TwitterClient twitter(API_key, API_secret);
	twitterP = &twitter;
	GUI guiAuth(guiStyle);
	guiAuthP = &guiAuth;
	GUI guiTweet(guiStyle);
	guiTweetP = &guiTweet;
	twitterInit();

	//Escで終了させない
	System::SetExitEvent(WindowEvent::CloseButton);

	TextureAsset::Register(L"items", L"Assets/Items.png");
	TextureAsset::Register(L"i1", L"Assets/b01.png");
	TextureAsset::Register(L"i2", L"Assets/b02.png");
	TextureAsset::Register(L"i3", L"Assets/b03.png");
	TextureAsset::Register(L"i4", L"Assets/b04.png");
	TextureAsset::Register(L"i5", L"Assets/b05.png");
	TextureAsset::Register(L"i6", L"Assets/b06.png");
	TextureAsset::Register(L"i7", L"Assets/b07.png");
	TextureAsset::Register(L"i8", L"Assets/b08.png");
	TextureAsset::Register(L"i9", L"Assets/b09.png");
	TextureAsset::Register(L"i10", L"Assets/b10.png");
	TextureAsset::Register(L"i11", L"Assets/b11.png");
	TextureAsset::Register(L"i12", L"Assets/b12.png");
	TextureAsset::Register(L"i13", L"Assets/b13.png");
	TextureAsset::Register(L"i14", L"Assets/b14.png");
	TextureAsset::Register(L"i15", L"Assets/b15.png");
	TextureAsset::Register(L"i16", L"Assets/b16.png");
	TextureAsset::Register(L"i17", L"Assets/b17.png");
	TextureAsset::Register(L"i18", L"Assets/b18.png");
	TextureAsset::Register(L"i19", L"Assets/b19.png");
	TextureAsset::Register(L"i20", L"Assets/b20.png");
	TextureAsset::Register(L"mouse1", L"Assets/point.png");
	TextureAsset::Register(L"mouse2", L"Assets/villageWand.png");
	TextureAsset::Register(L"mouse3", L"Assets/point2.png");
	TextureAsset::Register(L"mouse4", L"Assets/point3.png");
	TextureAsset::Register(L"mouse5", L"Assets/point4.png");
	TextureAsset::Register(L"mouse6", L"Assets/point5.png");

	FontAsset::Register(L"font32", 16, Typeface::Black);
	FontAsset::Register(L"font64", 64, Typeface::Black);
	FontAsset::Register(L"font8", 9, Typeface::Default);

	SoundAsset::Register(L"camera", L"Assets/camera.wav");
	SoundAsset::Register(L"music1", L"Assets/Touch_Next.mp3");
	SoundAsset::Register(L"1", L"Assets/1.mp3");
	SoundAsset::Register(L"2", L"Assets/2.mp3");
	SoundAsset::Register(L"3", L"Assets/3.mp3");
	SoundAsset::Register(L"4", L"Assets/4.mp3");
	SoundAsset::Register(L"5", L"Assets/5.mp3");
	SoundAsset::Register(L"6", L"Assets/6.mp3");
	SoundAsset::Register(L"7", L"Assets/7.mp3");
	SoundAsset::Register(L"8", L"Assets/8.mp3");
	SoundAsset::Register(L"9", L"Assets/9.mp3");
	SoundAsset::Register(L"10", L"Assets/10.mp3");
	SoundAsset::Register(L"11", L"Assets/11.mp3");

	soundEffectsVolume = 0.5;
	SoundAsset(L"1").setVolume(soundEffectsVolume);
	SoundAsset(L"2").setVolume(soundEffectsVolume);
	SoundAsset(L"3").setVolume(soundEffectsVolume);
	SoundAsset(L"4").setVolume(soundEffectsVolume);
	SoundAsset(L"5").setVolume(soundEffectsVolume);
	SoundAsset(L"6").setVolume(soundEffectsVolume);
	SoundAsset(L"7").setVolume(soundEffectsVolume);
	SoundAsset(L"8").setVolume(soundEffectsVolume);
	SoundAsset(L"9").setVolume(soundEffectsVolume);
	SoundAsset(L"10").setVolume(soundEffectsVolume);
	SoundAsset(L"11").setVolume(soundEffectsVolume);

	Window::SetTitle(L"Platanus");
	Window::SetStyle(WindowStyle::Sizeable);
	Window::SetFullscreen(false, Size(1280, 720));
	Window::SetBaseSize(Window::Size());
	Window::Maximize();
	Graphics::SetBackground(Palette::Blue);

	Camera2D mainCamera(Vec2(CHIP_SIZE*CHIP_X_MAX / 2, CHIP_SIZE*CHIP_Y_MAX / 2), 0.05);
	camera = &mainCamera;


	//スピード設定
	GUI speedGui(guiStyle);
	speedGui.setTitle(L"PHSCS設定");
	speedGui.add(GUIText::Create(L"基礎倍速"));
	speedGui.addln(L"sl1", GUISlider::Create(0, 4, 3));
	speedGui.add(GUIText::Create(L"二次倍速"));
	speedGui.addln(L"sl2", GUISlider::Create(0, 5, 3));
	speedGui.addln(L"s1", GUIText::Create(L"倍速", widgetStyle));
	speedGui.horizontalLine(L"a");
	speedGui.addln(GUIText::Create(L"選択した国家の生産量", widgetStyle));
	speedGui.addln(L"nunit", GUIText::Create(L"ユニット数", widgetStyle));
	speedGui.addln(L"rice", GUIText::Create(L"米", widgetStyle));
	speedGui.addln(L"wood", GUIText::Create(L"木材", widgetStyle));
	speedGui.addln(L"sake", GUIText::Create(L"日本酒", widgetStyle));
	speedGui.setPos(Window::Size().x, 0);
	speedGui.show();

	//THISの割り当て
	for (int i = 0; i < REGION_MAX; i++) regions[i].THIS = i;
	for (int i = 0; i < ZONE_MAX; i++) zones[i].THIS = i;
	for (int i = 0; i < UNIT_MAX; i++) units[i].THIS = i;
	for (int i = 0; i < FACILITY_MAX; i++) facilities[i].THIS = i;

	speedGui.hide();
	while (!setupByPOMS());
	speedGui.show();

	SoundAsset(L"music1").setLoop(true);
	SoundAsset(L"music1").pause();
	SoundAsset(L"music1").setVolume(musicVolume);
	SoundAsset(L"music1").play();

	Mat3x2 mainMat3x2 = camera->getMat3x2();
	const Font font(16);


	bool superSpeed = false;



	int mouseRPressCount = 0;


	while (System::Update())
	{
		int s1 = (int)Pow(10, speedGui.slider(L"sl1").value);
		int s2 = (int)Pow(10, speedGui.slider(L"sl2").value);
		speedGui.text(L"s1").text = Format(s1, L"x", s2, L"=", s1*s2, L"倍速");

		Window::SetBaseSize(Window::Size());

		MicrosecClock c;

		if (Input::KeyF5.pressed && !twitterGuiEnabled)
		{
			SoundAsset(L"music1").pause();
			speedGui.hide();
			while (!setupByPOMS());
			speedGui.show();
			SoundAsset(L"music1").play();
		}
		camera->update();
		{
			const auto t1 = camera->createTransformer();

			mouseOveredChip = NULL;
			drawDebugChips();
			if (Graphics2D::GetTransform()._11 >= 2.0 / (double)CHIP_SIZE)
			{
				drawUnits();
				drawFacilitiesB();
				drawZones();
			}
			Rect(0, 0, CHIP_SIZE*CHIP_X_MAX, CHIP_SIZE*CHIP_Y_MAX).drawFrame(8, 8, Palette::Yellow);

			drawInterface();

			Graphics2D::ClearTransform();
			Mouse::ClearTransform();

			{
				Point pos(0, 256);
				const Point icon_size(64, 64);
				//明るさの設定
				if (nightEnabled) TextureAsset(L"i1").resize(icon_size).draw(pos);
				else TextureAsset(L"i2").resize(icon_size).draw(pos);
				if (Rect(pos, icon_size).mouseOver && Input::MouseL.clicked) nightEnabled = !nightEnabled;

				//Twitterアイコン
				pos = Point(0, 192);
				if (!twitterGuiEnabled) TextureAsset(L"i5").resize(icon_size).draw(pos);
				else TextureAsset(L"i6").resize(icon_size).draw(pos);

				//建設モード切替
				pos = Point(0, 128 + 192);
				if (!constructingModeEnabled) TextureAsset(L"i7").resize(icon_size).draw(pos);
				else TextureAsset(L"i8").resize(icon_size).draw(pos);
				if (Rect(pos, icon_size).mouseOver && Input::MouseL.clicked) {
					const bool f = constructingModeEnabled;
					closeAllInterfaceMode();
					constructingModeEnabled = !f;
					SoundAsset(L"2").play();
				}

				//破壊モード
				pos = Point(0, 128 + 256);
				if (!destroyerModeEnabled) TextureAsset(L"i9").resize(icon_size).draw(pos);
				else TextureAsset(L"i10").resize(icon_size).draw(pos);
				if (Rect(pos, icon_size).mouseOver && Input::MouseL.clicked) {
					const bool f = destroyerModeEnabled;
					closeAllInterfaceMode();
					destroyerModeEnabled = !f;
					SoundAsset(L"2").play();
				}
				//二次破壊モード
				if (destroyerModeEnabled)
				{
					pos.moveBy(64, 0);
					if (!roadDestroyerModeEnabled) TextureAsset(L"i17").resize(icon_size).draw(pos);
					else TextureAsset(L"i18").resize(icon_size).draw(pos);
					if (Rect(pos, icon_size).mouseOver && Input::MouseL.clicked) {
						const bool f = roadDestroyerModeEnabled;
						closeAllInterfaceMode();
						roadDestroyerModeEnabled = !f;
						SoundAsset(L"2").play();
					}
					pos.moveBy(64, 0);
					if (!facilityDestroyerModeEnabled) TextureAsset(L"i19").resize(icon_size).draw(pos);
					else TextureAsset(L"i20").resize(icon_size).draw(pos);
					if (Rect(pos, icon_size).mouseOver && Input::MouseL.clicked) {
						const bool f = facilityDestroyerModeEnabled;
						closeAllInterfaceMode();
						facilityDestroyerModeEnabled = !f;
						SoundAsset(L"2").play();
					}
					destroyerModeEnabled = true;
				}

				//道路敷設モード
				pos = Point(0, 128 + 256 + 64);
				if (!makingRoadModeEnabled) TextureAsset(L"i15").resize(icon_size).draw(pos);
				else TextureAsset(L"i16").resize(icon_size).draw(pos);
				if (Rect(pos, icon_size).mouseOver && Input::MouseL.clicked) {
					const bool f = makingRoadModeEnabled;
					closeAllInterfaceMode();
					makingRoadModeEnabled = !f;
					SoundAsset(L"2").play();
				}

				//村づくりモード
				pos = Point(0, 128 + 256 + 128);
				if (!fallingDownModeEnabled) TextureAsset(L"i11").resize(icon_size).draw(pos);
				else TextureAsset(L"i12").resize(icon_size).draw(pos);
				if (Rect(pos, icon_size).mouseOver && Input::MouseL.clicked) {
					const bool f = fallingDownModeEnabled;
					closeAllInterfaceMode();
					fallingDownModeEnabled = !f;
					SoundAsset(L"2").play();
				}

				if (Input::Key1.clicked) { SoundAsset(L"2").play(); closeAllInterfaceMode(); constructingModeEnabled = true; }
				if (Input::Key2.clicked) { SoundAsset(L"2").play(); closeAllInterfaceMode(); destroyerModeEnabled = true; roadDestroyerModeEnabled = true; }
				if (Input::Key3.clicked) { SoundAsset(L"2").play(); closeAllInterfaceMode(); destroyerModeEnabled = true; facilityDestroyerModeEnabled = true; }
				if (Input::Key4.clicked) { SoundAsset(L"2").play(); closeAllInterfaceMode(); makingRoadModeEnabled = true; }
				if (Input::Key5.clicked) { SoundAsset(L"2").play(); closeAllInterfaceMode(); fallingDownModeEnabled = true; }

				if (Input::MouseR.clicked) mouseRPressCount = 15;
				if (mouseRPressCount > 0) mouseRPressCount--;
				if (Input::MouseR.released && mouseRPressCount > 0) closeAllInterfaceMode();
			}



			//明るさの設定
			if (nightEnabled)
			{
				int light = 128;
				Color sky = Color(255, 255, 255, 0);
				double a = (double)(sec - 14400) / 14400.0;
				if (a >= 0 && a <= 1) sky = Color(0, 0, 0, light - (uint32)(a * light));
				a = (double)(86400 - 14400 - sec) / 14400.0;
				if (a >= 0 && a <= 1) sky = Color(0, 0, 0, light - (uint32)(a * light));
				if (sec > 86400 - 14400 || sec < 14400) sky = Color(0, 0, 0, light);
				Rect(0, 0, Window::Size()).draw(sky);
			}


			if (constructingModeEnabled) Cursor::SetStyle(CursorStyle::Custom(TextureAsset(L"mouse2")));
			else if (facilityDestroyerModeEnabled || roadDestroyerModeEnabled) Cursor::SetStyle(CursorStyle::Custom(TextureAsset(L"mouse3")));
			else if (fallingDownModeEnabled) { Cursor::SetStyle(CursorStyle::Custom(TextureAsset(L"mouse4"))); }
			else if (makingRoadModeEnabled)
			{
				if (Input::MouseL.pressed) Cursor::SetStyle(CursorStyle::Custom(TextureAsset(L"mouse6")));
				else  Cursor::SetStyle(CursorStyle::Custom(TextureAsset(L"mouse5")));
			}
			else Cursor::SetStyle(CursorStyle::Custom(TextureAsset(L"mouse1")));
		}
		camera->draw(Color(0, 0, 0, 0));
		int j = 0;
		if (Input::KeyT.clicked) superSpeed = !superSpeed;
		for (;;)
		{
			actionPoint = 1;
			if (Input::KeyShift.pressed) actionPoint = 60;
			if (Input::KeySpace.pressed) actionPoint = 1200;
			if (superSpeed) actionPoint = 3600;
			if (Input::KeyEnter.pressed) actionPoint = s1;
			sec += actionPoint;
			if (86400 < sec)
			{
				for (auto& r : regions)
				{
					for (int i = 0; i < LOG_ITEMS_MAX; i++)
					{
						r.ItemsAmount2[i] = r.ItemsAmount[i];
						r.ItemsAmount[i] = 0;
						r.ItemsID2[i] = r.ItemsID[i];
					}
					r.NumItems = 0;
				}
				sec -= 86400;
				season++;
				if (season > 12) { season = 1; year++; }
			}
			//FacilityのUnitをセット
			updateUnits();
			j += actionPoint;

			updateAchievement();
			if (Input::KeyEnter.pressed)
			{
				s2--;
				if (s2 == 0) break;
			}
			else
			{
				if (c.us() > 16666 || !superSpeed) break;
				if (!superSpeed && j >= 2000) break;
			}
		}

		//情報タブ
		speedGui.text(L"wood").text = Format(L"木材の生産量:");
		speedGui.text(L"rice").text = Format(L"米の生産量:");
		speedGui.text(L"sake").text = Format(L"日本酒の生産量:");
		if (mouseOveredChip != NULL && mouseOveredChip->getJoinedRegion() != NULL)
		{
			Region& r = *mouseOveredChip->getJoinedRegion();
			speedGui.text(L"nunit").text = Format(L"ユニット数:", r.NumUnits);

			for (int i = 0; i < LOG_ITEMS_MAX; i++)
			{
				if (r.ItemsAmount2[i] > 0)
				{
					switch (r.ItemsID2[i])
					{
					case Rice:
						speedGui.text(L"rice").text = Format(L"米の生産量:", r.ItemsAmount2[i]);
						break;
					case Wood:
						speedGui.text(L"wood").text = Format(L"木材の生産量:", r.ItemsAmount2[i]);
						break;
					case Sake:
						speedGui.text(L"sake").text = Format(L"日本酒の生産量:", r.ItemsAmount2[i]);
						break;
					default:
						break;
					}
				}
			}




		}
		const String s = Format(L"Platanus マップID:", downloadMap, L"\n",
			year, L"年目", season, L"月", sec / 3600, L"時 ", (sec / 60) % 60, L"分\n",
			j, L"ループ\n",
			numUnits, L"ユニット\n",
			twitterText1);

		font().draw(2, 1, Palette::Black, 1.0);
		font(s).draw(1, 2, Palette::Black, 1.0);
		font(s).draw(0, 1, Palette::Black, 1.0);
		font(s).draw(1, 0, Palette::Black, 1.0);
		font(s).draw(1, 1, Palette::White, 1.0);


		twitterUpdate();

		if (fillter > 0)
		{
			RectF(Window::Size()).draw(Color(0, 0, 255, fillter));
			fillter -= 8;
		}

		//Escキーの処理
		if (Input::KeyEscape.clicked)
		{
			Cursor::SetStyle(CursorStyle::Default);
			ScreenCapture::Request();
			System::Update();
			Image scrCap;
			ScreenCapture::GetFrame(scrCap);
			Texture screen(scrCap);
			Texture screen2(scrCap.gaussianBlurred(30, 30));
			screen.draw();
			WidgetStyle widgetStyle2;
			widgetStyle2.width = 120;
			GUI guiOption(guiStyle);
			guiOption.setTitle(L"オプション");
			guiOption.addln(L"back", GUIButton::Create(L"戻る", widgetStyle2));
			guiOption.addln(L"save", GUIButton::Create(L"セーブ", widgetStyle2));
			guiOption.addln(L"load", GUIButton::Create(L"ロード", widgetStyle2));
			guiOption.addln(L"configuration", GUIButton::Create(L"環境設定", widgetStyle2));
			guiOption.addln(L"end", GUIButton::Create(L"終了", widgetStyle2));
			guiOption.setCenter(Window::Center());
			guiOption.show();
			Stopwatch t(true);
			SoundAsset(L"11").play();
			while (System::Update())
			{
				if (t.ms() < 200)
				{
					screen.draw();
					screen2.draw(AlphaF(t.ms() / 200.0));
				}
				else screen2.draw();
				if (guiOption.button(L"back").pushed || Input::KeyEscape.clicked) { SoundAsset(L"10").play(); break; }
				if (guiOption.button(L"save").pushed) { if (saveAll()) { SoundAsset(L"9").play();  break; } }
				if (guiOption.button(L"load").pushed) { if (loadAll()) { SoundAsset(L"9").play();  break; } }
				if (guiOption.button(L"end").pushed) { System::Exit(); }
				if (guiOption.button(L"configuration").pushed)
				{
					GUI guiConfiguration(guiStyle);
					guiConfiguration.setTitle(L"環境設定");
					guiConfiguration.addln(L"mts", GUIToggleSwitch::Create(L"音楽", L"音楽", SoundAsset(L"music1").isPlaying(), widgetStyle2));
					guiConfiguration.addln(L"lts", GUIToggleSwitch::Create(L"結合線", L"結合線", lineEnabled, widgetStyle2));
					guiConfiguration.addln(GUIText::Create(L"効果音音量", widgetStyle2));
					guiConfiguration.addln(L"sl1", GUISlider::Create(0.0, 1.0, soundEffectsVolume, true, widgetStyle2));
					guiConfiguration.addln(GUIText::Create(L"音楽音量", widgetStyle2));
					guiConfiguration.addln(L"sl2", GUISlider::Create(0.0, 1.0, musicVolume, true, widgetStyle2));
					guiConfiguration.add(L"b1", GUIButton::Create(L"保存", widgetStyle2));
					guiConfiguration.setCenter(Window::Center());
					guiConfiguration.show();
					guiOption.hide();
					SoundAsset(L"11").play();
					while (System::Update())
					{
						if (t.ms() < 200)
						{
							screen.draw();
							screen2.draw(AlphaF(t.ms() / 200.0));
						}
						else screen2.draw();

						if (guiConfiguration.toggleSwitch(L"mts").hasChanged)
						{
							if (SoundAsset(L"music1").isPaused()) SoundAsset(L"music1").play();
							else SoundAsset(L"music1").pause();
							SoundAsset(L"2").play();
						}
						if (guiConfiguration.toggleSwitch(L"lts").hasChanged)
						{
							lineEnabled = !lineEnabled;
							SoundAsset(L"2").play();
						}
						if (guiConfiguration.slider(L"sl1").hasChanged)
						{
							soundEffectsVolume = guiConfiguration.slider(L"sl1").value;
							SoundAsset(L"1").setVolume(soundEffectsVolume);
							SoundAsset(L"2").setVolume(soundEffectsVolume);
							SoundAsset(L"3").setVolume(soundEffectsVolume);
							SoundAsset(L"4").setVolume(soundEffectsVolume);
							SoundAsset(L"5").setVolume(soundEffectsVolume);
							SoundAsset(L"6").setVolume(soundEffectsVolume);
							SoundAsset(L"7").setVolume(soundEffectsVolume);
							SoundAsset(L"8").setVolume(soundEffectsVolume);
							SoundAsset(L"9").setVolume(soundEffectsVolume);
							SoundAsset(L"10").setVolume(soundEffectsVolume);
							SoundAsset(L"11").setVolume(soundEffectsVolume);
							SoundAsset(L"2").play();
						}
						if (guiConfiguration.slider(L"sl2").hasChanged)
						{
							musicVolume = guiConfiguration.slider(L"sl2").value;
							SoundAsset(L"music1").setVolume(musicVolume);

						}
						if (guiConfiguration.button(L"b1").pushed || Input::KeyEscape.clicked) { SoundAsset(L"10").play(); break; }
					}
					guiConfiguration.hide();
					guiOption.show();
				}
			}
			t.reset();
		}
	}
}

bool	isNight()
{
	if (sec > 21600 && sec < 64800)
	{
		return false;
	}
	return true;
}