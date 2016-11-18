#pragma once

#include "gameNode.h"

class ship : public gameNode
{
protected:
	image* _image;
	RECT _rc;
	float _x, _y;
	float _angle;
	float _speed;
	float _radius;

public:
	ship();
	~ship();

	virtual HRESULT init(const char* imageName, int x, int y);
	virtual void release() override;
	virtual void update() override;
	virtual void render() override;

	virtual void draw();
	virtual void keyControl();
	virtual void move();

	void setAngle(float angle) { _angle = angle; }
	float getAngle() { return _angle; }

	//============ Ãß°¡ µÊ ============== 
	virtual void focusDraw();
	virtual void defaultDraw(RECT rcFocus);

	image* getImage() { return _image; }
	RECT getRect() { return _rc; }
};

