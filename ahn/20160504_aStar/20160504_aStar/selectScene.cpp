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
	sprintf_s(str, "����� ����Ʈ ���̴�");
	TextOut(getMemDC(), 100, 300, str, strlen(str));

	HFONT oldFont;
	HFONT font1;

	//font Create
	/*
	CreateFont(60, 		 //���� ����
		0, 				 //���� ��
		0, 				 //���ڿ� ��� ����
		0, 				 //���� ����
		600,			 //���� ����
		1,				 //���� (�Ұ�)
		1,				 //���� (�Ұ�)
		0,				 //��Ҽ�
		HANGEUL_CHARSET, //���� ����
		0,				 //��� ��Ȯ��
		0,				 //Ŭ���� ��Ȯ��
		0, 				 //����� ��
		0,				 //�ڰ�
		TEXT("����ü"));	 //��Ʈ
		*/

	//font1 = CreateFont(60, 0, 100, 0, 600, 1, 1, 1, HANGEUL_CHARSET, 0, 0, 0, 0, TEXT("�ü�ü"));
	//oldFont = (HFONT)SelectObject(getMemDC(), font1);
	//sprintf_s(str, "���� ����... �����");
	//TextOut(getMemDC(), 200, 200, str, strlen(str));
	//SelectObject(getMemDC(), oldFont);
	//DeleteObject(font1);

	////��� 2
	//RECT rcText = { 100, 400, 800, 500 };
	//HFONT font2 = CreateFont(60, 0, 0, 0, FW_NORMAL,
	//	false, false, 0,
	//	DEFAULT_CHARSET,
	//	OUT_SCREEN_OUTLINE_PRECIS,
	//	CLIP_DEFAULT_PRECIS,
	//	DRAFT_QUALITY,
	//	DEFAULT_PITCH | FF_SWISS,
	//	"���� ���");

	//oldFont = (HFONT)SelectObject(getMemDC(), font2);
	//DrawText(getMemDC(), TEXT("Ű���� ��� �ʹ�"), 16, &rcText, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
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