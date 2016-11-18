#include "stdafx.h"
#include "selectScene.h"


selectScene::selectScene()
{
}


selectScene::~selectScene()
{
}

HRESULT selectScene::init()
{
	IMAGEMANAGER->addFrameImage("buttonAnlgeUp", "image/btnAngleUp.bmp", 0, 0, 186, 104, 1, 2, true, RGB(255, 0, 255));
	IMAGEMANAGER->addFrameImage("buttonAnlgeDown", "image/btnAngleDown.bmp", 0, 0, 186, 104, 1, 2, true, RGB(255, 0, 255));

	_buttonAngleUp = new button;
	_buttonAngleUp->init("buttonAnlgeUp", WINSIZEX / 2, WINSIZEY / 2,
		PointMake(0, 1), PointMake(0, 0), onAngleUp, this);

	_buttonAngleDown = new button;
	_buttonAngleDown->init("buttonAnlgeDown", WINSIZEX / 2, WINSIZEY / 2 + 50,
		PointMake(0, 1), PointMake(0, 0), onAngleDown, this);

	_sampleImage = IMAGEMANAGER->addImage("sampleImage", "image/civil_war.bmp", 540, 288, false, false);

	_visible = false;

	return S_OK;
}

void selectScene::release()
{
	SAFE_DELETE(_buttonAngleUp);
	SAFE_DELETE(_buttonAngleDown);
}

void selectScene::update()
{
	_buttonAngleUp->update();
	_buttonAngleDown->update();

	if (KEYMANAGER->isOnceKeyDown(VK_RETURN)) SCENEMANAGER->changeScene("starwars");
}

void selectScene::render()
{
	char str[128];
	sprintf_s(str, "여기는 셀렉트 씬이다");
	TextOut(getMemDC(), 100, 300, str, strlen(str));

	HFONT oldFont;
	HFONT font1;

	//font Create
	/*
	CreateFont(60, 		 //문자 높이
		0, 				 //문자 폭
		0, 				 //문자열 출력 각도
		0, 				 //문자 방향
		600,			 //문자 굵기
		1,				 //기울기 (불값)
		1,				 //밑줄 (불값)
		0,				 //취소선
		HANGEUL_CHARSET, //문자 셋팅
		0,				 //출력 정확도
		0,				 //클리핑 정확도
		0, 				 //출력의 질
		0,				 //자간
		TEXT("돋움체"));	 //폰트
		*/

	//font1 = CreateFont(60, 0, 100, 0, 600, 1, 1, 1, HANGEUL_CHARSET, 0, 0, 0, 0, TEXT("궁서체"));
	//oldFont = (HFONT)SelectObject(getMemDC(), font1);
	//sprintf_s(str, "답이 없네... 살려줘");
	//TextOut(getMemDC(), 200, 200, str, strlen(str));
	//SelectObject(getMemDC(), oldFont);
	//DeleteObject(font1);

	////출력 2
	//RECT rcText = { 100, 400, 800, 500 };
	//HFONT font2 = CreateFont(60, 0, 0, 0, FW_NORMAL,
	//	false, false, 0,
	//	DEFAULT_CHARSET,
	//	OUT_SCREEN_OUTLINE_PRECIS,
	//	CLIP_DEFAULT_PRECIS,
	//	DRAFT_QUALITY,
	//	DEFAULT_PITCH | FF_SWISS,
	//	"맑은 고딕");

	//oldFont = (HFONT)SelectObject(getMemDC(), font2);
	//DrawText(getMemDC(), TEXT("키보드 사고 싶다"), 16, &rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	//SelectObject(getMemDC(), oldFont);
	//DeleteObject(font2);

	_buttonAngleUp->render();
	_buttonAngleDown->render();

	if (_visible) _sampleImage->render(getMemDC(), 100, 100);
}

void selectScene::onAngleUp(void* obj)
{
	selectScene* ss = (selectScene*)obj;
	ss->setVisible(false);

	DATABASE->setElementDataAngle("battle", DATABASE->getElementData("battle")->angle + 0.2f);
}

void selectScene::onAngleDown(void* obj)
{
	selectScene* ss = (selectScene*)obj;
	ss->setVisible(true);

	DATABASE->setElementDataAngle("battle", DATABASE->getElementData("battle")->angle - 0.2f);
}