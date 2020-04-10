/*----------------------------------------*/
/* "MEURTRES EN SERIE"                    */
/* Copyright Cobra Soft 1987              */
/* Decompiled by ergonomy_joe in 2020     */
/*----------------------------------------*/
#include <stdio.h>
#include <regs.h>
#include <ctype.h>

#include "sercq.h"

char *malloc();

#define EVT_CONVERSATION  1
#define EVT_OBJECT        2
#define EVT_DESCRIPTION   3
#define EVT_ACTION        4

/*----------------------------------------*/
/*--          initialised               --*/
/*----------------------------------------*/
unsigned char D_0002[0x10] = {
	0x00,0x03,0x0C,0x0F,
	0x30,0x33,0x3C,0x3F,
	0xC0,0xC3,0xCC,0xCF,
	0xF0,0xF3,0xFC,0xFF
};
unsigned char D_0012[4] = {0x00,0x55,0xAA,0xFF};
/*-- --*/
/*in/out piers coordinates*/
int D_0016[5][4] = {
	{272,274,272,276},
	{136,292,134,294},
	{162, 98,164, 96},
	{ 66,266, 68,266},
	{108,236,108,234}
};
/*----------------------------------------*/
/*--                BSS                 --*/
/*----------------------------------------*/
/*1A2E,1A3E*/union REGS RegsIn,RegsOut;
/*1A4E*/unsigned char *MUSIC_data;
/*1A50*/void *PBuffer1;
/*1A52*/void *PBuffer2;
/*-- --*/
/*1A54*/unsigned char *PDATA_Events;
/*1A56*/unsigned char *PDATA_Strings;
/*1A58*/unsigned char *PDATA_Map;
/*1A5A*/unsigned char *PDATA_Portraits;
/*1A5C*/unsigned char *PDATA_CharaOffsets;/*indexes in PDATA_CharaNames*/
/*1A5E*/unsigned char *PDATA_CharaNames;
/*-- --*/
/*1A60,1A62*/int Map_x,Map_y;
/*1A64*/int Speed;
/*1A66,1A68*/int Clock_H,Clock_M;
/*1A6A*/int CurrentPlace;/*current "dessind7.0xx" file in PBuffer2*/
/*1A6C*/int CurrentConversation;/*current "tem0xx.txt" file in PBuffer2*/
int D_1A6E;/*counter for GameRefresh*/
/*1A70*/int MovementFlags[4];
/*1A78*/int CurBike;
/*1A7A,1A7E*/int Bikes_x[2],Bikes_y[2];
/*1A82,1A84*/int Boat_x,Boat_y;
/*1A86,1A88*/int Something_X,Something_Y;/*related to explosion*/
int D_1A8A;/*flag: treasure*/
int D_1A8C;/*flag: CARRINGTON*/
int D_1A8E;/*backup pixel*/
/*1A90*/unsigned char TmpString[0x16];/*TODO or less?*/
/*1AA6,1AA8*/int CursorX,CursorY;
/*1AAA,1AAC,1AAE,1AB0*/int WindowLeft,WindowTop,WindowRight,WindowBottom;
/*1AB2,1AB4*/int WindowWidth,WindowHeight;
/*1AB6*/int TextColor;
/*1AB8*/unsigned char IconBuffer[4 * 4 * 50];
/*----------------------------------------*/
/*--                                    --*/
/*----------------------------------------*/
/*003E*/int BGColor = 0;

/*0000*/DoCommand(_ch)
int _ch;
{
	ClearKbdBuf();
	switch(_ch)
	{
		case 'M'/*usic*/:
			if(MUSIC_flag)
				MUSIC_stop();
			else
				MUSIC_start();
		break;
		case 'B'/*ertin*/: case 'G'/*illes*/:
			ClrConsole();
			PutString(/*0040*/"\n\nProgrammation par Gilles BERTIN\n");
		break;
		case 'E':
			BGColor = (BGColor + 1) & 0xf;
			SetBGColor(BGColor);
		break;
		case 'C'/*oordonnes*/:
			PutString(/*0063*/"\nCoordonn{es : ");
			PutNumber(Map_x);
			PutString(/*0073*/" - ");
			PutNumber(199 - Map_y);
			PutChar('\n');
		break;
		case 'R'/*egarder*/: case 27: Command_R(); break;
		case 'F'/*ouiller*/: Command_F(); break;
		case 'I'/*nterroger*/: Command_I(); break;
		case 'A'/*ction*/: Command_A(); break;
		case 'D'/*eplacement*/: Command_D(); break;
		case 'H'/*mise a l'Heure*/: Command_H(); break;
		case 'P'/*ersonnages*/: Command_P(3, 0); break;
		case '!'/*quit*/: return 27;
	}

	return 0;
}

/*00F5*/GameStep()
{
	int bp_02;
	int key;

	bp_02 = GetKey(&key);
	switch(bp_02)
	{
		/*-- direction --*/
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
			return MapMove(bp_02);
		/*-- validation --*/
		case 8: Command_D(); return 0;
		/*-- no key? --*/
		case 9: PositionBlink(); return 0;
		/*-- other char --*/
		case 10: return DoCommand(key);
	}
}

/*014F*/GameExit()
{
	MUSIC_clean();
	TextMode();
	exit(0);
}

/*0163*/main(argv, argc)
int argv;
char **argc;
{
	int i;
	int key;
	int bp_06;

	InitIcons();
	PBuffer1 = malloc(6000);
	PBuffer2 = malloc(19999);
	LoadDataBin();
	MUSIC_init();
	chdir(/*0077*/"sercq");
	MUSIC_loadFile(/*007D*/"cobra.cmp");
	chdir(/*0087*/".");
	if(argv > 1)
	{
		GraphMode();
		Credits();
		Delay(40);
		Demo();
		GameExit();
		return;
	}
	/*-- game --*/
	GraphMode();
	Credits();
	/*-- --*/
	bp_06 = 10;
	do {
		i = GetKey(&key);
		Delay(10);
	}while(i == 9 && bp_06--);
	/*-- --*/
	GraphMode();
	Prologue();
	/*-- --*/
	bp_06 = 15;
	do {
		i = GetKey(&key);
		Delay(10);
	}while(i == 9 && bp_06--);
	/*-- --*/
	for(i = 0; i < 5; i ++)
	{
		D_0016[i][0] = D_0016[i][0] / 2; D_0016[i][1] = 199 - D_0016[i][1] / 2;
		D_0016[i][2] = D_0016[i][2] / 2; D_0016[i][3] = 199 - D_0016[i][3] / 2;
	}
	/*-- --*/
	GraphMode();
	InitGameScreen();
	PutIcon(Map_x, Map_y, Speed, 0);
	do {
		Clock_H = 11; Clock_M = 55;
		ClockFace(Clock_H, Clock_M);
		Clock_H = 11; Clock_M = 0;
		ClockFace(Clock_H, Clock_M);

		ResetSomeStates();
		bp_06 = 0;
		CurrentPlace = -1;
		Command_R();
		ClearKbdBuf();
		MUSIC_start();
		/*---------------*/
		/*-- main loop --*/
		while((bp_06 = GameStep()) == 0);
		/*--           --*/
		/*---------------*/
		if(bp_06 == 1)
		{
			/*-- programme de Test --*/
			ClrConsole();
			RestoreIcon(Map_x, Map_y);
			PutString(/*0089*/"Il est 19 heures.\n");
			Delay(30);
			if(Speed == SPEED_SEA)
			{
				PutString(/*009C*/"Vous voil@ sur le chemin du retour");
				PutString(/*00BF*/"avez-vous trouv{ le tr{sor et {lu-");
				PutString(/*00E2*/"cid{ toute cette affaire.Souhaitez");
				PutString(/*0105*/"vous r{pondre au questionnaire ?\n");
				PutString(/*0127*/"Tapez O pour oui ou N pour non");
				while((i = GetKey_0(&key)) != 1);
				if(key == 'O' || key == 'o')
				{
					Ending();
					ClrConsole();
					InitGameScreen();
					PutIcon(Map_x, Map_y, Speed, 0);
				}
			}
		}
	} while(bp_06 != 27);
	GameExit();
}

/*----------------------------------------*/
/*-- LIB1                               --*/
/*----------------------------------------*/

/*03A2*/SetCursorPosition(_col, _row)
int _col;
int _row;
{
	int ret;

	RegsIn.h.ah = 2;/*set cursor position*/
	RegsIn.h.bh = 0;
	RegsIn.h.dh = _row;
	RegsIn.h.dl = _col;
	ret = sysint(0x10, &RegsIn, &RegsOut);
}

/*03D7*/GetCursorPosition(_pCol, _pRow)
int *_pCol;
int *_pRow;
{
	int ret;

	RegsIn.h.ah = 3;/*read cursor position*/
	RegsIn.h.bh = 0;
	ret = sysint(0x10, &RegsIn, &RegsOut);
	*_pRow = RegsOut.h.dh;
	*_pCol = RegsOut.h.dl;
}

/*0414*/WriteCharacter(_ch, _color)
unsigned char _ch;
int _color;
{
	int ret;

	RegsIn.h.ah = 0xa;/*write character at cursor*/
	RegsIn.h.al = _ch;
	RegsIn.h.bh = 0;
	RegsIn.h.bl = _color;
	RegsIn.x.cx = 1;
	ret = sysint(0x10, &RegsIn, &RegsOut);
}

/*044F*/ClrConsole()
{
	int i, j;

	for(j = WindowTop; j <= WindowBottom; j ++)
	{
		for(i = WindowLeft; i <= WindowRight; i ++)
		{
			SetCursorPosition(i, j);
			WriteCharacter(' ', TextColor);
		}
	}
	CursorX = WindowLeft;
	CursorY = WindowTop;
	SetCursorPosition(CursorX, CursorY);
}

/*04B0*/SetTextColor(_color)
int _color;
{
	TextColor = _color;
}

/*04BD*/Window(_left, _top, _right, _bottom, _color)
int _left;
int _top;
int _right;
int _bottom;
int _color;
{
	WindowLeft = _left;
	WindowTop = _top;
	WindowRight = _right;
	WindowBottom = _bottom;
	WindowWidth = _right - _left;
	WindowHeight = _bottom - _top;
	SetTextColor(_color);
	ClrConsole();
}

/*04F8*/SetCursorPos(_relX, _relY)
int _relX;
int _relY;
{
	CursorX = WindowLeft + _relX;
	CursorY = WindowTop + _relY;
	SetCursorPosition(CursorX, CursorY);
}

/*051C*/NewLine()
{
	int ret;

	RegsIn.h.ah = 6;/*scroll window up*/
	RegsIn.h.bh = 0;
	RegsIn.h.cl = WindowLeft;
	RegsIn.h.ch = WindowTop;
	RegsIn.h.dl = WindowRight;
	RegsIn.h.dh = WindowBottom;
	RegsIn.h.al = 1;
	ret = sysint(0x10, &RegsIn, &RegsOut);
}

/*0562*/PutChar_0(bp04)
unsigned char bp04;
{
	SetCursorPosition(CursorX, CursorY);
	WriteCharacter(bp04, TextColor);
	if(CursorX ++ == WindowRight)
	{
		CursorX = WindowLeft;
		if(CursorY ++ == WindowBottom)
		{
			CursorY --;
			NewLine();
		}
	}
}

/*05AE*/PutChar(_ch)
unsigned char _ch;
{
	SetCursorPosition(CursorX, CursorY);
	switch(_ch)
	{
		case '\n':
			CursorX = WindowLeft;
			if(CursorY ++ == WindowBottom)
			{
				CursorY --;
				NewLine();
			}
		break;
		case '\b':
			if(CursorX != WindowLeft)
			{
				CursorX --;
			} else if(CursorY != WindowTop)
			{
				CursorX = WindowRight;
				CursorY --;
			}
		break;
		case '{': PutChar_0(0x82); break;/*eaigu*/
		case '}': PutChar_0(0x8a); break;/*egrave*/
		case '@': PutChar_0(0x85); break;/*agrave*/
		case '~': PutChar_0(0x88); break;/*ecirc*/
		case '|': PutChar_0(0x97); break;/*ugrave*/
		default:
			PutChar_0(_ch);
	}
}

/*0672*/PutString(_str)
unsigned char *_str;
{
	while(*_str)
		PutChar(*_str ++);
}

/*068F*/PutResString(_str)
unsigned char *_str;
{
	while(*_str < 0x80)
		PutChar(*_str ++);
	PutChar(*_str & 0x7f);/*or toascii(*_str)*/
	PutChar('\n');
}

/*----------------------------------------*/
/*--                                    --*/
/*----------------------------------------*/

/*06C7*/PutNumber(_val)
int _val;
{
	int i;
	unsigned char *str;

	str = TmpString;
	*str = 0;
	do {
		for(i = 6; i >= 0; i --)
			str[i + 1] = str[i];
		*str = _val % 10 + '0';
		_val /= 10;
	}while(_val);
	PutString(str);
}

C_0721(_str, _color_ignored)
unsigned char *_str;
int _color_ignored;
{
	int row, col;

	GetCursorPosition(&row, &col);
	C_396E(_str, row, col, 1);
}

/*----------------------------------------*/
/*-- LIB2                               --*/
/*----------------------------------------*/

/*074C*/GetKey_0(_pKey)
int *_pKey;
{
	int _p_02;

	RegsIn.h.ah = 6;/*directConsole I/O*/
	RegsIn.h.dl = 0xff;
	if((sysint(0x21, &RegsIn, &RegsOut) & 0x40) == 0)
	{
		if(RegsOut.h.al)
		{
			*_pKey = RegsOut.h.al;
			return 1;
		}
		RegsIn.h.ah = 6;/*directConsole I/O*/
		RegsIn.h.dl = 0xff;
		if((sysint(0x21, &RegsIn, &RegsOut) & 0x40) == 0)
		{
			*_pKey = RegsOut.h.al;
			return 2;
		}
	}
	*_pKey = 0;

	return 0;
}

/*07C6*/ClearKbdBuf()
{
	int ret;

	RegsIn.h.ah = 0xc;/*clear keyboard buffer and invoke keyboard function*/
	RegsIn.h.al = 0;
	ret = sysint(0x21, &RegsIn, &RegsOut);
}

/*07EF*/GetKey(_pKey)
int *_pKey;
{
	int bp_02;
	int _p_04, _p_06;

	bp_02 = GetKey_0(_pKey);
	switch(bp_02)
	{
		/*-- --*/
		case 0: return 9;
		/*-- --*/
		case 1:
			switch(*_pKey)
			{
				case '\r':
				case ' ':
					return 8;
				case '8': return 0;
				case '9': return 1;
				case '6': return 2;
				case '3': return 3;
				case '2': return 4;
				case '1': return 5;
				case '4': return 6;
				case '7': return 7;
				default:
					if(islower(*_pKey) & 2)
						*_pKey = _toupper(*_pKey);
					return 10;
			}
		case 2:
			switch(*_pKey)
			{
				case 0x48: return 0;
				case 0x49: return 1;
				case 0x4d: return 2;
				case 0x51: return 3;
				case 0x50: return 4;
				case 0x4f: return 5;
				case 0x4b: return 6;
				case 0x47: return 7;
				default:
					return 9;
			}
	}
}

C_0911()
{
	int bp_02, key;

	while(bp_02 = GetKey_0(&key));
}

/*----------------------------------------*/
/*-- LIB3                               --*/
/*----------------------------------------*/

/*092C*/SetBGColor(_bg_color)
int _bg_color;
{
	int ret;

	RegsIn.h.ah = 0xb;
	RegsIn.h.bh = 0;
	RegsIn.h.bl = _bg_color;
	ret = sysint(0x10, &RegsIn, &RegsOut);
}

/*095B*/GraphMode()
{
	int ret;

	mode('m');

	RegsIn.h.ah = 0xb;
	RegsIn.h.bh = 0;
	RegsIn.h.bl = 0;
	ret = sysint(0x10, &RegsIn, &RegsOut);

	RegsIn.h.ah = 0xb;
	RegsIn.h.bh = 1;
	RegsIn.h.bl = 0;
	ret = sysint(0x10, &RegsIn, &RegsOut);
}

/*09B5*/TextMode()
{
	int ret;

	RegsIn.h.ah = 0;
	RegsIn.h.al = 2;
	ret = sysint(0x10, &RegsIn, &RegsOut);
}

/*09DE*/GetPixel(_x, _y)
int _x;
int _y;
{
	return (peekb((_y / 2) * 80 + (_y % 2) * 0x2000 + _x / 4, 0xb800) >> (3 - _x % 4) * 2) & 3;
}

/*padding*/

unsigned char D_0146[4] = {0xC0,0x30,0x0C,0x03};
unsigned char D_014A[4] = {0x3F,0xCF,0xF3,0xFC};

/*0A3F*/Plot(_x, _y, _color)
int _x;
int _y;
int _color;
{
	int voffset, bp_04;

	voffset = (_y / 2) * 80 + (_y % 2) * 0x2000 + _x / 4;
	bp_04 = _x % 4;
	pokeb(voffset, 0xb800,
		(peekb(voffset, 0xb800) & D_014A[bp_04]) |
		(D_0146[bp_04] & D_0012[_color])
	);
}

unsigned char D_014E[4] = {0xC0,0x30,0x0C,0x03};

/*(uncalled)*/
/*0AD4*/Plot_bis(_x, _y)
int _x;
int _y;
{
	int voffset;

	voffset = (_y / 2) * 80 + (_y % 2) * 0x2000 + _x / 4;
	pokeb(voffset, 0xb800, peekb(voffset, 0xb800) ^ D_014E[_x % 4]);
}

/*0B4B*/DrawLine(_x0, _y0, _x1, _x2, _color)
int _x0;
int _y0;
int _x1;
int _x2;
int _color;
{
	color(_color);
	line(_x0, _y0, _x1, _x2);
}

/*0B6B*/DrawLine_bis(_x0, _y0, _x1, _y1, _color)
int _x0;
int _y0;
int _x1;
int _y1;
int _color;
{
	color(_color);
	line(_x0, 199 - _y0, _x1, 199 - _y1);
}

/*----------------------------------------*/
/*--                                    --*/
/*----------------------------------------*/

/*(a little different from DIGIT.C)*/
/*0B93*/RenderPortrait(_ptr, _color1, _color2)
unsigned char *_ptr;


int _color1;
int _color2;
{
	unsigned char bp_01;
	unsigned char bp_02, bp_03;
	int voffset;
	int voffset_0;
	int y;
	unsigned i, j;

	voffset_0 = (158 / 2) * 80 + 1;
	y = 158;
	bp_02 = D_0012[_color1];
	bp_03 = D_0012[_color2];
	for(j = 0; j <= 39; j ++)
	{
		if(y ++ & 1)
		{
			voffset = voffset_0 + 0x2000;
			voffset_0 += 80;
		} else {
			voffset = voffset_0;
		}
		for(i = 4; i > 0; i --)
		{
			bp_01 = D_0002[(*_ptr >> 4) & 0xf];
			pokeb(voffset ++, 0xb800, ((bp_01 ^ 0xff) & bp_02) | (bp_01 & bp_03));
			bp_01 = D_0002[*_ptr ++ & 0xf];
			pokeb(voffset ++, 0xb800, ((bp_01 ^ 0xff) & bp_02) | (bp_01 & bp_03));
		}
	}
	/*-- --*/




	/*-- --*/
}

/*0CAB*/Portrait(_charaId, _color1, _color2)
int _charaId;
int _color1;
int _color2;
{
	unsigned char *ptr;

	if(_charaId == 0x24)
		_charaId = 1;
	ptr = PDATA_Portraits + _charaId * 160;
	RenderPortrait(ptr, _color1, _color2);
}

int D_0152 = 0;

/*'P'ersonnages*/
/*0CE5*/Command_P(_color1, _color2)
int _color1;
int _color2;
{
	int i, bp_04, key;
	unsigned char *ptr;

	ClrConsole();
	/*-- ?!? --*/
	if(D_0152 == 1)
	{
		PutString(/*0154*/"\n\n      Arrrg!!! un BUGGGG...");
		for(i = 0; i < 380; i += 4)
		{
			ptr = PDATA_Portraits + i + 2;
			RenderPortrait(ptr, _color1, _color2);
		}
		ClrConsole();
	}
	/*-- --*/
	PutString(/*0172*/"\n\n      LES PERSONNAGES");
	for(i = 0; i < (5600 + 4); i += 4)
	{
		ptr = PDATA_Portraits + i;
		RenderPortrait(ptr, _color1, _color2);
		if(bp_04 = GetKey_0(&key))
			break;
	}
	/*-- --*/
	D_0152 ++;
	D_0152 %= 4;
	/*-- --*/
	ClrConsole();
	Portrait(0x23, 3, 0);
}

/*0D92*/ComputeXY(_direction, _pX, _pY)
int _direction;
int *_pX;
int *_pY;
{
	switch(_direction)
	{
		case 0: *_pX = Map_x;     *_pY = Map_y - 1; break;
		case 1: *_pX = Map_x + 1; *_pY = Map_y - 1; break;
		case 2: *_pX = Map_x + 1; *_pY = Map_y;     break;
		case 3: *_pX = Map_x + 1; *_pY = Map_y + 1; break;
		case 4: *_pX = Map_x;     *_pY = Map_y + 1; break;
		case 5: *_pX = Map_x - 1; *_pY = Map_y + 1; break;
		case 6: *_pX = Map_x - 1; *_pY = Map_y;     break;
		case 7: *_pX = Map_x - 1; *_pY = Map_y - 1; break;
	}
}

C_0E5B(_direction, _pX, _pY)
int _direction;
int *_pX;
int *_pY;
{
	ComputeXY(_direction, _pX, _pY);
	if(GetPixel(*_pX, *_pY) == 0)
		return 1;
	ComputeXY((_direction + 1) % 8, _pX, _pY);
	if(GetPixel(*_pX, *_pY) == 0)
		return 1;
	ComputeXY((_direction + 7) % 8, _pX, _pY);
	if(GetPixel(*_pX, *_pY) == 0)
		return 1;
	ComputeXY((_direction + 2) % 8, _pX, _pY);
	if(GetPixel(*_pX, *_pY) == 0)
		return 1;
	ComputeXY((_direction + 6) % 8, _pX, _pY);
	if(GetPixel(*_pX, *_pY) == 0)
		return 1;

	return 0;
}

/*0F55*/MapMove(_direction)
int _direction;
{
	int bp_02;
	int newX, newY;

	ClearKbdBuf();
	RestoreIcon(Map_x, Map_y);
	bp_02 = GetPixel(newX, newY);
	switch(Speed)
	{
		case SPEED_FOOT:
			ComputeXY(_direction, &newX, &newY);
			if(GetPixel(newX, newY) != 3)
			{
				Map_x = newX;
				Map_y = newY;
				GameRefresh();
			}
		break;
		case SPEED_SEA:
			ComputeXY(_direction, &newX, &newY);
			if(GetPixel(newX, newY) == 3)
			{
				Map_x = newX;
				Map_y = newY;
				GameRefresh();
			}
		break;
		case SPEED_BIKE:
		case SPEED_ROAD:
			if(C_0E5B(_direction, &newX, &newY)) {
				Map_x = newX;
				Map_y = newY;
				GameRefresh();
			}
		break;
	}
	MapZoom(Map_x, Map_y);
	PutIcon(Map_x, Map_y, Speed, 0);
	/*-- time out? --*/
	if(Clock_H == 7)
		return 1;
	/*-- --*/

	return 0;
}

/*105A*/RefreshMovementFlags(_pCurPier)
int *_pCurPier;
{
	int i;

	for(i = 0; i <= 3; i ++)
		MovementFlags[i] = 0;

	RestoreIcon(Map_x, Map_y);

	switch(GetPixel(Map_x, Map_y))
	{
		case 1:
		case 2:
			MovementFlags[SPEED_FOOT] = 1;
		break;
		case 3:
			MovementFlags[SPEED_SEA] = 1;
		break;
		case 0:
			MovementFlags[SPEED_ROAD] = 1;
			MovementFlags[SPEED_FOOT] = 1;
			for(i = 0; i < 2; i ++)
			{
				if(Bikes_x[i] == Map_x && Bikes_y[i] == Map_y)
				{
					MovementFlags[SPEED_BIKE] = 1;
					CurBike = i;
				}
			}
		break;
	}

	PutIcon(Map_x, Map_y, Speed, 2);

	for(i = 0; i < 5; i ++)
	{
		if(D_0016[i][0] == Map_x && D_0016[i][1] == Map_y)
		{
			MovementFlags[SPEED_SEA] = 1;
			*_pCurPier = i;
		}
		if(D_0016[i][2] == Map_x && D_0016[i][3] == Map_y)
		{
			MovementFlags[SPEED_FOOT] = 1;
			*_pCurPier = i;
		}
	}
}

char *D_018A[4] = {
	/*0192*/"@ pied             ",
	/*01A6*/"en v{lo            ",
	/*01BA*/"@ pied sur la route",
	/*01CE*/"en bateau          "
};

/*'D'eplacement*/
/*1183*/Command_D()
{
	int i;
	int prev_choice;
	int count;
	int speeds[4];
	int new_choice;
	int key;
	int pier_x, pier_y;
	int cur_pier;

	ClrConsole();
	if(Speed == SPEED_BIKE)
	{
		Bikes_x[CurBike] = Map_x;
		Bikes_y[CurBike] = Map_y;
	}
	RefreshMovementFlags(&cur_pier);
	count = 0;
	for(i = 0; i <= 3; i ++)
	{
		if(MovementFlags[i])
			speeds[count ++] = i;
	}
	for(i = 0; i < count; i ++)
	{
		SetCursorPos(1, i);
		PutString(D_018A[speeds[i]]);
	}
	new_choice = 0;
	prev_choice = 0;
	do {
		SetCursorPos(1, prev_choice);
		PutString(D_018A[speeds[prev_choice]]);

		prev_choice = new_choice;
		SetTextColor(2);
		SetCursorPos(1, prev_choice);
		PutString(D_018A[speeds[prev_choice]]);

		SetTextColor(1);
		Delay(4);
		ClearKbdBuf();
		while((i = GetKey(&key)) == 9);
		switch(i)
		{
			case 0: new_choice = (new_choice + count - 1) % count; break;
			case 4: new_choice ++; new_choice %= count; break;
		}
	}while(i != 8);

	ClrConsole();
	RestoreIcon(Map_x, Map_y);
	/*-- d{barcad}re --*/
	if(Speed != SPEED_SEA && speeds[new_choice] == SPEED_SEA)
	{
		pier_x = D_0016[cur_pier][2]; pier_y = D_0016[cur_pier][3];
		if(Boat_x != pier_x || Boat_y != pier_y)
		{
			ClrConsole();
			PutString(/*01E2*/"Votre bateau n'est pas l@.\n");
			PutIcon(Map_x, Map_y, Speed, 1);
			return;
		}
		Map_x = pier_x; Map_y = pier_y;
	}
	/*-- embarcad}re --*/
	if(Speed == SPEED_SEA && speeds[new_choice] != SPEED_SEA)
	{
		Boat_x = Map_x; Boat_y = Map_y;
		Map_x = D_0016[cur_pier][0]; Map_y = D_0016[cur_pier][1];
	}
	/*-- --*/
	Speed = speeds[new_choice];
	PutString(/*01FE*/"Vous ~tes ");
	PutString(D_018A[Speed]);
	PutIcon(Map_x, Map_y, Speed, 0);
}

int D_0209 = 0;
int D_020B = 0;

/*13B1*/PositionBlink()
{
	if(D_020B ++ < 40)
		return;
	D_020B = 0;
	if(D_0209 == 0)
		D_0209 = 1;
	else
		D_0209 = 0;
	Plot(Map_x, Map_y, D_0209);
}

C_13F3()
{
	int i;

	for(i = 1; i < 10; i ++)
		Sound_1();
}

char __020D = 0;/*padding*/

C_1412()
{
	unsigned char *str;
	int i;

	str = TmpString;
	PutString(/*020E*/"Entrez le code : ");
	GetString(str);
	ClrConsole();
	if(strncmp(str, /*0220*/"8086") == 0)
	{
		PutString(/*0225*/"Un scanner r}gl{ sur 87.4 Mhz, un ");
		PutString(/*0248*/"fer @ souder, un magn{tophone, un ");
		PutString(/*026B*/"syntoniseur et *3");
		for(i = 1; i < 5; i ++)
			Sound_1();
	}
}

/*Action:Arthur?*/
C_1471()
{
	unsigned char *str;

	str = TmpString;
	PutString(/*027D*/"A");
	GetString(str);
	ToUpperString(str);
	ClrConsole();
	if(strncmp(str, /*027F*/"RTHUR") == 0)
	{
		Portrait(0x07, 3, 0);
		CurrentConversation = -1;
		CharacterSpeaks(0x2b);
	}
}

C_14C9()
{
	unsigned char *str;

	str = TmpString;
	PutString(/*0285*/"QUID ? ");
	GetString(str);
	ToUpperString(str);
	if(strncmp(str, /*028D*/"CAVO"))
	{
		ClrConsole();
		return;
	}
	PutString(/*0292*/"Un papier: j'ai trouv{ le tr{sor  ");
	PutString(/*02B5*/"avant vous. Devant la cupidit{ des");
	PutString(/*02D8*/"hommes, je l'ai recach{.\n");
	PutString(/*02F2*/"VDNEM LODECH RAMDRON MDCCCXC SDE  ");
	PutString(/*0315*/"IPTSE OV DCCXXXXV SDRA YTE ET NOM");
	D_1A8A = 1;
}

int D_0337 = 0;

C_1532()
{
	unsigned char *str;

	if(D_0337 == 1)
	{
		Portrait(0x04, 3, 0);
		D_0337 = 0;
		PutString(/*0339*/"Ni coup, ni blessure, mort natu-  ");
		PutString(/*035C*/"relle ? Passez @ mon cabinet @ 16H");
	} else {
		str = TmpString;
		PutString(/*037F*/"A");
		GetString(str);
		ToUpperString(str);
		ClrConsole();
		if(strncmp(str, /*0381*/"RTHUR") == 0)
		{
			Portrait(0x07, 3, 0);
			PutString(/*0387*/"Il est mort !");
			D_0337 = 1;
		}
	}
}

/*Action:Alfred?*/
C_15BB()
{
	unsigned char *str;

	str = TmpString;
	PutString(/*0395*/"A");
	GetString(str);
	ToUpperString(str);

	if(strncmp(str, /*0397*/"LFRED") == 0)
	{
		Portrait(0x14, 3, 0);
		CurrentConversation = -1;
		CharacterSpeaks(0x2a);
	}
}

/*Action:Safe?*/
C_1610(bp06)
int bp06;
{
	int bp_02;
	int bp_04;
	int inLoop;
	int bp_08;
	int bp_0a;
	int key;
	int bp_0e;
	int bp_10;
	int _p_12;

	bp_02 = 0;
	bp_04 = 0;
	inLoop = 1;
	bp_08 = 0;
	bp_0e = 0;
	bp_10 = 0;
	PutString(/*039D*/"\n\n    Composez votre code : ");
	bp_02 = 0;
	while(inLoop)
	{
		PutChar_0(bp_04 + '0');
		PutString(/*03BA*/"\b");
		switch(bp_0a = GetKey(&key))
		{
			case 8: inLoop = 0; break;
			case 2:
				bp_04 = (bp_04 + 1) % 10;
				Sound_2(200, 40);
				if(bp_08 == 1)
				{
					bp_10 ++;
					if(bp_10 == 2)
					{
						bp_10 = 0;
						bp_08 ++;
					}
				} else {
					bp_08 = 3;
				}
			break;
			case 6:
				bp_04 = (bp_04 + 9) % 10;
				Sound_2(200, 40);
				switch(bp_08)
				{
					case 0:
						bp_10 ++;
						if(bp_10 == 4)
						{
							bp_10 = 0;
							bp_08 ++;
						}
					break;
					case 1: bp_08 = 3; break;
					case 2:
						bp_10 ++;
						if(bp_10 == 6)
						{
							inLoop = 0;
							bp_0e = 1;
						}
					break;
				}
			break;
		}
	}
	ClrConsole();
	if(bp_0e)
	{
		Sound_2(500, 1000);
		PutString(/*03BC*/"Le coffre contient *21");
	}
}

C_1754()
{
	int i;
	unsigned char *str;/*TODO initialize:bug?*/

	switch(D_1A8A)
	{
		case 0: break;
		case 1:
			PutString(/*03D3*/"Que faites-vous ? ");
			GetString(str);
			ToUpperString(str);
			ClrConsole();
			if(strncmp(str, /*03E6*/"MONTE") == 0)
			{
				PutString(/*03EC*/"Il y a une boite en fer.");
				D_1A8A = 2;
			}
		break;
		case 2:
			PutString(/*0405*/"Bravo, vous avez trouv{ le tr{sor!");
			PutString(/*0428*/"Il faut le ramener avant 19 H. Ce ");
			PutString(/*044B*/"sera difficile si vous }tes @ pied");
			if(MUSIC_flag == 0)
				MUSIC_start();
			Delay(40);
			ClrConsole();
			Portrait(0x20, 3, 0);
			PutString(/*046E*/"Il s'approche de vous...\n");
			Delay(20);
			PutString(/*0488*/"Il est arm{ !\n");
			Delay(20);
			PutString(/*0497*/"Il se suicide !\n");
			for(i = 0; i < 10; i ++)
			{
				Portrait(0x20, 2, 0);
				Delay(1);
				Portrait(0x20, 3, 0);
				Delay(1);
			}
		break;
	}
}

__1866()
{
	unsigned char *str;/*TODO initialize:bug?*/
	int key;/*TODO initialize*/
	int bp_06;

	PutString(/*04A8*/"A");
	GetString(str);
	ToUpperString(str);
	if(strncmp(str, /*04AA*/"LLO") == 0)
	{
		PutString(/*04AE*/"Allo cher ami ? Ici CARRINGTON je ");
		PutString(/*04D1*/"vous croyais @ Venise apr}s votre ");
		PutString(/*04F4*/"enqu}te sur P{rignac. Venez chez  ");
		PutString(/*0517*/"moi, pr{sentez-vous sous le nom   ");
		PutString(/*053A*/"d'Alfred...");
		while(key != ' ')
			bp_06 = GetKey_0(&key);
	}
}

__18D4()
{
	unsigned char *bp_02;
	int i;
	int key;/*TODO initialize*/

	/*-- backup screen & clear --*/
	CurrentConversation = -1;
	CurrentPlace = -1;
	bp_02 = PBuffer2;
	i = 0;
	for(i = 0; i < 0x4000; i ++)
		*bp_02 ++ = peekb(i, 0xb800);
	C_3660();/*fastClrScr*/
	/*-- --*/
	while(key != ' ')
		i = GetKey_0(&key);
	/*-- restore screen --*/
	bp_02 = PBuffer2;
	i = 0;
	for(i = 0; i < 0x4000; i ++)
		pokeb(i, 0xb800, *bp_02 ++);
	/*-- --*/
}

char *D_0546 = /*0548*/"45544143363636";

/*Action:morse code?*/
C_1969()
{
	unsigned char *bp_02;
	char *bp_04;
	int bp_06;
	int bp_08;
	int i;
	int _p_0c;

	if(MUSIC_flag)
		MUSIC_stop();
	/*-- backup screen & clear --*/
	CurrentConversation = -1;
	CurrentPlace = -1;
	bp_02 = PBuffer2;
	i = 0;
	for(i = 0; i < 0x4000; i ++)
		*bp_02 ++ = peekb(i, 0xb800);
	C_3660();/*fastClrScr*/
	/*-- --*/
	for(bp_04 = D_0546, bp_06 = 0; bp_06 < strlen(D_0546); bp_04 ++, bp_06 ++)
	{
		for(bp_08 = 0; bp_08 < (*bp_04 & 0xf); bp_08 ++)
		{
			Sound_2(600, 40);
			Delay(3);
		}
		if((bp_06 % 2) == 0)
			Delay(5);
		else
			Delay(9);
	}
	/*-- restore screen --*/
	bp_02 = PBuffer2;
	i = 0;
	for(i = 0; i < 0x4000; i ++)
		pokeb(i, 0xb800, *bp_02 ++);
	/*-- --*/
	LoadDataBin();
}

char __0557 = 0;/*padding*/

/*'A'ction*/
/*1A67*/Command_A()
{
	unsigned char *ptr;
	int bp_04;
	int i;

	CheckTypedEvent(EVT_ACTION, &i, &ptr);
	ClrConsole();
	if(i != 1)
		return;
	switch(bp_04 = ptr[6])
	{
		case 1: C_1471(); break;/*Action:Arthur?*/
		case 2: C_1532(); break;
		case 3: C_1969(); break;/*Action:morse code?*/
		case 5: C_1412(); break;
		case 6: C_15BB(); break;/*Action:Alfred?*/
		case 7: C_1610(); break;/*Action:Safe?*/
		case 8: C_14C9(); break;
		case 9: C_1754(); break;
		case 10: C_13F3(); break;
	}
	C_0911();
}

/*----------------------------------------*/
/*-- LIB4                               --*/
/*----------------------------------------*/

/*1AF0*/SaveFile_0(_fname, _buffer, _count)
char *_fname;
unsigned char *_buffer;
int _count;
{
	FILE *f;
	int i;
	int key;/*TODO initialize*/

	f = fopen(_fname, /*0558*/"w");
	if(f == 0)
	{
		ClrConsole();
		PutString(/*055A*/"ERREUR DISQUE\n");
		PutString(/*0569*/"Appuyez sur la touche ESPACE\n");
		while(key != ' ')
			i = GetKey_0(&key);
		return;
	}
	for(i = 0; i < _count; i ++)
		fwrite(_buffer ++, 1, 1, f);
	fclose(f);
}

/*1B68*/SaveFile(_dir, _fname, _buffer, _count)
char *_dir;
char *_fname;
unsigned char *_buffer;
int _count;
{
	chdir(_dir);
	SaveFile_0(_fname, _buffer, _count);
	chdir(/*0587*/".");
}

/*1B8D*/LoadFile_0(_fname, _buffer)
char *_fname;
unsigned char *_buffer;
{
	FILE *f;
	int i;
	int key;/*TODO initialize*/

	f = fopen(_fname, /*0589*/"r");
	if(f == 0)
	{
		ClrConsole();
		PutString(/*058B*/"ERREUR DISQUE\n");
		PutString(/*059A*/"Appuyez sur la touche ESPACE\n");
		while(key != ' ')
			i = GetKey_0(&key);
		return;
	}
	while(fread(_buffer ++, 1, 1, f));
	fclose(f);
}

/*1BF9*/LoadFile(_dir, _fname, _buffer)
char *_dir;
char *_fname;
unsigned char *_buffer;
{
	chdir(_dir);
	LoadFile_0(_fname, _buffer);
	chdir(/*05B8*/".");
}

char D_05BA[22] = "dessind7.000";

/*load "dessind7.0xx" in buffer?*/
/*1C1B*/C_1C1B(bp04, bp06)
int bp04;
unsigned char *bp06;
{
	D_05BA[11] = bp04 % 10 + '0';
	D_05BA[10] = (bp04 % 100) / 10 + '0';
	LoadFile(/*05D0*/"sercq", D_05BA, bp06);
}

char D_05D6[22] = "tem000.txt";

/*load "tem0xx.txt" in buffer?*/
/*1C58*/C_1C58(bp04, _ptr)
int bp04;
unsigned char *_ptr;
{
	D_05D6[5] = bp04 % 10 + '0';
	D_05D6[4] = (bp04 % 100) / 10 + '0';
	LoadFile(/*05EC*/"sercq", D_05D6, _ptr);
}

/*----------------------------------------*/
/*--                                    --*/
/*----------------------------------------*/

int D_05F2 = 3;

/*render "dessind7.0xx" data*/
C_1C95(_ptr)
unsigned char *_ptr;
{
	unsigned start_x, start_y;
	unsigned i, j;
	int row, col;
	int voffset_0;
	unsigned bp_10;
	unsigned char *p;
	unsigned char bp_13;
	register int si, di;

	_ptr += 7;
	/*-- image --*/
	p = _ptr + 0x16; start_x = *p;
	p = _ptr + 0x18; start_y = (*p - 1) / 2;
	bp_13 = D_0012[D_05F2];
	voffset_0 = (128 / 2) * 80 + 42;
	p = _ptr + 0x21;
	if(1)
	{
		si = voffset_0;
		di = si + 0x2000;
	} else {
		di = voffset_0;
		si = di + 0x2000 - 80;
	}
	for(j = start_y; j > 0; j --)
	{
		for(i = start_x; i > 0; i --)
		{
			pokeb(si ++, 0xb800, (D_0002[*p & 0xf] & bp_13) ^ 0xff);
			pokeb(di ++, 0xb800, (D_0002[(*p++ >> 4) & 0xf] & bp_13) ^ 0xff);
		}
		si = si - 80 - start_x;
		di = di - 80 - start_x;
	}
	/*-- text --*/
	GetCursorPosition(&i, &j);
	row = 21;
	col = 17;
	p = _ptr;
	for(bp_10 = 0; bp_10 < 17; bp_10 ++)
	{
		SetCursorPosition(row ++, col);
		si = *p ++;
		if(isalpha(si))
			si |= 0x80;
		WriteCharacter(si, 3);
	}
	SetCursorPosition(i, j);
	/*-- --*/
}

/*1E16*/Pix2Screen(_ptr, _x, _y)
unsigned char *_ptr;
int _x;
int _y;
{
	unsigned width, height;
	unsigned i, j;
	int bp_0a;
	int _p_0c;
	register int si;
	register unsigned char *p;

	_ptr += 7;
	p = _ptr + 6; width = *p;
	p = _ptr + 8; height = *p;
	bp_0a = (_y / 2) * 80 + _x;
	p = _ptr + 0x10;
	for(j = height; j > 0; j --)
	{
		if((_y ++) & 1)
		{
			si = bp_0a + 0x2000;
			bp_0a += 80;
		} else {
			si = bp_0a;
		}
		for(i = width; i > 0; i --)
		{
			poke(si ++, 0xb800, *p ++);
		}
	}
}

/*1EBA*/PixFixPalette(_ptr, _color_0, _color_1, _color_2, _color_3)
unsigned char *_ptr;
int _color_0;
int _color_1;
int _color_2;
int _color_3;
{
	unsigned width, height;
	unsigned i, j;
	int _p_0a;
	unsigned palette[4];
	register int si;
	register unsigned char *p;

	palette[0] = _color_0;
	palette[1] = _color_1;
	palette[2] = _color_2;
	palette[3] = _color_3;

	_ptr += 7;
	p = _ptr + 6; width = *p;
	p = _ptr + 8; height = *p;

	p = _ptr + 0x10;
	for(j = height; j > 0; j --)
	{
		for(i = width; i > 0; i --)
		{
			si = (palette[(*p >> 0) & 3] << 0);
			si = si + (palette[(*p >> 2) & 3] << 2);
			si = si + (palette[(*p >> 4) & 3] << 4);
			si = si + (palette[(*p >> 6) & 3] << 6);
			*p ++ = si;
		}
	}
}

unsigned char D_05F4[4] = {0x00,0x05,0x0A,0x0F};
unsigned char D_05F8[4] = {0x00,0x50,0xA0,0xF0};

/*1FA0*/StretchPixel(_x, _y, _voffset, _flag)
int _x;
int _y;
int _voffset;
int _flag;
{
	unsigned char byte;

	byte = D_05F8[GetPixel(_x ++, _y)];
	if(_flag)
		byte |= D_05F4[GetPixel(_x, _y)];
	pokeb(_voffset, 0xb800, byte);
	pokeb(_voffset + 0x2000 - 80, 0xb800, byte);
}

/*2008*/MapZoom(_x, _y)
int _x;
int _y;
{
	int i;
	int voffset;

	_y -= 3;
	voffset = 0x1046;
	for(i = 7; i; i --)
	{
		StretchPixel(_x - 3, _y, voffset ++, 1);
		StretchPixel(_x - 1, _y, voffset ++, 1);
		StretchPixel(_x + 1, _y, voffset ++, 1);
		StretchPixel(_x + 3, _y ++, voffset, 0);
		voffset += 0x4d;
	}
}

/*2096*/LoadDataBin()
{
	unsigned char *pOffset, *base_ptr;
	int _p_06;

	LoadFile(/*05FC*/"sercq", /*0602*/"data.bin", PBuffer2);
	base_ptr = (unsigned char *)PBuffer2 + 7;
	PDATA_Events = base_ptr + 0x10;
	pOffset = base_ptr; PDATA_Strings = base_ptr + BufGetWord(pOffset);
	pOffset = base_ptr + 2; PDATA_Portraits = base_ptr + BufGetWord(pOffset);
	pOffset = base_ptr + 4; PDATA_CharaOffsets = base_ptr + BufGetWord(pOffset);
	pOffset = base_ptr + 6; PDATA_CharaNames = base_ptr + BufGetWord(pOffset);
	pOffset = base_ptr + 8; PDATA_Map = base_ptr + BufGetWord(pOffset);
}

/*2127*/InitGameScreen()
{
	Window(6, 20, 39, 24, 1);

	LoadFile(/*060B*/"sercq", /*0611*/"carte.pix", PBuffer1);
	PixFixPalette(PBuffer1, 0, 2, 1, 3);
	Pix2Screen(PBuffer1, 0, 1);

	LoadFile(/*061B*/"sercq", /*0621*/"titre.pix", PBuffer1);
	PixFixPalette(PBuffer1, 0, 1, 0, 0);
	Pix2Screen(PBuffer1, 37, 0);

	DrawLine(160,  45, 308,  45, 2);
	DrawLine(160, 181, 308, 181, 2);
	DrawLine(160,  45, 160, 181, 2);
	DrawLine(308,  45, 308, 181, 2);

	DrawLine( 3, 42, 36, 42, 2);
	DrawLine( 3,  1, 36,  1, 2);
	DrawLine( 3,  1,  3, 42, 2);
	DrawLine(36,  1, 36, 42, 2);

	DrawLine(  1,  44, 143,  44, 2);
	DrawLine(  1, 198, 143, 198, 2);
	DrawLine(  1,  44,   1, 198, 2);
	DrawLine(143,  44, 143, 198, 2);
}

char __062B = 0;/*padding*/

/*22F4*/ResetSomeStates()
{
	int hours;
	int minutes;

	RestoreIcon(Map_x, Map_y);
	/*-- --*/
	if(Something_X != 0)
		Plot(Something_X, Something_Y, 3);
	/*-- --*/
	if(Clock_H == 3 || Clock_H == 4 || (Clock_H == 5 && Clock_M < 43))
	{
		ResetSomething();
		hours = 3;
		minutes = 0;
		while(Clock_H != hours || Clock_M != minutes)
		{
			MoveSomething();
			minutes ++;
			if(minutes == 60) {
				minutes = 0;
				hours ++;
			}
		}
		Plot(Something_X, Something_Y, 1);
	}
	/*-- "something" related --*/
	if(Clock_H == 11 || Clock_H == 0 || Clock_H == 1 || Clock_H == 2)
		Plot(140, 54, 1);
	else
		Plot(140, 54, 3);
	/*-- --*/
	Map_x = 136; Map_y = 62;
	Speed = SPEED_FOOT;
	D_1A8E = GetPixel(Map_x, Map_y);/*note: already done in PutIcon*/
	PutIcon(Map_x, Map_y, Speed, 0);

	Bikes_x[0] = 110; Bikes_y[0] = 62;
	Bikes_x[1] = 137; Bikes_y[1] = 69;
	Boat_x = 136; Boat_y = 61;
	D_1A8A = 0;
	D_1A8C = 0;
}

/*242D*/IconShiftData(_pSrc, _pDst)
unsigned char *_pSrc;
unsigned char *_pDst;
{
	int i;
	unsigned char bp_03;
	unsigned char bp_04;

	bp_04 = 0;
	for(i = 0; i < 50; i ++)
	{
		bp_03 = *_pSrc ++;
		*_pDst ++ = (bp_03 >> 2) | bp_04;
		bp_04 = bp_03 << 6;
	}
}

/*247B*/IconCopyData(_pSrc, _pDst)
unsigned char *_pSrc;
unsigned char *_pDst;
{
	int i, j;

	for(i = 0; i < 10; i ++)
	{
		for(j = 0; j < 2; j ++)
		{
			*_pDst ++ = D_0002[*_pSrc >> 4];
			*_pDst ++ = D_0002[(*_pSrc ++) & 0xf];
		}
		*_pDst ++ = D_0002[0];
	}
}

/*24EA*/IconCreateData(_ptr, _speed)
unsigned char *_ptr;
int _speed;
{
	IconCopyData(_ptr, IconBuffer + _speed * 200);
	IconShiftData(IconBuffer       + _speed * 200, IconBuffer +  50 + _speed * 200);
	IconShiftData(IconBuffer +  50 + _speed * 200, IconBuffer + 100 + _speed * 200);
	IconShiftData(IconBuffer + 100 + _speed * 200, IconBuffer + 150 + _speed * 200);
}

unsigned char D_062C[50] = {0};
unsigned char D_065E[50] = {
	0xC0,0x00,/*xx..............*/
	0x70,0x00,/*.xxx............*/
	0x7C,0x00,/*.xxxxx..........*/
	0x38,0x00,/*..xxx...........*/
	0x3C,0x00,/*..xxxx..........*/
	0x26,0x00,/*..x..xx.........*/
	0x03,0x00,/*......xx........*/
	0x01,0x80,/*.......xx.......*/
	0x00,0xC0,/*........xx......*/
	0x00,0x60 /*.........xx.....*/
};
unsigned char D_0690[50] = {
	0x06,0x18,/*.....xx....xx...*/
	0x08,0x00,/*....x...........*/
	0x2F,0xF0,/*..x.xxxxxxxx....*/
	0x08,0x10,/*....x......x....*/
	0x3C,0x2C,/*..xxxx....x.xx..*/
	0x4A,0x52,/*.x..x.x..x.x..x.*/
	0x95,0xA1,/*x..x.x.xx.x....x*/
	0x85,0xA1,/*x....x.xx.x....x*/
	0x48,0x12,/*.x..x......x..x.*/
	0x30,0x0C /*..xx........xx..*/
};
unsigned char D_06C2[50] = {
	0x00,0x40,/*.........x......*/
	0x00,0x40,/*.........x......*/
	0x00,0xF0,/*........xxxx....*/
	0x00,0xA0,/*........x.x.....*/
	0x07,0xE0,/*.....xxxxxx.....*/
	0xFF,0xE0,/*xxxxxxxxxxx.....*/
	0x83,0xFF,/*x.....xxxxxxxxxx*/
	0x40,0x02,/*.x............x.*/
	0x40,0x02,/*.x............x.*/
	0x3F,0xFD /*..xxxxxxxxxxxx.x*/
};
unsigned char D_06F4[50] = {
	0x00,0x7E,/*.........xxxxxx.*/
	0x00,0x7E,/*.........xxxxxx.*/
	0x00,0xB2,/*........x.xx..x.*/
	0x01,0x72,/*.......x.xxx..x.*/
	0x02,0xFE,/*......x.xxxxxxx.*/
	0x3F,0xFD,/*..xxxxxxxxxxxx.x*/
	0x4F,0xF9,/*.x..xxxxxxxxx..x*/
	0x87,0xF1,/*x....xxxxxxx...x*/
	0x80,0x01,/*x..............x*/
	0xAA,0x95 /*x.x.x.x.x..x.x.x*/
};

/*25AD*/InitIcons()
{
	IconCreateData(D_065E, 0);
	IconCreateData(D_0690, 1);
	IconCreateData(D_06F4, 2);
	IconCreateData(D_06C2, 3);
}

/*25EB*/PutIcon(_x, _y, _speed, _color)
int _x;
int _y;
int _speed;
int _color;
{
	unsigned char *bp_02;
	unsigned char *ptr;
	unsigned char byt;
	unsigned char bp_06;
	int voffset;
	int i, j;

	/*-- backup pixel --*/
	D_1A8E = GetPixel(_x, _y);
	/*-- --*/
	_x ++;
	_y ++;
	bp_02 = IconBuffer + (_x % 4) * 50 + _speed * 200;
	bp_06 = D_0012[_color];
	voffset = (_y / 2) * 80 + _x / 4;
	ptr = D_062C;
	if(_y & 1)
	{
		for(i = 0; i < 5; i ++)
		{
			for(j = 0; j < 5; j ++)
			{
				byt = peekb(voffset + 0x2000, 0xb800);
				*ptr ++ = byt;
				pokeb(voffset ++ + 0x2000, 0xb800, ((*bp_02 ^ 0xff) & byt) | ((*bp_02 ++) & bp_06));
			}
			voffset += 80 - 5;
			for(j = 0; j < 5; j ++)
			{
				byt = peekb(voffset, 0xb800);
				*ptr ++ = byt;
				pokeb(voffset ++, 0xb800, ((*bp_02 ^ 0xff) & byt) | ((*bp_02 ++) & bp_06));
			}
			voffset -= 5;
		}
	} else {
		for(i = 0; i < 5; i ++)
		{
			for(j = 0; j < 5; j ++)
			{
				byt = peekb(voffset, 0xb800);
				*ptr ++ = byt;
				pokeb(voffset ++, 0xb800, ((*bp_02 ^ 0xff) & byt) | ((*bp_02 ++) & bp_06));
			}
			for(j = 0; j < 5; j ++)
			{
				byt = peekb(voffset + 0x1ffb, 0xb800);
				*ptr ++ = byt;
				pokeb(voffset ++ + 0x1ffb, 0xb800, ((*bp_02 ^ 0xff) & byt) | ((*bp_02 ++) & bp_06));
			}
			voffset += 0x46;
		}
	}
}

/*2875*/RestoreIcon(_x, _y)
int _x;
int _y;
{
	unsigned char *ptr;
	int voffset;
	int i, j;

	/*-- restore pixel --*/
	Plot(_x, _y, D_1A8E);
	/*-- --*/
	_x ++;
	_y ++;
	ptr = D_062C;
	voffset = (_y / 2) * 80 + _x / 4;
	if(_y & 1)
	{
		for(i = 0; i < 5; i ++)
		{
			for(j = 0; j < 5; j ++)
			{
				pokeb(voffset ++ + 0x2000, 0xb800, *ptr ++);
			}
			voffset += 80 - 5;
			for(j = 0; j < 5; j ++)
			{
				pokeb(voffset ++, 0xb800, *ptr ++);
			}
			voffset -= 5;
		}
	} else {
		for(i = 0; i < 5; i ++)
		{
			for(j = 0; j < 5; j ++)
			{
				pokeb(voffset ++, 0xb800, *ptr ++);
			}
			for(j = 0; j < 5; j ++)
			{
				pokeb(voffset ++ + 0x1ffb, 0xb800, *ptr ++);
			}
			voffset += 70;
		}
	}
}

int D_0726[2 * 60] = {
	26, 181,
	26, 180,
	27, 180,
	28, 180,
	29, 180,
	30, 179,
	31, 179,
	32, 178,
	32, 178,
	33, 177,
	33, 176,
	34, 175,
	34, 174,
	34, 173,
	34, 172,
	35, 172,
	34, 171,
	34, 170,
	34, 169,
	34, 168,
	33, 167,
	33, 166,
	32, 165,
	32, 165,
	31, 164,
	30, 164,
	29, 163,
	28, 163,
	27, 163,
	26, 163,
	26, 163,
	25, 163,
	24, 163,
	23, 163,
	22, 163,
	21, 164,
	20, 164,
	19, 165,
	19, 165,
	18, 166,
	18, 167,
	17, 168,
	17, 169,
	17, 170,
	17, 171,
	17, 172,
	17, 172,
	17, 173,
	17, 174,
	17, 175,
	18, 176,
	18, 177,
	19, 178,
	19, 178,
	20, 179,
	21, 179,
	22, 180,
	23, 180,
	24, 180,
	25, 180
};
int D_0816[2 * 60] = {
	26, 178,
	26, 178,
	27, 178,
	28, 178,
	28, 178,
	29, 177,
	29, 177,
	30, 177,
	31, 176,
	31, 175,
	31, 175,
	32, 174,
	32, 174,
	32, 173,
	32, 172,
	32, 172,
	32, 171,
	32, 170,
	32, 169,
	32, 169,
	31, 168,
	31, 168,
	31, 167,
	30, 166,
	29, 166,
	29, 166,
	28, 165,
	28, 165,
	27, 165,
	26, 165,
	26, 165,
	25, 165,
	24, 165,
	23, 165,
	23, 165,
	22, 166,
	22, 166,
	21, 166,
	20, 167,
	20, 168,
	20, 168,
	19, 169,
	19, 169,
	19, 170,
	19, 171,
	19, 172,
	19, 172,
	19, 173,
	19, 174,
	19, 174,
	20, 175,
	20, 175,
	20, 176,
	21, 177,
	22, 177,
	22, 177,
	23, 178,
	23, 178,
	24, 178,
	25, 178
};
int D_0906 = 55;/*previous hours*/
int D_0908 = 0;/*previous minutes*/

/*29BB*/ClockFace(_hours, _minutes)
int _hours;
int _minutes;
{
	DrawLine(26, 172, D_0816[D_0906 * 2], D_0816[D_0906 * 2 + 1], 1);
	DrawLine(26, 172, D_0726[D_0908 * 2], D_0726[D_0908 * 2 + 1], 1);
	D_0906 = _hours * 5 + _minutes / 12;
	D_0908 = _minutes;
	DrawLine(26, 172, D_0816[D_0906 * 2], D_0816[D_0906 * 2 + 1], 0);
	DrawLine(26, 172, D_0726[D_0908 * 2], D_0726[D_0908 * 2 + 1], 0);
}

/*2A8F*/PlusOneMinute()
{
	if(Clock_M == 59) {
		if(Clock_H ++ == 11)
			Clock_H = 0;
		Clock_M = 0;
	} else {
		Clock_M ++;
	}
	ClockFace(Clock_H, Clock_M);
}

/*2ACA*/MoveSomething()
{
	if(Something_X == 140 && Something_Y > 2)
		Something_Y --;
	if(Something_X <= 140 && Something_Y == 2)
		Something_X --;
	if(Something_X == 50 && Something_Y >= 2)
		Something_Y ++;
}

/*2B0D*/RefreshSomething()
{
	Plot(Something_X, Something_Y, 3);
	MoveSomething();
	Plot(Something_X, Something_Y, 1);
}

/*2B3B*/ResetSomething()
{
	Something_X = 140; Something_Y = 54;
	Plot(Something_X, Something_Y, 1);
}

/*2B60*/GameRefresh()
{
	int i, j;
	int bp_06;
	int key;
	char *str;

	D_1A6E ++;
	D_1A6E = D_1A6E % 8;
	switch(Speed)
	{
		case SPEED_FOOT:
			PlusOneMinute();
			if(Something_X != 0)
				RefreshSomething();
		break;
		case SPEED_ROAD:
		case SPEED_SEA:
			if(D_1A6E % 2 == 0)
			{
				PlusOneMinute();
				if(Something_X != 0)
					RefreshSomething();
			}
		break;
		case SPEED_BIKE:
			if(D_1A6E % 8 == 0)
			{
				PlusOneMinute();
				if(Something_X != 0)
					RefreshSomething();
			}
		break;
	}
	/*-- --*/
	if(Clock_H == 3 && Clock_M == 0)
		ResetSomething();
	/*-- --*/
	if(Something_X == 50 && Something_Y == 26)
	{
		ClrConsole();
		PutString(/*090A*/"Une explosion d{chire l'atmosph}re...");
		for(i = 0; i < 4; i ++)
		{
			SetBGColor(4);
			Delay(2);
			SetBGColor(BGColor);
			Delay(2);
		}
		Plot(Something_X, Something_Y, 3);
		Something_X = 0; Something_Y = 0;
	}
	/*-- --*/
	if(Clock_H == 6 && Clock_M == 45)
		Sound_2(500, 2000);
	/*-- --*/
	if(Clock_H == 2 && Clock_M < 8 && D_1A8C == 0)
	if(Map_x >= 57 && Map_x <= 59)
	if(Map_y >= 132 && Map_y <= 134)
	{
		for(i = 0; i < 10; i ++)
		{
			Sound_2(180, 2000);
			for(j = 0; j < 8; j ++)
			{
				Delay(3);
				bp_06 = GetKey_0(&key);
				if(bp_06 == 1 && (key == 'a' || key == 'A'))
				{
					ClrConsole();
					PutString(/*0930*/"A");
					GetString(str);
					ToUpperString(str);
					if(strncmp(str, /*0932*/"LLO") == 0)
					{
						ClrConsole();
						Portrait(0x23, 3, 0);
						PutString(/*0936*/"Allo cher ami ? Ici CARRINGTON je ");
						PutString(/*0959*/"vous croyais @ Venise apr}s votre ");
						PutString(/*097C*/"enqu}te sur P{rignac. Venez chez  ");
						PutString(/*099F*/"moi, pr{sentez-vous sous le nom   ");
						PutString(/*09C2*/"d'Alfred...");
					}
					D_1A8C = 1;
					return;
				}
			}
		}
	}
}

/*mise a l''H'eure*/
/*2D8C*/Command_H()
{
	int inLoop;
	int bp_04;
	int key;
	int prev_hours, prev_minutes;

	inLoop = 1;
	prev_hours = Clock_H;
	prev_minutes = Clock_M;
	RestoreIcon(Map_x, Map_y);
	ClrConsole();
	PutString(/*09CE*/"\n\n      MISE A L'HEURE");
	do
	{
		while((bp_04 = GetKey_0(&key)) == 0);
		if(bp_04 == 2)
		{
			switch(key)
			{
				case 0x48: case 0x4b:/*left, up*/
					if(Clock_M)
					{
						Clock_M --;
					}
					else
					{
						if(Clock_H == 11)
							break;
						if(Clock_H == 0)
							Clock_H = 12;
						Clock_M = 59;
						Clock_H --;
					}
					ClockFace(Clock_H, Clock_M);
				break;
				case 0x4d: case 0x50:/*right, down*/
					if(Clock_M == 59)
					{
						if(Clock_H == 6)
							break;
						if(Clock_H ++ == 11)
							Clock_H = 0;
						Clock_M = 0;
					} else {
						Clock_M ++;
					}
					ClockFace(Clock_H, Clock_M);
				break;
			}
		} else {
			switch(key)
			{
				case '\r': case ' ': inLoop = 0; break;
			}
		}
	} while(inLoop);
	ClrConsole();
	D_1A8C = 0;
	PutIcon(Map_x, Map_y, Speed, 0);
	if(prev_hours != Clock_H || prev_minutes != Clock_M)
		ResetSomeStates();
}

char __09E5 = 0;/*padding*/

/*2EC4*/CheckTypedEvent(_type, pResult, _ppBasePtr)
int _type;
int *pResult;
unsigned char **_ppBasePtr;
{
	int event_x, event_y;
	int event_time;
	int charaId;
	unsigned char *ptr;
	unsigned char *base_ptr;

	event_x = Map_x;
	event_y = 199 - Map_y;
	event_time = ((Clock_H < 10)?Clock_H + 12:Clock_H) * 8 + Clock_M / 8;
	base_ptr =
	*_ppBasePtr = PDATA_Events;
	*pResult = 0;
	for(; 1; base_ptr += 8)
	{
		/*-- end of list? --*/
		ptr = base_ptr + 1;
		if(*ptr ++ == 0 && *ptr == 0)
			break;
		/*-- check x --*/
		ptr = base_ptr + 1;
		if(*ptr != event_x)
		{
			if((*base_ptr & 0x80) == 0)
				continue;
			if(*ptr != event_x + 1 && *ptr != event_x - 1)
				continue;
		}
		/*-- check y --*/
		ptr = base_ptr + 2;
		if(*ptr != event_y)
		{
			if((*base_ptr & 0x80) == 0)
				continue;
			if(*ptr != event_y + 1 && *ptr != event_y - 1)
				continue;
		}
		/*-- check start time --*/
		ptr = base_ptr + 3;
		if(event_time < *ptr)
			continue;
		/*-- check finish time --*/
		ptr = base_ptr + 4;
		if(event_time >= *ptr)
			continue;
		/*-- --*/
		*_ppBasePtr = base_ptr;
		*pResult = 2;
		if((*base_ptr & 0xf) == _type)
		{
			*pResult = 1;
			break;
		}
	}
	/*-- display portrait --*/
	ptr = *_ppBasePtr + 5;
	if(*pResult)
		charaId = *ptr;
	else
		charaId = 0x23;
	Portrait(charaId, 3, 0);
	if(charaId < 0x23 && _type != EVT_ACTION)
		PutResString(PDATA_CharaNames + BufGetWord(PDATA_CharaOffsets + charaId * 3));
	/*-- --*/
	if(charaId == 0x0f || (charaId == 0x0d && Map_x == 116 && Map_y == 71))
		if(MUSIC_flag == 0)
			MUSIC_start();
	/*-- --*/
}

/*3092*/PutConversation(_ptr)
unsigned char *_ptr;
{
#ifdef KARAMOON_PATCH
#define NB_CHARA_LINES 34
#define NB_TEXT_LINES 4
	int key;
	int nbChara;

	nbChara = 0;
	_ptr += 5;
	while (*_ptr)
	{
		nbChara++;

		if (*_ptr == '#')
		{
			PutString(/*09E6*/"\n");
			nbChara += (NB_CHARA_LINES - nbChara % NB_CHARA_LINES);
		}
		else
			PutChar(*_ptr);

		if (nbChara == NB_TEXT_LINES * NB_CHARA_LINES)
		{
			nbChara = 0;
			while (key != ' ')
			GetKey_0(&key);
		}

		_ptr++;
	}
#undef NB_CHARA_LINES 34
#undef NB_TEXT_LINES 4
#else
	int i;
	int key;
	int _p_06, _p_08;
	int bp_0a;

	bp_0a = 0;
	_ptr += 5;
	while(*_ptr)
	{
		if(*_ptr == '#')
			PutString(/*09E6*/"\n");
		else
			PutChar(*_ptr);
		/*-- '\n' or ' ' --*/
		if(GetKey(&key) == 8)
		{
			while(GetKey(&key) == 8);
			PutString(/*09E8*/"_\b");
			while(GetKey(&key) != 8)
			{
				Delay(3);
				if(bp_0a ^= 1)
					PutString(/*09EB*/"_\b");
				else
					PutString(/*09EE*/" \b");
			}
			while(GetKey(&key) == 8);
			PutString(/*09F1*/" \b");
		}
		/*-- --*/
		_ptr ++;
		Delay(1);
		for(i = 0; i < 1000; i ++);
	}
#endif
}

/*3156*/CharacterSpeaks(_charaId)
int _charaId;
{
	/*-- --*/
	if(_charaId != CurrentConversation)
	{
		CurrentConversation = _charaId;
		CurrentPlace = -1;
		C_1C58(CurrentConversation, PBuffer1);/*load "tem0xx.txt" in buffer?*/
	}
	PutConversation(PBuffer1);
}

/*'I'nterroger*/
/*3182*/Command_I()
{
	unsigned char *ptr;
	int i;

	ClrConsole();
	CheckTypedEvent(EVT_CONVERSATION, &i, &ptr);
	if(i != 1)
		return;
	i = ptr[6];
	/*-- --*/
	if(i != CurrentConversation)
	{
		CurrentConversation = i;
		CurrentPlace = -1;
		C_1C58(CurrentConversation, PBuffer1);/*load "tem0xx.txt" in buffer?*/
	}
	PutConversation(PBuffer1);
}

/*'R'egarder*/
/*31D8*/Command_R()
{
	unsigned char *ptr;
	unsigned char bp_03;
	int _p_05, _p_07;
	int i;

	/*-- --*/
	ptr = PDATA_Map + ((199 - Map_y - 40) / 8) * 36 + (Map_x / 8) * 2;
	bp_03 = *ptr & 0x1f;
	if(bp_03 != CurrentPlace)
	{
		CurrentPlace = bp_03;
		CurrentConversation = -1;
		C_1C1B(bp_03, PBuffer1);/*load "dessind7.0xx" in buffer?*/
		C_1C95(PBuffer1);/*render "dessind7.0xx" data*/
	}
	/*-- --*/
	ClrConsole();
	/*-- --*/
	CheckTypedEvent(EVT_DESCRIPTION, &i, &ptr);
	if(i == 1)
		PutResString(PDATA_Strings + BufGetWord(ptr + 6));
	/*-- --*/
	for(i = 0; i < 5; i ++)
	{
		if(D_0016[i][0] == Map_x && D_0016[i][1] == Map_y)
		{
			PutString(/*09F4*/"Il y a un embarcad}re.\n");
			break;
		}
		if(D_0016[i][2] == Map_x && D_0016[i][3] == Map_y)
		{
			PutString(/*0A0C*/"Il y a un d{barcad}re.\n");
			break;
		}
	}
}

/*'F'ouiller*/
/*32DD*/Command_F()
{
	unsigned char *ptr;
	int i;

	ClrConsole();
	CheckTypedEvent(EVT_OBJECT, &i, &ptr);
	switch(i)
	{
		case 1:
			PutString(/*0A24*/"Vous trouvez :\n");
			PutResString(PDATA_Strings + BufGetWord(ptr + 6));
		break;
		default:
			PutString(/*0A34*/"Il n'y a rien.");
	}
}

char __0A43 = 0;/*padding*/

/*----------------------------------------*/
/*-- LIB5                               --*/
/*----------------------------------------*/

/*3330*/BufGetWord(_p)
unsigned char *_p;
{
	int low, high;

	low = *_p ++;
	high = *_p;

	return (high << 8) + low;
}

/*3358*/Delay(count)
int count;
{
	int ret;
	int deci;/*TODO initialize*/

	RegsIn.h.ah = 0x2c;/*gettime*/
	while(count)
	{
		ret = sysint(0x21, &RegsIn, &RegsOut);
		if(RegsOut.h.dl / 10 != deci)
		{
			count --;
			deci = RegsOut.h.dl / 10;
		}
	}
}

/*33A4*/ToUpperString(_str)
char *_str;
{
	int _p_02;

	for(; *_str; _str ++)
	{
		if(islower(*_str))
			*_str = _toupper(*_str);
	}
}

/*33CD*/GetString(_str)
char *_str;
{
	int col, row;
	int bp_06;
	int key;
	int inLoop;
	int len;
	int _p_0e;

	GetCursorPosition(&col, &row);
	*_str = 0;
	inLoop = 1;
	len = 1;
	while(inLoop)
	{
		bp_06 = GetKey_0(&key);
		if(bp_06 == 1)
		{
			if(isprint(key))
			{
				PutChar_0(key);
				*_str ++ = key;
				*_str = 0;
				if(len ++ == 20)
					inLoop = 0;
			}
			if(key == '\b' && len > 1)
			{
				PutString(/*0A44*/"\b \b");
				len --;
				_str --;
				*_str = 0;
			}
			if(key == '\r')
			{
				inLoop = 0;
				PutString(/*0A48*/"\n");
			}
		}
	}
}

/*----------------------------------------*/
/* MUSIC
/*----------------------------------------*/
/*0A4A*/int MUSIC_flag = 0;
int D_0A4C = 0;
long D_0A4E = 0;/*original int 1Ch*/
/*4 octave chromatic scale frequencies*/
int D_0A52[] = {
	18268,17240,16270,15355,14491,13675,12906,12179,11494,10847,10237, 9661,
	 9134, 8620, 8135, 7677, 7245, 6838, 6453, 6090, 5747, 5424, 5118, 4830,
	 4567, 4310, 4068, 3839, 3623, 3419, 3226, 3045, 2873, 2712, 2559, 2415,
	 2284, 2155, 2034, 1919, 1811, 1709, 1613, 1522, 1437, 1356, 1280, 1208,
	 1142, 1078, 1017,  960,  906,  855,  807,  761,  718,  678,  640,  604
};
char D_0ACA = 0;
char D_0ACB = 0;
int D_0ACC[] = {
	 0,
	48,
	36,0,
	24,0,0,0,
	12
};
int D_0ADE = 0;
int D_0AE0 = 0;

char __0AE2[] = {0, 0};

/*347D*/MUSIC_loadFile(_fname)
char *_fname;
{
	int f;

	f = open(_fname, 0);
	read(f, MUSIC_data, 5);
	read(f, MUSIC_data, 0x1200);
	close(f);
}

/*34BF*/MUSIC_init()
{
	MUSIC_data = (unsigned char *)malloc(0x1200);
	C_35DD();/*setTimerRoutine*/
}

/*----------------------------------------*/
/*sound.asm*/
/*----------------------------------------*/

/*----------------------------------------*/
/*demo.c*/
/*----------------------------------------*/
