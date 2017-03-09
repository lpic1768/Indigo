#include"Basic.h"
# include <Siv3D.hpp>
# include <HamFramework.hpp>

double soundVolume = 0.1;

void Main()
{
	//Window設定
	Window::SetTitle(L"Platanus");
	Window::SetStyle(WindowStyle::Sizeable);
	Window::SetFullscreen(false, Size(1280, 720));
	Window::Maximize();

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

	//ボタンアセット
	for (int i = 1; i <= 22; i++) TextureAsset::Register(Format(L"b", i), Format(L"Assets/b", i, L".png"));
	TextureAsset::Register(L"button", L"Assets/Button.png");

	SoundAsset::Register(L"music1", L"Assets/Touch_Next.mp3");
	for (int i = 1;; i++)
	{
		const FilePath soundAssetsPath = Format(L"Assets/", i, L".mp3");
		if (!FileSystem::Exists(soundAssetsPath)) break;
		SoundAsset::Register(Format(i), soundAssetsPath);
	}

	Graphics::SetBackground(Palette::Blue);

	InitAll();
	Camera2D camera(Vec2(chipX, chipY) / 2 * ChipImageSize, 0.1);

	Font font(24);

	while (System::Update())
	{
		Window::SetBaseSize(Window::Size());

		if (Input::KeyF5.clicked) InitAll();

		camera.update();
		{
			const auto t1 = camera.createTransformer();

			updateAll();

			drawAll();

		}
		camera.draw(Palette::Orange);

		UpdateInterface();



		font(L"Platanus\n", month + 1, L"月 ", getHour(), L"時 ", getMinute(), L"分").draw();
	}
}
