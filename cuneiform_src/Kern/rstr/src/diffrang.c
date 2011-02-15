/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

����������� ��������� ��������������� � ������������� ��� � ���� ��������� ����,
��� � � �������� �����, � ����������� ��� ���, ��� ���������� ��������� �������:

      * ��� ��������� ��������������� ��������� ���� ������ ���������� ���������
        ���� ����������� �� ��������� �����, ���� ������ ������� � �����������
        ����� �� ��������.
      * ��� ��������� ��������������� ��������� ���� � ������������ �/��� �
        ������ ����������, ������������ ��� ���������������, ������ �����������
        ��������� ���� ���������� �� ��������� �����, ���� ������ ������� �
        ����������� ����� �� ��������.
      * �� �������� Cognitive Technologies, �� ����� �� ����������� �� �����
        ���� ������������ � �������� �������� ��������� �/��� �����������
        ���������, ���������� �� ���� ��, ��� ���������������� �����������
        ����������.

��� ��������� ������������� ����������� ��������� ���� �/��� ������� ������ "���
��� ����" ��� ������-���� ���� ��������, ���������� ���� ��� ���������������,
������� �������� ������������ �������� � ����������� ��� ���������� ����, �� ��
������������� ���. �� �������� ��������� ���� � �� ���� ������ ����, �������
����� �������� �/��� �������� �������������� ���������, �� � ���� ������ ��
��Ѩ� ���������������, ������� ����� �����, ���������, ����������� ���
������������� ������, ��������� � �������������� ��� ���������� ����������
������������� ������������� ��������� (������� ������ ������, ��� ������,
������� ���������, ��� ������ �/��� ������ �������, ���������� ��-�� ��������
������� ��� �/��� ������ ��������� �������� ��������� � ������� �����������,
�� �� ������������� ����� ��������), �� �� ������������� ���, ���� ���� �����
�������� ��� ������ ���� ���� �������� � ����������� ����� ������� � ������.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of the Cognitive Technologies nor the names of its
      contributors may be used to endorse or promote products derived from this
      software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


/*�������������������������������������������������������������Ŀ
	�  ���ਬ������ 㣫�� : CURVE � ��㣨�                      �
	���������������������������������������������������������������
*/
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
        #include "status.h"
        #include "discrim.h"
        #include "tuner.h"
        #include "ligas.h"

/************************************************************/
extern int16_t  LeftDistance(uchar *RASTER,int16_t dx);
extern int16_t  RightDistance(uchar *RASTER,int16_t dx);
extern segment * go_line(segment *s,uint16_t num);

/************************************************************/
static int16_t long_line( puchar seg,int16_t w,int16_t line,int16_t part);


#define bytlen(c) ((c+7)>>3)

#define NUM_LINES       5       /* �᫮ �����                  */
#define NUM_OFF_LIN    	4	/* max num of skip lines	*/
#define DIST_LIN        4	/* max ࠧ��稥 � ����� 	*/
#define EMPTY_LIM	8	/* ����� �����			*/
#define LEN_DIST	2	/* �᫮ ���⮢ ����� ��� ���᪠*/

#define A_NR 0                  /* ⨯ "�� ��।����"		*/
#define A_S  1			/* ⨯ "���"     		*/
#define A_C  2                  /* ⨯ "��㣫����"		*/
#define A_A  4                  /* ⨯ "㣮�"      		*/
#define A_E  8                  /* ⨯ "���⮩"    		*/
#define N_C  16			/* ⨯ "��� ��㣫����"		*/


static int16_t height;
static int16_t buf_lines[NUM_LINES];	/* ���� ���� �����	*/

typedef struct
	{
	uchar 		tl, 		/* ������� ����� 	*/
			tr,             /* ������� ������	*/
			bl,             /* ������  ����� 	*/
			br;             /* ������  ������	*/
	}CRN ;				/* 4 㣫�		*/

CRN cut_crn={0}, cut_crn0={0}, cut_crn1={0};
#include "diffrang.h"

CRN	angles,				/* ���� 㣫�� 		*/
*default_corners ;                      /* 㣫� �� 㬮�砭��    */

static int16_t non_similar_angles(uchar def_a, uchar a,
#ifdef UFA
  uchar cut_sign,
#endif
  int16_t dy)
{
int16_t ret=0;

if( a==255 )
        return 0; /* angle have empty lines */

if( def_a==A_C && a==0 )
        {/* ��ࠧ��-��㣫����, ᪠窮� ���, ����� > 8     */
        if( dy <= SMALL_KEGL || fax1x2 )
                ret = 0;
        else
                ret = 2;
#ifdef UFA
        if( cut_sign ) ret>>=1;
#endif
        }

if( def_a==N_C && a>1 && a!=SERIF )
        {
         /* ��ࠧ��-����㣫����, ���� 2 ᪠窠  */
        if( dy<LARGE_KEGL )
                ret = 2;
        else
          {
          if( a>2 )
                ret = 2;
          else  ret = 1;
          }
        }

if( def_a==A_C && a==SERIF )
        {
        ret = 2;
        }

return ret;
}

int16_t corner_type( uchar crn )
{
if( crn == SERIF )  return SERIF;
else if( crn == 0 ) return NON_CURVE;
else  if( crn > 1 )
        {
        if( height<LARGE_KEGL )   return CURVE;
        else if( crn>2 )     return CURVE;
        }
return UNDEF;
}

/* ���ਬ����� �� ࠧ��稥 㣫�� �� ���᫥��� ⨯�� angles */
static int16_t check_angles(uchar let,int16_t dy)
{
int16_t non_sim=0,non_num=0,p;
int16_t deskr[]={0,40,80,200,240};	// 0, 50, 120, ...

/* default_corners - ������ 㣫� */
if( (p=non_similar_angles(default_corners[let].tl,angles.tl,
#ifdef UFA
  cut_crn.tl ,
#endif
  dy))!=0  )
  {non_sim +=p ; non_num++;}
if( (p=non_similar_angles(default_corners[let].tr,angles.tr,
#ifdef UFA
  cut_crn.tr ,
#endif
  dy))!=0  )
  {non_sim +=p ; non_num++;}
if( (p=non_similar_angles(default_corners[let].bl,angles.bl,
#ifdef UFA
  cut_crn.bl ,
#endif
  dy))!=0  )
  {non_sim +=p ; non_num++;}
if( (p=non_similar_angles(default_corners[let].br,angles.br,
#ifdef UFA
  cut_crn.br ,
#endif
  dy))!=0  )
  {non_sim +=p ; non_num++;}

#ifdef UFA
if( non_num>1 && non_sim<4 )
  non_sim = 4;
#endif
return( deskr[  non_sim>>1 ] );
}

/* ��।����� ⨯ 㣫� �� NUM_LINES ����ﭨ� �� buf_lines 	*/
/* �饬 ⮫쪮 ⨯ CURVE					*/
static uchar calc_one_angle(int16_t h)
{
int16_t i,old=buf_lines[0],neue,num_jmps,s;
 int16_t back_jmps,equ;

 equ=0;
for(s=i=1,num_jmps=0;i<h;i++)
        {
	neue = buf_lines[i];
	if( neue>old )
                {
                num_jmps++;
		s += neue - old;
                }
	if( neue==old )
                equ++;
	if( i>1 && neue==buf_lines[i-2] )
                equ++;

	old = neue;
        }
if ( num_jmps>2 ||              /* 3 ᪠窠 ���         */
     num_jmps && s>2  )         /* ������ ᪠箪       */
        return SERIF;     /* ��宦� �� �������  */
if( (num_jmps || equ>2) && h==4 && buf_lines[i]-old>2 )
	return SERIF;	  /* ��宦� �� �������  */
back_jmps = num_jmps;

for(num_jmps=0,old=buf_lines[0],i=1;i<h;i++)
	{
	neue = buf_lines[i];
	if( neue<old )
		num_jmps++;
	if( neue>old && i==2 )
                return 0;  /* �� ��㣫���� : ���� ⮫騭� 1 */

	old = neue;
	}
	/* ��㣫���� : ����� 2-� ᪠窮� */
if( num_jmps==1 && back_jmps>=1 ||   /* 1+ 1- */
    num_jmps>1  && back_jmps>1 )     /* 2+ 2- */
        num_jmps   = 0;
return(uchar)num_jmps ;
}

/* ���᫨�� NUM_LINES ����ﭨ� ᫥�� �� ���� 			*/
/* �ய�᪠�� ��砫�� ����� ��-�� ᪠窮� ����� DIST_LIN		*/
/* ������ 㣫� ��।������ ���஬ � ��뢠�饩 �ணࠬ��		*/
/* ���ࠢ����� �������� ������ D_X					*/
static uchar get_lines(uchar *RASTER, int16_t D_X, int16_t dx,
                int16_t (*Distance)(uchar *, int16_t) , int16_t h)
{
int16_t i,old,neue;

memset( buf_lines,0,sizeof(buf_lines));

old = Distance(RASTER,dx);
if( old==-1 )
        return 255;     /* empty line */
RASTER += D_X;

for( i=1;i<NUM_OFF_LIN;i++,RASTER+=D_X)
	{
	neue = Distance(RASTER,dx);
	if( neue==-1 )
                return 255;     /* empty line */

	if( abs(neue-old)<DIST_LIN )
		{
		RASTER += D_X;
		break;
		}
	if( i<NUM_OFF_LIN-1 )
		old = neue; /* ��। ��室�� ��࠭塞 ��� ����� */
	}
buf_lines[0] = old;
buf_lines[1] = neue;
for(i=2;i<h;i++,RASTER+=D_X)
        {
        buf_lines[i] = Distance(RASTER,dx);
        if( buf_lines[i]==-1 )
                return 255;     /* empty line */
        }

if( h==4 )
        {
        buf_lines[i] = Distance(RASTER,dx);
        if( buf_lines[i]==-1 )
                return 255;     /* empty line */
        }

return( calc_one_angle(h) );
}

uchar Let_Width_Bottom_Right_Curve[]="�����";
/* ���ਬ����� �� 㣫� ᨬ���� let, h - ����, type - ���ᨢ�����   */
int16_t discr_angles(uchar let, int16_t h, int16_t type)
{

default_corners = (CRN *)(type ? def_corn_cs : def_corn);

memcpy(&cut_crn,&cut_crn0,sizeof(cut_crn));
#ifdef UFA
default_corners[(uchar)'�'].tr=0;
if( MEMCHR(Let_Width_Bottom_Right_Curve,let,
    sizeof(Let_Width_Bottom_Right_Curve)) )
      memcpy(&cut_crn,&cut_crn1,sizeof(cut_crn));
#endif

if( multy_language )
  {
  default_corners[(uchar)'5'].tl=N_C;
  default_corners[(uchar)'5'].bl=A_C;

  if (!is_turkish_language(language)) // 21.05.2002 E.P.
	default_corners[(uchar)'�'].tr=N_C;

  default_corners[(uchar)'�'].tr=N_C;
  }

return check_angles(let,h);
}

/* ᪮�쪮 ��ப ���� �� �࠭��� ���������� */
static uchar is_angle(uchar *RASTER, int16_t D_X, int16_t dx,
                int16_t (*Distance)(uchar *, int16_t) , int16_t h, int16_t dest)
{
int16_t zero,i;

for(zero=i=0;i<h;i++,RASTER+=D_X)
  zero += ( Distance(RASTER,dx)==dest );

return zero>2;
}

void calc_angles(struct rst *_rst,puchar segment,uchar ang[],uchar cutl,uchar cutr)
{
uchar *rt,*rb;
int16_t D_X,dx;
int16_t skip,h;
int16_t right_dest;

        height = _rst->h;
        h = _rst->h<LARGE_KEGL?4:5;
        D_X =bytlen(_rst->w);
        rt=_rst->raster ;
        rb=rt + D_X*(_rst->h-1);
        dx = bytlen(_rst->w);
        right_dest=D_X*8-_rst->w;

memset(&cut_crn0,0,sizeof(cut_crn));
memset(&cut_crn1,0,sizeof(cut_crn));

        skip = long_line(segment,(int16_t)(_rst->w/3),0,0);
        angles.tl =get_lines(rt+D_X*skip,D_X,dx,LeftDistance,h);
        if( cutl && is_angle(rt+D_X*skip,D_X,dx,LeftDistance,4,0) )
          cut_crn1.tl=1;

        skip = long_line(segment,(int16_t)(_rst->w/3),(int16_t)(_rst->h-1),0);
        angles.bl =get_lines(rb-D_X*skip,(int16_t)(-D_X),dx,LeftDistance,h);
        if( cutl && is_angle(rb-D_X*skip,(int16_t)(-D_X),dx,LeftDistance,4,0) )
          cut_crn1.bl=1;

        skip = long_line(segment,(int16_t)(_rst->w/3),0,1);
        angles.tr =get_lines(rt+D_X*skip,D_X,dx,RightDistance,h);
        if( cutr && is_angle(rt,(int16_t)(D_X*skip),dx,RightDistance,4,right_dest) )
          cut_crn1.tr=1;

        skip = long_line(segment,(int16_t)(_rst->w/3),(int16_t)(_rst->h-1),1);
        angles.br =get_lines(rb-D_X*skip,(int16_t)(-D_X),dx,RightDistance,h);
        if( cutr && is_angle(rb-D_X*skip,(int16_t)(-D_X),dx,RightDistance,4,right_dest) )
          cut_crn1.br=1;

memcpy(ang,&angles,4*sizeof(ang[0]));
return ;
}

int16_t long_line( puchar seg,int16_t w,int16_t line,int16_t part)
{
segment * segm;

 segm = go_line((segment*)seg,line);
 if( part == 0){ if(segm->segblack == 0) segm++;}
 else  while(segm->segwhite != 0x9000)   segm++;

return  ( segm->segblack <= w );
}

