#pragma once

#include "gameNode.h"
#define TILE_X 20		//가로 타일 수
#define TILE_Y 20		//세로 타일 수
#define WIDTH 40		//타일 가로 길이
#define HEIGHT 40		//타일 세로 길이
#define BIGNUM 5000		//충분히 큰 값 (F 초기값)

enum TILE_TYPE
{
	TILE_TYPE_EMPTY,	//빈 타일 ( 지나갈 수 있는 )
	TILE_TYPE_START,
	TILE_TYPE_END,
	TILE_TYPE_WALL
};

enum ASTAR_STATE
{
	ASTAR_STATE_SEARCHING,	//검색 중
	ASTAR_STATE_FOUND,		//경로 발견
	ASTAR_STATE_NOWAY,		//경로가 없다
	ASTAR_STATE_END			//타일 배치 완료 전
};

struct tagTile
{
	RECT rc;
	COLORREF color;
	bool walkable;		//지나갈 수 있는지 여부
	bool listOn;		//openList 추가 되었는지 여부
	int i, j;			//배열 인덱스 i, j 값
	int F, G, H;
	char str[128];
	tagTile* parent;
	TILE_TYPE type;
};


class aStar : public gameNode
{
private:
	tagTile _tile[TILE_X][TILE_Y];
	vector<tagTile*> _openList;
	vector<tagTile*> _closeList;

	ASTAR_STATE _aStarState;

	TILE_TYPE _selectType;
	COLORREF _selectTypeColor;

	int _startX, _startY;
	int _endX, _endY;
	int _lastIndex;		//가장 나중에 추가된 클로즈 리스트의 인덱스

	bool _startPointSet;
	bool _endPointSet;

	int Ci;				//마지막으로 추가된 클로즈 리스트의 i
	int Cj;				//마지막으로 추가된 클로즈 리스트의 j
	int Cg;				//마지막으로 추가된 클로즈 리스트의 G


public:
	aStar();
	~aStar();

	HRESULT init();
	void relase();
	void update();
	void render();

	void tileComposition();			//타일 배치
	void tileInitializing();		//타일 배치 후 타일 속성에 맞게 초기화
	void addOpenList();				//오픈리스트에 추가
	void calculateH();				//오픈리스트 내 타일의 H값 계산
	void calculateF();				//오픈리스트 내 타일의 F값 계산
	void addCloseList();			//가장 작은 F 선택 후 클로즈 리스트에 추가
	void checkArrive();				//도착 했는지 여부를 체크
	void showWay(tagTile* tile);	//도착 후 시작지점과 도착지점을 잇는 경로를 보여주는 함수
};

