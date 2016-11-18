#pragma once

#include "gameNode.h"

typedef void(*CALLBACK_FUNCTION)(void);
typedef void(*CALLBACK_FUNCTION_PARAMETER)(void*);

enum BUTTONDIRECTION
{
	BUTTONDIRECTION_NULL,
	BUTTONDIRECTION_UP,
	BUTTONDIRECTION_DOWN
};

class button : public gameNode
{
private:
	BUTTONDIRECTION _direction;
	const char* _imageName;
	image* _image;
	RECT _rc;
	float _x, _y;
	void* _obj;

	POINT _btnDownFramePoint;
	POINT _btnUpFramePoint;

	CALLBACK_FUNCTION _callbackFunction; //콜백함수 원형 선언
	CALLBACK_FUNCTION_PARAMETER _callbackFunctionParameter;
public:
	button();
	~button();

	HRESULT init(const char* imageName, int x, int y,
		POINT btnDownFramePoint, POINT btnUpFramePoint, 
		CALLBACK_FUNCTION cbFunction);

	HRESULT init(const char* imageName, int x, int y,
		POINT btnDownFramePoint, POINT btnUpFramePoint, 
		CALLBACK_FUNCTION_PARAMETER cbFunction, void* obj);

	void release();
	void update();
	void render();
};

