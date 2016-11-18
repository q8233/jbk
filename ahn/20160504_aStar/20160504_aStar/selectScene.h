#pragma once

#include "gameNode.h"
#include "button.h"

class selectScene : public gameNode
{
private:
	button* _buttonAngleUp;
	button* _buttonAngleDown;

	image* _sampleImage;

	bool _visible;

public:
	selectScene();
	~selectScene();

	HRESULT init();
	void release();
	void update();
	void render();

	static void onAngleUp(void* obj);
	static void onAngleDown(void* obj);

	image* getSampleImage() { return _sampleImage; }
	void setVisible(bool visible) { _visible = visible; }
};

