/*----------------------------------------*/
/* "MEURTRES EN SERIE"                    */
/* Copyright Cobra Soft 1987              */
/* Decompiled by ergonomy_joe in 2020     */
/*----------------------------------------*/
#include <ctype.h>

#include "sercq.h"

/*3674*/Explanation()
{
	ClrConsole();
	SetTextColor(2);
	PutString(/*0AE4*/"          PROGRAMME DE TEST\n\n\n");
	SetTextColor(3);
	PutString(/*0B03*/"  Ce programme ne vous indiquera jamais ");
	PutString(/*0B2C*/"la solution compl}te mais il vous permet");
	PutString(/*0B55*/"de tester un certain nombre de points   ");
	PutString(/*0B7E*/"particuliers.\n");
	PutString(/*0B8D*/"  Pour consid{rer que vous avez fini le ");
	PutString(/*0BB6*/"jeu, il faut avoir vu tous les personna-");
	PutString(/*0BDF*/"ges, tous leurs t{moignages quand ils en");
	PutString(/*0C08*/"ont plusieurs, et d{couvert l'emplace-  ");
	PutString(/*0C31*/"ment de chaque indice.\n");
	PutString(/*0C49*/"  Les r{ponses au questionnaire doivent ");
	PutString(/*0C72*/"~tre donn{es selon un format sp{cial :\n");
	PutString(/*0C9A*/"- toujours en majuscule\n");
	PutString(/*0CB3*/"- personne : son pr{nom puis son nom\n");
	PutString(/*0CD9*/"- heure : chiffre puis H sans espace\n");
	PutString(/*0CFF*/"- indice : son num{ro seul\n");
	PutString(/*0D1B*/"- liste : mots ou noms s{par{s par /\n");
	PutString(/*0D41*/"  sans espace\n");
	PutString(/*0D50*/"- Coordonn{es : s{par{es par un .\n\n\n");
}

/*3726*/Contest()
{
	SetTextColor(3);
	PutString(/*0D75*/"Un concours sera organis{...\n");
	PutString(/*0D93*/"1er Prix : Un week-end @ Sark !\n");
	PutString(/*0DB4*/"Tenez-vous inform{ en consultant notre\n");
	PutString(/*0DDC*/"serveur t{l{matique (Voir notice).\n\n");
	PutString(/*0E01*/"Pour relancer le test tapez T, sinon\n");
	PutString(/*0E27*/"sur n'importe quelle autre touche.");
}

/*3765*/AskQuestion(_str, _checksum)
char *_str;
int _checksum;
{
	int i;

	i = 0;
	SetTextColor(1);
	/*-- display question --*/
	PutString(_str);
	/*-- read answer --*/
	_str = (char *)TmpString;
	/*-- --*/
	GetString(_str);
	/*-- compute answer's checksum --*/
	while(*_str)
		i += *_str ++;
	/*-- verify checksum --*/
	if(i == _checksum)
		PutString(/*0E4A*/"\nREPONSE VRAIE\n\n\n");
	else
		PutString(/*0E5C*/"\nREPONSE FAUSSE\n\n\n");
}

/*37BC*/Questionnaire()
{
	AskQuestion(/*0E6F*/"A  quelle  heure  Backlinder  est-il    mort ? ", 0x7F);
	AskQuestion(/*0E9F*/"Qui a vol{ le v{lo du pasteur ? ", 0x334);
	AskQuestion(/*0EC0*/"Ou trouve-t-on un pendu ? ", 0x101);
	AskQuestion(/*0EDB*/"Combien de morts, au total ? ", 0x38);
	AskQuestion(/*0EF9*/"Qui meurt @ 17H45 ? ", 0xA91);
	AskQuestion(/*0F0E*/"A qui se rapporte l'indice 8 ? ", 0x428);
	AskQuestion(/*0F2E*/"En quoi est {crit l'indice 6 ? ", 0x189);
	AskQuestion(/*0F4E*/"A quelle heure Gwendal quitte-il son    domicile ? ", 0x10E);
	AskQuestion(/*0F82*/"Coordonn{es de la cabane de Johnny ? ", 0x15A);
	AskQuestion(/*0FA8*/"Quelle indice dans la malle ? ", 0x33);
	AskQuestion(/*0FC7*/"Quel indice chez Macomber ? ", 0x63);
	AskQuestion(/*0FE4*/"Qui vous appelle au t{l{phone ? ", 0x40F);
	AskQuestion(/*1005*/"Que vous dit le fantome ? ", 0x1BF);
	AskQuestion(/*1020*/"Combien d'indices changent de place ? ", 0x32);
	AskQuestion(/*1047*/"Dickson meurt @ quelle heure ? ", 0x110);
	AskQuestion(/*1067*/"Coordonn{es du tuba ? ", 0x12F);
	AskQuestion(/*107E*/"Qui a tu{ Backlinder ? ", 0x334);
	AskQuestion(/*1096*/"Ou a {t{ pris l'indice 1 ? ", 0x131);
	AskQuestion(/*10B2*/"A  quel  indice  se  rapporte  l'indice 22 ? ", 0x37);
	AskQuestion(/*10E0*/"Que lit-on sur l'autre morceau du       parchemin ? ", 0x991);
	AskQuestion(/*1115*/"Quid ? ", 0x129);
	AskQuestion(/*111D*/"Coordonn{es du Tr{sor ? ", 0x104);
}

/*38F7*/Ending()
{
	int bp_02;
	int key;

	Window(0, 0, 39, 24, 3);
	do {
		Explanation();
		Questionnaire();
		Contest();
		while((bp_02 = GetKey_0(&key)) != 1);
	}while(key == 'T' || key == 't');
}

#asm
PUBLIC Sound_1_	;393F
Sound_1_:
	MOV	BX,100h

	IN	AL,61h
	AND	AL,NOT 3	;FC
	MOV	DX,BX
C_3948:
	XOR	AL,2
	OUT	61h,AL
	MOV	CX,DX
C_394E:
	LOOP	C_394E
	DEC	BX
	JNZ	C_3948

	RET
#endasm

#asm
PUBLIC Sound_2_	;3954
Sound_2_:
	PUSH	BP
	MOV	BP,SP
	MOV	DX,[bp+04]
	MOV	BX,[bp+06]

	IN	AL,61h
	AND	AL,NOT 3
C_3961:
	XOR	AL,2
	OUT	61h,AL
	MOV	CX,DX
C_3967:
	LOOP	C_3967
	DEC	BX
	JNZ	C_3961

	POP	BP
	RET
#endasm

/*----------------------------------------*/
/*
/*----------------------------------------*/

C_396E(_str, _col, _row, _color)
unsigned char *_str;
int _col;
int _row;
int _color;
{
	unsigned char ch;

	while(*_str)
	{
		SetCursorPosition(_col ++, _row);
		ch = *_str ++;
		if(isalpha(ch))
			ch = ch | 0x80;
		WriteCharacter(ch, _color);
	}
}

/*39C2*/Prologue()
{
	int row;

	row = 3;
	C_396E(/*1136*/"            PROLOGUE", 2, row ++, 2);
	row += 3;
	C_396E(/*114B*/"UN MEURTRE A ETE COMMIS A SERCQ !", 2, row ++, 3);
	C_396E(/*116D*/"CONNAISSANT VOTRE REPUTATION DE ", 2, row ++, 3);
	C_396E(/*118E*/"FIN LIMIER, ON A FAIT APPEL A VOUS.", 2, row ++, 3);
	row ++;
	C_396E(/*11B2*/"IL EST ONZE HEURES. DANS QUELQUES", 2, row ++, 3);
	C_396E(/*11D4*/"MINUTES VOUS SEREZ SUR L'ILE,", 2, row ++, 3);
	C_396E(/*11F2*/"L'AFFAIRE NE FAIT QUE COMMENCER !", 2, row ++, 3);
	row ++;
	C_396E(/*1214*/"MAIS VOTRE PLANNING EST CHARGE...", 2, row ++, 3);
	C_396E(/*1236*/"UNE AUTRE ENIGME VOUS ATTEND EN", 2, row ++, 3);
	C_396E(/*1256*/"ITALIE. LE BATEAU PART A DIX-NEUF", 2, row ++, 3);
	C_396E(/*1278*/"HEURES. TOUT DEVRA ETRE ELUCIDE.", 2, row ++, 3);
	row += 2;
	C_396E(/*1299*/"               BONNE CHANCE !", 2, row ++, 3);
}

/*3B0B*/Credits() {
	int row;

	row = 2;
	C_396E(/*12B7*/"        MEURTRES EN SERIE", 2, row ++, 2);
	C_396E(/*12D1*/"         _______________ ", 2, row ++, 2);
	row += 3;
	C_396E(/*12EB*/"REALISATION    : BERTRAND BROCARD", 2, row ++, 3);
	row ++;
	C_396E(/*130D*/"GRAPHISMES     : CHRISTIAN DESCOMBES", 2, row ++, 3);
	row ++;
	C_396E(/*1332*/"MUSIQUE        : CHARLES CALLET", 2, row ++, 3);
	C_396E(/*1352*/"                 JEAN-JACQUES MAHE", 2, row ++, 3);
	row ++;
	C_396E(/*1375*/"PROGRAMMATION  : GILLES BERTIN", 2, row ++, 3);
	row ++;
	C_396E(/*1394*/"CONSEILS EN 'C': ROLAND MORLA", 2, row ++, 3);
	row += 3;
	C_396E(/*13B2*/"MEURTRES EN SERIE  EST UN PRODUIT", 2, row ++, 1);
	C_396E(/*13D4*/"      'HITECH PRODUCTIONS'       ", 2, row ++, 1);
	C_396E(/*13F6*/"   EDITE PAR 'COBRA SOFT' SUR    ", 2, row ++, 1);
	C_396E(/*1418*/"PC - ATARI ST - THOMSON - AMSTRAD", 2, row ++, 1);
}

/*3C5A*/DemoDelay(_count)
int _count;
{
	int bp_02;
	int key;

	Delay(_count);
	bp_02 = GetKey(&key);
	if(bp_02 != 9)
		GameExit();
}

/*3C7E*/DemoPlaces(_placeId, _count)
int _placeId;
int _count;
{
	int i;

	ClrConsole();
	SetCursorPos(0, 1);
	PutString(/*143A*/"\n");
	C_0721(/*143C*/"QUELQUES PAYSAGES CARACTERISTIQUES", 1);
	PutString(/*145F*/"\n\n");
	C_0721(/*1462*/"        DE L'ILE DE SERCQ", 1);
	for(i = 0; i < _count; i ++)
	{
		C_1C1B(_placeId + i, PBuffer1);/*load "dessind7.0xx" in buffer?*/
		C_1C95(PBuffer1);/*render "dessind7.0xx" data*/
		DemoDelay(14);
	}
}

/*3CF4*/DemoCharacters(_charaId, _count)
int _charaId;
int _count;
{
	int i;

	ClrConsole();
	SetCursorPos(1, 1);
	PutString(/*147C*/"\n\n");
	C_0721(/*147F*/"   QUELQUES PERSONNAGES DU JEU", 1);
	for(i = 0; i < _count; i ++)
	{
		Portrait(_charaId + i, 3, 0);
		DemoDelay(10);
	}
	Portrait(0x23, 3, 0);
}

/*3D61*/DemoTime(_count)
int _count;
{
	int i;

	_count = _count / 2;
	ClrConsole();
	SetCursorPos(0, 1);
	PutString(/*149E*/"\n");
	C_0721(/*14A0*/"DANS 'MEURTRES EN SERIE'", 1);
	PutString(/*14B9*/"\n\n");
	C_0721(/*14BC*/"LE TEMPS AUSSI A SON IMPORTANCE...", 1);
	for(i = 0; i < _count; i ++)
	{
		PlusOneMinute();
		DemoDelay(2);
		PlusOneMinute();
		Delay(1);
	}
}

/*14DF*/int DemoMode = 0;

/*3DDB*/DemoMovement()
{
	int bp_02;

	ClrConsole();
	SetCursorPos(0 ,1);
	PutString(/*14E1*/"\n");
	C_0721(/*14E3*/"COMME SUR L'ILE DE SERCQ, VOUS", 1);
	PutString(/*1502*/"\n\n");
	C_0721(/*1505*/"POUVEZ VOUS DEPLACER ", 1);
	SetCursorPos(21, 3);
	PutString(/*151B*/"            ");
	SetCursorPos(21, 3);
	switch(DemoMode)
	{
		case 0:
			Speed = SPEED_ROAD;
			Map_x = 78; Map_y = 21;
			C_0721(/*1528*/"A PIED");
			PutIcon(Map_x, Map_y, Speed, 0);
			while(Map_x != 82 || Map_y != 83)
			{
				bp_02 = MapMove(4);
				DemoDelay(2);
			}
		break;
		case 1:
			Speed = SPEED_BIKE;
			C_0721(/*152F*/"EN VELO");
			PutIcon(Map_x, Map_y, Speed, 0);
			while(Map_x != 63 || Map_y != 141)
			{
				bp_02 = MapMove(4);
				DemoDelay(2);
			}
		break;
		case 2:
			Speed = SPEED_SEA;
			Map_x = 126; Map_y = 30;
			C_0721(/*1537*/"EN BATEAU");
			PutIcon(Map_x, Map_y, Speed, 0);
			while(Map_y != 3)
			{
				bp_02 = MapMove(7);
				DemoDelay(2);
			}
			while(Map_x != 79 || Map_y != 3)
			{
				bp_02 = MapMove(6);
				DemoDelay(2);
			}
		break;
	}
	RestoreIcon(Map_x, Map_y);
	if(DemoMode ++ == 2)
		DemoMode = 0;
}

/*3F98*/Demo()
{
	GraphMode();
	Prologue();
	Delay(40);
	GraphMode();
	InitGameScreen();
	Portrait(0x23, 3, 0);

	Clock_H = 11; Clock_M = 55;
	ClockFace(Clock_H, Clock_M);
	Clock_H = 11; Clock_M = 0;
	ClockFace(Clock_H, Clock_M);

	while(1)
	{
		MUSIC_start();
		/*-- --*/
		DemoPlaces(0x00, 8);
		DemoCharacters(0x00, 9);
		DemoTime(120);
		DemoPlaces(0x08, 8);
		DemoCharacters(0x09, 9);
		DemoMovement();
		/*-- --*/
		DemoPlaces(0x10, 8);
		DemoCharacters(0x12, 9);
		DemoTime(120);
		DemoPlaces(0x18, 8);
		DemoCharacters(0x1b, 9);
		DemoMovement();
		/*-- --*/
		MUSIC_stop();
	}
}

char __1541 = 0;/*padding*/
