#pragma once

#include <vector>

class animation
{
public:
	typedef std::vector<POINT> _vFrameList;	//프레임 접근은 임의 접근이 가능해야 함.
	typedef std::vector<int> _vPlayList;	//프레임 플레이 리스트

private:
	int _frameNum;				 //프레임 수
								 
	_vFrameList _frameList;		 //프레임 위치 리스트
	_vPlayList _playList;		 //플레이 리스트
								 
	int _frameWidth;			 //프레임 가로 크기
	int _frameHeight;			 //프레임 세로 크기
								 
	BOOL _loop;					 //루프 여부
	float _frameUpdateSec;		 //프레임 업데이트 초
	float _elapsedSec;			 //프레임 지난 초
								 
	DWORD _nowPlayIdx;			 //현재 플레이 인덱스
	BOOL _play;					 //현재 플레이 여부

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

	//플레이 여부 확인
	inline BOOL isPlay() { return _play; }

	//프레임 위치를 얻는다
	inline POINT getFramePos() { return _frameList[_playList[_nowPlayIdx]]; }

	//프레임 가로 크기 얻는다
	inline int getFrameWidth() { return _frameWidth; }

	//프레임 세로 크기 얻는다
	inline int getFrameHeight() { return _frameHeight; }
};

