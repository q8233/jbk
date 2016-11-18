#pragma once

#include "gameNode.h"
#define TILE_X 20		//���� Ÿ�� ��
#define TILE_Y 20		//���� Ÿ�� ��
#define WIDTH 40		//Ÿ�� ���� ����
#define HEIGHT 40		//Ÿ�� ���� ����
#define BIGNUM 5000		//����� ū �� (F �ʱⰪ)

enum TILE_TYPE
{
	TILE_TYPE_EMPTY,	//�� Ÿ�� ( ������ �� �ִ� )
	TILE_TYPE_START,
	TILE_TYPE_END,
	TILE_TYPE_WALL
};

enum ASTAR_STATE
{
	ASTAR_STATE_SEARCHING,	//�˻� ��
	ASTAR_STATE_FOUND,		//��� �߰�
	ASTAR_STATE_NOWAY,		//��ΰ� ����
	ASTAR_STATE_END			//Ÿ�� ��ġ �Ϸ� ��
};

struct tagTile
{
	RECT rc;
	COLORREF color;
	bool walkable;		//������ �� �ִ��� ����
	bool listOn;		//openList �߰� �Ǿ����� ����
	int i, j;			//�迭 �ε��� i, j ��
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
	int _lastIndex;		//���� ���߿� �߰��� Ŭ���� ����Ʈ�� �ε���

	bool _startPointSet;
	bool _endPointSet;

	int Ci;				//���������� �߰��� Ŭ���� ����Ʈ�� i
	int Cj;				//���������� �߰��� Ŭ���� ����Ʈ�� j
	int Cg;				//���������� �߰��� Ŭ���� ����Ʈ�� G


public:
	aStar();
	~aStar();

	HRESULT init();
	void relase();
	void update();
	void render();

	void tileComposition();			//Ÿ�� ��ġ
	void tileInitializing();		//Ÿ�� ��ġ �� Ÿ�� �Ӽ��� �°� �ʱ�ȭ
	void addOpenList();				//���¸���Ʈ�� �߰�
	void calculateH();				//���¸���Ʈ �� Ÿ���� H�� ���
	void calculateF();				//���¸���Ʈ �� Ÿ���� F�� ���
	void addCloseList();			//���� ���� F ���� �� Ŭ���� ����Ʈ�� �߰�
	void checkArrive();				//���� �ߴ��� ���θ� üũ
	void showWay(tagTile* tile);	//���� �� ���������� ���������� �մ� ��θ� �����ִ� �Լ�
};

