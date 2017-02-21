#include"Header.h"

int twitterCameraFlash = 0;
int twitterTextAlphaColor = 0;

Image* scrCapImageP;

void twitterUpdate()
{

	Image thumbnail;


	//ツイートするかどうか
	if (Rect(0, 192, 64, 64).mouseOver && Input::MouseL.clicked)
	{
		constructingModeEnabled = false;


		if (!twitterGuiEnabled)
		{

			//スクショを取る
			ScreenCapture::Request();
			System::Update();
			ScreenCapture::GetFrame(*scrCapImageP);
			Imaging::Fit(*scrCapImageP, thumbnail, 300, 300);
			guiTweetP->texture(L"scrCapImageP").texture = Texture(thumbnail);

			//フラッシュ
			twitterCameraFlash = 255;
			SoundAsset(L"camera").play();

			//スクショ
			if (!twitterAuthEnabled) { twitterP->openTokenPage(); guiAuthP->show(); }
			else
			{
				guiTweetP->show();
				guiTweetP->textArea(L"text").setText(Format(twitterText1, twitterText2, L"みんなもPlatanusで遊ぼう！\n開発版なのでフリーでダウンロードできます↓\n http://www.nicovideo.jp/mylist/58089940 \n@LPC1768_ #PlatanusPart16"));
			}

			twitterGuiEnabled = true;
		}
		else closeTwitterGUI();
	}
	if (twitterGuiEnabled)
	{
		if (twitterAuthEnabled)
		{
			if (guiTweetP->button(L"selectImage").pushed) { guiTweetP->hide(); guiAuthP->hide(); twitterGuiEnabled = false; }

			if (guiTweetP->button(L"tweet").pushed)
			{
				twitterP->tweetWithMedia(guiTweetP->textArea(L"text").text, *scrCapImageP);
				closeTwitterGUI();
				SoundAsset(L"1").play();
				return;
			}
			if (guiTweetP->getRect().mouseOver && Input::MouseR.clicked) closeTwitterGUI();
		}
		else
		{
			if (twitterTextAlphaColor > 0) 
			{
				guiAuthP->text(L"text").style.color = Color(255, 0, 0, twitterTextAlphaColor);
				twitterTextAlphaColor -= 4; 
			}
			guiAuthP->button(L"auth").enabled = (guiAuthP->textField(L"PIN").text.length == 7);

			if (guiAuthP->button(L"auth").pushed)
			{
				if (twitterP->verifyPIN(guiAuthP->textField(L"PIN").text))
				{
					guiAuthP->release();
					guiTweetP->show();
					guiTweetP->textArea(L"text").setText(Format(twitterText1, twitterText2, L"みんなもPlatanusで遊ぼう！\n開発版なのでフリーでダウンロードできます↓\n http://www.nicovideo.jp/mylist/58089940 \n@LPC1768_ #PlatanusPart16"));

					twitterAuthEnabled = true;
				}
				else twitterTextAlphaColor = 255;
			}
			if (guiAuthP->getRect().mouseOver && Input::MouseR.clicked) closeTwitterGUI();
		}
	}


	//フラッシュ
	if (twitterCameraFlash > 0)
	{
		Rect(Point(0, 0), Window::Size()).draw(Color(255, 255, 200, twitterCameraFlash));
		twitterCameraFlash -= 3;
	}
}
void closeTwitterGUI()
{
	if (!twitterAuthEnabled) guiAuthP->hide();
	else guiTweetP->hide();
	twitterGuiEnabled = false;
}

void twitterInit()
{
	guiAuthP->setTitle(L"認証PIN の入力");
	guiAuthP->addln(L"PIN", GUITextField::Create(7));
	guiAuthP->add(L"auth", GUIButton::Create(L"認証", *widgetStyleP));
	guiAuthP->add(L"text", GUIText::Create(L"認証失敗", *widgetStyleP));
	guiAuthP->text(L"text").style.color = Color(255, 0, 0, 0);
	guiAuthP->setPos(128, 64);
	guiAuthP->hide();
	guiTweetP->setTitle(L"スクショをツイートする");
	guiTweetP->add(L"text", GUITextArea::Create(12, 12, 140));
	guiTweetP->textArea(L"text").setText(L"Platanusで遊んでるよ！\n@LPC1768_ #PlatanusPart16");
	guiTweetP->addln(L"scrCapImageP", GUITexture::Create(Texture()));
	guiTweetP->add(L"selectImage", GUIButton::Create(L"キャンセル", *widgetStyleP));
	guiTweetP->add(L"tweet", GUIButton::Create(L"スクショをツイート", *widgetStyleP));
	guiTweetP->setPos(128, 64);
	guiTweetP->hide();

}