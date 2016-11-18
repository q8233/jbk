#include "stdafx.h"
#include "aStar.h"


aStar::aStar()
{
}


aStar::~aStar()
{
}

HRESULT aStar::init()
{
	_aStarState = ASTAR_STATE_END; //�ʱ� aStar �˰��� ���� = Ÿ�� ��ġ �Ϸ� ��.

	_selectType = TILE_TYPE_EMPTY;
	_selectTypeColor = RGB(255, 255, 255);

	_startPointSet = false;
	_endPointSet = false;

	for (int i = 0; i < TILE_Y; i++)
	{
		for (int j = 0; j < TILE_X; j++)
		{
			_tile[i][j].type = TILE_TYPE_EMPTY;
			_tile[i][j].color = _selectTypeColor;
			_tile[i][j].parent = NULL;
			_tile[i][j].F = BIGNUM;
			_tile[i][j].H = 0;
			_tile[i][j].i = i;
			_tile[i][j].j = j;
			_tile[i][j].rc = RectMake(WINSIZEX - (TILE_X + 1) * WIDTH + WIDTH * j, WINSIZEY / 2 - (TILE_Y / 2) * HEIGHT + HEIGHT * i, WIDTH, HEIGHT);
		}
	}

	return S_OK;
}

void aStar::relase()
{

}

void aStar::update()
{
	if (_aStarState == ASTAR_STATE_END) //���� ��ġ ���� �ʾҴٸ�
	{
		tileComposition();//Ÿ�� ��ġ
	}

	//�������� �������� ��ġ�Ǿ� �ְ�, �����̽� ������ Ÿ�� �ʱ�ȭ����
	if (KEYMANAGER->isOnceKeyDown(VK_SPACE) && _startPointSet && _endPointSet && _aStarState == ASTAR_STATE_END)
	{
		tileInitializing();
	}

	if (_aStarState == ASTAR_STATE_END || _aStarState == ASTAR_STATE_FOUND || _aStarState == ASTAR_STATE_NOWAY) return;

	addOpenList();	//���¸���Ʈ�� �߰� �� G�� ���
	calculateH();	//H ���
	calculateF();	//F ���
	addCloseList();	//Ŭ�����Ʈ�� �߰�
	checkArrive();	//���� ���� üũ
}

void aStar::render()
{

	TextOut(getMemDC(), 15, 15, "1 : EMTPY", strlen("1 : EMTPY"));
	TextOut(getMemDC(), 15, 35, "2 : START", strlen("2 : START"));
	TextOut(getMemDC(), 15, 55, "3 : END", strlen("3 : END"));
	TextOut(getMemDC(), 15, 75, "4 : WALL", strlen("4 : WALL"));

	char str[128];
	if (_aStarState == ASTAR_STATE_END)
		sprintf_s(str, "press Space to Start");
	else if (_aStarState == ASTAR_STATE_FOUND)
		sprintf_s(str, "way found!");
	else if (_aStarState == ASTAR_STATE_NOWAY)
		sprintf_s(str, "no way!");
	else if (_aStarState == ASTAR_STATE_SEARCHING)
		sprintf_s(str, "now Searching....");

	TextOut(getMemDC(), 15, 120, str, strlen(str));

	HBRUSH newBrush, oldBrush;
	HFONT newFont, oldFont;
	newFont = CreateFont(9, 0, 0, 0, FW_NORMAL, false, false, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, DEFAULT_PITCH | FF_SWISS, "����");
	oldFont = (HFONT)SelectObject(getMemDC(), newFont);

	for (int i = 0; i < TILE_Y; i++)
	{
		for (int j = 0; j < TILE_X; j++)
		{
			newBrush = CreateSolidBrush(_tile[i][j].color);
			oldBrush = (HBRUSH)SelectObject(getMemDC(), newBrush);

			Rectangle(getMemDC(), _tile[i][j].rc.left, _tile[i][j].rc.top, _tile[i][j].rc.right, _tile[i][j].rc.bottom);
			SelectObject(getMemDC(), oldBrush);
			DeleteObject(newBrush);

			//F�� BIGNUM ���� ���� ���� FGH ȭ�鿡 ǥ������
			if (_tile[i][j].F < BIGNUM)
			{
				SetBkMode(getMemDC(), TRANSPARENT);
				sprintf_s(_tile[i][j].str, "%d", _tile[i][j].F);
				TextOut(getMemDC(), _tile[i][j].rc.left + 2, _tile[i][j].rc.top + 2, _tile[i][j].str, strlen(_tile[i][j].str));

				sprintf_s(_tile[i][j].str, "%d", _tile[i][j].G);
				TextOut(getMemDC(), _tile[i][j].rc.left + 2, _tile[i][j].rc.bottom - 12, _tile[i][j].str, strlen(_tile[i][j].str));

				sprintf_s(_tile[i][j].str, "%d", _tile[i][j].H);
				TextOut(getMemDC(), _tile[i][j].rc.right - 12, _tile[i][j].rc.bottom - 12, _tile[i][j].str, strlen(_tile[i][j].str));
			}

			if (_tile[i][j].parent == NULL) continue;

			int length = 15;

			MoveToEx(getMemDC(), (_tile[i][j].rc.left + _tile[i][j].rc.right) / 2, (_tile[i][j].rc.top + _tile[i][j].rc.bottom) / 2, NULL);
			LineTo(getMemDC(), (_tile[i][j].parent->j - _tile[i][j].j) * length + (_tile[i][j].rc.left + _tile[i][j].rc.right) / 2,
				(_tile[i][j].parent->i - _tile[i][j].i) * length + (_tile[i][j].rc.top + _tile[i][j].rc.bottom) / 2);
		}
	}

	SelectObject(getMemDC(), oldFont);
	DeleteObject(newFont);
}

void aStar::tileComposition()
{
	if (KEYMANAGER->isOnceKeyDown('1')) { _selectType = TILE_TYPE_EMPTY; _selectTypeColor = RGB(255, 255, 255); }
	else if (KEYMANAGER->isOnceKeyDown('2')) { _selectType = TILE_TYPE_START; _selectTypeColor = RGB(100, 255, 100); }
	else if (KEYMANAGER->isOnceKeyDown('3')) { _selectType = TILE_TYPE_END;   _selectTypeColor = RGB(255, 100, 100); }
	else if (KEYMANAGER->isOnceKeyDown('4')) { _selectType = TILE_TYPE_WALL;  _selectTypeColor = RGB(100, 100, 255); }

	if (KEYMANAGER->isStayKeyDown(VK_LBUTTON))
	{
		for (int i = 0; i < TILE_Y; i++)
		{
			for (int j = 0; j < TILE_X; j++)
			{
				if (PtInRect(&_tile[i][j].rc, _ptMouse))
				{
					if (_tile[i][j].type == TILE_TYPE_START) _startPointSet = false;
					if (_tile[i][j].type == TILE_TYPE_END) _endPointSet = false;

					_tile[i][j].type = _selectType;
					_tile[i][j].color = _selectTypeColor;

					if (_selectType == TILE_TYPE_START)
					{
						if (_startPointSet)
						{
							_tile[_startY][_startX].color = RGB(255, 255, 255);
							_tile[_startY][_startX].type = TILE_TYPE_EMPTY;
						}
						_startPointSet = true;
						_startX = j;
						_startY = i;
					}
					if (_selectType == TILE_TYPE_END)
					{
						if (_endPointSet)
						{
							_tile[_endY][_endX].color = RGB(255, 255, 255);
							_tile[_endY][_endX].type = TILE_TYPE_EMPTY;
						}
						_endPointSet = true;
						_endX = j;
						_endY = i;
					}
				}
			}
		}
	}

}

void aStar::tileInitializing()
{
	for (int i = 0; i < TILE_Y; i++)
	{
		for (int j = 0; j < TILE_X; j++)
		{
			if (_tile[i][j].type == TILE_TYPE_EMPTY)                     //�Ϲ�Ÿ��
			{
				_tile[i][j].walkable = true;
				_tile[i][j].listOn = false;
			}
			else if (_tile[i][j].type == TILE_TYPE_WALL)                 //����Ÿ��
			{
				_tile[i][j].walkable = false;
				_tile[i][j].listOn = false;
			}
			else if (_tile[i][j].type == TILE_TYPE_START)                //��������
			{
				_tile[i][j].walkable = true;
				_tile[i][j].listOn = true;
				_closeList.push_back(&_tile[i][j]);
			}
			else if (_tile[i][j].type == TILE_TYPE_END)                  //��������
			{
				_endX = j;
				_endY = i;
				_tile[i][j].walkable = true;
				_tile[i][j].listOn = false;
			}
		}
	}

	_aStarState = ASTAR_STATE_SEARCHING;
	_lastIndex = 0;

}

void aStar::addOpenList()
{
	Ci = _closeList[_lastIndex]->i;                  //���������� �߰��� Ŭ���� ����Ʈ�� i
	Cj = _closeList[_lastIndex]->j;                  //���������� �߰��� Ŭ���� ����Ʈ�� j
	Cg = _closeList[_lastIndex]->G;                  //���������� �߰��� Ŭ���� ����Ʈ�� G

	if (Ci != 0)      //0��° ���� �ƴ϶�� ��� ���� ���
	{
		if (_tile[Ci - 1][Cj].walkable)                  //��� ��� Ÿ���� ������ �� �ִٸ�
		{
			if (!_tile[Ci - 1][Cj].listOn)             //���¸���Ʈ�� ������ �� �Ǿ� �ִ� Ÿ���̶��
			{
				_tile[Ci - 1][Cj].listOn = true;                                   //���¸���Ʈ�� ���ԵǾ���
				_tile[Ci - 1][Cj].color = RGB(220, 255, 220);             //���� �����
				_tile[Ci - 1][Cj].G = Cg + 10;                                     //Ÿ���� G���� Ŭ���� ����Ʈ�� ���� G + 10
				_tile[Ci - 1][Cj].parent = _closeList[_lastIndex];  //Ÿ���� �θ� Ŭ���� ����Ʈ�� ���������� �߰��� Ÿ�Ϸ� ����
				_openList.push_back(&_tile[Ci - 1][Cj]);                     //���¸���Ʈ ���Ϳ� �߰�
			}
			else                                             //���¸���Ʈ�� ������ �Ǿ� �ִ� Ÿ���̶��
			{
				if (Cg + 10 < _tile[Ci - 1][Cj].G)                           //���� G������ ���� ����� G���� �۴ٸ�
				{
					_tile[Ci - 1][Cj].G = Cg + 10;                               //G�� ���Ӱ� ���
					_tile[Ci - 1][Cj].parent = _closeList[_lastIndex]; //Ÿ���� �θ�  Ŭ���� ����Ʈ�� ���������� �߰��� Ÿ�Ϸ� �缳��
				}
			}
		}
		if (Cj != 0)      //�»�� : 0��° ���� �ƴ϶��
		{
			if (_tile[Ci - 1][Cj - 1].walkable && _tile[Ci - 1][Cj].walkable && _tile[Ci][Cj - 1].walkable)  //�»�� Ÿ���� �����̳� �Ʒ��� ���� ���ٸ�
			{
				if (!_tile[Ci - 1][Cj - 1].listOn)
				{
					_tile[Ci - 1][Cj - 1].listOn = true;
					_tile[Ci - 1][Cj - 1].color = RGB(220, 255, 220);
					_tile[Ci - 1][Cj - 1].G = Cg + 14;                     //�밢���̹Ƿ� G + 14
					_tile[Ci - 1][Cj - 1].parent = _closeList[_lastIndex];
					_openList.push_back(&_tile[Ci - 1][Cj - 1]);
				}
				else
				{
					if (Cg + 14 < _tile[Ci - 1][Cj - 1].G)
					{
						_tile[Ci - 1][Cj - 1].G = Cg + 14;
						_tile[Ci - 1][Cj - 1].parent = _closeList[_lastIndex];
					}
				}
			}
		}
		if (Cj != TILE_X - 1)   //���� : ������ ���� �ƴ϶��
		{
			if (_tile[Ci - 1][Cj + 1].walkable && _tile[Ci - 1][Cj].walkable && _tile[Ci][Cj + 1].walkable)  //���� Ÿ���� �����̳� �Ʒ��� ���� ���ٸ�
			{
				if (!_tile[Ci - 1][Cj + 1].listOn)
				{
					_tile[Ci - 1][Cj + 1].listOn = true;
					_tile[Ci - 1][Cj + 1].color = RGB(220, 255, 220);
					_tile[Ci - 1][Cj + 1].G = Cg + 14;
					_tile[Ci - 1][Cj + 1].parent = _closeList[_lastIndex];
					_openList.push_back(&_tile[Ci - 1][Cj + 1]);
				}
				else
				{
					if (Cg + 14 < _tile[Ci - 1][Cj + 1].G)
					{
						_tile[Ci - 1][Cj + 1].G = Cg + 14;
						_tile[Ci - 1][Cj + 1].parent = _closeList[_lastIndex];
					}
				}
			}
		}
	}

	if (Cj != 0)      //���� : 0��° ���� �ƴ϶��
	{
		if (_tile[Ci][Cj - 1].walkable) //���� Ÿ���� �̵������ϴٸ�
		{
			if (!_tile[Ci][Cj - 1].listOn)
			{
				_tile[Ci][Cj - 1].listOn = true;
				_tile[Ci][Cj - 1].color = RGB(220, 255, 220);
				_tile[Ci][Cj - 1].G = Cg + 10;
				_tile[Ci][Cj - 1].parent = _closeList[_lastIndex];
				_openList.push_back(&_tile[Ci][Cj - 1]);
			}
			else
			{
				if (Cg + 10 < _tile[Ci][Cj - 1].G)
				{
					_tile[Ci][Cj - 1].G = Cg + 10;
					_tile[Ci][Cj - 1].parent = _closeList[_lastIndex];
				}
			}
		}
	}

	if (Cj != TILE_X - 1) //���� : ������ ���� �ƴ϶��
	{
		if (_tile[Ci][Cj + 1].walkable) //���� Ÿ���� �̵������ϴٸ�
		{
			if (!_tile[Ci][Cj + 1].listOn)
			{
				_tile[Ci][Cj + 1].listOn = true;
				_tile[Ci][Cj + 1].color = RGB(220, 255, 220);
				_tile[Ci][Cj + 1].G = Cg + 10;
				_tile[Ci][Cj + 1].parent = _closeList[_lastIndex];
				_openList.push_back(&_tile[Ci][Cj + 1]);
			}
			else
			{
				if (Cg + 10 < _tile[Ci][Cj + 1].G)
				{
					_tile[Ci][Cj + 1].G = Cg + 10;
					_tile[Ci][Cj + 1].parent = _closeList[_lastIndex];
				}
			}
		}
	}

	if (Ci != TILE_Y - 1)   //������ ���� �ƴ϶�� �ϴ� ���� ���
	{
		if (_tile[Ci + 1][Cj].walkable) //�ϴ� ��� Ÿ���� �̵������ϴٸ�
		{
			if (!_tile[Ci + 1][Cj].listOn)
			{
				_tile[Ci + 1][Cj].listOn = true;
				_tile[Ci + 1][Cj].color = RGB(220, 255, 220);
				_tile[Ci + 1][Cj].G = Cg + 10;
				_tile[Ci + 1][Cj].parent = _closeList[_lastIndex];
				_openList.push_back(&_tile[Ci + 1][Cj]);
			}
			else
			{
				if (Cg + 10 < _tile[Ci + 1][Cj].G)
				{
					_tile[Ci + 1][Cj].G = Cg + 10;
					_tile[Ci + 1][Cj].parent = _closeList[_lastIndex];
				}
			}
		}
		if (Cj != 0)            //���ϴ� : 0��° ���� �ƴ϶��
		{
			if (_tile[Ci + 1][Cj - 1].walkable && _tile[Ci + 1][Cj].walkable &&_tile[Ci][Cj - 1].walkable)   //���ϴ� Ÿ���� �������̳� ���� ���� ���ٸ�
			{
				if (!_tile[Ci + 1][Cj - 1].listOn)
				{
					_tile[Ci + 1][Cj - 1].listOn = true;
					_tile[Ci + 1][Cj - 1].color = RGB(220, 255, 220);
					_tile[Ci + 1][Cj - 1].G = Cg + 14;
					_tile[Ci + 1][Cj - 1].parent = _closeList[_lastIndex];
					_openList.push_back(&_tile[Ci + 1][Cj - 1]);
				}
				else
				{
					if (Cg + 14 < _tile[Ci + 1][Cj - 1].G)
					{
						_tile[Ci + 1][Cj - 1].G = Cg + 14;
						_tile[Ci + 1][Cj - 1].parent = _closeList[_lastIndex];
					}
				}
			}
		}
		if (Cj != TILE_X - 1)          //���ϴ� : ������ ���� �ƴ϶��
		{
			if (_tile[Ci + 1][Cj + 1].walkable && _tile[Ci + 1][Cj].walkable &&_tile[Ci][Cj + 1].walkable) //���ϴ� Ÿ���� �����̳� ���� �̵������ϴٸ�
			{
				if (!_tile[Ci + 1][Cj + 1].listOn)
				{
					_tile[Ci + 1][Cj + 1].listOn = true;
					_tile[Ci + 1][Cj + 1].color = RGB(220, 255, 220);
					_tile[Ci + 1][Cj + 1].G = Cg + 14;
					_tile[Ci + 1][Cj + 1].parent = _closeList[_lastIndex];
					_openList.push_back(&_tile[Ci + 1][Cj + 1]);
				}
				else
				{
					if (Cg + 14 < _tile[Ci + 1][Cj + 1].G)
					{
						_tile[Ci + 1][Cj + 1].G = Cg + 14;
						_tile[Ci + 1][Cj + 1].parent = _closeList[_lastIndex];
					}
				}
			}
		}
	}

}

void aStar::calculateH()
{
	for (int i = 0; i < _openList.size(); i++)
	{
		int vertical = (_endX - _openList[i]->j) * 10;      //���� H��
		int horizontal = (_endY - _openList[i]->i) * 10;    //���� H��

		if (vertical < 0) vertical *= -1;                   //������ �ݴ��̸� ���� �ο�(���밪)
		if (horizontal < 0) horizontal *= -1;

		_openList[i]->H = vertical + horizontal;            //�� H�� = ����H + ����H
	}
}

void aStar::calculateF()
{
	for (int i = 0; i < _openList.size(); i++)
	{
		_openList[i]->F = _openList[i]->G + _openList[i]->H;               //F�� ���
	}

}

void aStar::addCloseList()
{
	if (_openList.size() == 0)   //�˻��ߴµ��� openlist�� ����� 0�̶�� ���̻� ã�� ���� ����
	{
		_aStarState = ASTAR_STATE_NOWAY; //��ΰ� ����
		return;                          //���� �۾� ����
	}

	int index = 0;                 //���¸���Ʈ �� ���� F�� ���� Ÿ���� �ε���
	int lowest = BIGNUM;           //���¸���Ʈ �� ���� ���� F��
	for (int i = 0; i < _openList.size(); i++)
	{
		if (_openList[i]->F < lowest) //���¸���Ʈ�� F�� ���� �����ź��� �� �۴ٸ�
		{
			lowest = _openList[i]->F; //lowest�� �� F��
			index = i;                //index�� �� index��
		}
	}

	_openList[index]->color = RGB(180, 180, 255);  //Ŭ��� �߰��� Ÿ���� ���� : ���Ķ�

	_closeList.push_back(_openList[index]);      //Ŭ�����Ʈ�� �߰�
	_openList.erase(_openList.begin() + index);  //Ŭ�����Ʈ�� �߰��� Ÿ���� ���¸���Ʈ���� ����

	_lastIndex++;   //���� ���߿� �߰��� Ŭ������ �ε���
}


void aStar::checkArrive()
{
	if (_closeList[_lastIndex]->i == _endY && _closeList[_lastIndex]->j == _endX) //Ŭ�����Ʈ�� i j�� ���������� ���ٸ�
	{
		_aStarState = ASTAR_STATE_FOUND;         //ã�Ҵ�
		_closeList[_lastIndex]->color = RGB(255, 100, 100); //�������� ���� ����
		showWay(_closeList[_lastIndex]);         //��� �����ش�
	}

}

void aStar::showWay(tagTile* tile)
{
	if (!(tile->i == _endY && tile->j == _endX))   //���������� �ƴ϶��
		tile->color = RGB(255, 180, 180);          //���� ����ȫ
	tile = tile->parent;       //Ÿ���� �θ� �����ؼ� showWay()�Լ��� �ٽ� �ִ´�

	if (tile->parent == NULL)  //Ÿ���� �θ� ���������̸� �׸� ������
		return;
	else
		showWay(tile);         //showWay() �ٽ� ȣ��

}