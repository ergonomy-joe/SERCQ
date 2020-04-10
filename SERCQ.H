/*----------------------------------------*/
/* "MEURTRES EN SERIE"                    */
/* Copyright Cobra Soft 1987              */
/* Decompiled by ergonomy_joe in 2020     */
/*----------------------------------------*/
#ifndef __SERCQ_H__
#define __SERCQ_H__

#define SPEED_FOOT 0
#define SPEED_BIKE 1
#define SPEED_ROAD 2
#define SPEED_SEA  3

/*0A4A*/extern int MUSIC_flag;

/*1A50*/extern void *PBuffer1;
/*1A60,1A62*/extern int Map_x,Map_y;
/*1A64*/extern int Speed;
/*1A66,1A68*/extern int Clock_H,Clock_M;
/*1A90*/extern unsigned char TmpString[];

#endif
