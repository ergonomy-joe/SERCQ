/*----------------------------------------*/
/* "MEURTRES EN SERIE"                    */
/* Copyright Cobra Soft 1987              */
/* Decompiled by ergonomy_joe in 2020     */
/*----------------------------------------*/
#include <stdio.h>
#include <regs.h>
#include <ctype.h>

extern unsigned char *malloc();
extern char *scdir();
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
/*----------------------------------------*/
/*088C,089C*/union REGS RegsIn,RegsOut;
/*08AC*/unsigned char *PBuffer1;
/*08AE*/unsigned char *PBuffer2;
unsigned char *D_08B0;
unsigned char *D_08B2;
unsigned char D_08B4[160];
char D_0954[0x80];
/*09D4,09D6*/int CursorX,CursorY;
/*09D8,09DA,09DC,09DE*/int WindowLeft,WindowTop,WindowRight,WindowBottom;
/*09E0,09E2*/int WindowWidth,WindowHeight;
/*09E4*/int TextColor;
/*----------------------------------------*/

char *D_0016[] = {
	/*0020*/" CREATION D'UN NOUVEAU MEDAILLON ",
	/*0042*/" EDITION D'UN MEDAILLON EXISTANT ",
	/*0064*/" VOIR LES MEDAILLONS ",
	/*007A*/" INTEGRATION D'UN MEDAILLON AU JEU ",
	/*009E*/" FIN DE 'COBRA DIGIT'"
};

/*0000*/MenuItem(_item, _color)
int _item;
int _color;
{
	SetTextColor(_color);
	SetCursorPos(2, _item + _item + 9);
	PutString(D_0016[_item]);
	SetTextColor(3);
}

/*0036*/TopMenu(_item)
int _item;
{
	int i;
	int key;
	int inLoop;
	int cur_item;

	inLoop = 1;
	cur_item = _item;
	Window(1, 1, 39, 24, 3);
	ClrConsole();
	SetCursorPos(2, 2);
	PutString(/*00B4*/"\n\n        C O B R A   D I G I T\n");
	PutString(/*00D5*/"        _____________________\n\n\n");
	for(i = 0; i < 5; i ++)
	{
		MenuItem(i, 3);
	}
	MenuItem(cur_item, 1);
	while(inLoop)
	{
		while((i = GetKey(&key)) == 9);
		switch(i)
		{
			case 0:
				MenuItem(cur_item, 3);
				cur_item = (cur_item + 4) % 5;
				MenuItem(cur_item, 1);
			break;
			case 4:
				MenuItem(cur_item, 3);
				cur_item ++;
				cur_item = cur_item % 5;
				MenuItem(cur_item, 1);
			break;
			case 8:
				inLoop = 0;
			break;
		}
		C_0CC1();
	}

	return cur_item;
}

char *C_013A(_dir, _pattern)
char *_dir;
char *_pattern;
{
	int i, key;
	char *ptr;

	chdir(_dir);
	Window(1, 1, 39, 24, 3);
	ClrConsole();
	ptr = D_0954;
	PutString(/*00F6*/"S{lection d'un fichier :");
	do {
		if((ptr = scdir(_pattern)) == 0)
		{
			chdir(/*010F*/".");
			ptr = scdir(_pattern);
			chdir(_dir);
			while((ptr = scdir(_pattern)) == 0);
		}
		SetCursorPos(1, 4);
		PutString(ptr);
		PutString(/*0111*/"                    ");
		while((i = GetKey(&key)) == 9);
	}while (i != 8);

	return ptr;
	/*???*/
	chdir(/*0126*/".");
}

/*01EB*/MED_seeAll()
{
	int i, key;
	char *ptr;

	chdir(/*0128*/"sercq");
	Window(1, 1, 39, 24, 3);
	ClrConsole();
	PutString(/*012E*/"    VOIR LES MEDAILLONS");
	ptr = D_0954;
	do {
		if(ptr = scdir(/*0146*/"*.med"))
		{
			SetCursorPos(1, 4);
			PutString(ptr);
			PutString(/*014C*/"                    ");
			LoadFile(/*0161*/"sercq", ptr, D_08B4);
			RenderPortrait(D_08B4, 160, 100, 3, 0);
		while((i = GetKey(&key)) == 9);
		}
	}while(ptr && key != 27);
	chdir(/*0167*/".");
}

/*02A2*/ComputeNewXY(_pX, _pY, _direction)
int *_pX;
int *_pY;
int _direction;
{
	switch(_direction)
	{
		case 0: (*_pX); (*_pY) --; break;
		case 1: (*_pX) ++; (*_pY) --; break;
		case 2: (*_pX) ++; (*_pY); break;
		case 3: (*_pX) ++; (*_pY) ++; break;
		case 4: (*_pX); (*_pY) ++; break;
		case 5: (*_pX) --; (*_pY) ++; break;
		case 6: (*_pX) --; (*_pY); break;
		case 7: (*_pX) --; (*_pY)--; break;
	}
	if(*_pX < 0) *_pX = 0;
	if(*_pY < 0) *_pY = 0;
	if(*_pX >= 32) *_pX = 31;
	if(*_pY >= 40) *_pY = 39;
}

int D_0169 = 0;

C_035B(_x, _y)
int _x;
int _y;
{
	D_0169 ++;
	if(D_0169 == 40)
		BigPlot(_x, _y, 0);
	if(D_0169 == 0x50)
	{
		BigPlot(_x, _y, 3);
		D_0169 = 0;
	}
}

/*039B*/getMedPixel(_x, _y)
int _x;
int _y;
{
	int bp_02, bp_04;

	bp_02 = _x / 8 + _y * 4;
	bp_04 = 0x100 >> (_x % 8 + 1);
	if(D_08B4[bp_02] & bp_04)
		return 3;
	return 0;
}

/*03E3*/Editor()
{
	int i;
	int key;
	int x, y;
	int color;
	int bp_0c;
	int grid_on;

	x = 0;
	y = 0;
	grid_on = 0;
	Window(1, 1, 39, 24, 3);
	ClrConsole();
	BigPortrait();
	SetCursorPos(1, 18);
	for(i = 0x18; i <= 0x1b; PutChar(i ++));
	PutString(/*016B*/"   d{placements\n");
	PutString(/*017C*/"ESPACE inverse couleur\n");
	PutString(/*0194*/"ESC    sauvegarde et fin\n");
	color = getMedPixel(x, y);
	RenderPortrait(D_08B4, 240, 50, 3, 0);
	do {
		switch(i = GetKey(&key))
		{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
				BigPlot(x, y, color);
				ComputeNewXY(&x, &y, i);
				color = getMedPixel(x, y);
			break;
			case 8:
				color = (color == 0)?3:0;
				BigPlot(x, y, color);
				i = x / 8 + y * 4;
				bp_0c = 0x100 >> (x % 8 + 1);
				D_08B4[i] = D_08B4[i] ^ bp_0c;
				RenderPortrait(D_08B4, 240, 50, 3, 0);
			break;
			case 9:
				C_035B(x, y);
			break;
			case 0xa:
				if(key == 'G' || key == 'g')
				{
					if(grid_on)
					{
						BigPortrait();
						grid_on = 0;
					}
					else
					{
						DrawGrid();
						grid_on = 1;
					}
				}
			break;
		}
	} while(key != 27);
}

/*0594*/MED_edit()
{
	char *fname;
	int _p_04;

	fname = C_013A(/*01ae*/"sercq", /*01b4*/"*.med");
	Window(1, 1, 39, 24, 3);
	ClrConsole();
	LoadFile(/*01ba*/"sercq", fname, D_08B4);
	Editor();
	SaveFile(/*01c0*/"sercq", fname, D_08B4, 160);
}

/*05F2*/MED_create()
{
	char *fname;
	int bp_04;
	int i;

	Window(1, 1, 39, 24, 3);
	fname = D_0954;
	do {
		ClrConsole();
		PutString(/*01C6*/"  CREATION D'UN NOUVEAU MEDAILLON\n\n\n");
		PutString(/*01EB*/"Entrez le nom du m{daillon\n");
		PutString(/*0207*/"  (sans extension):\n\n");
		GetString(fname);
		i = 0;
		if(strlen(fname) == 0)
			i = 1;
		if(strlen(fname) > 8)
			i = 1;
		if(bp_04 = index(fname, '.'))
			i = 1;
	}while(i);
	for(i = 0; i < 160; i ++)
	{
		D_08B4[i] = 0;
	}
	strcat(fname, /*021D*/".med");
	Editor();
	SaveFile(/*0222*/"sercq", fname, D_08B4, 160);
}

/*06BD*/MED_addToGame()
{
	char *fname;
	int i;

	fname = C_013A(/*0228*/"sercq", /*022E*/"*.med");
	Window(1, 1, 39, 24, 3);
	ClrConsole();
	PutString(/*0234*/"Int{gration du m{daillon au jeu en cours.\n");
	LoadFile(/*025f*/"sercq", fname, D_08B4);
	LoadFile(/*0265*/"sercq", /*026b*/"data.bin", PBuffer2);
	D_08B0 = PBuffer2 + BufGetWord(PBuffer2 + 9) + 7;
	D_08B2 = D_08B0 + 0x15E0;
	for(i = 0; i < 160; i ++)
	{
		*(D_08B2 ++) = D_08B4[i];
	}
	SaveFile(/*0274*/"sercq", /*027a*/"data.bin", PBuffer2, 0x2564);
}

/*0770*/main()
{
	int _p_02, _p_04;
	int bp_06;

	bp_06 = 4;
	PBuffer1 = malloc(6000);
	PBuffer2 = malloc(9999);
	GraphMode();
	do {
		switch(bp_06 = TopMenu(bp_06))
		{
			case 0: MED_create(); break;
			case 1: MED_edit(); break;
			case 2: MED_seeAll(); break;
			case 3: MED_addToGame(); break;
		}
	}while(bp_06 != 4);
	TextMode();
}

/*----------------------------------------*/
/*-- LIB1                               --*/
/*----------------------------------------*/

/*07D7*/SetCursorPosition(_col, _row)
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

/*080C*/GetCursorPosition(_pCol, _pRow)
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

/*0849*/WriteCharacter(_ch, _color)
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

/*0884*/ClrConsole()
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

/*08E5*/SetTextColor(_color)
int _color;
{
	TextColor = _color;
}

/*08F2*/Window(_left, _top, _right, _bottom, _color)
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

/*092D*/SetCursorPos(_relX, _relY)
int _relX;
int _relY;
{
	CursorX = WindowLeft + _relX;
	CursorY = WindowTop + _relY;
	SetCursorPosition(CursorX, CursorY);
}

/*0951*/NewLine()
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

/*0997*/PutChar_0(bp04)
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

/*09E3*/PutChar(_ch)
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

/*0AA7*/PutString(_str)
unsigned char *_str;
{
	while(*_str)
		PutChar(*_str ++);
}

/*0AC4*/PutResString(_str)
unsigned char *_str;
{
	while(*_str < 0x80)
		PutChar(*_str ++);
	PutChar(*_str & 0x7f);/*or toascii(*_str)*/
	PutChar('\n');
}

/*----------------------------------------*/
/*-- LIB2                               --*/
/*----------------------------------------*/

/*0AFC*/GetKey_0(_pKey)
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

/*0B76*/ClearKbdBuf()
{
	int ret;

	RegsIn.h.ah = 0xc;/*clear keyboard buffer and invoke keyboard function*/
	RegsIn.h.al = 0;
	ret = sysint(0x21, &RegsIn, &RegsOut);
}

/*0B9F*/GetKey(_pKey)
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

C_0CC1()
{
	int bp_02, key;

	while(bp_02 = GetKey_0(&key));
}

/*----------------------------------------*/
/*--                                    --*/
/*----------------------------------------*/

/*0CDC*/BigPlot(_x, _y, _color)
int _x;
int _y;
int _color;
{
	int i, j;

	for(i = 0; i < 3; i ++)
	{
		for(j = 0; j < 3; j ++)
		{
			Plot(_x * 3 + i + 16, _y * 3 + j + 10, _color);
		}
	}
}

/*(a little different from SERCQ.C)*/
/*0D38*/RenderPortrait(_ptr, _x, _y, _color1, _color2)
unsigned char *_ptr;
int _x;
int _y;
int _color1;
int _color2;
{
	unsigned char bp_01;
	unsigned char bp_02, bp_03;
	int voffset;
	int voffset_0;
	int y;
	unsigned i, j;

	voffset_0 = (_y / 2) * 80 + _x / 4;
	y = _y;
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
	DrawLine_bis(_x -  1, _y -  1, _x + 32, _y -  1, 1);
	DrawLine_bis(_x -  1, _y + 40, _x + 32, _y + 40, 1);
	DrawLine_bis(_x -  1, _y + 40, _x -  1, _y -  1, 1);
	DrawLine_bis(_x + 32, _y + 40, _x + 32, _y -  1, 1);
	/*-- --*/
}

/*0EFB*/BigByte(_index)
int _index;
{
	int x, y;
	int _p_06;
	int i;
	int mask;

	mask = 0x80;
	y = _index / 4;
	x = (_index % 4) * 8;
	for(i = 0; i < 8; i ++)
	{
		if(D_08B4[_index] & mask)
			BigPlot(x + i, y, 3);
		else
			BigPlot(x + i, y, 0);
		mask >>= 1;
	}
}

/*0F73*/BigPortrait()
{
	int i;

	for(i = 0; i < 160; i ++)
	{
		BigByte(i);
	}
	/*-- --*/
	DrawLine_bis(15, 9, 112, 9, 1);
	DrawLine_bis(15, 130, 112, 130, 1);
	DrawLine_bis(15, 9, 15, 130, 1);
	DrawLine_bis(112, 9, 112, 130, 1);
}

/*0FFC*/DrawGrid()
{
	int i;

	for(i = 1; i < 4; i ++)
	{
		DrawLine_bis(i * 24 + 16, 9, i * 24 + 16, 130, 1);
	}
	for(i = 8; i < 39; i += 8)
	{
		DrawLine_bis(15, i * 3 + 10, 112, i * 3 + 10, 1);
	}
}

/*----------------------------------------*/
/*-- LIB3                               --*/
/*----------------------------------------*/

/*108b*/SetBGColor(_bg_color)
int _bg_color;
{
	int ret;

	RegsIn.h.ah = 0xb;
	RegsIn.h.bh = 0;
	RegsIn.h.bl = _bg_color;
	ret = sysint(0x10, &RegsIn, &RegsOut);
}

/*10BA*/GraphMode()
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

/*1114*/TextMode()
{
	int ret;

	RegsIn.h.ah = 0;
	RegsIn.h.al = 2;
	ret = sysint(0x10, &RegsIn, &RegsOut);
}

/*113D*/GetPixel(_x, _y)
int _x;
int _y;
{
	return (peekb((_y / 2) * 80 + (_y % 2) * 0x2000 + _x / 4, 0xb800) >> (3 - _x % 4) * 2) & 3;
}

unsigned char __0283 = 0;/*padding*/

unsigned char D_0284[4] = {0xC0,0x30,0x0C,0x03};
unsigned char D_0288[4] = {0x3F,0xCF,0xF3,0xFC};

/*119E*/Plot(_x, _y, _color)
int _x;
int _y;
int _color;
{
	int voffset, bp_04;

	voffset = (_y / 2) * 80 + (_y % 2) * 0x2000 + _x / 4;
	bp_04 = _x % 4;
	pokeb(voffset, 0xb800,
		(peekb(voffset, 0xb800) & D_0288[bp_04]) |
		(D_0284[bp_04] & D_0012[_color])
	);
}

unsigned char D_028C[4] = {0xC0,0x30,0x0C,0x03};

/*(uncalled)*/
/*1233*/Plot_bis(_x, _y)
int _x;
int _y;
{
	int voffset;

	voffset = (_y / 2) * 80 + (_y % 2) * 0x2000 + _x / 4;
	pokeb(voffset, 0xb800, peekb(voffset, 0xb800) ^ D_028C[_x % 4]);
}

/*12AA*/DrawLine(_x0, _y0, _x1, _x2, _color)
int _x0;
int _y0;
int _x1;
int _x2;
int _color;
{
	color(_color);
	line(_x0, _y0, _x1, _x2);
}

/*12CA*/DrawLine_bis(_x0, _y0, _x1, _y1, _color)
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
/*-- LIB4                               --*/
/*----------------------------------------*/

/*12F2*/SaveFile_0(_fname, _buffer, _count)
char *_fname;
unsigned char *_buffer;
int _count;
{
	FILE *f;
	int i;
	int key;/*TODO initialize*/

	f = fopen(_fname, /*0290*/"w");
	if(f == 0)
	{
		ClrConsole();
		PutString(/*0292*/"ERREUR DISQUE\n");
		PutString(/*02A1*/"Appuyez sur la touche ESPACE\n");
		while(key != ' ')
			i = GetKey_0(&key);
		return;
	}
	for(i = 0; i < _count; i ++)
		fwrite(_buffer ++, 1, 1, f);
	fclose(f);
}

/*136A*/SaveFile(_dir, _fname, _buffer, _count)
char *_dir;
char *_fname;
unsigned char *_buffer;
int _count;
{
	chdir(_dir);
	SaveFile_0(_fname, _buffer, _count);
	chdir(/*02BF*/".");
}

/*138F*/LoadFile_0(_fname, _buffer)
char *_fname;
unsigned char *_buffer;
{
	FILE *f;
	int i;
	int key;/*TODO initialize*/

	f = fopen(_fname, /*02C1*/"r");
	if(f == 0)
	{
		ClrConsole();
		PutString(/*02C3*/"ERREUR DISQUE\n");
		PutString(/*02D2*/"Appuyez sur la touche ESPACE\n");
		while(key != ' ')
			i = GetKey_0(&key);
		return;
	}
	while(fread(_buffer ++, 1, 1, f));
	fclose(f);
}

/*13FB*/LoadFile(_dir, _fname, _buffer)
char *_dir;
char *_fname;
unsigned char *_buffer;
{
	chdir(_dir);
	LoadFile_0(_fname, _buffer);
	chdir(/*02F0*/".");
}

char D_02F2[22] = "dessind7.000";

/*load "dessind7.0xx" in buffer?*/
/*141D*/C_141D(bp04, bp06)
int bp04;
unsigned char *bp06;
{
	D_02F2[11] = bp04 % 10 + '0';
	D_02F2[10] = (bp04 % 100) / 10 + '0';
	LoadFile(/*0308*/"sercq", D_02F2, bp06);
}

char D_030E[22] = "tem000.txt";

/*load "tem0xx.txt" in buffer?*/
/*145A*/C_145A(bp04, _ptr)
int bp04;
unsigned char *_ptr;
{
	D_030E[5] = bp04 % 10 + '0';
	D_030E[4] = (bp04 % 100) / 10 + '0';
	LoadFile(/*0324*/"sercq", D_030E, _ptr);
}

/*----------------------------------------*/
/*-- LIB5                               --*/
/*----------------------------------------*/

/*1497*/BufGetWord(_p)
unsigned char *_p;
{
	int low, high;

	low = *_p ++;
	high = *_p;

	return (high << 8) + low;
}

/*14BF*/Delay(count)
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

/*150B*/ToUpperString(_str)
char *_str;
{
	int _p_02;

	for(; *_str; _str ++)
	{
		if(islower(*_str))
			*_str = _toupper(*_str);
	}
}

/*1534*/GetString(_str)
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
/*--                                    --*/
/*----------------------------------------*/
