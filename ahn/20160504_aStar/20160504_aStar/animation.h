#pragma once

#include <vector>

class animation
{
public:
	typedef std::vector<POINT> _vFrameList;	//������ ������ ���� ������ �����ؾ� ��.
	typedef std::vector<int> _vPlayList;	//������ �÷��� ����Ʈ

private:
	int _frameNum;				 //������ ��
								 
	_vFrameList _frameList;		 //������ ��ġ ����Ʈ
	_vPlayList _playList;		 //�÷��� ����Ʈ
								 
	int _frameWidth;			 //������ ���� ũ��
	int _frameHeight;			 //������ ���� ũ��
								 
	BOOL _loop;					 //���� ����
	float _frameUpdateSec;		 //������ ������Ʈ ��
	float _elapsedSec;			 //������ ���� ��
								 
	DWORD _nowPlayIdx;			 //���� �÷��� �ε���
	BOOL _play;					 //���� �÷��� ����

public:
	animation();
	~animation();

	HRESULT init(int totalW, int totalH, int frameW, int frameH);

	void setDefPlayFrame(BOOL reverse = FALSE, BOOL loop = FALSE);

	void setPlayFrame(int* playArr, int arrLen, BOOL loop = FALSE);
	void setPlayFrame(int start, int end, BOOL reverse = FALSE, BOOL loop = FALSE);

	void setFPS(int framePerSec);

	void frameUpdate(float elapsedTime);

	void start();
	void stop();
	void pause();
	void resume();

	//�÷��� ���� Ȯ��
	inline BOOL isPlay() { return _play; }

	//������ ��ġ�� ��´�
	inline POINT getFramePos() { return _frameList[_playList[_nowPlayIdx]]; }

	//������ ���� ũ�� ��´�
	inline int getFrameWidth() { return _frameWidth; }

	//������ ���� ũ�� ��´�
	inline int getFrameHeight() { return _frameHeight; }
};

