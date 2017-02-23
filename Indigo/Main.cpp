#include"Basic.h"
# include <Siv3D.hpp>
# include <HamFramework.hpp>



void Main()
{
	//Window設定
	Window::SetTitle(L"Platanus");
	Window::SetStyle(WindowStyle::Sizeable);
	Window::SetFullscreen(false, Size(1280, 720));
	Window::Maximize();

	FontAsset::Register(L"drawPlaceNameFont", 24, Typeface::Medium, FontStyle::Outline);
	FontAsset(L"drawPlaceNameFont").changeOutlineStyle(TextOutlineStyle(Palette::Black, Palette::White, 1.0));

	//Placeアセット
	TextureAsset::Register(L"house", L"Assets/House.png");
	TextureAsset::Register(L"market", L"Assets/Market.png");
	TextureAsset::Register(L"farm", L"Assets/Farm.png");

	//ボタンアセット
	for (int i = 1; i <= 22; i++) TextureAsset::Register(Format(L"b", i), Format(L"Assets/b", i, L".png"));


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

		font.draw(L"Platanus");
	}
}
