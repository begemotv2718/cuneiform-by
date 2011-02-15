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

//"����������������" �������� �������� ����� ����������
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "struct.h"
#include "func.h"
#include "cut_glue.h"
#include "dmconst.h"
#include "linutil.h"

 extern uchar db_status;  // snap presence byte
 extern uchar db_trace_flag;  // 2 - more detailed estimate (ALT-F7)

//����� ��������� �������
 static cell *LC;                  //cell ����� �� �������
 static raster *r;                 //�����
 static int16_t ncut;
 static struct cut_elm *cut_list;  //������ ncut �������
 static seg_vers **vers_list;      //������� ���������������� ���������

//������� ��������� ����������
 static int16_t ib2;  //��������� �������� ������� ��� �������� �����
 static int16_t ib1;  // �� ��, ���� ��������� ����� �� ������
 static int16_t il;   //��������� ��������� ������� ��� �������� �����
 static int16_t iec;  //������� ��� �������� �� ������� �����������
 static int16_t ie2;  //��������� �������� ������� ��� �������� ������
 static int16_t ie1;  // �� ��, ���� ��������� ����� �� ������
 static int16_t ir;   //��������� ��������� ������� ��� �������� ������
 static int16_t ibc;  //������� ��� �������� �� ������� �����������
 static int16_t iemax;       //������ �������
 static char right_dust; //����: ������ dust (������� ����� ��� �������)
 static char fl_b;       //����: ��� �������� ����� ���������� "�"
 static uchar connect_;    //���� ������ �� ������� �����������

 static int16_t w1,w2,h2;

static char l2r(int16_t *ib1, int16_t *ib2, int16_t *il);
static char lcut_out(int16_t ib, int16_t ii, int16_t ie, int16_t wmin, int16_t wmax,
                     int16_t set, int16_t tol, int16_t *imax, int16_t *pmax);
static char r2l(int16_t *ir, int16_t *ie2, int16_t *ie1, int16_t mode);
static char rcut_out(int16_t ib, int16_t ii, int16_t ie, int16_t wmin, int16_t wmax,
                     int16_t set, int16_t tol, int16_t *imax, int16_t *pmax, int16_t mode);
static int16_t inc(struct cut_elm **cutp, int16_t i, int16_t ie, int16_t pass, int16_t x);
static int16_t dec(struct cut_elm **cutp, int16_t i, int16_t ie, int16_t pass, int16_t x);
static char one_cut(int16_t i1, int16_t ib, int16_t ie, int16_t i2);
static char ladjust_cut(int16_t ib, int16_t *ib0, int16_t *il, int16_t ie, int16_t d, int16_t *pmax);
static char radjust_cut(int16_t ib, int16_t *ir, int16_t *ie0, int16_t ie, int16_t d,
                        int16_t *pmax, int16_t mode);
static char adjust_cut(cell *LC, raster *r, struct cut_elm *cut_list,
       seg_vers **vers_list, int16_t ncut, int16_t ib, int16_t *i0, int16_t ie, int16_t d);
static void spec_pairs();
static int16_t right_bound(raster *r, int16_t x1, int16_t x2, int16_t y1, int16_t y2);
static char glue_right_dust();
static good_path(struct cut_elm *cut_list, int16_t ncut);

/*------------------------------------------------------------------
  dp_pass0  ������� ��室 � ���� r �� ������⢥ ncut
            �祪 cut_list; vers_list - १����� ��� �⤥����
            ᥣ���⮢; LC - cell ᫥�� �� ����
------------------------------------------------------------------*/
void dp_pass0(cell *CP, raster *rp, struct cut_elm *cutp,
              seg_vers **versp, int16_t n)
{
  int16_t i,j,x;
  int16_t i1,i2;
  uchar let;
  int16_t cc;
  struct cut_elm *cut;
  SVERS *vers;
  int16_t pass;
//  seg_vers *cur_vers;

  if ((ncut=n)<2)  return;
  LC=CP; r=rp; cut_list=cutp; vers_list=versp;
  iemax=ncut-1;  right_dust=0;  fl_b=0;  connect_=1;
  ie1=iemax;  vers=&cut_list[ie1].versm;
  ie2=ir=(let(vers) || dust(vers)) ? ie1-1 : ie1;
  ib1=ib2=0;      vers=&cut_list[1].versm;
  if (let(vers))  ib2=1;
  else
    if (cut_list->gvarm & c_cg_cutdone)  ib2=on_path(0,ie2,cut_list);
  iec=ib2;  ibc=ie2;
  w1=sym_width-(sym_width>>2);  w2=sym_width+(sym_width>>2);  h2=r->h<<1;
  cut=cut_list+ie1;
/*
//��饯�塞 �ࠢ� dust'�
  if (dust(&cut->versm) && cut->duflm==0)
  {
    right_dust=1;  iemax--;  cut--;
//    if (dust(&cut->versm) && cut->duflm==0)  iemax--;
    ir=ie1=ie2=iemax;
  }
*/
//�ࠩ��� ��訥 �ᯮ����� � ᮯ������騬� dust'���
  if (ie1 != ie2 && cut->duflm)
    addij(LC,r,cut_list,vers_list,ncut,ie2,ie1,0);
  if (ib1 != ib2)
  {
    cut=cut_list+ib2;
    if (cut->duflm)
		addij(LC,r,cut_list,vers_list,ncut,ib1,ib2,0);
    if ((let=cut->versm.vers[0].let)==(uchar)'�' &&
			!is_russian_baltic_conflict(let) ||		// 17.07.2001 E.P.
			let==(uchar)'�'
	   )
      fl_b=1;
  }

  if (ib2>=ie2)
  {
    addij(LC,r,cut_list,vers_list,ncut,ib1,ie1,0);
    goto test_right_dust;
  }

  do
  {
    if (!fl_b && iec != iemax)
    {
#ifndef MY_DEBUG
    if (det_trace)
#endif
      cg_show_rast(LC,r,"left to right",cut_list); //���� - �� �࠭

      cc=l2r(&ib1,&ib2,&il);
      if (cc)  goto finish;

      if (il<iemax && iec>il &&
          (cut=cut_list+il)->dh==0 &&
          (x=cut->x-cut_list->x) < h2 && x > sym_width>>1 &&
          cut->versm.vers[0].prob>=190 &&
          !not_connect_sect(0,il,cut_list))
      {                 //��⠢��, ��� ����, � ���஡㥬 �����
        i1=i2=il;
        cc=l2r(&i1,&i2,&i);
        if (cc)         //����� - ���, ��୥��� � ������⥫쭮�� �����
        {
  if (debug_on)
#ifndef MY_DEBUG
    if (det_trace)
#endif
    {
      sprintf(snap_text,"right side reached, return %d-%d",0,il);
      cg_show_rast(LC,r,snap_text,cut_list); //���� - �� �࠭
    }
          ib1=i1;  ib2=i2;  i1=i2=il;  il=i;
          cc=r2l(&i,&i2,&i1,0);
          goto finish;
        }
      }
    }
    if (ibc != 0 && abs(cut_list[ie2].x-cut_list[ib2].x)>minw)
    {
    //᫥�� �� ��諮, �஡㥬 �ࠢ� ������

  if (debug_on)
#ifndef MY_DEBUG
    if (det_trace)
#endif
    {
      sprintf(snap_text,"left: reliable=%d advance=%d; try right",ib2,il);
      cg_show_rast(LC,r,snap_text,cut_list); //���� - �� �࠭
    }

      cc=r2l(&ir,&ie2,&ie1,2);
      if (cc)  goto finish;

      if (ir>0 && ibc<ir &&
          (cut=cut_list+ir)->dh==0 &&
          (x=cut_list[iemax].x-cut->x) < h2 && x > sym_width>>1 &&
          cut_list[iemax].versm.vers[0].prob>=190 &&
          !not_connect_sect(ir,iemax,cut_list))
      {                 //��⠢��, ��� ����, � ���஡㥬 �����
        i1=i2=ie2;
        cc=r2l(&i,&i2,&i1,2);
        if (cc)         //����� - ���, ��୥��� � ������⥫쭮�� �����
        {
  if (debug_on)
#ifndef MY_DEBUG
    if (det_trace)
#endif
    {
      sprintf(snap_text,"left side reached, return %d-%d",ir,iemax);
      cg_show_rast(LC,r,snap_text,cut_list); //���� - �� �࠭
    }
          ie1=i1;  ie2=i2;  i1=i2=ir;  ir=i;
          cc=l2r(&i1,&i2,&i);
          goto finish;
        }
      }
    }
    if (abs(cut_list[ie2].x-cut_list[ib2].x)<=minw)
    {
      j=on_path(ie2,iemax,cut_list);
      if (j==0)  break;
      if (ib2==0)  addij(LC,r,cut_list,vers_list,ncut,0,j,0);
      else         one_cut(cut_list[ib2].px,ib2,ie2,j);
      goto test_right_dust;
    }

    if (il==ie2 && (iec>il || iec==iemax) &&
        ir==ib2 && (ibc<ir || ibc==0)     &&
        cut_list[ie2].versm.vers[0].prob>MINlet)
      goto finish;
    if (il==ie2 && (iec>il || iec==iemax) || ir==ib2 && (ibc<ir || ibc==0))
      if (cut_list[ie2].versm.vers[0].prob>=190)  goto finish;
    if (il==ir)
    {
      version *vers0;
      char *ilet;
      if (connect_ && cut_list[il].dh==0)
      {
        addij(LC,r,cut_list,vers_list,ncut,ib2,ie2,0);
        if (cut_list[ie2].versm.vers[0].prob>trs2)  goto finish;
      }
      if (ir != ie2 && cut_list[ie2].x-cut_list[ir].x > w2)
      {
        i1=i2=ir;
        if ( l2r(&i1,&i2,&i) || i>0 && on_path(i,iemax,cut_list) &&
             cut_list[i].versm.vers[0].prob>=190)
        {
          j=on_path(ir,iemax,cut_list);
          vers0=&cut_list[j].versm.vers[0];
          if ( (let=vers0->let)==(uchar)'�' &&
			  !is_russian_baltic_conflict(let) && 	// 17.07.2001 E.P.
			  !is_russian_turkish_conflict(let) 	// 21.05.2002 E.P.
			  )
            ie1=j;
          else
            if ( (ilet=strchr(letters_right_to_bad,let))!=0 &&
			    !is_russian_baltic_conflict(let) // 17.07.2001 E.P.
			   )
              if (vers0->prob < prob_right_to_bad[(uchar*)ilet-letters_right_to_bad])
                ie1=j;          //����� ���� ����� �㪢�
          ie2=ir;
        }
      }
      if (il != ib2 && cut_list[il].x-cut_list[ib2].x > w2)
      {
        i2=il;  i1 = (ie2==il) ? ie1 : i2;
        if (r2l(&i,&i2,&i1,0) || on_path(i,ib2,cut_list) &&
            cut_list[i2].versm.vers[0].prob>=190)
        {
          cut=cut_list+il;
          vers0=&cut->versm.vers[0];
          if ((let=vers0->let)==(uchar)'�' &&
			  !is_russian_baltic_conflict(let) && 	// 17.07.2001 E.P.
			  !is_russian_turkish_conflict(let) 	// 21.05.2002 E.P.
			  )
            ib1=cut->px;
          else
            if ( (ilet=strchr(letters_right_to_bad,let)) !=0 &&
			    !is_russian_baltic_conflict(let) // 17.07.2001 E.P.
			   )
              if (vers0->prob < prob_right_to_bad[(uchar*)ilet-letters_right_to_bad])
                ie1=cut->px;          //����� ���� ����� �㪢�
          ib2=il;
        }
      }
      if (ib2==ie2)  goto finish;
    }
//    if (il>ir)  break;
//    if (iec==iemax || ibc==0 || iec<ibc)  break;
    if (iec==iemax && ibc==0)  break;
    connect_ = 1-connect_;
  }
  while (!connect_);

  if (right_dust)
    if (glue_right_dust())
    {
      spec_pairs();  return;
    }

  if (debug_on)
#ifndef MY_DEBUG
    if (det_trace)
#endif
    {
      sprintf(snap_text,"right: reliable=%d advance=%d; ",ie2,ir);
      cg_show_rast(LC,r,snap_text,cut_list); //���� - �� �࠭
    }

  //�� �諨��, ����᪠�� �� ����� ����

  if (ie1-ib1 <= 2)
    if (!good_path(cut_list,ncut))  { ib1=ib2=0; ie2=ie1=ncut-1; }

DP:
  if (ie1-ib1 <= 2)
  {
    spec_pairs();  return;
  }

  il=ib1; ir=ie1;

  for (pass=1; pass<=4; pass++)
  {
    dp_bound(cut_list,vers_list,pass,&il,&ir);
    if (ir<=il)
    {
      if (ib1 != 0 || ie1 != ncut-1)  spec_pairs();   //�������� ��
      return;
    }
#ifndef MY_DEBUG
    if (det_trace)
#endif
    {
      sprintf(snap_text,"try DP %d-%d pass=%d",il,ir,pass);
      cg_show_rast(LC,r,snap_text,cut_list); //���� - �� �࠭
    }
    for ( i=il+1; i<=ir; i++ )
    {
      if ( !ben_cut(cut_list+i) )  //ࠧ�襭 �� ������ ��室�
      {
        snap_newpoint(i);
        for (j=i-1; j>=il; j--)
          if ( !ben_cut(cut_list+j) )
            if (addij(LC,r,cut_list,vers_list,ncut,j,i,0) & 8)  break                                                               ;
      }                                                     //8-�ப                                                               ��
    }
  }  //pass

  if (ib1==0 && ie1==ncut-1)  return;   //������ �� - ����� ��祣� ������

  if (good_path(cut_list,ncut))  { spec_pairs();  return; }

//�� �� �ᥬ� �����

  il=0;  ir=ncut-1;
  for (pass=1; pass<=4; pass++)
  {
    dp_bound(cut_list,vers_list,pass,&il,&ir);
    if (ir<=il)  return;
    if (il>=ib1 && ir<=ie1)  return; //�� �⮬ ���ࢠ�� 㦥 ��⠫�
/*
    {
      struct cut_elm *cute=cut_list+ir;
      for (cut=cut_list+(il+1); cut<cute; cut++)
        if ((cut->var & 0x7F)==40)  break;
      if (cut==cute)  return;  //����� �祪 �� �㤥�
    }
*/
#ifndef MY_DEBUG
    if (det_trace)
#endif
    {
      sprintf(snap_text,"try DP %d-%d pass=%d",il,ir,pass);
      cg_show_rast(LC,r,snap_text,cut_list); //���� - �� �࠭
    }
    for ( i=il+1; i<=ir; i++ )
    {
      if ( !ben_cut(cut_list+i) )  //ࠧ�襭 �� ������ ��室�
      {
        snap_newpoint(i);
        for (j=i-1; j>=il; j--)
          if ( !ben_cut(cut_list+j) )
            if (addij(LC,r,cut_list,vers_list,ncut,j,i,0) & 8)  break                                                               ;
      }                                                     //8-�ப                                                               ��
    }
  }  //pass
  return;

test_right_dust:
  if (right_dust)  glue_right_dust();
  if (!good_path(cut_list,ncut))  { ib1=ib2=0; ie2=ie1=ncut-1; goto DP; }
  spec_pairs();
  return;

finish:
  if (right_dust)  glue_right_dust();
  spec_pairs();
  return;
}

/*---------------------------------------------------------------------
  l2r ����� ��᫥����⥫쭮�� ��१���� �㪢 ᫥�� �� ���ࢠ��
      (*b2,ie2) (��� (*b1,ie2), �᫨ ᫥�� �㪢� �� ᯨ᪠);
      �����稢�����, �᫨ ���� ���ࢠ� ��� ࠧ१���� ��� ��।���
      �࠭�� ᮢ���� � ���設�� ��⨬��쭮�� ��� �ࠢ�;
      ������ ���� ���祭�� ib1, ib2, il;
---------------------------------------------------------------------*/
static char l2r(int16_t *ib1, int16_t *ib2, int16_t *il)
{
  int16_t i,x0;
  uchar let;
  struct cut_elm *cut;
  char cc;
  int16_t ib2p;       //�।���� ib2
  struct cut_elm *cute;//�࠭�� �� �������� �� �吝� ��������⠬
  int16_t pmax;       //������訩 १����
  int16_t set;               //������ ������⢠ �祪 ࠧ१����
  int16_t ib0;
  int16_t wmin,wmax;  //�����⨬�� �ਭ� �㪢�: ���. � ����.

  ib2p=*ib1;  *il=iec=*ib2;  cute=cut_list+iec;
  while (iec < iemax)
  {
    if (connect_)
    {
    //�롨ࠥ� ����������, �������� 㧪�� ��᪨, ����騥 ��������
      x0=cut_list[*ib2].x;
      for (i=iec+1,cut=cute+1; i<=iemax; i++,cut++)
      {
        if (cut->dh != 0)  continue;
        if (cut->rv.v3 > minp)  break;   //�ப�� ��ᢥ�
        if (cut->x-cute->x > sym_width &&
            cute->x-x0 > sym_width)  break;
        iec=i; cute=cut;                 //�������� �� �����
      }
      if (i>iemax)  { i--; cut--; }
      if (cut->x-cute->x <= sym_width || cute->x-x0 <= sym_width)
        { iec=i; cute=cut; }
    }
    else
      iec=iemax;

    //०��
    *il=*ib2;
    while (*il<iec)
    {
      version *vers0;
      char *ilet;
      int16_t ib1p=*ib1;
      if (*ib2<ie2 && ie2<=iec && cut_list[ie2].x-cut_list[*ib2].x <= w2)
      {
        ib0=*ib2;
        if ((addij(LC,r,cut_list,vers_list,ncut,ib0,ie2,0) & (2+4+8)) == 0)
          if (cut_list[ie2].versm.vers[0].prob>trs2)
            { *ib1=*il=ie2;  goto accept; }
      }
      pmax=-1;
      wmin=w1;  wmax=w2;
      for (set=1; set<=3; set++)
      {
        ib0=*ib2;                                   //�஡㥬 ᫥������
        cc=lcut_out(ib0,*ib2,iec,wmin,wmax,set,trs2,il,&pmax);
        if (cc)  { *ib1=*il; goto accept; }
      }
      wmin=minw; wmax=h2;
      for (set=1; set<=4; set++)
      {
        ib0=*ib2;
        cc=lcut_out(ib0,*ib2,iec,wmin,wmax,set,trs2,il,&pmax);
                                                    //�஡㥬 ᫥������
        if (cc)  { *ib1=*il; goto accept; }
        if (*ib1 != *ib2)
        {             //᫥�� �㪢� ����� ���� ����� ����� �ப��
          ib0=*ib1;
          cc=lcut_out(ib0,*ib2,iec,minw,h2,set,trs2,il,&pmax);
                                                    //����塞 �।�����
          if (cc) { *ib1=*ib2; goto accept; }
        }
      }
      if (*ib2>ib2p)
      {
        ib0=*ib2;
        cc=ladjust_cut(ib2p,&ib0,il,iec,minw,&pmax);
        if (cc) { *ib1=*il; goto accept; }
      }
      return 0;

accept:
      *ib2=*il; ib2p=ib0; let=0;
      vers0=&cut_list[*il].versm.vers[0];  let=vers0->let;
      if (let=='-')
        *ib1=ib1p;
      else
        if ( (let==(uchar)'�' || let==(uchar)'�' || let==(uchar)'�' ||
              strchr(sticks_left_to_bad,let)
			 ) &&
			 !is_russian_baltic_conflict(let) &&// 17.07.2001 E.P.
			 !is_russian_turkish_conflict(let) 	// 21.05.2002 E.P.
		   )
          *ib1=ib0;
        else
          if ( (ilet=strchr(letters_left_to_bad,let))!=0 &&
			  !is_russian_baltic_conflict(let)	// 17.07.2001 E.P.
			  )
            if (vers0->prob < prob_left_to_bad[(uchar*)ilet-letters_left_to_bad])
              *ib1=ib0;            //����� ���� ����� �㪢�

      if (debug_on)
#ifndef MY_DEBUG
        if (det_trace)
#endif
        {
          sprintf(snap_text,"keep: (%d,%d) %c",ib0,*il,let);
          show_and_wait(snap_text);
        }
      if (*ib2>=ie2)
        if (*ib2==ie2 || on_path(*ib2,iemax,cut_list))
		  return 1;
      if ( (let==(uchar)'�' || let==(uchar)'�') &&
		  !is_russian_baltic_conflict(let) 	// 17.07.2001 E.P.
		 )
      {
        fl_b=1; return 0;
      }
    }
  }
  return 1;
}

/*------------------------------------------------------------------
  lcut_out  ��१��� ���� �㪢� �� ᥣ���� (ib,ie) ᫥�� (��祬
            �� ᥣ���� (ib,ii) ): �����  ���������� ���� trs2,
            � �᫨ �� �㦥 - ���������; set ��।���� ������⢮
            �����⨬�� �祪 (�ᥣ� ncut), �᫨ set<0 - ���� ��
            �ᥬ �窠�;
            �����頥� 1, �᫨ ���� tol, � 0, �᫨ �㦥;
            imax � pmax - ��������� �ࠢ�� �࠭�� �㪢� � ��
            ����⭮���
------------------------------------------------------------------*/
static char lcut_out(int16_t ib, int16_t ii, int16_t ie, int16_t wmin, int16_t wmax,
                     int16_t set, int16_t tol, int16_t *imax, int16_t *pmax)
{
  struct cut_elm  *cut; int16_t i;
  struct cut_elm *cute=cut_list+ie;
  int16_t xb=cut_list[ib].x;
  int16_t x0=xb+sym_width;      //��砫쭮� �祭��
  int16_t xe=xb+wmax;           //�ࠢ�� �࠭��
  struct cut_elm  *cutl,*cutr; int16_t il,ir;   //�祭�� ᫥�� � �ࠢ� �� x0
  uchar p;
  int16_t sete=set;
  uchar cc;
  seg_vers *cur_vers;

  xb += wmin;
  if (set<0)  { set=1; sete=4; }
  for ( ; set<=sete; set++)
  {
  //���� ࠧ१ �� ����ﭨ� ����� sym_width, ����� ᬥ頥��� � ��� ��஭�
    i=ii+1; cut=cut_list+i; il=ir=-128;
    while (cut<=cute && cut->x <= xe)
    {
      char var=cut->var & 0x7F;
      if (cut->x > xb && in_set(var,set))
        if(cut->x <= x0)  { il=i;  cutl=cut; }
        else              { ir=i;  cutr=cut; break; }
      cut++; i++;
    }

    while (il>=0 || ir>=0)
    {
      if (il<0)    goto right;
      else
        if (ir<0)  goto left;
        else
          if (cutr->x-x0 < x0-cutl->x)  goto right;
left:
      cc=addij(LC,r,cut_list,vers_list,ncut,ib,il,0);
      if (cc & 4)                     //㧪��
        il=-128;
      else
      {
        if ((cc & (2+4+8)) == 0)
          if (cur_vers=find_vers(ib,il,vers_list))
          {
            p=0;
            if (let_or_bad(&cur_vers->vers))
            {
              p=cur_vers->vers.vers[0].prob;
              if (p>*pmax) { *pmax=p; *imax=il; }
              if (p>tol)  return 1;
            }
            else
              if (cur_vers->vers.vers[0].let=='-')
              {
                *imax=il; return 1;
              }
          }
        il=dec(&cutl,il,ii,set,xb);
        if (il==ii) il=-128;
      }
      continue;
right:
      cc=addij(LC,r,cut_list,vers_list,ncut,ib,ir,0);
      if (cc & 8)                     //�ப��
        ir=-128;
      else
      {
        if ((cc & (2+4+8)) == 0)
          if (cur_vers=find_vers(ib,ir,vers_list))
          {
            p=0;
            if (let_or_bad(&cur_vers->vers))
            {
              p=cur_vers->vers.vers[0].prob;
              if (p>*pmax) { *pmax=p; *imax=ir; }
              if (p>tol)  return 1;
            }
            else
              if (cur_vers->vers.vers[0].let=='-')
              {
                *imax=ir; return 1;
              }
          }
        ir=inc(&cutr,ir,ie,set,xe);
      }
    }
  }
  return 0;
}

/*---------------------------------------------------------------------
  r2l ����� ��᫥����⥫쭮�� ��१���� �㪢 �ࠢ� �� ���ࢠ��
      (ib2,ie2) (��� (ib1,ie2), �᫨ �ࠢ� �㪢� �� ᯨ᪠);
      �����稢�����, �᫨ ���� ���ࢠ� ��� ࠧ१���� ��� ��।���
      �࠭�� ᮢ���� � ���設�� ��⨬��쭮�� ��� ᫥��;
      ������ ���� ���祭�� ie1, ie2, ir;
---------------------------------------------------------------------*/
static char r2l(int16_t *ir, int16_t *ie2, int16_t *ie1, int16_t mode)
{
  int16_t i,x0;
  uchar let;
  seg_vers *cur_vers;
  struct cut_elm *cut;
  char cc;
  int16_t ie2p;   //�।���� ie2
  struct cut_elm *cutb;  //�࠭�� �� �������� �� �吝� ��������⠬
  int16_t pmax;       //������訩 १����
  int16_t set;        //������ ������⢠ �祪 ࠧ१����
  int16_t ie0;
  int16_t wmin,wmax;  //�����⨬�� �ਭ� �㪢�: ���. � ����.

  ie2p=*ie1;  *ir=ibc=*ie2;  cutb=cut_list+ibc;
  while (ibc > 0)
  {
    if (connect_)
    {
    //�롨ࠥ� ����������, �������� 㧪�� ��᪨, ����騥 ��������
      x0=cut_list[*ie2].x;
      for (i=ibc-1,cut=cutb-1; i>=0; i--,cut--)
      {
        if (cut->dh != 0)  continue;
        if (cut->rv.v3 > minp)  break;   //�ப�� ��ᢥ�
        if (cutb->x-cut->x > sym_width &&
            x0-cutb->x > sym_width)  break;
        ibc=i; cutb=cut;                 //�������� �� �����
      }
      if (i<0)  { i++; cut++; }
      if (cutb->x-cut->x <= sym_width || x0-cutb->x <= sym_width)
        { ibc=i;  cutb=cut; }
    }
    else
      ibc=0;

    //०��
    *ir=*ie2;
    while (*ir > ibc)
    {
      version *vers0;
      char *ilet;
      int16_t ie1p=*ie1;
      if (ib2<*ie2 && ibc<=ib2 && cut_list[*ie2].x-cut_list[ib2].x <= w2)
      {
        ie0=*ie2;
        if ((addij(LC,r,cut_list,vers_list,ncut,ib2,ie0,0) & (2+4+8)) == 0)
          if (cut_list[*ie2].versm.vers[0].prob>trs2)
            { *ie1=*ir=ib2;  goto accept; }
      }
      pmax=-1;
      wmin=w1;  wmax=w2;
      for (set=1; set<=3; set++)
      {
        ie0=*ie2;                                   //�஡㥬 ᫥������
        cc=rcut_out(ibc,ie0,ie0,wmin,wmax,set,trs2,ir,&pmax,mode);
        if (cc)  { *ie1=*ir; goto accept; }
      }
      wmin=minw; wmax=h2;
      for (set=1; set<=4; set++)
      {
        ie0=*ie2;
        cc=rcut_out(ibc,ie0,ie0,wmin,wmax,set,trs2,ir,&pmax,mode);
                                                    //�஡㥬 ᫥������
        if (cc)  { *ie1=*ir; goto accept; }
        if (*ie1 != *ie2)
        {                 //�ࠢ� �㪢� ����� ���� ����� ����� �ப��
          ie0=*ie1;
          cc=rcut_out(ibc,*ie2,ie0,minw,h2,set,trs2,ir,&pmax,mode);
                                                  //����塞 �।�����
          if (cc) { *ie1=*ie2; goto accept; }
        }
      }
      if (*ie2<ie2p)
      {
        ie0=*ie2;
        cc=radjust_cut(ibc,ir,&ie0,ie2p,minw,&pmax,mode);
        if (cc) { *ie1=*ir; goto accept; }
      }
      return 0;

accept:
      *ie2=*ir; ie2p=ie0; let=0;
      if (cur_vers=find_vers(*ir,ie0,vers_list))
      {
        vers0=&cur_vers->vers.vers[0];  let=vers0->let;
        if (let=='-')
          *ie1=ie1p;
        else
          if ( (let==(uchar)'�' || strchr(sticks_right_to_bad,let) ) &&
			   !is_russian_baltic_conflict(let) &&	// 17.07.2001 E.P.
			   !is_russian_turkish_conflict(let) 	// 21.05.2002 E.P.
			 )
            *ie1=ie0;
          else
            if ( (ilet=strchr(letters_right_to_bad,let))!=0 &&
			    !is_russian_baltic_conflict(let) // 17.07.2001 E.P.
			   )
              if (vers0->prob < prob_right_to_bad[(uchar*)ilet-letters_right_to_bad])
                *ie1=ie0;          //����� ���� ����� �㪢�
      }

      if (debug_on)
#ifndef MY_DEBUG
        if (det_trace)
#endif
        {
          sprintf(snap_text,"keep: (%d,%d) %c",*ir,ie0,let);
          show_and_wait(snap_text);
        }
      if (ib2>=*ie2)
        if (ib2==*ie2 || on_path(*ie2,ib2,cut_list))  return 1;
    }
  }
  return 1;
}

/*------------------------------------------------------------------
  rcut_out  ��१��� ���� �㪢� �� ᥣ���� (ib,ie) �ࠢ� (��祬
            �� ᥣ���� (ii,ie) ): ����� ���������� ���� trs2,
            � �᫨ �� �㦥 - ���������;
            set ��।���� ������⢮ �����⨬�� �祪 (�ᥣ� ncut),
            �᫨ set<0 - ���� �� �ᥬ �窠�;
            �����頥� 1, �᫨ ���� tol, � 0, �᫨ �㦥;
            imax � pmax - ��������� �ࠢ�� �࠭�� �㪢� � ��
            ����⭮���
------------------------------------------------------------------*/
static char rcut_out(int16_t ib, int16_t ii, int16_t ie, int16_t wmin, int16_t wmax,
                     int16_t set, int16_t tol, int16_t *imax, int16_t *pmax, int16_t mode)
{
  struct cut_elm  *cut; int16_t i;
  struct cut_elm *cutb=cut_list+ib;
  int16_t xe=cut_list[ie].x;
  int16_t x0=xe-sym_width;      //��砫쭮� �祭��
  int16_t xb=xe-wmax;           //����� �࠭��
  struct cut_elm  *cutl,*cutr; int16_t il,ir;   //�祭�� ᫥�� � �ࠢ� �� x0
  uchar p;
  int16_t sete=set;
  uchar cc;
  seg_vers *cur_vers;

  xe -= wmin;
  if (set<0)  { set=1; sete=4; }
  for ( ; set<=sete; set++)
  {
  //���� ࠧ१ �� ����ﭨ� ����� sym_width, ����� ᬥ頥��� � ��� ��஭�
    i=ii-1; cut=cut_list+i; il=ir=-128;
    while (cut>=cutb && cut->x >= xb)
    {
      char var=cut->var & 0x7F;
      if (cut->x < xe && in_set(var,set))
        if(cut->x >= x0)  { ir=i;  cutr=cut; }
        else              { il=i;  cutl=cut; break; }
      cut--; i--;
    }

    while (il>=0 || ir>=0)
    {
      if (il<0)    goto right;
      else
        if (ir<0)  goto left;
        else
          if (cutr->x-x0 < x0-cutl->x)  goto right;
left:
      cc=addij(LC,r,cut_list,vers_list,ncut,il,ie,(char)mode);
      if (cc & 8)                     //�ப��
        il=-128;
      else
      {
        if ((cc & (2+4+8)) == 0)
          if (cur_vers=find_vers(il,ie,vers_list))
          {
            p=0;
            if (let_or_bad(&cur_vers->vers))
            {
              p=cur_vers->vers.vers[0].prob;
              if (p>*pmax) { *pmax=p; *imax=il; }
              if (p>tol)  return 1;
            }
            else
              if (cur_vers->vers.vers[0].let=='-')
              {
                *imax=il; return 1;
              }
          }
        il=dec(&cutl,il,ib,set,xb);
      }
      continue;
right:
      cc=addij(LC,r,cut_list,vers_list,ncut,ir,ie,(char)mode);
      if (cc & 4)                     //㧪��
        ir=-128;
      else
      {
        if ((cc & (2+4+8)) == 0)
          if (cur_vers=find_vers(ir,ie,vers_list))
          {
            p=0;
            if (let_or_bad(&cur_vers->vers))
            {
              p=cur_vers->vers.vers[0].prob;
              if (p>*pmax) { *pmax=p; *imax=ir; }
              if (p>tol)  return 1;
            }
            else
              if (cur_vers->vers.vers[0].let=='-')
              {
                *imax=ir; return 1;
              }
          }
        ir=inc(&cutr,ir,ii,set,xe);
        if (ir==ii) ir=-128;
      }
    }
  }
  return 0;
}
/*-------------------------------------------------------------------
  inc �����頥� ����� �����⨬�� �� ������⢥ set �窨 ����� i-��
      � ie-�� � �� �ࠢ�� x
-------------------------------------------------------------------*/
static int16_t inc(struct cut_elm **cutp, int16_t i, int16_t ie, int16_t set, int16_t x)
{
  if (i>=ie) return -128;
  (*cutp)++; i++;
  while ((*cutp)->x < x)
  {
    char var=(*cutp)->var & 0x7F;
    if (i==ie || in_set(var,set))  return i;
    (*cutp)++; i++;
  }
  return -128;
}

/*-------------------------------------------------------------------
  dec �����頥� ����� �����⨬�� �� ������⢥ set �窨 ����� ie-��
      � i-��  � �� ����� x
-------------------------------------------------------------------*/
static int16_t dec(struct cut_elm **cutp, int16_t i, int16_t ie, int16_t set, int16_t x)
{
  if (i<=ie) return -128;
  (*cutp)--; i--;
  while ((*cutp)->x > x)
  {
    char var=(*cutp)->var & 0x7F;
    if (i==ie || in_set(var,set))  return i;
    (*cutp)--; i--;
  }
  return -128;
}

/*-------------------------------------------------------------------
  one_cut  ��室�� ࠧ१ ᥣ���� (i1,i2) � �।���� (ib,ie), �⮡�
           ��� �� ���� ��᮪ �� ��訩;
           �����頥� 1, �᫨ ࠧ१���� �� ��� ��訥 ���, ���� - 0
-------------------------------------------------------------------*/
static char one_cut(int16_t i1, int16_t ib, int16_t ie, int16_t i2)
{
  struct cut_elm  *cut; int16_t i;
  struct cut_elm *cutb=cut_list+ib;
  struct cut_elm *cute=cut_list+ie;
  int16_t xb=cutb->x;
  int16_t xe=cute->x;
  int16_t x0=(cutb->x+cute->x)>>1;  //��砫쭮� �祭��
  struct cut_elm  *cutl,*cutr; int16_t il,ir;   //�祭�� ᫥�� � �ࠢ� �� x0
  char fll,flr;
  SVERS *vers;
  int16_t set;

  if (debug_on)
#ifndef MY_DEBUG
    if (det_trace)
#endif
    {
      sprintf(snap_text,"one cut: %d-%d",ib,ie);
      cg_show_rast(LC,r,snap_text,cut_list); //���� - �� �࠭
    }

  if (cut_list[i2].x-cut_list[i1].x<=sym_width)  return 0;
  xb=x0-(sym_width>>2); xe=x0+(sym_width>>2);
  for (set=1; set<=4; set++)
  {
  //���� ࠧ१ �ਬ�୮ �� �।���, ����� ᬥ頥��� � ��� ��஭�
    i=ib+1; cut=cutb+1; il=ir=-128;
    while (cut<=cute && cut->x < xe)
    {
      char var=cut->var & 0x7F;
      if (cut->x > xb && in_set(var,set))
        if(cut->x <= x0)  { il=i;  cutl=cut; }
        else              { ir=i;  cutr=cut; break; }
      cut++; i++;
    }

    while (il>=0 || ir>=0)
    {
      if (il<0)    goto right;
      else
        if (ir<0)  goto left;
        else
          if (cutr->x-x0 < x0-cutl->x)  goto right;
left:
      i=il; cut=cutl; il=dec(&cutl,il,ib,set,xb);
      if (il==i1) il=-128;  goto rec;
right:
      i=ir; cut=cutr; ir=inc(&cutr,ir,ie,set,xe);
      if (ir==i2) ir=-128;
rec:
      fll=flr=0;
      addij(LC,r,cut_list,vers_list,ncut,i1,i,0);
      addij(LC,r,cut_list,vers_list,ncut,i,i2,0);
      if (let_or_bad(vers=&cut->versm))   fll=vers->vers[0].prob>trs2;
      if (let_or_bad(vers=&cute->versm))  flr=vers->vers[0].prob>trs2;
      if (fll && flr)  return 1;
      if (fll || flr)  return 0;
    }
    xb=cutb->x; xe=cute->x;
  }
  return 0;
}

/*-------------------------------------------------------------------
  ladjust_cut  ����� ��������� ࠧ१� ib0 � �।���� +-d ���ᥫ��;
               �����頥� 1, �᫨ १���� ���襭, ���� - 0;
               ib,ie - �।���� �࠭��� ᫥�� � �ࠢ�
               il � pmax - �������� �ࠢ�� �࠭�� � ����⭮���
-------------------------------------------------------------------*/
static char ladjust_cut(int16_t ib, int16_t *ib0, int16_t *il, int16_t ie, int16_t d, int16_t *pmax)
{
  int16_t i0=*ib0;
  int16_t i=i0; struct cut_elm  *cut=cut_list+i; //⥪��� �।���
  int16_t iln;                                   //⥪�騩 �ࠢ� �ࠩ
  int16_t xb=cut->x-d, xe=cut->x+d;              //�࠭��� ���᪠
  int16_t pl,pr;    //����⭮�� ���ᨩ ᫥�� � �ࠢ� �� i
  uchar good;    //�������� ����⭮��� ᫥��
  int16_t dir=((cut+1)->x-cut->x < cut->x-(cut-1)->x) ? 1 : -1;
                                              // ���ࠢ����� ᬥ饭��
  int16_t set;
  SVERS *vers;
  int16_t idir;
  int16_t wmin,wmax;  //�����⨬�� �ਭ� �㪢�: ���. � ����.
  char cc;

  if (i==ib || i==ie || cut->dh==0)  return 0;

  if (debug_on)
#ifndef MY_DEBUG
    if (det_trace)
#endif
    {
      sprintf(snap_text,"adjust cut: %d-%d-%d",ib,i,*il);
      cg_show_rast(LC,r,snap_text,cut_list); //���� - �� �࠭
    }

  good=(let_or_bad(vers=&cut_list[i].versm)) ? vers->vers[0].prob : 0;

  //ᬥ頥��� � ���� ��஭�, ���� pmin ���蠥���
  for (idir=0; idir<2; idir++)
  {
    i=i0+dir;  cut=cut_list+i;
    while ( (dir>0) ? cut->x <= xe && i<=ie : cut->x >= xb && i>ib )
    {
      addij(LC,r,cut_list,vers_list,ncut,ib,i,0);
      pl=(let_or_bad(vers=&cut->versm)) ? vers->vers[0].prob : 0;
      if (pl<=trs2)  break;   //���� �ᯮ�⨫���
      if (i==ie)  { *ib0=*il=i; *pmax=pl; return 1; }
      wmin=w1;  wmax=w2;  pr=-1;  iln=i;
      for (set=1; set<=3; set++)
      {
        cc=lcut_out(i,i,ie,wmin,wmax,set,trs2,&iln,&pr);
                                             //�� ����� �࠭��� ��ࠢ�
        if (cc) { *ib0=i; *il=iln; *pmax=pr; return 1; }
      }
      wmin=minw; wmax=h2;
      for (set=1; set<=4; set++)
      {
        cc=lcut_out(i,i,ie,wmin,wmax,set,trs2,&iln,&pr);
                                             //�� ����� �࠭��� ��ࠢ�
        if (cc) { *ib0=i; *il=iln; *pmax=pr; return 1; }
      }
      if (pl<=good) break;    //���� �� �⠫�
      good=(uchar)pl; *ib0=i; *il=iln; *pmax=pr;
      cut += dir;  i += dir;
    }
    dir= -dir;
  }
  return 0;
}

/*-------------------------------------------------------------------
  radjust_cut  ����� ��������� ࠧ१� ie0 � �।���� +-d ���ᥫ��;
               �����頥� 1, �᫨ १���� ���襭, ���� - 0;
               ib,ie - �।���� �࠭��� ᫥�� � �ࠢ�
               ir � pmax - �������� ����� �࠭�� � ����⭮���
-------------------------------------------------------------------*/
static char radjust_cut(int16_t ib, int16_t *ir, int16_t *ie0, int16_t ie, int16_t d,
                        int16_t *pmax, int16_t mode)
{
  int16_t i0=*ie0;
  int16_t i=i0; struct cut_elm  *cut=cut_list+i; //⥪��� �।���
  int16_t irn;                                   //⥪�騩 ���� �ࠩ
  int16_t xb=cut->x-d, xe=cut->x+d;              //�࠭��� ���᪠
  int16_t pl,pr;   //����⭮�� ���ᨩ ᫥�� � �ࠢ� �� i
  uchar good;   //�������� ����⭮��� �ࠢ�
  int16_t dir=((cut+1)->x-cut->x < cut->x-(cut-1)->x) ? 1 : -1;
                                              // ���ࠢ����� ᬥ饭��
  int16_t set;
  seg_vers *cur_vers;
  SVERS *vers;
  int16_t idir;
  int16_t wmin,wmax;  //�����⨬�� �ਭ� �㪢�: ���. � ����.
  char cc;

  if (i==ib || i==ie || cut->dh==0)  return 0;

  if (debug_on)
#ifndef MY_DEBUG
    if (det_trace)
#endif
    {
      sprintf(snap_text,"adjust cut: %d-%d-%d",*ir,i,ie);
      cg_show_rast(LC,r,snap_text,cut_list); //���� - �� �࠭
    }

  good=0;
  if (cur_vers=find_vers(i,ie,vers_list))
    if(let_or_bad(vers=&cur_vers->vers))  good=vers->vers[0].prob;

  //ᬥ頥��� � ���� ��஭�, ���� pmin ���蠥���
  for (idir=0; idir<2; idir++)
  {
    i=i0+dir;  cut=cut_list+i;
    while ( (dir>0) ? cut->x <= xe && i<ie : cut->x >= xb && i>=ib )
    {
      addij(LC,r,cut_list,vers_list,ncut,i,ie,(char)mode);
      pr=0;
      if (cur_vers=find_vers(i,ie,vers_list))
        if (let_or_bad(vers=&cur_vers->vers)) pr=vers->vers[0].prob;
      if (pr<=trs2)  break;   //���� �ᯮ�⨫���
      if (i==ib)  { *ie0=*ir=i; *pmax=pr; return 1; }
      wmin=w1;  wmax=w2;  pl=-1;  irn=i;
      for (set=1; set<=3; set++)
      {
        cc=rcut_out(ib,i,i,wmin,wmax,set,trs2,&irn,&pl,mode);
                              //�� ����� �࠭��� �����
        if (cc) { *ie0=i; *ir=irn; *pmax=pl; return 1; }
      }
      wmin=minw; wmax=h2;
      for (set=1; set<=4; set++)
      {
        cc=rcut_out(ib,i,i,wmin,wmax,set,trs2,&irn,&pl,mode);
                              //�� ����� �࠭��� �����
        if (cc) { *ie0=i; *ir=irn; *pmax=pl; return 1; }
      }
      if (pr<=good) break;    //���� �� �⠫�
      good=(uchar)pr; *ie0=i; *ir=irn; *pmax=pl;
      cut += dir;  i += dir;
    }
    dir= -dir;
  }
  return 0;
}

/*---------------------------------------------------------------
  spec_pairs "��", "�" � ��.
----------------------------------------------------------------*/
static void spec_pairs()
{
  struct cut_elm *cut;
  int16_t i,ip; //⥪�饥 � �।��饥 �祭��
  SVERS *vers;                   //⥪��� �����
  uchar let,letp;                 //⥪��� � �।���� �㪢�
  struct cut_elm *cuts;
  int16_t is,x0;  //�ࠢ�� �࠭�� "�"
  int16_t m_row;                     //�।��� ������� �����

  det_snap(LC, "special pairs");

  i=ncut-1;
  letp=0;
  ip=i;
  while (i>0)
    {
     cut=cut_list+i;
	 vers=&cut->versm;
	 let=vers->vers[0].let;
     if (dust(vers))
       {
        cuts=cut_list+cut->px;  //���� �ࠩ dust'�
        if (cuts->dh != 0 &&      //dust ��१�� ᫥��
            addij(LC,r,cut_list,vers_list,ncut,cuts->px,i,0)==0 &&
            cut->versm.vers[0].prob > cuts->versm.vers[0].prob
           ) ;                     //�ਪ�����
          else
            if (cut->dh != 0)       //dust ��१�� �ࠢ�
              addij(LC,r,cut_list,vers_list,ncut,i,ip,0);
       }
      else
        if ( cut->dh != 0 && strchr("����",let) &&
		    !is_russian_baltic_conflict(let) && // 17.07.2001 E.P.
			!is_russian_turkish_conflict(let)&&	// 21.05.2002 E.P.
		     (letp==(uchar)'�' || letp==(uchar)'�')
		   )
          {
           int16_t  j,il=cut->px;
           int16_t x0=cut->x-((cut_list[ip].x-cut->x)>>1);
           uchar *letr=&cut_list[ip].versm.vers[0].let;
           for (j=i-1,cuts=cut-1; j>il && cuts->x >= x0; j--,cuts--)
             {
              if (addij(LC,r,cut_list,vers_list,ncut,il,j,0)>1)
				 break;
//              if (cut_list[j].versm.vers[0].prob<trs2)
//			       break;
              if (addij(LC,r,cut_list,vers_list,ncut,j,ip,0)>1)
				 continue;
              if ( (*letr==(uchar)'�' || *letr==(uchar)'�')  &&
					!is_russian_baltic_conflict(*letr) && // 17.07.2001 E.P.
					!is_russian_turkish_conflict(*letr)	// 21.05.2002 E.P.
				 )
				 break;
             }
          }
         else
            {
             if (cut->dh != 0 &&
				 strchr("����������",let) &&			// "�����������"
			     !is_russian_baltic_conflict(let) &&	// 17.07.2001 E.P.
				 !is_russian_turkish_conflict(let) &&	// 21.05.2002 E.P.
				 (letp==(uchar)'�' || letp==(uchar)'�'))	// ��
               {
                m_row=my_bases.bm+(int16_t)((int32_t)nIncline*(r->left+cut->x)/2048);
                x0=right_bound(r,0,cut->x,(int16_t)(m_row-r->top+1),(int16_t)(r->h-1));
               }
              else
                if (cut->dh != 0 && strchr("��",let) &&
					!is_russian_baltic_conflict(let) && // 17.07.2001 E.P.
				    (letp==(uchar)'�' || letp==(uchar)'�'))
                  {
                   m_row=my_bases.bm+(int16_t)((int32_t)nIncline*(r->left+cut->x)/2048);
                   x0=right_bound(r,0,cut->x,0,(int16_t)(m_row-r->top));
                  }
                 else
                   if (cut->dh != 0 && (letp==(uchar)'�' || letp==(uchar)'�') &&
					   !is_russian_baltic_conflict(letp) // 17.07.2001 E.P.
					  )
                     {
                      x0=cut->x-((cut_list[ip].x-cut->x)>>2);
                     }
                    else
                      if ((let==(uchar)'�' || let==(uchar)'�') &&
					      strchr(letters_right_to_bad,letp) &&
			              letp != (uchar)'�' && letp != (uchar)'�' &&
						  !is_russian_baltic_conflict(let)	&&	// 17.07.2001 E.P.
						  !is_russian_baltic_conflict(letp)		// 17.07.2001 E.P.
						 )
                        x0=(cut_list[cut->px].x + (cut->x<<1))/3;
                       else
                         if (let=='|' && strchr(letters_right_to_bad,letp) &&
							 !is_russian_baltic_conflict(letp) && // 17.07.2001 E.P.
                             letp != (uchar)'�' && letp != (uchar)'�')
                           {
                            i=cut->px;
                            if (i==0)
							  break;
                            cut=cut_list+i;
							x0=cut->x;
                           }
                          else
                            x0=0;
             if (x0 != 0)
               {
                //�饬 ������訩 � x0 ࠧ१
                is=i;
		        cuts=cut;
                while (cuts->x > x0)
		          {
		           is--;
		           cuts--;
		          };
                if ( (cuts+1)->x-x0 < x0-cuts->x )
			      is++;
                //�ᯮ�����
                addij(LC,r,cut_list,vers_list,ncut,cut->px,is,0);
                addij(LC,r,cut_list,vers_list,ncut,is,ip,0);
               }
            }
     letp=let; ip=i;
     i=cut->px;
    }
}

/*--------------------------------------------------------------
  right_bound  ��室�� ���न���� x �ࠢ�� �࠭��� ����ࠦ����
               � ������ (x1,y1) (x2,y2) ���� r; ���� ���孨�
               㣮� ���� - (0,0)
--------------------------------------------------------------*/
static int16_t right_bound(raster *r, int16_t x1, int16_t x2, int16_t y1, int16_t y2)
{
  int16_t i;
  int16_t wb=(r->w+7)/8;    //�ਭ� ���� � �����;
  int16_t h1=y2-y1+1;       //���� ������
  uchar *b0=r->pict + wb*y1;  //��砫� ��ਧ��⠫�
  uchar *b1=b0+x1/8;          //���孨� ����
  uchar *b2=b0+x2/8;          // � �ࠢ� ����� ������
  char p;               //�஥��� �⮫����
  uchar *bx=b2;          //���孨� ���� �⮫����
  uchar *ib;             //⥪�騩  -"-
  uchar m1=0xff>>(x1%8); //�������騥 ���� � �஥�樨
  uchar m2=0xff<<(7-x2%8);

  do
  {
    for (p=0,i=0,ib=bx; i<h1; i++,ib += wb)  p |= *ib;  //�஥���
    if (bx==b2) p &= m2;
    if (bx==b1) p &= m1;
    i=0;
    while (p<0)  { p = p<<1;  i++; }        //�᫮ ������ ᫥��
    if (i>0)  return ((bx-b0)<<3)+i-1;
    bx--;                                   //᫥���騩 ���� ᫥��
  }
  while (bx>=b1);
  return 0;
}

/*--------------------------------------------------------------
  glue_right_dust ��ᮥ����� �ࠢ� dust'�, �᫨ �㦭�
--------------------------------------------------------------*/
static char glue_right_dust()
{
  SVERS *vers;
  if (let_or_bad(vers=&cut_list[iemax].versm))
    if (vers->vers[0].prob>trs2)  return 1;
  ie1=ie2=ncut-1;
  addij(LC,r,cut_list,vers_list,ncut,ib2,ie1,0);
  if (let_or_bad(vers=&cut_list[ie1].versm))
    if (vers->vers[0].prob>trs2)  return 1;
  addij(LC,r,cut_list,vers_list,ncut,il,ie1,0);
  if (let_or_bad(vers=&cut_list[ie1].versm))
    if (vers->vers[0].prob>trs2)  return 1;
  return 0;
}

/*-------------------------------------------------------------
  good_path �����頥� 1, �᫨ ���� � ᯨ᪥ ncut �祪 �� ᮤ�ন�
            ��१����� dust'�� � �।��� � �� ����⭮�� >190
-------------------------------------------------------------*/
static good_path(struct cut_elm *cut_list, int16_t ncut)
{
  int16_t ncut1=ncut-1;
  int16_t i=ncut1;  struct cut_elm *cut=cut_list+i;
  int16_t in;  struct cut_elm *cutn;
  SVERS *vers;

  while (i>0)
  {
    in=cut->px;  cutn=cut_list+in; vers=&cut->versm;
    if (dust(vers))
    {
      if (i<ncut1 && in>0 && (cut->dh != 0 || cutn->dh != 0))  return 0;
    }
    else
      if (vers->vers[0].prob<190)  return 0;
    i=in; cut=cutn;
  }
  return 1;
}
