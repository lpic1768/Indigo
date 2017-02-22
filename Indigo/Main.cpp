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
	TextureAsset::Register(L"b1", L"Assets/b01.png");
	TextureAsset::Register(L"b2", L"Assets/b02.png");
	TextureAsset::Register(L"b3", L"Assets/b03.png");
	TextureAsset::Register(L"b4", L"Assets/b04.png");
	TextureAsset::Register(L"b5", L"Assets/b05.png");
	TextureAsset::Register(L"b6", L"Assets/b06.png");
	TextureAsset::Register(L"b7", L"Assets/b07.png");
	TextureAsset::Register(L"b8", L"Assets/b08.png");
	TextureAsset::Register(L"b9", L"Assets/b09.png");
	TextureAsset::Register(L"b10", L"Assets/b10.png");
	TextureAsset::Register(L"b11", L"Assets/b11.png");
	TextureAsset::Register(L"b12", L"Assets/b12.png");
	TextureAsset::Register(L"b13", L"Assets/b13.png");
	TextureAsset::Register(L"b14", L"Assets/b14.png");
	TextureAsset::Register(L"b15", L"Assets/b15.png");
	TextureAsset::Register(L"b16", L"Assets/b16.png");
	TextureAsset::Register(L"b17", L"Assets/b17.png");
	TextureAsset::Register(L"b18", L"Assets/b18.png");
	TextureAsset::Register(L"b19", L"Assets/b19.png");
	TextureAsset::Register(L"b20", L"Assets/b20.png");


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

	InitAll();
	Camera2D camera(Vec2(chipX, chipY) / 2 * ChipImageSize, 0.1);

	Font font(24);
	Graphics::SetBackground(Palette::Blue);

	while (System::Update())
	{
		Window::SetBaseSize(Window::Size());

		if (Input::KeyF5.clicked) InitAll();

		camera.update();
		{
			const auto t1 = camera.createTransformer();


			drawAll();

		}
		camera.draw(Palette::Orange);

		UpdateInterface();

		font.draw(L"Platanus");
	}
}
