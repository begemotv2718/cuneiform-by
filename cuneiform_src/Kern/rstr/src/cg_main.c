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

//������ ��楤��� ࠧ१����-᪫�������
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "struct.h"
#include "cuthdr.h"
#include "dmconst.h"
#include "func.h"
#include "ligas.h"
#include "lang.h"
#include "linutil.h"
#include "status.h"
#include "p2libr.h"
//#include "Alik_rec.h"
#include "cut_glue.h"
#include "dpuma.h"

#include "compat_defs.h"

extern uchar langUkr , langSer;
//#define  PROTOCOL

#define  STRRCHR  strrchr
#define  STRSPN   strspn

#define  MAX_GATE  RASTER_WIDTH  //����. ��ᢥ� ����� ᨬ����
#define  MAX_WIDTH RASTER_WIDTH  //����. �ਭ� ᨬ����

#define  CUT_PEN   15  //���� �� ࠧ१
#define  PASS_PEN  45  //���� �� �ய�饭�� dust
#define  CUT_rlI   30  //���� ��१���� "rlI"
#define  PEN_TOP_DUST    15      //���� ��� �,�,�,�,�, �᫨ ᢥ��� ���� dust
#define  RO_DUST_OFF     80      //����. �訡�� ��� �ᯮ�������� ��� dust'��
#define  RO_COMPOSE     210      //���ᨬ��쭠� �訡�� ��� ᮡ࠭��� �㪢�
#define  RO_BAD  MAX_RO-MINlet   //���ᨬ��쭠� �訡�� � ���宩 �㪢�
#define  RELIABLE_A     190      //�������� "�" (�� ����� ���� �����
#define  RELIABLE_9     150      //�������� "9"  ��㣮�� ᨬ����)
#define  I_wo_point     150      //����⭮��� ��� "i" ��� �窨
#define  G_to_T   100  //�� ��� '�'
#define  P_to_L    35  //�� ��� '�'
#define  O_to_B    60  //�� ��� '�'
#define  mi_PEN    60  //���� ��� 'm', �᫨ ���� i
#define  m_nc_PEN  30  //���� ��� 'm', �᫨ ���吝��
#define  H_PEN     30  //���� ��� ᮡ࠭��� 'H'
#define  IEE_IU_PEN 40 //���� � ��⠭��� '�' �  '��'

//---------------------  types   --------------------------------------

struct cell_list_struct  //ᯨ᮪ cell'��
{
  int16_t N;                 //������⢮
  cell *cells[MAX_SECT];
};
typedef struct cell_list_struct cell_list;

struct dubble_list_struct  //ᯨ᮪ ࠧ��� cell'��
{
  cell_list b;  //������⢮ � ᯨ᮪ ������ cell'��
  cell_list d;  //������⢮ � ᯨ᮪ dust'��
};
typedef struct dubble_list_struct dubble_list;

struct grup_struct      //��㯯� cell'��
{
  int16_t n;       //������⢮
  int16_t a;       //���孨� �ࠩ
  int16_t b;       //������ �ࠩ
};
typedef struct grup_struct grup;

//-------------------  variables  -------------------------------------

 //extern char accent_tab[];
 extern uchar db_status;  // snap presence byte
 extern uchar db_trace_flag;  // 2 - more detailed estimate (ALT-F7)
 extern Bool pass4_in;   //䫠�: ��ன ��室 �� ��࠭��
 extern int16_t  sMALL_SIZE; //�������쭠� ���� "����讣�" �����
 Handle hSnapBLcut;

 uchar sticks_left_to_bad[]  =  {"  1/!|l1IJ)}[]"};  // 0,1st pos reserved for liga_i, liga_exm
												    // 2nd	for turkish i_sans_accent - Nick 12.06.02
 uchar letters_left_to_bad[] =  {" nrvtcC(u<>율���������ᑣ�"}; // 0th pos for sticks " nrvtcC(u<>����������������"

 static char ltmp0[] = "kDPbh����������������";
 static char ltmp1[] = "m"; /* m */
 static char ltmp2[] = "nm"; /* r */
 static char ltmp3[] = "w"; /* v */
 static char ltmp4[] = "u"; /* t */
 static char ltmp5[] = "ao"; /* c */
 static char ltmp6[] = "O6"; /* C */
 static char ltmp7[] = "oO��06"; /* ( */
 static char ltmp8[] = "w"; /* u */
 static char ltmp9[] = "od�"; /* < */
 static char ltmp10[] = "xX���"; /* > */
 static char ltmp11[] = "�"; /* � */
 static char ltmp12[] = "�"; /* � */
 static char ltmp13[] = "���"; /* � */
 static char ltmp14[] = "��"; /* � */
 static char ltmp15[] = "��"; /* � */
 static char ltmp16[] = "�"; /* � */
 static char ltmp17[] = "�";
 static char ltmp18[] = "�"; /* � */
 static char ltmp19[] = "���"; /* � */
 static char ltmp20[] = "���"; /* � */
 static char ltmp21[] = "���"; /* � */
 static char ltmp22[] = "��"; /* � */
 static char ltmp23[] = "��"; /* � */
 static char ltmp24[] = "�"; /* � */
 static char ltmp25[] = "�"; /* � */
 static char ltmp26[] = "�"; /* � */

 char *results_left_to_bad[] = {
         ltmp0, ltmp1, ltmp2, ltmp3, ltmp4, ltmp5, ltmp6, ltmp7, ltmp8, ltmp9,
         ltmp10, ltmp11, ltmp12, ltmp13, ltmp14, ltmp15, ltmp16, ltmp17, ltmp18, ltmp19,
         ltmp20, ltmp21, ltmp22, ltmp23, ltmp24, ltmp25, ltmp26
      };

 uchar prob_left_to_bad[] =
      { MAX_RO, MAX_RO, MAX_RO, MAX_RO, MAX_RO, MAX_RO, MAX_RO, MAX_RO, MAX_RO,
 //     stick   n       r       v       t       c       C       (       u
        MAX_RO, MAX_RO, MAX_RO, MAX_RO, 240, 240, MAX_RO, MAX_RO, MAX_RO,
 //     <       >       �       �       �    �    �       �       �
        240, 240, 240, MAX_RO, 240, MAX_RO, MAX_RO, MAX_RO, MAX_RO };
 //     �    �    �    �       �    �       �       �       �

/*
	0 pos reserved for liga_i
	1st pos reserved for liga_exm,
	2-3 pos for turkish II_dot_accent, i_sans_accent 21.05.2002 E.P.
*/
 uchar sticks_right_to_bad[] =  {"1111/!|l1I[]"};


 uchar letters_right_to_bad[] = {" cJnNvt)u><���斨�����������"};	// " cJnNvt)u><������������������"
                                                      // 0th pos for sticks

 static char tmp0[] = "dU������������";
 static char tmp1[] = "k"; /* c */
 static char tmp2[] = "U"; /* J */
 static char tmp3[] = "m"; /* n */
 static char tmp4[] = "W"; /* N */
 static char tmp5[] = "w"; /* v */
 static char tmp6[] = "u"; /* t */
 static char tmp7[] = "oO��0"; /* ) */
 static char tmp8[] = "w"; /* u */
 static char tmp9[] = "bop6D��"; /* > */
 static char tmp10[] = "kxX�����"; /* < */
 static char tmp11[] = "�"; /* � */
 static char tmp12[] = "�"; /* � */
 static char tmp13[] = "��"; /* � */
 static char tmp14[] = "��"; /* � */
 static char tmp15[] = "�"; /* � */
 static char tmp16[] = "�"; /* � */
 static char tmp17[] = "��"; /* � */
 static char tmp18[] = "�"; /* � */
 static char tmp19[] = "�"; /* � */
 static char tmp20[] = "�"; /* � */
 static char tmp21[] = "�"; /* � */
 static char tmp22[] = "�"; /* � */
 static char tmp23[] = "�"; /* � */
 static char tmp24[] = "��"; /* � */
 static char tmp25[] = "�"; /* � */
 static char tmp26[] = "�"; /* � */
 static char tmp27[] = "�"; /* � */
 static char tmp28[] = "�"; /* � */

 char *results_right_to_bad[] =
   {tmp0, tmp1, tmp2, tmp3, tmp4, tmp5, tmp6, tmp7, tmp8, tmp9,
    tmp10, tmp11, tmp12, tmp13 ,tmp14 ,tmp15, tmp16, tmp17, tmp18, tmp19,
    tmp20, tmp21, tmp22, tmp23 ,tmp24 ,tmp25, tmp26, tmp27, tmp28};
 //  �   �   �
 uchar prob_right_to_bad[] =
      { MAX_RO, MAX_RO, MAX_RO, MAX_RO, MAX_RO, MAX_RO, MAX_RO, MAX_RO, MAX_RO,
 //     stick   c       J       n       N       v       t       )       u
        MAX_RO, MAX_RO, MAX_RO, MAX_RO, 240, 240, MAX_RO, MAX_RO, 240, 240,
 //     >       <       �       �       �    �    �       �       �    �
        MAX_RO, 240, 240, MAX_RO, MAX_RO, MAX_RO, 240, MAX_RO, MAX_RO, 240 };
 //     �       �    �    �       �       �       �    �       �       �

#define RESLEN 26 //���ᨬ��쭠� ����� ����祭�� results_left_to_bad �
                  // results_right_to_bad

 static uchar  left_to_bad[] = {" /!l1IJ)}]cCnrvt������(u<>윯�|\x0"}; // 0 =>liga_exm.  10.09.2000 E.P.
 static uchar right_to_bad[] = {"/|!l1I]nvt)u><�����ᑪ��箎\x0"};
 static uchar       sticks[] = {"  1lI1]!"}; // 0,1 -> liga_i,liga_exm // 08.09.2000 E.P.
											// 2 -> turkish i_sans_accent	Nick 12.06.02
 static uchar  left_to_dust[] = {"���1!"};
 static uchar right_to_dust[] = {"�����!"};

 static uchar unify_width[] =   //ᨬ���� "�⠭���⭮�" �ਭ�
  {"023456789��������������������������������������������������"};

 B_LINES my_bases; //������ �����
 int16_t        blank; //�ਭ� �஡���
 int16_t minw;         //�������쭠� �ਭ� �㪢�
 int16_t minp;         //�������쭠� �ਭ� �窨
 int16_t ps3;          // 1/3 ����� ���筮� �㪢�
 int16_t sym_dist=0;   //�।��� ����ﭨ� ����� ᨬ������
 int16_t line_sym_width=0;  //�।��� �ਭ� ᨬ���� � ��ப�
 int16_t sym_width=0;       // � �� � ⥪�饬 ᫮��
 int16_t cut_width=0;  //�� �ਭ� > cut_width ����� १���

 char snap_text[80]={0},*snap=snap_text;

 uchar trs2;             // >trs2 - �㪢� ���� �� ��� �⭮襭���
 uchar trg;              //��ண ��� ᪫�������

// char bs_cut_en=0;

#ifdef PROTOCOL
 extern int16_t line_number;
// static int16_t rec_count=0;      //���稪 ��� ����⨪�
// static int16_t t;
#endif

//---------------------  macros  --------------------------------------


#define loc_err(x) (MAX_RO-(x)->vers[0].prob) //�����쭠� ���
#define norm(m,l)  ( (m)*(l)>>2 )             //��ନ஢�� ����

#define stick(x)   ( strchr(sticks,(x)) )

#define Alik cuts_point_methode

//------------------  functions  --------------------------------------

static char get_sym_width(int16_t *sw, int16_t *mw, int16_t *sd);
static int16_t *arr_top(int16_t *arr, int16_t len);
static int16_t *arr_bot(int16_t *arr, int16_t len);
static int16_t hist_sq(int16_t *hist, int16_t *top);
static int16_t get_sym_width2();
static int16_t get_sym_width3(cell *WB, cell *WE);

static void mark_own_dust(cell *B);
//static cell *process_word (cell *WB, cell *WE);
static cell *process_frame (cell *WB, cell *WE);
static int16_t one_glue (int16_t n, cell **S, int16_t tol);
static cell *cut_glue (cell *LC, cell *E, char ovfl);
static cell *break_cell(cell *B);
static cell *overlap_cell( cell *C, cell *D );
static int16_t compose_raster(raster *r, cell **clist, int16_t N);
static int16_t init_dp(struct cut_elm *cut_list, seg_vers **vers_list,
                   cell_list *list, int16_t rastlc, int16_t rastdr);
int16_t get_cuts (cell *C, struct cut_elm *list, int16_t nmax );
static void fict_sect(struct cut_elm *cut, int16_t x, int16_t px);
static void init_sect(struct cut_elm *cpnti);
static void cor_sect(cell *C, struct cut_elm *cut, int16_t left, int16_t down);
static char dp_passi(cell *LC, raster *r, struct cut_elm *cut_list,
                     seg_vers **vers_list, int16_t ncut, int16_t pass);
static uchar accept_segment(cell *C, raster *r0, struct cut_elm *cut_list,
                           seg_vers **vers_list, int16_t i1, int16_t i0, char mode);
static char *res_for(uchar let, uchar *sticks, uchar *letters, char **results);
static char *unite_list(char *resstr,int16_t i1,int16_t i0,struct cut_elm *cut_list,
                        seg_vers **vers_list);
static char *strXcrs(char *resstr, char *str1, char *str2);
static int16_t select_cells(cell *C,MN *mn1,int16_t pos1, int16_t pos2, uchar cut_fl,
                        cell_list *left_list, cell_list *right_list);
int16_t recogij(cell *C, cell **org_cells, int16_t N, uchar cut_fl,
                   uchar cut_fine,int16_t inc, int16_t *roi, uchar *gvar,
                   SVERS *vers, int16_t *width );
static uchar classify (cell **cells, int16_t N, grup *box, grup *bottom,
                      grup *top, grup *far_bottom, grup *far_top);
static int16_t horiz_proj(cell **cells, int16_t N, uchar *proj, int16_t size);
static void change(void **arr, int16_t i, int16_t j);
static int16_t recog_one(cell *B0, s_glue *GL, int16_t tol, SVERS *vers);
//static char wide (cell *C);
static int16_t recog_set(cell *C,cell **org_cells, int16_t N, uchar cut_fl,
                     uchar cut_fine, int16_t inc,
                     int16_t tolbox, uchar *gvar, SVERS *vers, int16_t *width);
static cell *unite (cell *C, cell **org_cells, int16_t N, s_glue *GL, uchar *gvar);
static uchar accept_vers(char *snap, struct cut_elm *cut_list,
                        int16_t i1, int16_t i0, SVERS *vers, char gvar, int16_t width,
                        int16_t cur_meas);
static uchar not_connect_chain(int16_t i1, int16_t i0, struct cut_elm *cut_list);
static uchar dust_chain(int16_t i1, int16_t i0, struct cut_elm *cut_list);
static cell *recover_path(void *kita, raster *r, struct cut_elm *cut_list,
                          int16_t N, cell *LC, cell *RC, char ovfl);
static int16_t locate(cell *B, struct cut_elm *cut_list, int16_t N, int16_t left);
static void set_type(cell *B, SVERS *vers);
static int16_t is_stick(cell *B);
static void paste();
static void compose_cg_flags(cell *B, cell *L, cell *R);
//static int16_t full_recog (cell *B1, s_glue *gl0, int16_t tolbox, int16_t trs);
static void corr_cut();
static int16_t have_upper_dot(cell *c);
static int16_t forbid_stick_cut(cell *c, SVERS *vers, int16_t h, cut_pos *cpos,
                            int16_t edge);
static int16_t contain(cell *new, cell *old);
cell * create_my_cell(MN * mn, cell * ci, char bdiff, char dflag);

//dm2.c
int16_t glue_to_o (uchar c2, uchar c3, cell *BC, cell *EC);

void test_match_cell_word(B_LINES *my_bases, int16_t cut_width);

#ifdef PROTOCOL
#include "..\stats\protocol.c"
#endif

static void close_ds(struct cut_elm *, int16_t , int16_t );  //AK 23.03.97 add    ???
void set_bad_vers(SVERS *);                          //AK 23.03.97 add
//extern void det_snap(cell *, char *);                //AK 23.03.97 add

/**************************************************************************/

void cuts_glues()
{
 cell *B;
 cell     *WB,*WE; //��砫� � ����� ᫮��
 int16_t      end_col; //�ࠢ�� �࠭�� ᫮��

 Z=&string;

 sticks[0] = liga_i;		// 08.09.2000 E.P.
 sticks[1] = liga_exm;		// 10.09.2000 E.P.

 left_to_bad[0] = liga_exm;	// 10.09.2000 E.P.


 sticks_left_to_bad[0] =liga_i;
 sticks_left_to_bad[1] =liga_exm;
 sticks_right_to_bad[0]=liga_i;
 sticks_right_to_bad[1]=liga_exm;

 // �������� �����
 if(is_turkish_language(language))
	{
	sticks_right_to_bad[2]=II_dot_accent;
	sticks_right_to_bad[3]=i_sans_accent;

	sticks_left_to_bad[2] = i_sans_accent;  // Nick 12.06.02
	sticks[2] = i_sans_accent;			// Nick 12.06.02
	}
 else	// 16.06.2002 E.P.
	{
	sticks_right_to_bad[2]='1';
	sticks_right_to_bad[3]='1';
	sticks_left_to_bad[2] = '1';
	sticks[2] = '1';
	}

// if (pass4_in) { trs2=trg=180; }
 if (pass4_in) { trs2=trg=200; }
 else          { trs2=220; trg=190; }

 snap_newpass('a');
 get_b_lines(NULL,&my_bases);
 WB=cell_f()->next;
 if (debug_on)
 { char bb[90];
   sprintf (bb,"cuts/glues begin\n bs= %d %d %d %d, Ps=%d, nb= %d %d %d %d",
     my_bases.b1, my_bases.b2, my_bases.b3, my_bases.b4,
     my_bases.ps,
     my_bases.n1, my_bases.n2, my_bases.n3, my_bases.n4);
   glsnap('a',WB,bb);
 }

//�業����� ����稭� �஡��� � ���. �ਭ� �㪢�

 blank=my_bases.ps>>1;  // 1/2 ����� ���筮� �㪢�
 ps3=my_bases.ps/3;
 minw=my_bases.ps/5;  minp=minw>>1;
 line_sym_width=my_bases.ps; cut_width=0;
 if ( language == LANG_RUSSIAN && !pass4_in )
 {
   get_sym_width(&line_sym_width,&cut_width,&sym_dist);
//   if (get_sym_width(&sym_width,&cut_width))  minw=sym_width>>2;
//   else                                       minw=my_bases.ps/5;
//   minp=minw>>1;
 }
 else
   minw=minp;
//   minw=minp=my_bases.ps/10;
// minw=MAX(minw,1); minp=max(minp,1);

//���塞 �業�� � ���⠢�塞 �窨 ��� 'i'

adjust_3x5(TRUE);


//�� ࠧ �஡㥬 �業��� �ਭ� ᨬ����
 if (line_sym_width==0)  line_sym_width=get_sym_width2();
 sprintf(snap,"dist=%d  width: aver=%d max=%d",
         sym_dist,line_sym_width,cut_width);
 WB=cell_f()->next;
 det_snap(WB,"");

#ifdef MATCH_WORD
 test_match_cell_word(&my_bases,cut_width);
#endif

 perc();  //// '�' � '%'

 WB=cell_f()->next;
 do  // 横� �� ᫮���
 {
   WE=WB; end_col=0;
   do
   {
     B=WE;
     WE=B->next;
     end_col=MAX(end_col,B->col+B->w);
   }
   while ( WE->next && ( WE->col - end_col ) < blank );
   WE=B;
   sym_width = (line_sym_width) ? line_sym_width : get_sym_width3(WB,WE);
   WB=process_word(WB,WE);
 }
 while ( WB->next ) ;

 if(language==LANG_RUSSIAN)
	 paste();


// if (pass4_in && bs_cut_en)
 if (pass4_in && LDPUMA_SkipEx(hSnapBLcut,FALSE,FALSE,1))
 {
   glsnap('o',B, "pass2 cut by bases");
   base_lines_cut();
   glsnap('o',B, "cuts_glues's end");
 }

#ifdef PROTOCOL
//  wr_prot ("pro",'c',(int16_t)'\n');
#endif

//�᫨ p>MINlet, � "����"

 B=cell_f()->nextl;
 glsnap('a',B, "cut end, corr start");
 corr_cut();
 B=cell_f()->nextl;
 glsnap('a',B, "corr end");

}

/*----------------------------------------------------------------------
  get_sym_width -  ������ �।��� sw � ���ᨬ����� mw �ਭ� ᨬ����
                   � �������쭮� ����ﭨ� ����� ᨬ������ sd;
                   �᫨ �ᯥ譮, �����頥� 1, ���� �����頥� 0 � mw=0,
                   sd=0, sw=my_bases.ps
----------------------------------------------------------------------*/
static char get_sym_width(int16_t *sw, int16_t *mw, int16_t *md)
{
 cell *B=cell_f()->next;
 char proj[MAX_COL],*p=proj;     //�஥��� ��ப� �� ��ਧ��⠫��� ���
 int16_t  maxcl=0;                   //�� �����
 int16_t gate_hist[MAX_GATE]={0};    //���⮣ࠬ�� "�����" ��१���
 int16_t width_hist[MAX_WIDTH]={0};  //���⮣ࠬ�� "����" ��१���
 int16_t *top1,*top2,*bot;           //���設� I � II ��� � �࠭�� ����� ����
 int16_t sq_hist=0,main_sq;          //���頤�: ���� � II ����
 int16_t ps2=my_bases.ps>>1;
 int16_t w,pick;

 memset(proj,'0',MAX_COL);

//�஥��� ��ப� �� ��ਧ��⠫��� ���
 do
 {
    int16_t beg=B->r_col; int16_t len=B->w;
    int16_t end=beg+len;
    if (end>=MAX_COL)  break;
    maxcl=MAX(maxcl,end);
    memset(proj+beg,'1',len);
    B=B->next;
 }
 while ( B->next ) ;
 proj[maxcl]=0;

  p += STRSPN(p,"0");          //�㫨 � ��砫� ��ப�
  w=STRSPN(p,"1");             //��ࢠ� �஥���
  if ( w<MAX_WIDTH )  width_hist[w]++;
  p += w;
  while ( w>0 )
  {
    w=STRSPN(p,"0");
    if ( w<MAX_GATE )  gate_hist[w]++;
    p += w;
    w=STRSPN(p,"1");
    if ( w<MAX_WIDTH )
    {
      width_hist[w]++; sq_hist++;
    }
    p += w;
  }

  top1=arr_top(width_hist+1,ps2);           // I  ����
  top2=arr_top(width_hist+ps2,my_bases.ps); // II ���� (�ਭ� �㪢)
  bot=arr_bot(top1,(int16_t)(top2-top1));              //�࠭�� ����� ����
  main_sq=hist_sq(bot,top2);                //���頤� II ����
  if ( main_sq+(main_sq<<1) <= sq_hist<<1 )
  {                         //�業�� �����⮢�ୠ
    *sw=0;  *mw=0;  *md=0;
    return 0;
  }
  else
  {
    *sw=top2-width_hist;            //���設� II ����
    while (*top2 != 0)  top2++;     //�� �ࠢ� �ࠩ+1
    *mw=top2-1-width_hist;
    top1=arr_top(gate_hist+1,ps2);
    pick=(*top1)>>1;
    while ( *top1>pick && top1>gate_hist )  top1--;
    *md=top1-gate_hist;
    return 1;
  }
}

/*----------------------------------------------------------------
  arr_top  ��� ���� ���ᨬ��쭮�� ����� � ���ᨢ� arr �����
           len
----------------------------------------------------------------*/
static int16_t *arr_top(int16_t *arr, int16_t len)
{
  int16_t  i,topv=*arr, *topi=arr, *cur=arr+1;
  for (i=1; i<len; i++,cur++)
    if (*cur>topv)
    {
      topv=*cur; topi=cur;
    }
  return topi;
}
/*----------------------------------------------------------------
  arr_top  ��� ���� �������쭮�� ����� (�᫨ ��᪮�쪮, � -
           ��᫥����� �� ���) � ���ᨢ� arr ����� len
----------------------------------------------------------------*/
static int16_t *arr_bot(int16_t *arr, int16_t len)
{
  int16_t  i,botv=*arr, *boti=arr, *cur=arr+1;
  for (i=1; i<len; i++,cur++)
    if (*cur<=botv)
    {
      botv=*cur; boti=cur;
    }
  return boti;
}

/*-----------------------------------------------------------------
  mode_sq  ������ ���頤� ���⮣ࠬ�� hist, ��࠭�祭��� �㫥�
           �ࠢ� �� ���設� top
 ----------------------------------------------------------------*/
static int16_t hist_sq(int16_t *hist, int16_t *top)
{
  int16_t *cur, sq=0;
  for (cur=hist; *cur != 0 || cur<top; cur++)  sq += *cur;
  return sq;
}

/*-----------------------------------------------------------------
  get_sym_width2() �業����� �ਭ� ᨬ���� �� �ᥩ ��ப�
-----------------------------------------------------------------*/
static int16_t get_sym_width2()
{
 cell *B;
 uchar *ip,*ip2;
 int16_t ws=0;           //�᪮��� �ਭ�
 uchar hist[256]={0}; //���⮣ࠬ�� �ਭ�
 uchar top=0,*itop;   //���設� ���⮣ࠬ��
 int16_t s=0,s1=0;       //���頤� ���⮣ࠬ��: ���� � � ������ (3/4;5/4)ws

//��ந� ���⮣ࠬ��

 B=cell_f()->nextl;
 while ( !fict(B) )
 {
   if (let(B) && strchr(unify_width,B->vers[0].let) &&
		!is_russian_baltic_conflict(B->vers[0].let)	&&  // 17.07.2001 E.P.
		!is_russian_turkish_conflict(B->vers[0].let)	// 21.05.2002 E.P.
	  )
     { hist[B->w]++; ws += B->w; s++; }
   B=B->nextl;
 }
 if (s==0)  return 0;
 ws /= s;

//�饬 ���ᨬ� � ������ �।��� �ਭ�

 ip2=hist+ws+(ws>>1);
 for (ip=ip2-ws; ip<=ip2; ip++)
   if (*ip>top)  { top=*ip; itop=ip; }

//१���� ���⮢�७, �᫨ � ������ ���ᨬ㬠 ��।��祭� ����� 3/4s

 if (top>2)  ws=itop-hist;
 ip2=itop+(ws>>2);
 for (ip=itop-(ws>>2); ip<=ip2; ip++)  s1 += *ip;

 if ( s-s1 <= s>>2 )  return ws;
#ifdef PROTOCOL
  wr_prot ("multfont",'d',line_number);
#endif
 return 0;
}

/*-----------------------------------------------------------------
  get_sym_width3() �業����� �ਭ� ᨬ���� �� ⥪�饬� ᫮��
-----------------------------------------------------------------*/
static int16_t get_sym_width3(cell *WB, cell *WE)
{
  cell *B;
  int16_t  s=0,ws=0;

  for (B=WB; B != WE; B=B->next)
    if (let(B) && strchr(unify_width,B->vers[0].let) &&
		 !is_russian_baltic_conflict(B->vers[0].let) &&	// 17.07.2001 E.P.
		 !is_russian_turkish_conflict(B->vers[0].let)	// 21.05.2002 E.P.
	   )
		{ ws += B->w; s++; }
  if (s>0)  return  ws/s;
  return  my_bases.ps-(my_bases.ps>>3);
}

/*-----------------06-22-95 11:12am--------------------------------
  process_word - ����� � ������������� "����������" ��������, �����
                 cell'��� WB � WE; ���������� WE->next
-----------------------------------------------------------------*/
cell *process_word (cell *WB, cell *WE)
{
  cell  *B;    //������ "�����������" �������
  cell  *E;    //�����        -"-
  int16_t end_col; //������ ����  -"-
  int16_t max_gate;//����. "�������" ������ �������������� �������
  cell *RW;    //cell ������ �� �����
  cell *ST;    //����� ����� "�"
  uchar let;
  cell *C;

 RW=WE->next; B=WB; max_gate=my_bases.ps/3;

 B=WB;
 do  // 横� �� "��������" ���⪠�
 {
   E=B; end_col=B->col+B->w; ST=NULL;

   while (1)
   {
     if ( language == LANG_RUSSIAN )
		{
        let=E->vers[0].let;

       if ( let(E) &&
				(
				let==(uchar)'�' &&
					!is_russian_turkish_conflict(let) || // 21.05.2002 E.P.
				let==(uchar)'�')
				)
       {                 //"�" �� �ᥣ�� ������� �� ��⠬
         if ( !complete_recog(E) )
           if ( E != WB && !let(E->prev) || E != WE && !let(E->next) )
           {
             let_to_bad(E);
             full_recog(E,NULL,0,trs2);
           }
       }
       else
         if ( !dust(E) && E->nvers)
           if ( ST && strchr("���C�",let) &&		// "���C��"
				!is_russian_baltic_conflict(let) &&	// 17.07.2001 E.P.
				!is_russian_turkish_conflict(let)	// 21.05.2002 E.P.
			  )
             set_bad(ST);   //"��","��","��" ��� "��" - ���� �����������
           else
             if ( E->vers[0].let=='|' ) ST=E;
             else                       ST=NULL;
     }

     if ( E==WE )  break;
     if ( E==cell_l() )  break; // OLEG 08 dec 99

     C=E->next;
     if ( C->col - end_col > max_gate )
       if (pass4_in)
       {
         while ( dust(C) && C != WE )  C=C->next;
         if (dust(C))  break;
         if (!let_or_bad(C->prevl))  break;   //䨪⨢�� � ��砫� ��ப�
         if (!is_stick(C) || !is_stick(C->prevl)) break;
       }
       else
         break;
     E=E->next;
     end_col=MAX(end_col,E->col+E->w);
   }
   if ( E==cell_l() )  break; // OLEG 08 dec 99
   B=process_frame(B,E);

 }
 while ( B != RW  ) ;

 return B;
}

/*-----------------02-20-95 06:41pm---------------------------------------
  process_frame - ���� � �ᯮ�������� "�����" ���⪮�, �����
                  cell'��� WB � WE; �����頥� WE->next
-------------------------------------------------------------------------*/
static cell *process_frame (cell *WB, cell *WE)
{
 cell *LW,*RW;//cell'� ᫥�� � �ࠢ� �� ᫮��
 cell *C;     //⥪�騩 cell
 cell *B,*E;  //��砫� � ᫥���騩 �� ���殬 "���宣�" ���⪠
 cell *E2;    //��室��� ���祭�� E, ����� E ᤢ������� �����
 cell *E1,*E1P;//�ࠢ� �� ���� ���ࢠ��, ��ࠡ��뢠����� cut_glue
 int16_t   w,dh;  //��� ࠧ����
 int16_t      n;  //������⢮ cell'�� � ���
 int16_t  maxcl;  //�ࠢ� �⮫��� + 1
 uchar   let;  //����� ����� cell'�
 int16_t      p;  //�� ����⭮���
 int16_t   h1,h2;

 LW=WB->prev; RW=WE->next;

 B=WB;
 do  // 横� �� "���娬" ���⪠�
 {

//   C=NULL;
   while ( let(B) || !may_glue(B) )      //�饬 ��砫�
   {
     B=B->next;
     if ( B==RW )
		 return B;
   };


   E=B->next;      //�饬 �����
   h1=B->row; h2=h1+B->h;
   maxcl=B->col+B->w;
   for ( n=1; !let(E) && may_glue(E) && ( E != RW ); n++ )
   {
     h1=MIN(h1,E->row);
     h2=MAX(h2,E->row+E->h);
     maxcl=MAX(maxcl,E->col+E->w);
     w=maxcl-B->col;
     E=E->next;
   }
   dh=h2-h1;             //���� ����

   if ( n>1 )            //��᪮�쪮 ��᪮� - �����
   {
    Bool dis=FALSE; // OLEG : SERBIAN PASS4


    if(!pass4_in)
      {
      if( language==LANG_RUSSIAN && langSer &&
        (B->flg&c_f_let) && B->nvers && (B->vers[0].let==SERB_j||B->vers[0].let=='j') &&
        ij_dot(B)==0 )
        dis = TRUE;
      }

     if( !dis )
     if ( dh<=RASTER_HEIGHT && w < dh + dh - (dh>>1) && w > minw )
       if ( (C=B->prev) == LW || !lefter(B,C->r_col+C->w) )
         if ( E == RW || lefter(E->prev,E->r_col) )
		 {
           if ( one_glue(n,&B,trs2) )
			   continue;  //����� �� ᫮��
		 }
   }
   else   //n==1
   {
     if ( dust(B) )    //������� dust ����� � ᯥ樠���� �����
     {
       int16_t bottom=B->row+B->h;        //������ �࠭�� dust'�
       int16_t middle=(B->row+bottom)>>1; //�।���

       //���ࠢ�
       C=B->next;
       if ( C != RW && !dust(C) && may_glue(C) )  //�� ����� ᫮��
                                        //C - dust, �᫨ C - �窠 ��� "i"
         if ( (let=C->vers[0].let) != '!' && let !='?' )
         {
           int16_t midc=C->row+(C->h>>1);
           char fl =  middle > C->row && bottom < midc &&
                      (let==(uchar)'�' || let==(uchar)'�');
           if ( B->r_col+B->w > C->r_col || fl ) //C � dust ��४�뢠����
           {                                     //��� ����� ���� "�"("�")
             if ( !complete_recog(C) )
             {
               let_to_bad(C);
               full_recog(C,NULL,0,trs2);
             }
             p=C->vers[0].prob;
             //�� ��� ࠧ��������� �㪢:
             if ( fl )  p -= 60; //�,�
             else
               if ( middle < C->row &&
						let==(uchar)'�' &&
							!is_russian_turkish_conflict(let) // 21.05.2002 E.P.
				)  p -= 50; //�
               else
                 if ( B->row > midc &&
					 let==(uchar)'�' &&
						!is_russian_turkish_conflict(let) // 21.05.2002 E.P.
					 )  p -= 30; //�,�

             if ( one_glue(2,&B,p) )
				 continue;  //����� �� ᫮��
           }
         }

       //������
       C=B->prev;
       if ( C != LW && !dust(C) && may_glue(C) )  //�� ��砫� ᫮��
         if ( (let=C->vers[0].let) != '!' && let !='?' )
         {
           int16_t midc=C->row+(C->h>>1);
           char fl = middle > C->row && bottom < midc && let=='1'; //����� ���� "�"
           if ( fl || lefter(B,C->r_col+C->w) ) //����� ����"�"
           {                                    // ��� dust ����뢠���� C
             if ( !complete_recog(C) )
             {
               let_to_bad(C);
               full_recog(C,NULL,0,trs2);
             }
             p=C->vers[0].prob;
             //�� ��� ࠧ��������� �㪢:
             if ( fl || bottom < midc && let==(uchar)'�'   //�,�
                     || B->row > midc &&
						let==(uchar)'�' &&
							!is_russian_turkish_conflict(let) // 21.05.2002 E.P.
				) //�,�
               p -= 30;
             else
               if ( middle < C->row &&
					let==(uchar)'�' &&
						!is_russian_turkish_conflict(let) // 21.05.2002 E.P.
				  )  p -= 50;  //�
               else
                 if ( (let==(uchar)'�' || let==(uchar)'�') && B->row > C->row &&
                       middle < C->row+C->h   //�
                      ||
                      (let==(uchar)'�' || let==(uchar)'�') && middle>C->row && middle<midc )//�
                          p -= 20;
             B=C;

             if ( one_glue(2,&B,p) )
				continue;  //����� �� ᫮��
           }
         }
       B=E;
       continue;                 //dust �� �ਪ������ - �ய�᪠��
     }
   }


//���������  �� ������ cell'� ����� � ������, ���� ������������� dust'��
// � (���) ���������� � ������
//�����:

   C=B->prev;  let=C->vers[0].let;
   if ( C != LW && !dust(C) && (may_glue(C) || let==(uchar)'�' || let==(uchar)'�'))
   {
     char  add=0;          //����: �������� ������ �����
     if ( dust(B) && lefter(B,C->r_col+C->w) )  //C ������������� dust'��
     {
       if ( !complete_recog(C) )  //�� 3x5, �᫨ �� �ᯮ���������
       {
         let_to_bad(C);
         full_recog(C,NULL,0,trs2);
       }
       if ( bad(C) || strchr(left_to_dust,let) )  add=1;
     }
     if ( !add && strchr(left_to_bad,let) &&
			!is_russian_baltic_conflict(let) &&	// 17.07.2001 E.P.
			!is_russian_turkish_conflict(let)	// 21.05.2002 E.P.
		)
     {
       if ( !complete_recog(C) )  //�� 3x5, �᫨ �� �ᯮ���������
       {
         let_to_bad(C);
         full_recog(C,NULL,0,trs2);
       }
       add=1;
     }
     if (add)
     {
       B=C;  set_let(B);          //������塞 C, �� १��� �� �㤥�
       if ( let=='!' )
       {
         C=B->prev;
         if ( C != LW && dust(C) )
           if ( C->r_col+C->w > B->r_col )  B=C;  //�窠 �� '!'
       }
     }
     else
       while ( dust(B) && lefter(B,C->r_col+C->w) && B != E )  B=B->next;
   }
   if (B==E)  continue;

//�ࠢ�:

   E2=E; C=E->prev;
   if ( E != RW && may_glue(E) )
     if ( dust(C) && !lefter(C,E->r_col) &&
          strchr(right_to_dust,E->vers[0].let) ||
          strchr(right_to_bad,E->vers[0].let) &&
		   !is_russian_baltic_conflict(E->vers[0].let) &&	// 17.07.2001 E.P.
		   !is_russian_turkish_conflict(E->vers[0].let)		// 21.05.2002 E.P.
		)
     {
       maxcl=MAX(maxcl,E->col+E->w);
       if ( !complete_recog(E) )  //�� 3x5, �᫨ �� �ᯮ���������
       {
         let_to_bad(E);
         full_recog(E,NULL,0,trs2);
       }
       set_let(E);  let=E->vers[0].let;
       E=E->next;
       if ( E != RW && let=='!' && dust(E) && E->r_col < maxcl )
         E=E->next;       //E - �窠 �� '!'
       E2=E;
     }
     else                 //�ࠢ� �� ������塞
     {
       while ( dust(C) && !lefter(C,E->r_col) && C != B )  C=C->prev;
       E=C->next;
     }
   if ( E==B->next && dust(B) )  { B=E2; continue; }

//ࠧ१����-᪫�������

   E1P=NULL;
   while (1)     //�� ������ࢠ��� ���� RASTER_WIDTH
   {
     while (1)
     {
       w=maxcl-B->col; E1=E;
       if ( w>RASTER_WIDTH ) //���� �뤥���� ������ࢠ� ���� RASTER_WIDTH
       {
         int16_t right_col=MININT; int16_t right_col1=MININT;
         for ( E1=B; E1 != E; E1=E1->next )
         {
           right_col=MAX(right_col,E1->col+E1->w);
           if (right_col-B->col>RASTER_WIDTH) break;
           right_col1=right_col;
         }
         w=right_col1-B->col;
       }
       if ( E1 != E1P || B==E )  break;
     //����稫�� �� �� ������ࢠ�, �� � ���� ࠧ
       B=E1;                  //�ய�᪠�� ��� ���������
     }
     if ( w<minw || B==E ) break;
     C=B->prev;
     E1P=E1;
     B=cut_glue(C,E1,(char)(E1 != E));
     if ( E1==E )  break;
     if ( B==NULL )  B=E1;
   }        //ࠧ१����-᪫������� �����祭�
   B=E2;
 }
 while ( B != RW ) ;
 return B;
}

/*************************************************************************/
/*    one_glue  ��⠥��� ᪫���� n ��������⮢, ��稭�� � *S, �          */
/*              �ᯮ����� �, �� ��������:                            */
/*                �᫨ 㤠筮 (����⭮��� ����� tol), �����頥� 1     */
/*              � *S 㪠�뢠�� �� ᫥���騩 �� ᪮���������� ����⮬, */
/*                �᫨ ��㤠筮, �����頥� 0 � *S 㪠�뢠�� ��          */
/*              ���� ���ᯮ������ �����                            */
/*************************************************************************/

static int16_t one_glue (int16_t n, cell **S, int16_t tol)
{
  cell    *LB;   //cell ᫥�� �� ��砫�
  cell     *B;   //⥪�騩 cell
  s_glue  GL={0};//ᯨ᮪ ᪫�������� ���������
  MN      *mn;   //�஬������ १���� ᪫����
  int16_t     i;     //�ᯮ����⥫�� ��६����
  int16_t nstick=0;  //�᫮ �����
  uchar  fl_b=0;  //䫠� "�"
  int16_t  inc;      //��।����� ������
  uchar cg_flag=0;

  B=*S;
  glsnap('a',B,"glue begin");

  n=MIN(n,MAX_CELLS_IN_LIST-1);
  LB=B->prev;

//��⠢�塞 ᯨ᮪

  GL.ncell=(uchar)n; GL.ngluc=1;
  for ( i=0; i<n; i++ )
  {
    cg_flag |= B->cg_flag;
    if(language == LANG_RUSSIAN && !dust(B))
      if (nstick<2)
        switch(fl_b)
        {
          uchar let;

          case 0:       //���� ࠧ
            if ( B->nvers && ((let=B->vers[0].let)==(uchar)'�' || let==(uchar)'�') )
               {
			    fl_b=2;
			    break;
		       }
             else  fl_b=1;

          case 1:       //���� �� "�"
              nstick += is_stick(B);
          break;

          case 2:       //���� "�" � "����讬�" �� ������
          nstick=2;
          break;
        }
      else
        nstick=3;
    GL.celist[i]=B;  GL.complist[i]=B->env;
    B=B->next;
  }
  inc = erection_compose_inc(n, GL.celist);

//�����

  if ( nstick < 2 )
  {
    if (mn=glue(GL.complist, 2))
      if ( !mn->mnnext )
      {                              //᪫������
        B=create_my_cell(mn,LB,0,0);  //ᮢ��㯭� �����
        if ( dust(B) )  goto ret;
        B->cg_flag=cg_flag;
        inter_diff(B);
        set_erection(B, inc);
        if ( full_recog(B,NULL,tol,tol) )
        {                    //�ਭ����� १����
          B->cg_flag &= ~c_cg_cut;
          for ( i=0; i<n; i++ ) del_cell(GL.celist[i]);
          glsnap('a',B,"glued OK");
          *S=B->next;
          return 1;
        }
        goto ret;           //᪫���� ��㤠筠
      }
  }

// �ᯮ�������� ��� ᪫����

  B=comp_to_cell(LB,GL.complist,n,0,0);
  B->cg_flag=cg_flag;
  inter_diff(B);
  GL.var=GL.fres=GL.arg=0; GL.flarg=GFsrc;
  if ( !dust(B) && crecell(B,&GL,3) >=0 )  //ᮢ��㯭� ���� - � BOX'�,
  {                                        //B �� ���������
    B->broken_II = nstick==2;
    set_erection(B, inc);
    dmBOX(B,&GL);
    if ( B->nvers )
      if ( B->vers[0].prob>tol )
      {                      //�ਭ����� १����
        for (i=0; i<n; i++) del_cell (GL.celist[i]);
        glsnap('a',B,"BOX OK");
        *S=B->next;
        return 1;
      }
  }

// ��㤠�

ret:
  del_cell(B);
  glsnap('a',LB->next,"glue end");

  return 0;
}

/*-----------------02-21-95 06:28pm-----------------------------------
  cut_glue  ���������� ������� ������ ����� LC � E (LC � E �� ���.);
            ���������� E, ���� ovfl == 0 � ��������� �� cell, ���� ����
            ������������, ���� ovfl != 0; ���� ������������� ��
            ����������� ���������� NULL
---------------------------------------------------------------------*/
static cell *cut_glue (cell *LC, cell *E, char ovfl)
{
  int16_t i,j;
  cell *C;             //ࠡ�稩 cell
  cell *B=LC->next;    //��砫�
  cell_list org_cells; //��室�� cell'�
  raster r;       //ᮢ��㯭� ����
  int16_t st_inc;          //�।��� ������
  int16_t ncut;            //��饥 �᫮ �祭��
  struct cut_elm cut_list[MAX_CUTS],*cut;  //ᯨ᮪ �祭�� � ⥪�饥 �祭��
  struct dp_vers_struct vers_pool;   //��� ���ᨩ dp
#define vers_list vers_pool.node[0]
  int16_t pass;       //����� ��室�
  void *kita;     //㪠��⥫� �⥪� kit
  int16_t b4=my_bases.b2+(my_bases.ps<<1);

  snap_newcut();
  glsnap('a',B,"cut/glue begin");

#ifdef PROTOCOL
//  clock_on();
//  rec_count=0;
#endif

#pragma warning ( disable : 4047 4024 )

  memset(&vers_pool.node,0,MAX_SEG_VERS*sizeof(seg_vers *));
  vers_list=(seg_vers*)&vers_pool.pool;

  //������ ��� ���

  for (i=0; i<MAX_SECT && B != E; i++,B=B->next)
	  org_cells.cells[i]=B;

  st_inc = erection_compose_inc(i, org_cells.cells);

  //���믠�� ��⠢�� cell'�

  B=LC->next;
  while ( B != E )
  {
    if (!(B=break_cell(B)))
    {
      if (bad(B)) set_let(B); //�� ���믠���� - �� ०��
      B=B->next;
    }
  }

  //᪫������ �������騥�� ��� �� ��㣠

  if (!glue_overlap(LC,E))
    return NULL;

  //ᯨ᮪ org_cells

  B=LC->next;
  for ( j=0,C=B; C != E; C=C->next )
  {
    if ( dust(C) )
    {
      int16_t mid=C->row+(C->h>>1);
      if ( C->h>my_bases.ps>>1 && mid<my_bases.b1 || mid>b4 )  continue;            //� �ᥤ��� ��ப�
    }                                             //� �ᥤ��� ��ப�

    if ( j>=MAX_SECT )
    {
      glsnap('a',B,"too many cells");
      return NULL;
    }
    org_cells.cells[j++]=C;
  }

  if ( j==0 )
	  return NULL;
  if ( j==1 && org_cells.cells[0]->w<=cut_width )
  {
     glsnap('a',E,"cut/glue end");  return NULL;
  }
  org_cells.N=j;


//����஥��� ����

  kita=give_kit_addr();
  if (!compose_raster(&r,(cell**)&org_cells.cells,org_cells.N) || r.w>RASTER_WIDTH)
    { glsnap('a',B,"raster too large"); return NULL;}

//ᯨ᮪ �祭�� � ��砫�� ���祭��


  ncut=init_dp(cut_list,&vers_list,&org_cells,r.left,(int16_t)(r.top+r.h-1));

  cut_list->rv.v2=st_inc;
  for (i=0; i<org_cells.N; i++)
    org_cells.cells[i]=(cell *)del_save_cell(org_cells.cells[i]);


#ifndef MY_DEBUG
  if (!det_trace)  cg_show_rast(LC,&r,"",cut_list); //���� - �� �࠭
#endif

  for ( i=2,cut=cut_list+1; i<ncut; i++,cut++ )
    cut->var |= NOT_CUT;  //�� १���

  if (language==LANG_RUSSIAN)  dp_pass0(LC,&r,cut_list,&vers_list,ncut);
  else
    for ( pass=1; pass<=3; pass++ )
      if (dp_passi(LC,&r,cut_list,&vers_list,ncut,pass))  break;

  snap_stop_cuts();

  if (debug_on)
#ifndef MY_DEBUG
  if (det_trace)
#endif
  {
    char snap[500]="DP end";
    *show_dp(snap+6,cut_list,(int16_t)(ncut-1))=0;
    snap_newcell(LC);
    snap_show_text(snap);
    snap_monitor();
  }
/*
  {
    int16_t t=clock_read();
    wr_prot ("time",'d',t);
  }
*/

#ifdef PROTOCOL
//  wr_prot ("numrecog",'d',rec_count_let(rec_count,cut_list,ncut)*10);
#endif

//����⠭������� cell'�� ��⨬��쭮�� ���

  C=recover_path(kita,&r,cut_list,ncut,LC,E,ovfl);

  if ( C )
  {
//    int16_t p=low_prob(cut_list,ncut-1);
//    if (p != 255) wr_prot ("lowprob",'i',p);
    glsnap('a',E,"cut/glue end");
    if (ovfl && C != E)  glsnap('a',C,"return to cell displayed");
  }
  else
  {
    for (i=0; i<org_cells.N; i++)  rest_cell(org_cells.cells[i],E);
    glsnap('a',E,"path not recovered");
  }

  return C;
#undef vers_list
}

/*------------------------------------------------------------------
  break_cell - ���믠�� ��⠢��� cell �� ���������� �吝���;
               �����頥� 㪠��⥫� �� ᫥���騩 (�� ���믠���)
-------------------------------------------------------------------*/
static cell *break_cell(cell *B)
{
  MN *mn1;
  uchar *rp1;                   //�஬������ ����
  cell *C=NULL,*RC=B->next;    //AK C=NULL

  if (!composed(B))  return RC;

  det_snap(B,"cell fuse");
  rp1=(uchar*)save_raster(B);
  if (!(mn1=c_locomp(rp1,(int16_t)((B->w+7)>>3),B->h,B->r_row,B->r_col)))
  {
    det_snap(B,"locomp error"); return NULL;
  }
  while (mn1)
  {
    C=create_my_cell(mn1,B,0,0);
    set_just(C);
    inter_diff(C);
    mn1=mn1->mnnext;
  }
  del_cell(B);
  return RC;
}

/*-----------------03-01-95 04:52pm---------------------------------
  compose_raster  �� ����� raster �ନ��� ���� N cell'��, ��������
                  ᯨ᪮� clist, � ������ ࠧ���� ���� w,h � ��-
                  �न���� ur,lc ������ ���孥�� 㣫�;
                  �����頥� 1, �᫨ �ᯥ譮, � 0, �᫨ ���
-------------------------------------------------------------------*/
static int16_t compose_raster(raster *r, cell **clist, int16_t N)
{
  cell *c;
  uchar *rp1;                   //�஬������ ����
  c_comp *elist[MAX_CUTS];
  int16_t i,top,bot,left,right;

//��ꥤ��塞 �� cell'� � ����
//����塞 ࠧ���� � ���न���� ����

  c=new_cell();
 for (top=left=10000,bot=right=-10000,i=0; i<N && i<MAX_CUTS; i++)
  {
  if (clist[i]->r_row<top)
   top=clist[i]->r_row;
  if (clist[i]->r_row+clist[i]->h>bot)
   bot=clist[i]->r_row+clist[i]->h;
  if (clist[i]->r_col<left)
   left=clist[i]->r_col;
  if (clist[i]->r_col+clist[i]->w>right)
   right=clist[i]->r_col+clist[i]->w;
  elist[i]=clist[i]->env;
  }
 c->r_row=top;
 c->h=bot-top;
 c->r_col=left;
 c->w=right-left;
 c->row=c->r_row-(int16_t)((int32_t)nIncline*c->r_col/2048);
 c->col=c->r_col+(int16_t)((int32_t)nIncline*c->r_row/2048);
  insert_cell1(c,clist[0]);
  if ( c->w > 128 || c->h > 63 )
  {
    del_cell(c); return 0;
  }
 c->env=compose_comp(i,elist);

//�ନ�㥬 ����

  rp1=(uchar*)save_raster(c);
  memcpy(r->pict,rp1,((c->w+7)>>3)*c->h);
  r->w=c->w; r->h=c->h; r->top=c->r_row; r->left=c->r_col;
  del_cell(c);

  return 1;

}

/*-----------------03-02-95 01:22pm----------------------------------
  init_dp  �ନ��� ᯨ᮪ �祭�� ��� 楯�窨 list, ᯨ᮪ ����砥�
           ����� � �ࠢ�� �࠭��� 楯�窨; ��������� �祭�� ��।���-
           ���� �⭮�⥫쭮 ����� rastlc � ������ rastdr �࠭�� ᮢ�-
           �㯭��� ����; �ନ��� ��砫�� ����, ��������� cell'
           ��� �� list;
           �����頥� �᫮ �祭��
---------------------------------------------------------------------*/
static int16_t init_dp(struct cut_elm *cut_list, seg_vers **vers_list,
                   cell_list *list, int16_t rastlc, int16_t rastdr)
{
  char x;
  int16_t i,j,ro;
  int16_t  nc;        //������⢮ �祭�� cell'�
  int16_t ncut=1;     //��饥 �᫮ �祭��
  int16_t dust_sect=0;//䫠�: �祭�� �� dust'��
  int16_t mincl=MAXINT,maxcl=rastlc;  //���. � ���. �業�� ��� �ࠢ��� ��� dust-ᥪ樨
  struct cut_elm *seci=cut_list+1; //⥪�饥 �祭��
  SVERS                    *versi; //��� ���ᨨ
  cell *C,box;    //�ᯮ������� ⮫쪮 row, col, w, h - ࠧ���� ��஡��
  memset(&box,0,sizeof(cell));

/* dust-ᥪ�� �ନ�����, �᫨:
 - ������� dust, ����� �� ����뢠���� �ᥤ���� "����訬�" ��������⠬�
 - dust �� �ᯮ������� ��室�� �� ��� ��� �������  */

//���⠢�塞 �祭��

  fict_sect(cut_list,0,0);  //0-� 䨪⨢��� �祭��
  for ( i=0; i<list->N; i++ )
  {

    C=list->cells[i];
    if ( dust(C) )
    {

      if ( C->r_col>=maxcl )
      {
        if ( dust_sect )
        {                  //����뢠�� ����� dust-ᥪ��
          close_ds(seci,(int16_t)(maxcl-rastlc),(int16_t)(ncut-1));
          ncut++; seci++;
          if ( ncut==MAX_CUTS ) {dust_sect=0; break;}
        }
        else
          (seci-1)->x=maxcl-rastlc;
                           //���뢠�� ����� dust-ᥪ��
        dust_sect=1; mincl=C->r_col+C->w;
      }
      else
        if ( !dust_sect    //dust ��४�뢠���� "����訬" ��������⮬
             && C->r_col+C->w>maxcl
             && C->row+C->h<my_bases.b3+(my_bases.ps>>1)
             && ( C->row>my_bases.bm || is_defis(C) )
           )               //����� ���� �窠 ��� ������
        {                  //���뢠�� dust-ᥪ��
          (seci-1)->x=((seci-1)->x+(C->r_col-rastlc))>>1;
          dust_sect=1; mincl=maxcl=C->r_col+C->w;
        }
      maxcl=MAX(maxcl,C->r_col+C->w);
      mincl=MIN(mincl,C->r_col+C->w);
    }
    else  //�� dust
    {

      if ( dust_sect )
      {
        dust_sect=0;
        if ( mincl<=C->r_col )          //����뢠��  dust-ᥪ��
        {
          if ( maxcl>C->r_col ) x=mincl-rastlc;
          else                  x=maxcl-rastlc;


          close_ds(seci,x,(int16_t)(ncut-1));
//          close_ds(seci,MAX(mincl,C->r_col-1)-rastlc,ncut-1);
//          close_ds(seci,MIN(maxcl,C->r_col-1)-rastlc,ncut-1);
          ncut++;
		  seci++;

          if ( ncut==MAX_CUTS )
			  break;
        }
        else
		{
			if (mincl==maxcl && ((seci-1)->x + rastlc + mincl)>>1 < C->r_col)
			{
				//����뢠��  dust-ᥪ��
			  close_ds(seci,(int16_t)(mincl-rastlc),(int16_t)(ncut-1));
			  ncut++;
			  seci++;

			  if ( ncut==MAX_CUTS )
				  break;
			}
//        else                            //dust-ᥪ�� ��㫨�����
//          (seci-1)->x=((seci-1)->x+C->r_col-rastlc)>>1;
		}
      }
      else  // !dust_sect
	  {

        if ( maxcl>C->r_col )           //���嫥��
        {
//          x=((seci-1)->x+C->r_col-rastlc)>>1;
//          ro=middle(C)-rastlc;
//          (seci-1)->x=MIN(x,ro);
          if ( lefter(C,(seci-1)->x+rastlc) )  //��४�뢠���� �।��騬
          {                        // "����訬" - ��室���� ��� � dust'��
            maxcl=MAX(maxcl,C->r_col+C->w); continue;
          }
          else
            (seci-1)->x=((seci-1)->x+C->r_col-rastlc)>>1;
        }
        else
          (seci-1)->x=maxcl-rastlc;
	  }

      maxcl=MAX(maxcl,C->r_col+C->w);
      nc=0;
      if ( bad(C) &&
           ( C->w > cut_width ||
             C->r_col < (seci-1)->x+rastlc ||
             C->r_col+C->w > C->nextl->r_col ) //��४�뢠���� � �ᥤﬨ
         )
      {
		  //०��
        nc=get_cuts(C,seci,(int16_t)(MAX_CUTS-ncut-1));

        for ( j=ncut; j<ncut+nc; j++ )         //���४��㥬 �� ���������
          cor_sect(C,&cut_list[j],rastlc,rastdr);  //cell'�� � ����

        ncut+=nc;
		seci+=nc;
      }

      nc=ncut-nc-1;                     //�祭�� ᫥�� �� C
      fict_sect(seci,(int16_t)(C->r_col+C->w-rastlc),nc);
      if (nc==0)
      {
        cut_list->gvarm = C->cg_flag;          //�ਧ��� ࠧ१�
        cut_list->gvarr = C->cg_flag_fine;     //⨯ ࠧ१�
      }
      seci->lv.v2 = C->row;
      seci->lv.v3 = C->col;
      seci->rv.v2 = C->row+C->h;
      seci->rv.v3 = C->col+C->w;
      ncut++;
	  seci++;

      if ( ncut==MAX_CUTS )
		  break;
    }

  }

  if ( dust_sect )                 //��᫥���� ᥪ�� -  dust-ᥪ��
  {
    close_ds(seci,(int16_t)(maxcl-rastlc),(int16_t)(ncut-1));
    ncut++;
  }
  else
  {
    ro=maxcl-rastlc;  (seci-1)->x=MIN(ro,127);
  }
  cut_list->x=0;      //����� �ᯮ������ �� ���뢠��� ��� ���㫨஢����
                      //��ࢮ� dust-ᥪ樨

//��।��塞 cell'�  �� �祭��

  for ( j=0; j<list->N; j++ )
  {
    C=list->cells[j];
    for ( i=1,seci=cut_list+1; i<ncut; i++,seci++ )
      if ( seci->dh == 0 && lefter(C,seci->x+rastlc) ) break;
    if ( i==ncut ) continue;        //ࠧ१�� �� 墠⨫�

    seci->duflm++;                  //�᫮ ��������� � ᥣ����
    if (dust(C))
    {
      if ( is_defis(C) )
      {
        C->nvers=1; C->vers[0].let='-';  save_vers(C,&seci->versm);
      }
    }
    else
      if (seci->rv.v1++)  // ����� 1 "����讣�" - ���㫨�㥬 ���ᨨ
      {
        seci->versm.flg=0;  set_bad_vers(&seci->versm);
      }
      else
        if (!just(C)) save_vers(C,&seci->versm);//just(C) �� �� �ᯮ��������
    seci->lv.v2 = MIN(seci->lv.v2,C->row);
    seci->lv.v3 = MIN(seci->lv.v3,C->col);
    seci->rv.v2 = MAX(seci->rv.v2,C->row+C->h);
    seci->rv.v3 = MAX(seci->rv.v3,C->col+C->w);
    if (seci->px==0) cut_list->gvarm |= C->cg_flag & c_cg_cutl;
    if (i=ncut-1)    cut_list->gvarm |= C->cg_flag & c_cg_cutr;
  }

//����塞 ����

  for ( i=1; i<ncut; i++ )
  {
    seci=cut_list+i;
    versi=&seci->versm;
    if ( seci->dh == 0 )    //�࠭�� ��室���� cell'�
    {
      char isdust;

      C=&box;
      C->w=seci->rv.v3-seci->lv.v3;
      C->row=seci->lv.v2;
      C->col=seci->lv.v3;
      C->h=seci->rv.v2-seci->lv.v2;
      if (--seci->duflm)    //���� ᮯ������騥 dust'�
        isdust=C->row>my_bases.bm || C->row+C->h<my_bases.b2 ||
               if_dust(C)&0xC;
      else
        isdust=dust(versi);
      if (isdust)
        if (C->w<minp)       ro=0;
        else                 ro=norm(PASS_PEN,my_bases.ps);
      else
        if ( versi->nvers )  ro=norm(loc_err(versi),C->w);
        else                 ro=norm(MAX_RO,C->w);
      seci->rv.v1=ro;
      seci->rv.v2=C->w;
      seci->lv.v1=cut_list[seci->px].lv.v1+ro;
      if (versi->flg != 0)           //�ᯮ�������� - ��࠭塞
      {
        (*vers_list)->px=seci->px;
        if (seci->duflm)  ro=-ro;    //�⮡� �ᯮ������� � dust'���
        (*vers_list)->ro=ro;
        (*vers_list)->gvar=0;
        (*vers_list)->width=C->w;
        memcpy(&(*vers_list)->vers,versi,sizeof(SVERS));
        (*vers_list)->next=*(vers_list+i);
        *(vers_list+i)=*vers_list;
        (*vers_list)++;
      }
      seci=cut_list+(seci->px);
      seci->rv.v3=C->col-(seci->lv.v3+seci->rv.v2); //�ਭ� ��ᢥ�
    }
    else                  //ࠧ१ ��室���� cell'�
    {
      seci->lv.v1=seci->rv.v1=MAXINT;  //��� ᥣ���� (0,i)
      seci->rv.v3=0;       //�ਭ� ��ᢥ� � i-⮬ �祭��
    }
  }
  seci=cut_list+ncut;
  (seci--)->x=127;
  seci->rv.v3=MAX_COL;
  cut_list[0].rv.v3=MAX_COL;
  cut_list[0].rv.v1=seci->lv.v1;    //��� �ᥣ� ���
  return ncut;
}

/*-----------------02-28-95 01:51pm------------------------------------
  fict_sect  ��⠭�������� �祭�� ����� ����⠬�
             (ࠧ१ 䠪��᪨ �� �ந��������);
---------------------------------------------------------------------*/
static void fict_sect(struct cut_elm *cut, int16_t x, int16_t px)
{
//   init_sect(cut);
//   cut->x=x; cut->dh=0; cut->h=0;  cut->px=px;
  memset(cut,0,sizeof(struct cut_elm));
  cut->x=MIN(x,127); cut->px=(char)px;
  cut->lv.v2=cut->lv.v3=MAXINT;  cut->rv.v2=cut->rv.v3=MININT;
}

/*-----------------03-09-95 02:34pm----------------------------------
  init_sect  ��砫�� ���祭�� ��� ����⥫� �祭��
-------------------------------------------------------------------*/
static void init_sect(struct cut_elm *cpnti)
{
  cpnti->px=0;                 //�।��饥 �祭�� �� ��⨬��쭮� ���
  cpnti->lv.v1=cpnti->lv.v3=0; //����������� ���
  cpnti->lv.v2=0;              //����� ���
  cpnti->rv.v1=0;
  cpnti->var=0; cpnti->x=127;
  cpnti->duflm=cpnti->duflr=cpnti->gvarr=cpnti->gvarm=0;
  cpnti->versm.nvers=cpnti->versm.flg=0;
}

/*-----------------05-23-95 12:28pm----------------------------------
  close_ds  ����뢠�� dust-ᥪ��, �������� �祭�� *cut;
            x - ����न��� �祭�� cut � ����;
            px - ����� �祭�� - ��砫� dust-ᥪ樨;
-------------------------------------------------------------------*/
static void close_ds(struct cut_elm *cut, int16_t x, int16_t px)
{
  fict_sect(cut,x,px);
//  memset(&cut->versm,0,sizeof(SVERS));
  set_dust(&cut->versm);  set_bad_vers(&cut->versm);
  cut->duflr=1;
}

/*-----------------02-28-95 02:14pm------------------------------------
  cor_sect  ���४���� ��������� ࠧ१�� cell'� *C �⭮�⥫쭮
            ᮢ��㯭��� ���� � ���न��⠬� left � down
----------------------------------------------------------------------*/
static void cor_sect(cell *C, struct cut_elm *cut, int16_t left, int16_t down)
{
   cut->x+=C->r_col-left;
   cut->h+=down-(C->r_row+C->h-1);
}

/*-----------------02-23-95 04:13pm--------------------------------------
  get_cuts  �ନ��� ᯨ᮪ ࠧ१�� ��� cell'� (���ᨬ� nmax);
            �����頥� �᫮ �⮡࠭��� ࠧ१�� (>=0)
-----------------------------------------------------------------------*/
int16_t get_cuts (cell *C, struct cut_elm *list, int16_t nmax )
{
  int16_t i;             //��ࠬ��� 横��
  int16_t mw,mh,mrr,mrc; //ࠧ���� � ���न���� �����
  uchar *rp1;         //���� ��� ���᪠ �祭��
  int16_t  N0,N;         //������⢮ �祭��: ��� � �⮡࠭���
  struct cut_elm list0[MAX_CUTS];  //����� ᯨ᮪ �祭��
  struct cut_elm *cpnti,*spnti;    //��室�� � �⮡࠭�� �祭��
  char x_prev,v_prev; //���न��� � ⨯ ࠧ१� �।��饣� �祭��
//  uchar raster[RASTER_WIDTH*RASTER_HEIGHT/8];  //���� ��� ࠧ१����
  MN *mn1;
  uchar csv[32];       //��ࠬ���� �祭��
  cut_pos cpos;

 //��砫�� ���祭��

 mh=C->h; mw=C->w; mrr=C->r_row; mrc=C->r_col;

  if( mw<1 )
     return 0;  // OLEG: 16-04-2003

 for (i=0,cpnti=list0; i<MAX_CUTS; i++,cpnti++) init_sect(cpnti);

//���᫥��� �祪 ࠧ१�

 rp1=(uchar*)save_raster(C);

// clock_on();


 if( !Alik )
  N0=cut_points(mw, mh, rp1, list0);
 else
  N0=Alik_cut_points(mw, mh, rp1, list0, C->row); // ALIK : 01-31-96 05:50pm


// t=clock_read();
// wr_prot ("time",'d',t);

 N0 = MIN (N0,MAX_CUTS);

//�⡨ࠥ� �����⨬�

 x_prev=list0->x; v_prev=list0->var;
 for (i=1,                          //0-� ࠧ१ 䨪⨢��
      N=0,spnti=list,cpnti=list0+1; i<=N0 && N<nmax; i++,cpnti++)
 {

   if ( cpnti->x >= C->w-2 )  continue;          //������ � ���
   if ( cpnti->x <= 1 )       continue;          //������ � ���
   mn1=cut_rast(rp1,mw,mh,mrr,mrc,list0,(char)i,2,csv,&cpos); //०��
   if ( mn1 )
   {
     memcpy(spnti,cpnti,sizeof(struct cut_elm));
     if ( !mn1->mnnext )
     {                           //�� ࠧ१�����,
       spnti->var |= 32;         //��⠭�������� ����� ࠧ१
       spnti->h=spnti->dh=(char)mh;
       if ( spnti->x != x_prev || (spnti->var^v_prev) & 40 )
         { N++; spnti++; }       //�� ������� �।��騩 - ����砥�
     }
     else
       { N++; spnti++; }
     x_prev=cpnti->x;  v_prev=cpnti->var;
   }
   cut_rast(rp1,mw,mh,mrr,mrc,list0,(char)i,0,csv,&cpos); //����� ���⭮
 }
 return N;
}

/*-------------------------------------------------------------------
  dp_passi  i-� ��室 ��: ��訥 �����஢���� ᥣ����� �ࠢ� �
            ᫥�� �ய�᪠��, �� ����� - �� ࠧ१�; ncut - �᫮
            �祪 � cut_list; ��⠫�� ��ࠬ���� ��� glob_measure
-------------------------------------------------------------------*/
static char dp_passi(cell *LC, raster *r, struct cut_elm *cut_list,
                     seg_vers **vers_list, int16_t ncut, int16_t pass)
{
  int16_t i,j;
  int16_t ib=0,ie=ncut-1;             //��砫� � ����� ��

  dp_bound(cut_list,vers_list,pass,&ib,&ie);
  if (ie<=ib)  return 1;                  //�� �����祭�

#ifndef MY_DEBUG
  if (det_trace)
#endif
  {
    sprintf(snap_text,"pass=%d",pass);
    cg_show_rast(LC,r,snap_text,cut_list); //���� - �� �࠭
  }

//����塞 ����

  for ( i=ib+1; i<=ie; i++ )
  {
    if ( !ben_cut(cut_list+i) )  //ࠧ�襭 �� ������ ��室�
    {
      snap_newpoint(i);
      for (j=i-1; j>=ib; j--)
        if ( !ben_cut(cut_list+j) )
          if (addij(LC,r,cut_list,vers_list,ncut,j,i,0) & 8)  break; //8-�ப��
    }
  }
  return 0;
}

/*--------------------------------------------------------------------------
  dp_bound  ���������� ����� ib � ������ ie ������� �� � ��������� �������;
            cut_list - ������ ncut �������;
            vers_list - ������ ����������� ��� ������� �������;
            pass - ����� ������� ��;
--------------------------------------------------------------------------*/
void dp_bound(struct cut_elm *cut_list,seg_vers **vers_list,
                     int16_t pass, int16_t *ib, int16_t *ie)
{
  int16_t i,ip,ibp,i1,i2,i2p,et,type,x,ie1=*ie,ib1=*ib;
  struct cut_elm *cut,*cut1;
  SVERS *versi;   //���ᨨ i-�� �祭��
  uchar let,let1;
  char  resstr[RESLEN],*res; //�������� १���� ��ꥤ������ ᥣ���⮢

  for ( i=ib1; i<ie1; i++ )  cut_list[i].var |= NOT_CUT;  //�� १���

  {
    ip=i; *ie=ibp=0;
    i2=i2p=0; //��᫥���� ��訩 � ⥪�饬 � �।��饬 ᥣ�����
    while ( i>ib1 )
    {
      //��।��塞 ⨯ �����஢������ ᥣ����:
      // 0 - ��訩, 1 - ���宩, 2 - dust-ᥪ��
      type=0; i1=i; cut=cut_list+i;
      et=0;                      //�ࠢ� �ࠩ "�"
      do
      {
        seg_vers *cur_vers;
        cut->var &= ~NOT_CUT;    //ࠧ�蠥� ࠧ१ �� ��⨬��쭮�� ���
        versi=&cut->versm;
        if ( dust(versi) )
        {
          if (pass==1) type=1;
        }
        else
          if ( bad(versi)
               ||
               !( versi->nvers>0 && versi->vers[0].prob>trs2 )
               ||                //�� �ᯮ�������� � dust'���
               (cur_vers=find_vers(cut->px,i,vers_list)) && cur_vers->ro<0
             )  type=1;
          else
          {
            let=versi->vers[0].let;
            cut1=cut_list+cut->px;
            switch(let)
            {
              struct cut_elm *cuti;
              char weakp;

              case '|':
              case (uchar)'�': case (uchar)'�':
              if (pass==1 && strchr("���C�",	// "���C��"
								(cut_list+ip)->versm.vers[0].let) &&
                                //"��","��" � �.�.
				  !is_russian_baltic_conflict((cut_list+ip)->versm.vers[0].let) &&	// 17.07.2001 E.P.
				  !is_russian_turkish_conflict((cut_list+ip)->versm.vers[0].let)	// 21.05.2002 E.P.
				 )
              {
                if (et==0) et=ip;
                (cut_list+ip)->var &= ~NOT_CUT;       //����� १���
                if (let=='|')
                {                //ࠧ१� ����� "�"
                  cut1->var &= ~NOT_CUT;
                  if (cut1 != cut_list)
                    (cut_list+cut1->px)->var &= ~NOT_CUT;
                }
                else             //१��� "�"
                  for (cuti=cut1; cuti<cut; cuti++)  cuti->var &= ~NOT_CUT;
              }
              break;

              case (uchar)'�':
				  if (is_russian_turkish_conflict(let)) // 21.05.2002 E.P.
					  break;
			  case (uchar)'�':
              if ( cut1->dh != 0 &&
                   ((let1=cut1->versm.vers[0].let)==(uchar)'�' || let1==(uchar)'�') &&
                   not_connect_sect(cut->px,i,cut_list)
                 )  type=1;
              break;

              case (uchar)'�': case (uchar)'�':
              if ( cut1->dh != 0 )  //"�"->"�"
              {
                if (et==0) et=i;
                cut->var &= ~NOT_CUT;       //����� १���
                (cut_list+cut1->px)->var &= ~NOT_CUT;
                x=cut->x;                   //����� "�"
                x=cut1->x-((x-cut1->x)>>1); //��������� ��砫� "�"
                cuti=cut1-1;
                while (cuti->dh != 0 && cuti->x>x)
                {
                  if (point_fit(cuti,pass))  cuti->var &= ~NOT_CUT;
                  cuti--;
                }
              }
              i2=i;
              break;

              case (uchar)'�': case (uchar)'�':   //⮫쪮 ������ ࠧ१� ����� -
              weakp=0;              // "�" ����������
              for (cuti=cut1+1; cuti<cut; cuti++)
                if (cuti->var & 40)   weakp=1;
                else                { weakp=0;  break; }
              if (weakp)  type=1;
              else        i2=i;
              break;

              default:  i2=i;
            }
          }
        ip=i;
        i=cut->px;               //᫥���騩 �� ��⨬��쭮� ���
        cut=cut_list+i;
      }
      while ( cut->dh != 0 && i>ib1 );
      if ( i == (cut_list+i1)->px && dust(versi) )
        type = (pass==1) ? 1 : 2;
      if (i<ib1)  { i=ib1; cut=cut_list+ib1; }

      //���宩 ����� १���, ��訩 - �����
      if (type)
        for ( cut1=cut_list+i1; cut1>=cut; cut1-- )
          if (point_fit(cut1,pass))  cut1->var &= ~NOT_CUT;  //����� १���

      //��⠭�������� �࠭��� ��
      switch ( type )
      {
        case 0: if (et != 0)
                {
                  if (*ie==0) *ie=et;
                  *ib=i; ibp=ip;
                }
                break;
        case 1: *ib=i; ibp=ip;
                if ( *ie==0 )
                  if ( i2p )
                  {
                    *ie=(cut_list+i2p)->px;
                    res=unite_list(resstr,(cut_list+*ie)->px,i2p,
                                   cut_list,vers_list);
                    if ( !res || strlen(res) != 0 )  *ie=i2p;
                    (cut_list+*ie)->var &= ~NOT_CUT;  //����� १���
                  }
                  else    *ie=ie1;
                break;
        case 2: if (*ib==i1)  {  *ib=i; ibp=ip;  }
      }
      i2p=i2;
    } //end while i>0
/*
    if (pass==1)
      for (cut=cut_list+(*ib+1),cut1=cut_list+*ie; cut<cut1; cut++)
        if (cut->dh==0)  cut->var &= ~NOT_CUT;
*/
    //������塞 ᫥�� ��訩 �� ᯨ᪠

    if ( *ib > ib1 )
    {
      cut=cut_list+(*ib);  i1=cut->px;
      if (i1>=ib1 && strchr(letters_left_to_bad,cut->versm.vers[0].let) &&
		  !is_russian_baltic_conflict(cut->versm.vers[0].let) &&// 17.07.2001 E.P.
		  !is_russian_turkish_conflict(cut->versm.vers[0].let)	// 21.05.2002 E.P.
		  )
      {
        *ib=i1;  cut_list[*ib].var  &= ~NOT_CUT;  //����� १���
      }
    }
  }
}

/*-----------------04-30-96 11:00am----------------------------------
  addij ���������� ������� (i1,i0), ���� ��� �� ������������� (���
        mode=2 ���������� �� ���� � i1) � ������������ �����������
        ���� ������� �� ������ *cut_list (ncut �����) ������ *r0;
        ���������� ��� ���������� accept_segment
        vers_list - ������ ���������� �� ������ ��������� ����������� ���
        ������� ��������;
        *C ������ �������������� ������
--------------------------------------------------------------------*/
uchar addij(cell *C, raster *r0, struct cut_elm *cut_list,
           seg_vers **vers_list, int16_t ncut, int16_t i1, int16_t i0, char mode)
{
  int16_t i,j,ro;
  int16_t ie=ncut-1;
  uchar let1;
  seg_vers *cur_vers;
  struct cut_elm *seci0,*seci,*secj; //����⥫� �祭��
  uchar let0;       //⥪��� ����� i0-�� �祭��
  char msg[500],*s;
  uchar err=accept_segment(C,r0,cut_list,vers_list,i1,i0,mode);

  if (err) return err;

//���४��㥬 ���������� ����

  seci0=cut_list+i0;
  seci0->gvarr=1;                         //���ᨨ ����������
  for (seci=cut_list+ie-1; seci>seci0; seci--)  seci->gvarr=0;
  let0= (seci0->versm.nvers) ? seci0->versm.vers[0].let : 0;

  for (i=i0+1,seci=cut_list+i; i<=ie; i++,seci++)
  {
    char flngp;
    int16_t ip=seci->px;
    seci->gvarr=(cut_list+ip)->gvarr;
    let1= (seci->versm.nvers) ? seci->versm.vers[0].let : bad_char;
    if ( ip>=i0)
    {
      if ( ip==i0 &&
           ( let0==(uchar)'�' && let1 != (uchar)'|' &&
             (seci->x-seci0->x) < (seci0->x-(cut_list+seci0->px)->x)
             ||
             let1==(uchar)'|' && let0 != (uchar)'�'
         ) )
      {                   //⥪��� versi ������ ���� ��������
        int16_t width=seci->x-seci0->x;
        width=MAX(width,my_bases.ps);
        seci->rv.v1=norm(MAX_RO,width);
        seci->lv.v1=seci0->lv.v1+seci->rv.v1;
        if (accept_segment(C,r0,cut_list,vers_list,i0,i,1)==0)
        {                            //����� ��������
          let1= (seci->versm.nvers) ? seci->versm.vers[0].let : bad_char;
        }
      }
      else
        seci->lv.v1=(cut_list+ip)->lv.v1+seci->rv.v1;
    }
    flngp = strchr("����",let1) == NULL;
    for (j=i-1,secj=cut_list+j; j>=i0; j--,secj--)
    {
      int16_t cur_meas;
      if (!secj->versm.flg)  continue;         //�� j ��� ���
      if (seci->x-secj->x > r0->h<<1)  break;
      if (!secj->gvarr ||                      //���ᨨ �� ����������
          !(cur_vers=find_vers(j,i,vers_list)) //�� �ᯮ��������
         )  continue;
      if ( flngp || j>=ip)
        if ( secj->lv.v1>seci->lv.v1 || secj->lv.v1>cut_list->rv.v1 )
          continue;                            //�業�� �� ������

      ro=abs(cur_vers->ro);
      cur_meas=secj->lv.v1+ro;         //��� i-�� �祭��
      if ( accept_vers(msg,cut_list,j,i,&cur_vers->vers,cur_vers->gvar,
                       cur_vers->width,cur_meas) )
      {
        seci->lv.v1=cur_meas;
        seci->rv.v1=ro;
        seci->rv.v2=cur_vers->width;
        seci->px=(char)j;
        seci->gvarm=cur_vers->gvar;
        memcpy(&seci->versm,&cur_vers->vers,sizeof(SVERS));
        seci->gvarr=1;                           //���ᨨ ����������
        let1= (seci->versm.nvers) ? seci->versm.vers[0].let : bad_char;
        flngp = strchr("����",let1) == NULL;
      }
    }
  }
  cut_list->rv.v1=(cut_list+ie)->lv.v1;
  if (debug_on)
#ifndef MY_DEBUG
    if (det_trace)
#endif
    {
      s=msg+sprintf(msg,"measures corrected");
      *show_dp(s,cut_list,(int16_t)(MIN(i0+10,ie)))=0;
      show_and_wait(msg);
    }
  return 0;
}

/*----------------------04-30-96 02:03pm-----------------------------
  accept_segment - �ᯮ����� ᥣ���� (i1,i0) ���� *r0, �������
                   ᯨ᪮� *cut_list;
                   mode=XX
                        ��Ĩ����஢��� �०��� ���ᨨ ᥣ����
                        ��Ĩ����஢��� i1-� ����;
                   vers_list - ���ᨢ 㪠��⥫�� ��� ����� १���⮢;
                   *C ������ ��ࠡ��뢠���� ��ப�;
                   ��� �����襭�� - �㬬� ���祭��:
                   0 - ����� �ਭ��             8 - ᫨誮� �ப��
                   1 - 㦥 �ᯮ����             16 - �� i1 ��� ���
                   2 - १���� ��� � ᯨ᪥   32 - �㦥 ���
                   4 - ᫨誮� 㧪��
-------------------------------------------------------------------*/
static uchar accept_segment(cell *C, raster *r0, struct cut_elm *cut_list,
                           seg_vers **vers_list, int16_t i1, int16_t i0, char mode)
{
  int16_t i,dh,fora;
  uchar let;
  cell **CI;
  seg_vers *cur_vers;
  uchar cut_fl;    //䫠�: ᥣ���� (i1,i0) ����祭 ࠧ१�����
  uchar cut_fine;  //� �� ��� �����
  struct cut_elm *seci0,*seci1; //����⥫� i0-�� � i1-�� �祭��
  SVERS *versi0,*versi1;        //���ᨨ i0-��,i1-�� �祭��
  int16_t x0,x1;       //���न���� i0-�� � i1-�� �祭��
  int16_t x0a,x1a;     //� �� ��᮫���
  SVERS vers;      //���ᨨ ᥣ���� (i1,i0)
  int16_t width;       //��� �ਭ�
  char gvar;       //ᯮᮡ ᡮન
  int16_t ro,cur_meas; //���� �����쭠� � ������쭠� ⥪�騥 ���� i0-�� �祭��
#define ROI_LEN 4
#define ROI_SIZE ROI_LEN*sizeof(int16_t)
  int16_t roi[ROI_LEN]; //��⠢���騥 ro
  uchar csv0[32],csv1[32];       //��ࠬ���� �祭��
  cut_pos    cpos0={0},cpos1={0},cposc;
  MN *mn1;         //�८�ࠧ������ ����
  void *kit;       //㪠��⥫� �⥪� kit
  cell_list left_list,right_list; //ᯨ᪨ cell'�� ᫥�� � �ࠢ� �� �祭�� i1
  int16_t d_row;       //ࠧ����� ���न��� ॠ�쭮� � ������⮩ ��ப� �
                   //�祭�� i0
  char  resstr[RESLEN],*res; //�������� १���� ��ꥤ������ ᥣ���⮢
  uchar  retv=0;    //�����頥��� ���祭��
  char msg[500],*s=msg;

  s += sprintf(s,"%d-%d ",i1,i0);
  memset(&vers,0,sizeof(SVERS));
  memset(roi,0,ROI_SIZE);
  seci0=&cut_list[i0];
  x0=seci0->x;
  versi0=&seci0->versm;
  seci1=&cut_list[i1];
  x1=seci1->x;
  if ( i1>=i0 || x0-x1-1 < minw && seci1->dh != 0 && seci0->dh != 0 )
  {
    s += sprintf(s,"too thick");
    retv=4; goto ret;    //᫨誮� 㧪��
  }

//�饬 �।� ࠭�� �ᯮ�������

  if ( cur_vers=find_vers(i1,i0,vers_list) )
    if (cur_vers->ro<0)      //���� �ᯮ����� � dust'���
    {
      cur_vers->ro=-cur_vers->ro;
      width=cur_vers->width; //�� ��砩, �᫨ ��⠭���� ���� �����
      gvar=cur_vers->gvar;
      memcpy(&vers,&cur_vers->vers,sizeof(SVERS));
    }
    else                     //㦥 �ᯮ��������
      if (!(mode & 1))  return 1;

//����� �� ������ ����?

  if (i1>0 && !(mode & 2) && seci1->versm.flg)
  {
    let= (versi0->nvers) ? versi0->vers[0].let : 0;
    if (let == (uchar)'�' || let == (uchar)'�') fora=G_to_T;  //�� ��� '�'
    else if ( let == (uchar)'�' || let == (uchar)'�') fora=P_to_L;  //�� ��� '�'
         else                             fora=0;
    fora=seci1->lv.v1-norm(fora,x0-x1);
    if ( fora>seci0->lv.v1 || fora>cut_list->rv.v1 )
    {
      s += sprintf(s,"already worse");  retv=32; goto ret;
    }
  }

//�᫨ ��� �ᯮ������ ᥬ��� ��ꥤ������ � ��㣨�, �� ��ꥤ������
//�� �।�ᬮ�७� ᯨ᪮� - �ய�᪠��

  res=unite_list(resstr,i1,i0,cut_list,vers_list);
  if (res )
    if (strlen(res)==0)
    {
       s += sprintf(s,"not registered");  { retv=2; goto ret; }
    }

  versi1=&seci1->versm;
  x1a=x1+r0->left;
  x0a=x0+r0->left;

//ࠧ१���

  kit=give_kit_addr();
  cut_fl= ( seci0->dh != 0 ) ? c_cg_cutr : 0; //ࠧ१ �ࠢ�
  mn1=cut_rast((puchar)&r0->pict,r0->w,r0->h,r0->top,r0->left,cut_list,(char)i0,1,
               csv0,&cpos0);
  d_row=(int32_t)nIncline*(r0->left+x0)/2048;
  cpos0.row1 -= d_row;  cpos0.row2 -= d_row;   //��� forbid_stick_cut

  if ( seci1->dh != 0 ) cut_fl |= c_cg_cutl; //ࠧ१ ᫥��
  mn1=cut_rast((puchar)&r0->pict,r0->w,r0->h,r0->top,r0->left,cut_list,(char)i1,2,
               csv1,&cpos1);
  cpos1.row1 -= d_row;  cpos1.row2 -= d_row;

//����⠭�������� ����

  cut_rast((puchar)&r0->pict,r0->w,r0->h,r0->top,r0->left,cut_list,(char)i0,0,csv0,&cposc);
  cut_rast((puchar)&r0->pict,r0->w,r0->h,r0->top,r0->left,cut_list,(char)i1,0,csv1,&cposc);

//�뤥�塞 cell'� �� mn1

  dh=select_cells(C,mn1,x1a,x0a,cut_fl,&left_list,&right_list);
  if ( x0-x1-1-MAX(seci1->rv.v3,0) > (dh<<1)+(dh>>2) )   //᫨誮� �ப��
  {
    for ( i=0; i<left_list.N;   i++ )  del_cell(left_list.cells[i]);
    for ( i=0; i<right_list.N;  i++ )  del_cell(right_list.cells[i]);
    take_kit_addr(kit);
    s += sprintf(s,"too wide");
    retv=8; goto ret;
  }

//�⮡� �ᯮ������� ����� ��ᢠ����� ����� '�'

  let= (versi1->nvers) ? versi1->vers[0].let : 0;
  if ( let==(uchar)'�' )
    for ( i=0,CI=left_list.cells; i<left_list.N; i++,CI++ )
      if (!dust(*CI)) rest_vers(*CI,versi1);

//������ 䫠�

  if (cut_fl)
  {
    cell B;
    if (cut_fl & c_cg_cutr) cut_fine = (cut_fl & c_cg_cutl) ? 2 : 1;
    else                    cut_fine = 0;
    Alik_def_cut_flg(&B,seci0,seci1,r0->h,cut_fine);
    cut_fine=B.cg_flag_fine;
  }
   else
     cut_fine = 0;

//�ᯮ�����

  ro=recogij(C,right_list.cells,right_list.N,cut_fl,cut_fine,
             cut_list->rv.v2,roi,&gvar,&vers,&width);

  for ( i=0; i<left_list.N; i++ )  del_cell(left_list.cells[i]);
  take_kit_addr(kit);

  if ( vers.flg==0 )            //�� �ᯮ��������
  {
    if (debug_on) snap_show_raster(NULL,0,0);   //��࠭�� ���⨭��
    width=x0-x1;
    if ( i1 != i0-1 )
    {
      set_bad(&vers);  set_bad_vers(&vers);  ro=MAX_RO;
    }
    else
    {
      set_dust(&vers);  set_bad_vers(&vers);
      if (width>=minp) ro=PASS_PEN;
      else             ro=0;
      if (cut_fl) roi[3] += CUT_PEN<<1;
    }
  }

  if ( x0-x1 > width+(blank<<1) )  //�訡�� ࠧ१����
  {
    width=x0-x1;
    set_bad_vers(&vers);  ro=MAX_RO;
  }

  let=vers.vers[0].let;

//१���� ������ ���� � ᯨ᪥, �᫨ ⠪���� �������
  if (res)
    if ( vers.nvers && !strchr(res,let) )
    {
       ro=MAX_RO;
       s += sprintf (s,"not registered");
    }

  if ( language == LANG_RUSSIAN )
  {
    if ( width < my_bases.ps>>1 )  width=my_bases.ps>>1;
  }
  else
    if ( width < ps3 )  width=ps3;

  if (let=='m')
  {               //���� ��� 'm', �᫨ ᮡ࠭� �� ��᪮�
    if ( not_connect_sect(i1,i0,cut_list) )  roi[1] = m_nc_PEN;
  }
  else            //���� �� ᡮ�� ��१����� ��᪮�
    if (cut_fl)
    {
      i=i0;
      while( (i=not_connect_sect(i1,i,cut_list))>0 )
      {
        int16_t dr=(cut_list+i)->rv.v3;
        roi[1]=MAX(dr,0)*200/my_bases.ps;  //20 ������ �� 1/10ps
      }
    }

//���� ��� ��१����� �����
  if ( !dust(&vers) && ro<RO_BAD )
  {
    if ( cut_fl & c_cg_cutl && forbid_stick_cut(C,&vers,r0->h,&cpos1,2) )
      roi[2] += MONstick;
    if ( ro<RO_BAD &&
         cut_fl & c_cg_cutr && forbid_stick_cut(C,&vers,r0->h,&cpos0,1) )
      roi[2] += MONstick;
  }

  if ( dust(&vers) ) width=my_bases.ps;
  s += sprintf (s," ro=(%d",ro);
  for (i=1; i<ROI_LEN; i++)
  {
    int16_t x=roi[i];
    ro += x;  s += sprintf (s,"+%d",x);
  }
  ro = norm(ro,width);
  s += sprintf (s,")*%d/4=%d",width,ro);

//��࠭塞 १���� ��� ᥣ���� (i1,i0) �� ���饥

  if (!cur_vers) store_vers(NULL,vers_list,i1,i0,&vers,ro,width,gvar);

  if (!versi1->flg && i1>0)
  {
    s += sprintf (s,"no path");   //�� i1 ��� ���
    retv |= 16; goto ret;
  }
  cur_meas=seci1->lv.v1+ro;        //��� i0-�� �祭��
  if ( accept_vers(s,cut_list,i1,i0,&vers,gvar,width,cur_meas) )
  {
    if (debug_on)
#ifndef MY_DEBUG
    if (det_trace)
#endif
    {
      *show_dp(msg+strlen(msg),cut_list,i0)=0;
      show_and_wait(msg);
    }
    seci0->lv.v1=cur_meas;
    seci0->rv.v1=ro;
    seci0->rv.v2=width;
    seci0->px=(char)i1;
    seci0->gvarm=gvar;
    memcpy(versi0,&vers,sizeof(SVERS));
    if (cur_vers) store_vers(cur_vers,vers_list,i1,i0,&vers,ro,width,gvar);
    return retv;
  }
  else
    retv |= 32;

ret:
  if (debug_on)
#ifndef MY_DEBUG
  if (det_trace)
#endif
  {
    *show_dp(msg+strlen(msg),cut_list,i0)=0;
    show_and_wait(msg);
  }
  return retv;
}

/*---------------------------------------------------------------------
  unite_list - ��������� ������ ���������� ����������� ��� �����������
               ��������� ���������; ���� ����� ��������� ��������,
               ���������� NULL, � ���� ���������� ������ - ������ ������
---------------------------------------------------------------------*/
static char *unite_list(char *resstr,int16_t i1,int16_t i0,struct cut_elm *cut_list,
                        seg_vers **vers_list)
{                      // for segments of the left and right
  char  *res1,*res2;   // lists results
  SVERS *vers1,*vers2; // version
  int16_t  i=i0;           // boundary between them
  char  *res;          // res1 intersection and res2
  char  *res0=NULL;    // return value
  uchar let,pt;
  char  wrk1[RESLEN],wrk2[RESLEN];
  seg_vers *cur_vers;

  // 16.07.2001 E.P. conflict with a_bottom_accent_baltic 0xe0
  if (is_baltic_language(language))
	results_right_to_bad[9][6]=0;
  else
	results_right_to_bad[9][6] = (uchar)0xe0;  // Russian letter "p" appears in the text as "a"

  // 21.05.2002 E.P. Turkish-russian conflicts
  if (is_turkish_language(language))
	{
	results_left_to_bad[0][22] = 'k';
	results_left_to_bad[0][23] = 'k';
	results_left_to_bad[7][0] = '6';
	results_left_to_bad[7][2] = '6';
	results_left_to_bad[9][2] = 'd';
	results_left_to_bad[14][1] = '�';
	results_left_to_bad[16][0] = 0;
	results_left_to_bad[19][2] = '�';
	results_left_to_bad[20][1] = '�';
	results_left_to_bad[22][1] = '�';
	results_left_to_bad[23][0] = '�';

	results_right_to_bad[7][2]='o';
	results_right_to_bad[9][5]='b';
	results_right_to_bad[10][8]='k';
	results_right_to_bad[11][0]=0;
	results_right_to_bad[16][0]=0;
	results_right_to_bad[22][0]=0;

	}
  else
	{
	results_left_to_bad[0][22] = '�';
	results_left_to_bad[0][23] = '�';
	results_left_to_bad[7][0] = 'o';
	results_left_to_bad[7][2] = '�';
	results_left_to_bad[9][2] = '�';
	results_left_to_bad[14][1] = '�';
	results_left_to_bad[16][0] = '�';
	results_left_to_bad[19][2] = '�';
	results_left_to_bad[20][1] = '�';
	results_left_to_bad[22][1] = '�';
	results_left_to_bad[23][0] = '�';

	results_right_to_bad[7][2]='�';
	results_right_to_bad[9][5]='�';
	results_right_to_bad[10][8]='�';
	results_right_to_bad[11][0]='�';
	results_right_to_bad[16][0]='�';
	results_right_to_bad[22][0]='�';

	}

  *resstr=0;
  while( (i=not_connect_sect(i1,i,cut_list))>0 )
  {
    res=res1=res2=NULL;
    if ( (cur_vers=find_vers(i1,i,vers_list)) )
    {
      vers1=&cur_vers->vers;
      if ( !dust(vers1) && (cur_vers=find_vers(i,i0,vers_list)) )
        if ( !dust(vers2=&cur_vers->vers) )
        {
          if (vers1->nvers)
          {
            let=vers1->vers[0].let;
            pt = (let==(uchar)'�' &&
					!is_russian_turkish_conflict(let) // 21.05.2002 E.P.
				 ) ? trs2 : trg;
            if (vers1->vers[0].prob>=pt)
            {
              res1 = res_for(let,sticks_left_to_bad,letters_left_to_bad,
                             results_left_to_bad);
              if ( !res1 )  goto empty_list;      //����, �� ��� � ᯨ᪥
            }
          }
          if (vers2->nvers)
          {
            let=vers2->vers[0].let;
            pt = (let==(uchar)'�' &&
				!is_russian_turkish_conflict(let) // 21.05.2002 E.P.
				) ? trs2 : trg;
            if (vers2->vers[0].prob>=pt)
            {
              res2 = res_for(let,sticks_right_to_bad,letters_right_to_bad,
                             results_right_to_bad);
              if ( !res2 )  goto empty_list;
            }
          }
          if (res1 && res2)  res=strXcrs(wrk1,res1,res2);
          else   if (res1)   res=res1;
                 else        res=res2;
          if (res)
          {
		    // ��������� ����������� ���� �� results_left_to_bad, results_right_to_bad
		    // 17.07.2001 E.P.
			if (is_baltic_language(language) ||
				is_turkish_language(language) // 21.05.2002 E.P.
				)
			  {
			  int16_t n1,n2;
			  for(n1=0,n2=0;res[n2];n2++)
				{
				// ���������� ����������� ���
				if (is_russian_baltic_conflict(res[n2]))
					continue;

				// ���������� ����������� ��� 21.05.2002 E.P.
				if (is_russian_turkish_conflict(res[n2]))
					continue;

				// ��������� ���������
				if(n1<n2)
					res[n1++] = res[n2];
				}
			  res[n2]=0;	// ���� �� �����
			  if (!n2)
				   goto empty_list;
			  } // ����� ������� 17.07.2001 E.P.

            if (strlen(resstr)>0)
            {
              if (strlen(strXcrs(wrk2,res,resstr))==0)  goto empty_list;
              strcpy(resstr,wrk2);
            }
            else
              strcpy(resstr,res);
            res0=resstr;
          }
        }
    }
  }
  return res0;

empty_list:
  *resstr=0;
  return resstr;
}

/*--------------------------------------------------------------------
  res_for  ���������� ������ ����������� �� ������� results ��� �����
           let, ���� ��� ���������� � �������� sticks � letters, �����
           ���������� NULL
--------------------------------------------------------------------*/
static char *res_for(uchar let, uchar *sticks, uchar *letters, char **results)
{
  char *pos;

  if (sticks)
  {
    pos=strchr(sticks,let);
    if (pos || let==liga_i || // 08.09.2000 E.P.
		 language == LANG_TURKISH &&  // 30.05.2002 E.P.
			(let==i_sans_accent||let==II_dot_accent)
	   )
		return results[0];
  }
  pos=strchr(letters,let);
  if (pos &&
	  !is_russian_baltic_conflict(let) &&
	  !is_russian_turkish_conflict(let) // 21.05.2002 E.P.
	  ) 	// 17.07.2001 E.P.
	  return results[pos - (char*)letters];

  return NULL;
}

/*--------------------------------------------------------------------
  strXcrs  ��������� ������ �� ��������, ������������ ������������ �
           str1 � str2 � �������� �� � resstr; ���������� *resstr
--------------------------------------------------------------------*/
static char *strXcrs(char *resstr, char *str1, char *str2)
{
  char *res;
  uchar let;

  for (res=resstr; *str1; str1++)
  {
    let=*str1;
    if (strchr(str2,let))  *res++=let;
  }
  *res='\0';
  return resstr;
}

/*-----------------03-02-95 06:47pm-------------------------------------
  select_cells  �ନ��� cell'� �� �������� mn1, ��室�騥�� �����
                pos2, ����頥� � ��ப�, �������� *C, ��।����
                �� ᯨ᪠� left_list � right_list �⭮�⥫쭮
                ���न���� pos1 (pos1 � pos2 - ॠ���), ����砥�
                ��१���� cell'� � ᯨ᪥ right_list; �����頥�
                ����� ᯨ᪠ right_list ��� my_bases.b3
----------------------------------------------------------------------*/
static int16_t select_cells(cell *C,MN *mn1,int16_t pos1, int16_t pos2, uchar cut_fl,
                        cell_list *left_list, cell_list *right_list)
{
  int16_t i;
  int16_t minrow=my_bases.b2; //�������쭠� ��ப� ��� right_list
  int16_t li=0,ri=0;          //���稪� cell'�� ᫥�� � �ࠢ�
  cell *CI;

  for ( i=0; i<MAX_SECT && mn1; i++ )
  {
     CI=create_my_cell(mn1,C,0,0);
     CI->stick_inc=NO_INCLINE;
     if ( lefter(CI,pos1) )
       left_list->cells[li++]=CI;
     else
       if ( lefter(CI,pos2) )
       {
         //����砥� ��१����
         if (cut_fl & c_cg_cutl && CI->r_col==pos1+1)
           CI->cg_flag |= c_cg_cutl;
         if (cut_fl & c_cg_cutr && CI->r_col+CI->w==pos2)
           CI->cg_flag |= c_cg_cutr;

         if (dust(CI) && cut(CI) && CI->w==1)  //������� 䮪���
           del_cell(CI);
         else
         {
           right_list->cells[ri++]=CI;  minrow=MIN(minrow,CI->row);
         }
       }
       else
         del_cell(CI);
     mn1=mn1->mnnext;
  }
  left_list->N=li; right_list->N=ri;
  return my_bases.b3-minrow;
}

/*-----------------04-05-95 05:35pm-----------------------------------
  recognij  ��ࠧ�� cell �� ᯨ᪠ org_cells �� N cell'�� � �ᯮ�����
            ���; �����頥� ����, ��ਠ�� ��ꥤ������ gvar, ���ᨨ
            �ᯮ�������� vers � �ਭ� cell'�;
            cut_fl - cell'� ����祭� ��᫥ ࠧ१����,
            *C 㪠�뢠�� ⥪���� ��ப�; cut_fl ᮤ�ন� ���� c_cg_cutl
            � c_cg_cutr; inc - ��ࠬ��� ������� (���ᨢ�); *vers ��
            �室� ������ ᮤ�ঠ�� ���ᨨ �ᯮ�������� ��� dust'��
            (�᫨ ��� ����) ��� �� �㫨
---------------------------------------------------------------------*/
int16_t recogij(cell *C, cell **org_cells, int16_t N, uchar cut_fl,
                   uchar cut_fine,int16_t inc, int16_t *roi, uchar *gvar,
                   SVERS *vers, int16_t *width )
{
  int16_t i;
  cell *B,*B0,**P; //⥪�騩 cell
  int16_t ro =MAX_RO-vers->vers[0].prob;  //�訡�� �ᯮ��������
  int16_t rod=MAX_RO;  //� �� � dust'���
  uchar  gvard;     //��ਠ�� ᪫������� � dust'��
  SVERS versd;     //���ᨨ � dust'��
  int16_t  widthd;     //�ਭ� � dust'���
  grup box,top,bottom,far_top,far_bottom;   //����⥫� ��㯯 cell'��
  int16_t nstick=0;    //������⢮ ������ � ����� � ��室��� cell'��
  int16_t Nfar;        //������⢮ ���쭨�
  int16_t fora=0;      //�� ��� "�"
  uchar up_used=0;  //䫠�: ���孨� dust'� 㦥 �ᯮ�짮����
  uchar cut_up=0,cut_low=0,cut_far=0; //䫠��: ᮮ⢥�����騥 dust'� ��१���

  *gvar=0;

  if ( N==0 )
  {
    if (debug_on)
#ifndef MY_DEBUG
      if (det_trace)
#endif
      {
        snap_show_raster(NULL,0,0);  //��࠭�� ���⨭��
        show_and_wait("no cells");
      }
    return MAX_RO;
  }
  if ( N==1 && vers->flg==0 )
  {
    B0=org_cells[0];
    B0->cg_flag_fine=cut_fine;
    B0->cg_flag |= cut_fl;
    inter_diff(B0);
    set_erection(B0, inc);
    ro=recog_one(B0,NULL,trs2,vers);
    *width=B0->w;
    goto ret;
  }

  cg_show_list(org_cells,N,"to_recog");

//��।��塞 cell'� �� ��㯯��

  if ( !classify(org_cells,N,&box,&bottom,&top,&far_bottom,&far_top) )
  {
    vers->flg=0;
    if (debug_on)
#ifndef MY_DEBUG
      if (det_trace)
#endif
      {
        show_and_wait("unproper cells");
      }
    goto ret;
  }

//����塞 cut_* � 㤠�塞 �������

  P=org_cells+box.n;
  for (i=0; i<bottom.n; i++,P++) cut_low |= cut(*P);
  for (i=0;    i<top.n; i++,P++) cut_up  |= cut(*P);
  Nfar=far_top.n+far_bottom.n;
  for (i=0;   i<Nfar; i++,P++) { cut_up |= cut(*P); del_cell(*P); }
  N -= Nfar;
  if ( box.n>MAX_CELLS_IN_LIST-1 )
  {
    if (debug_on)
#ifndef MY_DEBUG
      if (det_trace)
#endif
      {
        show_and_wait("too many cells");
      }
    goto ret;
  }

//����� ����� ����� ������ �����

  if ( language == LANG_RUSSIAN )
    for ( i=0; i<box.n; i++ )
      if( !dust(B=org_cells[i]) )
        if (nstick<2)  nstick += is_stick(B);
        else           nstick = 3;
  *gvar = gvard = (uchar)nstick;

//�ᯮ�����

  if ( vers->flg==0 || box.n>1 )
    ro=recog_set(C,org_cells,box.n,cut_fl,cut_fine,inc,trs2,gvar,vers,width);
  if ( let_or_bad(vers) )
    switch(vers->vers[0].let)
    {
      case (uchar)'�':               //"�" � "�" ������� ��� � ��㣠
      case (uchar)'�':
      case (uchar)'�':               //"�" ������ ���� �������
      case '%':
        if ( *gvar & glued )
        {
          uchar  gvardd=2;     //��� ᪫�������
          rod=recog_set(C,org_cells,box.n,cut_fl,cut_fine,inc,trs2,
                        &gvardd,&versd,width);
          if ( rod<ro )
          {
            ro=rod;
            *gvar = gvardd;
            memcpy(vers,&versd,sizeof(SVERS));
          }
        }
      case (uchar)'�':
      case (uchar)'�':
        if ( top.n )  ro += PEN_TOP_DUST;    //�������� "�"
        break;
      case (uchar)'�':
        if ( ro<RO_DUST_OFF && top.b<my_bases.b2 && !bottom.n )  //"�"
          goto ret;
        break;
      case (uchar)'�':
		  if (is_russian_turkish_conflict(vers->vers[0].let)) // 21.05.2002 E.P.
			  break;
        if ( top.n )
        {
          //���塞 ���⠬� ᯨ᪨ top � bottom
          int16_t nd=MIN(top.n,bottom.n);
          cell *buff[MAX_CELLS_IN_LIST];
          memcpy(buff,org_cells+box.n,sizeof(cell *)*nd);
          memcpy(org_cells+box.n,org_cells+box.n+top.n,sizeof(cell *)*nd);
          memcpy(org_cells+box.n+top.n,buff,sizeof(cell *)*nd);

          fora=O_to_B;      //�� ��� "�"
          up_used=1;
          //�ᯮ����� � ���孨� dust'��
          rod=recog_set(C,org_cells,(int16_t)(box.n+top.n),cut_fl,cut_fine,
                        inc,trs2,&gvard,&versd,&widthd);
          if ( rod<ro+fora )
          {
            ro=rod;
            *gvar = gvard | up_dust;
            memcpy(vers,&versd,sizeof(SVERS));
            *width=widthd;
            if ( ro<RO_DUST_OFF )  goto ret;
          }
        }
        break;
      case '!':
      case '?':
        if ( bottom.a>my_bases.bm && bottom.b<my_bases.b3+3 )
          goto ret;
        break;
      case '�':
		if (is_russian_turkish_conflict(vers->vers[0].let)) // 21.05.2002 E.P.
			break;

        if ( bottom.n && !(vers->source & c_rs_BOX) )       //�� 3x5
          ro=recog_set(C,org_cells,box.n,cut_fl,cut_fine,
                       inc,0,gvar,vers,width);
        break;
      case 'i':
      case 'r':
        if ( ro<110 && bottom.n==0 ) goto ret;  //�⮡� �� ������ � 'l'
        break;
      default:
        if ( MAX_RO-ro > trs2 )  goto ret;
    }

  //�� �����

  if ( top.n && !up_used || bottom.n )
  {
    rod=recog_set(C,org_cells,(int16_t)(box.n+top.n+bottom.n),cut_fl,cut_fine,inc,trs2,
                  &gvard,&versd,&widthd);
    if ( let_or_bad(&versd) )
      if ( rod<ro+fora || dust(vers) )
      {
        ro=rod;
        *gvar = gvard | all;                //�� � ����� cell'�
        memcpy(vers,&versd,sizeof(SVERS));
        *width=widthd;
      }
  }

  //�᫨ ���� ����稫��� - ���믠��

  if (ro > RO_COMPOSE) *gvar=0;
  else
    if ( ro >= RO_BAD )
    {
      int16_t nc=box.n;
      if (*gvar & up_dust)  nc += top.n;
      if (*gvar & low_dust) nc += bottom.n;
      if ( nc<3 ) *gvar=0;
    }

ret:
  for ( i=0; i<N; i++ )  del_cell(org_cells[i]);
  if ( cut_fl )                    //���� �� ࠧ१
  {
    uchar let=vers->vers[0].let;
    if ( dust(vers) && let != '-' ||
         cut_up  && !(*gvar &  up_dust) && let != 'i' &&
			!(let ='�' &&
				!is_russian_turkish_conflict(let) // 21.05.2002 E.P.
			 ) ||
         cut_low && !(*gvar & low_dust) && let != '!' && let !='?' ||
         cut_far )
                               roi[3] += CUT_PEN<<1;
    if ( cut_fl & c_cg_cutr )  roi[3] += CUT_PEN;
    if ( cut_fl & c_cg_cutl )  roi[3] += CUT_PEN;
  }
  return ro;
}

/*-----------------07-21-95 01:54pm---------------------------------
  recog_one  �ᯮ����� cell B0 � �����頥� �訡�� �ᯮ�������� �
             ���ᨨ vers;
             *GL � tol - ��ࠬ���� ��� full_recog
-------------------------------------------------------------------*/
static int16_t recog_one (cell *B0, s_glue *GL, int16_t tol, SVERS *vers)
{
  int16_t ro =MAX_RO;  //�訡�� �ᯮ��������

  if ( dust(B0) )   //�� ����� - dust; �� �ᯮ�����
  {
    if (B0->w<minp)
      ro=0;
    else
    {
      ro=PASS_PEN;
      if ( is_defis(B0) )
      {
        B0->nvers=1; B0->vers[0].let='-';
      }
    }
    glsnap('a',B0,"");
  }
  else
    if (B0->w>=minw && !wide(B0))
    {
      if (full_recog(B0,GL,tol,2))  ro=loc_err(B0);
    }
    else
      glsnap('a',B0,"");
  save_vers(B0,vers);
  return ro;
}

/*----------------------------------------------------------------
  wide  �����頥� 1, �᫨ cell ᫨誮� �ப, ���� - 0
-----------------------------------------------------------------*/
/*
static char wide (cell *C)
{
  int16_t h=my_bases.b3-C->row;
  return ( C->w > (h<<1) + ((language==LANG_RUSSIAN) ? -(h>>3) : (h>>3)) );
}
*/

/*-----------------06-23-95 03:59pm-------------------------------
  recog_set  �ᯮ����� ᮢ��㯭���� N cell'�� org_cells ��� ����;
             �����頥� ���ᨨ vers � ����⭮��� ������襩;
             cut_fl 㪠�뢠��, �� cell'� ����祭� ࠧ१�����;
              inc - ��ࠬ��� ������� (���ᨢ�); gvar=glued �� �室�,
             �᫨ ᪫���� ࠧ�襭�, �� ��室� gvar - ��ਠ�� ���-
             ��������, width-�ਭ�; tolbox - ��ࠬ��� ��� full_recog;
-----------------------------------------------------------------*/
static int16_t recog_set(cell *C,cell **org_cells, int16_t N, uchar cut_fl,
                     uchar cut_fine, int16_t inc,
                     int16_t tolbox, uchar *gvar, SVERS *vers, int16_t *width)
{
  int16_t i;
  cell *B0=NULL;    //cell ��� �ᯮ��������
  s_glue GL={0};
  int16_t ro =MAX_RO;   //�訡�� �ᯮ��������
  uchar broken_II=0; //䫠�: ��� �����

  memset(vers,0,sizeof(SVERS));
  *width=0;
  if (*gvar>1)      // >1 �����
  {
    if (*gvar == 2)  broken_II=1;
    *gvar=0;        //�� ������
  }
  else
    *gvar=glued;    //������

  B0=unite(C,org_cells,N,&GL,gvar);
  if ( !B0 )  return ro;

  *width=B0->w;
  B0->cg_flag_fine=cut_fine;
  B0->cg_flag |= cut_fl;
  B0->broken_II=broken_II;
  set_erection(B0,inc);
  ro=recog_one(B0,&GL,tolbox,vers);
  if ( B0 != GL.celist[0] )  del_cell(B0);

  if ( *gvar & glued )
  {
    B0=GL.celist[0];
    for ( i=0; i<N; i++ )  org_cells[i]=rest_cell(org_cells[i],B0);
    for ( i=0; i<GL.ncell; i++ )  del_cell(GL.celist[i]);
  }

  return ro;
}

/*-----------------06-26-95 05:46pm--------------------------------
  unite  ��������� ���������� cell, �� ������ N cell'�� org_cells;
         ���� gvar==glued, �������� �������;
         �� ������:
           gvar==glued, ���� ���-������ ���������, ����� - 0;
           GL - cell'�, ���������� ����� ����������;
           org_cells - �������� cell'�, ���� �� ���������, � cell'�,
                       ����������� � kit, ���� ���������;
         ���������� ���������� cell
------------------------------------------------------------------*/
static cell *unite (cell *C, cell **org_cells, int16_t N, s_glue *GL, uchar *gvar)
{
  int16_t i;
  cell *B;
  MN *mn,*mn1;

  if ( N>MAX_CELLS_IN_LIST-1 )
  {
    if (debug_on)
#ifndef MY_DEBUG
      if (det_trace)
#endif
      {
        show_and_wait("too many cells");
      }
    return NULL;
  }

  for ( i=0; i<N; i++ )
  {
    B=org_cells[i];
    GL->celist[i]=B; GL->complist[i]=B->env;
  }
  GL->celist[N]=NULL; GL->complist[N]=NULL;
  GL->ncell=(uchar)N;

//�����

  if ( N==1 )  *gvar=0;
  if ( *gvar )
  {
    mn1=mn=glue((c_comp**)&GL->complist, 1);
    for ( i=0;  mn1; i++, mn1=mn1->mnnext );
    if ( i>0 && i<N )        //᪫������
    {
      for ( i=0;  mn; i++,mn=mn->mnnext )
      {
        B=create_my_cell(mn,C,0,0);
        GL->celist[i]=B; GL->complist[i]=B->env;
      }
      GL->ncell=(uchar)i;
      GL->celist[i]=NULL; GL->complist[i]=NULL;
      for ( i=0; i<N; i++ )
        org_cells[i]=(cell *)del_save_cell(org_cells[i]);
    }
    else
      *gvar=0;
  }

//�᫨ ��᪮�쪮 - ��ꥤ��塞

  B=GL->celist[0];
  if ( GL->ncell>1 )
  {
    *gvar |= united;
    B=comp_to_cell(B,GL->complist,GL->ncell,0,0);
  }

  inter_diff(B);

//  glsnap('a',B,"B");

  return B;
}

/*-----------------06-23-95 11:07am--------------------------------
  classify  ८࣠����� ᯨ᮪ cells �� N cell'��, ��ࠧ�� ��㯯�
            � ���浪� ����᫥���:
            -  ��ࠧ��騥 �᭮���� ���� (box);
            -  ������ dust'� (bottom);
            -  ���孨� dust'� (top);
            -  ���쭨� ������ dust'� (far_bottom);
            -  ���쭨� ���孨� dust'� (far_top);
            ��� ������ ��㯯� ���������: ������⢮ cell'�� �
            ��㯯� n � �࠭��� a � b (������ � ������);
            �����頥� 1 � ��砥 �ᯥ� � 0 � ��⨢��� ��砥
-----------------------------------------------------------------*/
static uchar classify (cell **cells, int16_t N, grup *box, grup *bottom,
                      grup *top, grup *far_bottom, grup *far_top)
{
  int16_t i;
  int16_t upper;        //������ ��ப� ����
  int16_t gb,ge;        //��砫� � ����� ��ᢥ�
  int16_t gba,gea,gma;  //� �� ���� �।��� ��᮫���
  int16_t width;               //�ਭ� ��ᢥ�: �ப��,
  int16_t wide=my_bases.ps>>1; // �᫨ �� ��� �� wide,
  int16_t mid=my_bases.ps/3;   // � � �।��� �� mid
  uchar proj[RASTER_HEIGHT+1];  //�஥��� �� ���⨪����� ���

  far_top->n=far_bottom->n=top->n=bottom->n=0;
  upper=horiz_proj(cells,N,(uchar*)&proj,RASTER_HEIGHT+1);

//�饬 ���쭨�

  far_top->a=far_top->b=upper;
  far_bottom->a=far_bottom->b=(STRRCHR(proj,'1')-((char*)proj)) + upper;
  ge=0;
  do                                   //横� �� ��ᢥ⠬
  {
    gb=STRSPN(proj+ge,"1")+ge;  //��砫� ��ᢥ�
    ge=STRSPN(proj+gb,"0")+gb;  //����� ��ᢥ� + 1
    gba=gb+upper; gea=ge+upper;
    gma=(gba+gea)>>1;
    width=ge-gb;
    if ( width>wide )           //�ப��
      if ( gma<my_bases.b2 )    //���孨� dust'�
      {
        i=0;
        while ( i<N )
          if ( cells[i]->row < gba )
          {
            change((void**)cells,i,(int16_t)(N-1)); N--; far_top->n++;
          }
          else
            i++;
        far_top->b=gba;
      }
      else
        if ( gma>my_bases.b3 )         //������ dust'�
        {
          i=0;
          while ( i<N )
            if ( cells[i]->row >= gea )
            {
              change((void**)cells,i,(int16_t)(N-1)); N--; far_bottom->n++;
            }
            else
              i++;
          far_bottom->a=gea;
          proj[gb]=0;
          break;
        }
        else                          //�ப�� ��ᢥ� � �।���
          return 0;
    else
      if ( width>=mid && gma>my_bases.b2 && gma<my_bases.b3) return 0;
  }
  while ( width>0 );

//�饬 �������

  ge=far_top->b-upper;
  ge=STRSPN(proj+ge,"0")+ge;    //���� �� ����
  top->a=top->b=ge+upper;
  bottom->a=bottom->b=(STRRCHR(proj,'1')-(char*)proj) + upper;
  do                            //横� �� ��ᢥ⠬
  {
    gb=STRSPN(proj+ge,"1")+ge;  //��砫� ��ᢥ�
    ge=STRSPN(proj+gb,"0")+gb;  //����� ��ᢥ� + 1
    gba=gb+upper; gea=ge+upper;
    width=ge-gb;
    if ( width>0 )
      if ( gba<=my_bases.b2 )           //���孨� dust'�
      {
        i=0;
        while ( i<N )
          if ( cells[i]->row < gba )
          {
            change((void**)cells,i,(int16_t)(N-1)); N--; top->n++;
            top->b=gba;
          }
          else
            i++;
      }
      else
        if ( gea>=my_bases.bm )         //������ dust'�
        {
          i=0;
          while ( i<N )
            if ( cells[i]->row >= gea )
            {
              change((void**)cells,i,(int16_t)(N-1)); N--; bottom->n++;
            }
            else
              i++;
          bottom->a=gea;
          proj[gb]=0;
          break;
        }
  }
  while ( width>0 );

  ge=top->b-upper;
  ge=STRSPN(proj+ge,"0")+ge;    //���� �� ����
  box->n=N;
  box->a=ge+upper;
  box->b=(STRRCHR(proj,'1')-(char*)proj) + upper;

  far_top->b--; top->b--;

  return 1;
}

/*-----------------06-26-95 03:27pm-----------------------------
   horiz_proj ������ �஥��� proj �� ���⨪����� ��� N
              cell'�� cells; �����頥� ᬥ饭�� ���ᨢ� proj
---------------------------------------------------------------*/
static int16_t horiz_proj(cell **cells, int16_t N, uchar *proj, int16_t size)
{
  int16_t i;
  int16_t upper=10000;   //������ �࠭�� cell'��
  int16_t beg,len;       //��砫� � ����� �஥�樨 cell'�
  cell *B;

  memset(proj,'0',size);
  proj[size-1]=0;// Piter add "-1"

//�饬 ������ �࠭��� (ᬥ饭��)

  for ( i=0; i<N; i++ )  upper=MIN(upper,(cells[i])->row);

//��ந� �஥���

  for ( i=0; i<N; i++ )
  {
    B=cells[i];
    beg=B->row-upper; len=B->h;
    if( beg<size )
        { // OLEG for CAM
        if ( beg+len>=size ) len=size-1-beg;
        memset(proj+beg,'1',len);
        }
  }

  proj[STRRCHR(proj,'1')-(char*)proj+1]=0;
  return upper;
}

/*-----------------06-26-95 04:18pm---------------------------------
  change  ����� ���⠬� i-� j-� ������ ���ᨢ� arr
------------------------------------------------------------------*/
static void change(void **arr, int16_t i, int16_t j)
{
  void *p;

  p=arr[i]; arr[i]=arr[j]; arr[j]=p;
}

/*-----------------07-07-95 11:39am-----------------------------
  accept_vers  ���������� 1, ���� ������ vers �������� (i1,i0)
               �����������, � 0 � ��������� ������;
               s - ����� ��� ������ �� �����
               res - �������������� ���������
               width - ������ �������� (i1,i0)
               gvar - ������ ������
               cur_meas - ����������� ���� � ������ ��������
                          ������ vers
----------------------------------------------------------------*/
static uchar accept_vers(char *s, struct cut_elm *cut_list,
                        int16_t i1, int16_t i0, SVERS *vers, char gvar, int16_t width,
                        int16_t cur_meas)
{
  uchar let=0,p;                        //AK let=0
  struct cut_elm *seci; SVERS *versi;
  struct cut_elm *secl,*secm,*secr=cut_list+i0;  int16_t il,im,xl,xm,xr;
                   //�����, �।��� � �ࠢ�� �祭�� � �� ���न����
  SVERS *vers0,*versl,*versr;  uchar let0,letl,letr;
                   //���ᨨ ᥣ���� (i2,i0), ��� ����� � �ࠢ�� ��⥩
  int16_t pen=0;       //����: � ����� ᡮન >0, � ����� ࠧ१���� <0
  int16_t ip=secr->px;                   //�।���� �窠 ��⨬��쭮�� ���
  static char except[]="(by exception)", rule[]="";
  char *reason=except;

  if (!secr->versm.flg)
  {
    reason=rule; goto ret1;   //�� �ᯮ��������
  }

  if (i1==ip)
  {
    uchar new_let=vers->vers[0].let;
    uchar old_let=secr->versm.vers[0].let;
    switch(new_let)
    {
  //���७�� '�' �� '�'
      case (uchar)'�':
      if ( old_let == (uchar)'�' &&
		  !is_russian_turkish_conflict(old_let) // 21.05.2002 E.P.
		  )  pen=-O_to_B;
      break;

  //���७�� '�'��  '�'
      case (uchar)'�':  case (uchar)'�':
      if ( ( old_let == (uchar)'�' || old_let == (uchar)'�')  &&
           !(gvar & (up_dust+low_dust))      //dust'� ⮫쪮 �� ��஡��
         )  pen=-40;
      break;
    }
  }
  else
  {
    if (i1<ip)  { il=i1; im=ip; vers0=vers; versr=&secr->versm; }
    else        { il=ip; im=i1; vers0=&secr->versm; versr=vers; }
    secm=cut_list+im;  secl=cut_list+il;  versl=&secm->versm;

    let0=vers0->vers[0].let;   xl=secl->x;
    letl=versl->vers[0].let;   xm=secm->x;
    letr=versr->vers[0].let;   xr=secr->x;

    switch(let0)
    {
    //���७�� '�' �� '�'
      case (uchar)'�':
		  if (is_russian_turkish_conflict(let0))
			  break; // 21.05.2002 E.P.
      case (uchar)'�':
         if ( (letr==(uchar)'�' || letr==(uchar)'�') && xm-xl < (xr-xl)>>1 )
           pen = (letl==(uchar)'�' || letl==(uchar)'�') ? 30 : G_to_T;
         break;

  //���७�� '�' �� '�'
      case (uchar)'�':  case (uchar)'�':
      if ( ( letr==(uchar)'�' || letr==(uchar)'�') && secm->dh != 0 && xm-xl <= (xr-xl)>>2
         )  pen=P_to_L;
      break;

  //"�" ��� "�"
      case (uchar)'�':
      if ( letl==(uchar)'�' &&
		  !is_russian_turkish_conflict(letl) && // 21.05.2002 E.P.
		  letr==(uchar)'�' )  pen=30;
      break;

  //"�" ��� "��"
      case (uchar)'�':
	  case (uchar)'�':
		  if (is_russian_turkish_conflict(let0)) // 21.05.2002 E.P.
			  break;
      if ( letl==(uchar)'|' || letl==liga_exm ||
		   letl==(uchar)'�' || letl==(uchar)'�' // ��
		 )
        if ( secl->dh==0 && secm->dh==0 )  //����� ������ �� ���������
        {              //��������� "�", ���� ����� ����� � "�"
          if ( secm->rv.v3 > secl->rv.v3<<1 )  pen=-IEE_IU_PEN; //����� "�"
          else                                 pen= IEE_IU_PEN; //����� "�"
        }
        else           //��������� "�", ���� ����� ��������� ������ � "�"
          if (secm->dh==0)  pen=-IEE_IU_PEN;
          else              pen= IEE_IU_PEN;
      if (pen>0 && vers0->source & 1)  pen = pen<<1;   //"�" �� ��⠬
      break;

  //����� ��� 'L', ���� ������� �� ����� � �����
      case 'L':
      if ( dust(versr) && secm->dh==0 && stick(letl) )  pen = -30;
      break;

  //����� ��� 'U', ���� ������� �� 'L' � �����
      case 'U':
      if ( secm->dh==0 && stick(letr)
           || letr=='i' && versr->vers[0].prob>I_wo_point  //'i' � �窮�
         )
        if ( letl=='l' || letl=='L' )
          pen = -2*CUT_PEN;
      break;

  //����� ��� ��������� 'H'
      case 'H':
      if ( secm->dh==0 && stick(letl) && stick(letr) )  pen = -H_PEN;
      break;

  //����� ��� 'm', ���� ������� �� 'i'
      case 'm':
      if ( letl=='i' || letr=='i' )  pen = -mi_PEN;
      break;

  // 'c-l' => 'd', �᫨ ᪫����
      case 'd':
      p=versr->vers[0].prob;
      if ( ( stick(letr) || letr=='f' && p<180 )      // 'l'
           && memchr("c(<",letl,3)                    // 'c'
         )
        if ( secm->dh != 0 )                          //'c' � 'l' ᪫����
        {
          if ( vers0->vers[0].prob>MINlet ) goto retu;//�ਭ����� 'd'
        }
        else                                          //'c' � 'l' �� ᪫����
          if ( p>MINlet && versl->vers[0].prob>MINlet )
            goto retb;                                //��⠢�塞 'c' � 'l'
      break;
    }
    if (i1<ip)  pen = -pen;
  }

  pen=norm(pen,width);
  if ( cur_meas+pen < secr->lv.v1 )
  {                              //����� ��� ����; �஢��塞 �᪫�祭��
    if (i1 != ip)
    {
    //��᪮�쪮 ���吝�� ᥪ権 ᮡࠫ��� � "���宩"
      if ( !dust(vers0) && vers0->vers[0].prob<MAX_RO-RO_COMPOSE
           && not_connect_chain(il,i0,cut_list) ) goto retb;

    //��ꥤ������ ���� ᥣ���⮢
      if (il==secm->px)
      {
      //����頥��� �ਪ������� 'i', �᫨ ����砥��� � �� �㪢�
        if ( letl==let0 &&
				(letr=='i' || letr==liga_i ||
					language == LANG_TURKISH &&  // 30.05.2002 E.P.
					(letr==i_sans_accent||letr==II_dot_accent)
				)  // X+i=X
               && versl->vers[0].prob>200 ||
             letr==let0 &&
				(letl=='i' || letl==liga_i ||
					language == LANG_TURKISH &&  // 30.05.2002 E.P.
					(letl==i_sans_accent||letl==II_dot_accent)
				)  // i+X=X
               && versr->vers[0].prob>200 )
          goto retb;
      }

      switch(let0)
      {
      //���������� "�������������" ����
        case (uchar)'�':	// '�'
        case (uchar)'�': // '�'
        case (uchar)'�':
        seci=secr;
        while ( seci>secl )
        {
          versi=&seci->versm;
          let=versi->vers[0].let;
          if ( ( let==(uchar)'�' &&
			  		!is_russian_turkish_conflict(let) // 21.05.2002 E.P.
			     ||
				let==(uchar)'�'
			   ) && versi->vers[0].prob>trs2
			 ) goto retb;

          seci=cut_list+(seci->px);
        }
        break;

      //����頥��� ������� "9" �� "�"
        case (uchar)'�':
        if ( letr=='9' && versr->vers[0].prob>=RELIABLE_9 )  goto retb;
        break;
      }
    }
    reason=rule;
    goto ret1;
  }

//����� ��� �㦥; �஢��塞 �᪫�祭��

  if (i1==ip)
  {
  //dust-ᥪ�� ᮡࠫ��� � ��-dust
    if ( dust(&secr->versm) && !dust(vers)
         && vers->vers[0].prob>=MAX_RO-RO_COMPOSE )  goto ret1;
  }
  else
  {
  //dust-ᥪ樨 ᮡࠫ��� � ��-dust
    if ( !dust(vers0) && vers0->vers[0].prob>=MAX_RO-RO_COMPOSE
         && dust_chain(il,i0,cut_list) ) goto retu;

  //����让 ����� ࠧ१�� �� 2 dust'�
    if ( let_or_bad(vers0) && dust(versr) && dust(versl) && il==secm->px
         && secm->dh != 0 )
      goto retu;
  }

  reason=rule;

  //�⢥࣠��
ret0:
    if (debug_on)
#ifndef MY_DEBUG
    if (det_trace)
#endif
      s += sprintf(s," pen=%d => decline %s",pen,reason);
    return 0;

  //�ਭ�����
ret1:
    if (debug_on)
#ifndef MY_DEBUG
    if (det_trace)
#endif
    {
      let = (vers->nvers) ? vers->vers[0].let : ' ';
      s += sprintf(s," pen=%d => accept %c %s",pen,let,reason);
    }
    return 1;

retu:    //��ꥤ�����
   if (i1<ip)  goto ret1;
   else        goto ret0;

retb:    //ࠧ१���
   if (i1<ip)  goto ret0;
   else        goto ret1;
}

/*-----------------06-15-95 03:19pm---------------------------------
  not_connect_chain  �஢���� ����稥 楯�窨 �� �����஢����� ᥪ-
                     権 �� i1-�� �� i0-�� �祭��;
                     cut_list - ᯨ᮪ �祭��
------------------------------------------------------------------*/
static uchar not_connect_chain(int16_t i1, int16_t i0, struct cut_elm *cut_list)
{
  struct cut_elm *cur;  //⥪�饥 �祭�� �� ��⨬��쭮� ���
  int16_t i;

  cur=cut_list+i0;
  i=cur->px;
  if ( (cut_list+i1)->dh != 0 || cur->dh != 0 || i<=i1)  return 0;

  while ( i>i1 )
  {
    cur=cut_list+i;
    if ( cur->dh != 0 )  return 0;  //�� ࠧ१
    i=cur->px;
  }
  if ( i==i1 ) return 1;
  else         return 0;
}

/*-----------------06-15-95 03:19pm---------------------------------
  dust_chain  �஢���� ����稥 楯�窨 �� dust-ᥪ権 ����� i1-� �
              i0-� �祭���
------------------------------------------------------------------*/
static uchar dust_chain(int16_t i1, int16_t i0, struct cut_elm *cut_list)
{
  struct cut_elm *cur,*last;

  for ( cur=&cut_list[i0], last=&cut_list[i1]; cur>last; cur-- )
   if ( !dust(&cur->versm) )  return 0;
  return 1;
}

/*-----------------05-23-95 12:45pm---------------------------------
  recover_path  ��������������� cell'� ������������ ����, ���������
                �������� cut_list �� N ������� � �������� �������� inc;
                LC,RC - ������� ��������� (�� ���.);
                ovfl - ����: ����������� ������� �� ���������� � �����;
                kita - ����� ��� ����� cell'��;
                ���������� ��������� �� cell, ���� ���� ��������� ���
                �����������; ��� ������� ���������� NULL
-------------------------------------------------------------------*/
static cell *recover_path(void *kita, raster *r, struct cut_elm *cut_list,
                          int16_t N, cell *LC, cell *RC, char ovfl)
{
  int16_t i,i0;
  cell *B;    // current cell
  int16_t ib=N-1;                       //����� �������, ���� ������������
  cell *opt_cell[MAX_SECT]={NULL};  //cell'� ������� ������������ ����
  cell *dcell[MAX_SECT];            //��� cell'� ������ ��������
  int16_t    Nd;                        //�� ����������
  grup box,top,bottom,far_top,far_bottom;   //��������� ����� cell'��
  struct cut_elm *seci,*seci0;      //��������� i-�� � i0-�� �������
  SVERS *versi0;                     //������ �������
  uchar csv[32];                     //��������� �������
  s_glue GL;
  cut_pos    cpos;
  MN *mn1;

  if( langBul && LC && LC->nvers > 0 &&
	  ( LC->vers[0].let == 154 || LC->vers[0].let ==234 ||
	    LC->vers[0].let == 156 || LC->vers[0].let ==236
	  )
	 )     // Nick 29.09.2000
  { // � ���������� ��� � - �� �������� ����� ����� �,�
	for( i0 = N-1; i0 > 0;i0 = seci0->px)
	{
     seci0=&cut_list[i0];
     versi0=&seci0->versm;
	 if( seci0->px <= 0 )
		 break;
    }

	if( i0 > 0 && versi0->vers[0].let == '|' )
	 return NULL;
  }


  if (ovfl)                    //���� ����㯨�� �����;  �㤠?
  {
    seci0=cut_list+ib;                //����㯠�� �� dust'�
    while (ib>0 && dust(&seci0->versm))
    {
      ib=seci0->px;  seci0=cut_list+ib;
    }
    if (seci0>cut_list)
    {
      char *ilet;
      version *v0;
      versi0=&seci0->versm;  v0=&versi0->vers[0];
      if (versi0->nvers && v0->prob<trs2) set_bad(versi0);
      if (seci0->px != 0)
        if ( bad(versi0) || v0->let==(uchar)'�' ||
            ( (ilet=strchr(letters_left_to_bad,v0->let)) &&
			  !is_russian_baltic_conflict(v0->let) &&	// 17.07.2001 E.P.
			  !is_russian_turkish_conflict(v0->let) &&	// 21.05.2002 E.P.
              v0->prob < prob_left_to_bad[(uchar*)ilet-letters_left_to_bad]
           ))
        {
          if (strchr("��������",v0->let) &&
			  !is_russian_turkish_conflict(v0->let) // 21.05.2002 E.P.
			  )	// "����������"
			  set_bad(versi0);
          ib=seci0->px;           //����㯠�� �� ���� ᨬ���
          seci0=cut_list+ib;      //����㯠�� �� dust'�
          while (ib>0 && dust(&seci0->versm))
          {
            ib=seci0->px;  seci0=cut_list+ib;
          }
        }
    }
  }

//������ �� ࠧ१�

    i = cut_list[N-1].px;    //(N-1)-� - 䨪⨢��
    while ( i>0 )
    {
      seci=&cut_list[i];
      mn1=cut_rast((puchar)&r->pict,r->w,r->h,r->top,r->left,cut_list,(char)i,2,csv,&cpos);
      i=seci->px;            //᫥���騩 �� ��⨬��쭮� ���
    }
    mn1=cut_rast((puchar)&r->pict,r->w,r->h,r->top,r->left,cut_list,0,3,csv,&cpos);
    if (!mn1) return NULL;

    take_kit_addr(kita);

//ᮧ���� ���������� � ��।��塞 �� �祭��

    for ( i=0; i<MAX_SECT && mn1; i++ )
    {
       B=create_my_cell(mn1,LC,0,0);
       inter_diff(B);
       set_erection(B, cut_list->rv.v2);
       i0=locate(B,cut_list,N,r->left);
       seci0=cut_list+i0;  seci=cut_list+seci0->px;
       B->cg_flag |= c_cg_cutdone;      //��ନ஢�� �ணࠬ��� cut_glue
       if ( seci->dh != 0 && B->r_col<=seci->x+r->left+1)
         B->cg_flag |= c_cg_cutl;
       if (seci0->dh != 0 && B->r_col+B->w>=seci0->x+r->left)
         B->cg_flag |= c_cg_cutr;
       B->complist=(void *)opt_cell[i0];
       opt_cell[i0]=B;
       mn1=mn1->mnnext;
    }

//��ꥤ��塞, �᫨ �㦭�, � ������塞 ���ᨨ

    i0 = N-1;
    while ( i0>0 )
    {
      seci0=&cut_list[i0];
      versi0=&seci0->versm;
        if ( let_or_bad(versi0) && (B=opt_cell[i0]) )
        {
          if ( seci0->gvarm )           //���������� ������஢�����
          {
          //�� �吝��� ᯨ᪠ - � dcell
            Nd=0;
            do
            {
              dcell[Nd++]=B;
              B=(cell *)B->complist;
            }
            while ( B );

          //�����䨪��� �� �ᯮ�������
            if ( classify(dcell,Nd,&box,&bottom,&top,&far_bottom,&far_top) )
            {
              Nd -= far_bottom.n+far_top.n;  //㤠�塞 �������
              if ( !(seci0->gvarm & up_dust) )
                Nd -= top.n;                 //㤠�塞 ���孨� dust'�
              if ( !(seci0->gvarm & low_dust) )
              {                            //㤠�塞 ������ dust'�
                memcpy(&dcell[box.n],&dcell[box.n+bottom.n],
                       sizeof(cell *)*top.n);
                Nd -= bottom.n;
              }
            }

          //ᮢ��㯭� �����
            seci0->gvarm &= glued;
            B=unite(LC,dcell,Nd,&GL,&seci0->gvarm);
            if (B)
            {
              set_erection(B, cut_list->rv.v2);
              if (GL.ncell>1)         //�᫨ ncell==1, ⮣�� celist[0]=B
                for ( i=0; i<GL.ncell; i++ )  del_cell(GL.celist[i]);
            }
          }
          else  //���������� �� ������஢�����
          {
            cell *C=B;
            while ( B )
            {
              if ( !dust(B) )  break;
              B=(cell *)B->complist;
            }
            if (!B && let_or_bad(versi0) && versi0->nvers)
            {                 //�� dust'�, �� ���� �����
              B=C;            //�饬 ���室�騩 �� ࠧ����
              while (B != 0)
              {
                if (B->h>sMALL_SIZE) break;
                B=(cell *)B->complist;
              }
              if (B)          //�८�ࠧ㥬 � ����让
              {
                cell *LB=B->prev;
                (B->next)->prev=LB; LB->next=B->next; // DELETE
                set_bad(B);
                set_bad_cell(B);
                insert_cell1(B,LB);
              }
            }
          }
          if (B)
            if ( !dust(B) )
            {
              rest_vers(B,versi0);
              seci=cut_list+seci0->px;
              if ( seci0->dh != 0 )  B->cg_flag |= c_cg_cutr;
              if (  seci->dh != 0 )  B->cg_flag |= c_cg_cutl;
              if (seci==cut_list)
              {
                B->cg_flag = B->cg_flag | cut_list->gvarm & c_cg_cutl;
                B->cg_flag_fine = B->cg_flag_fine | cut_list->gvarr &
                  ( c_cg_cut_tl | c_cg_cut_ml | c_cg_cut_bl | c_cg_cut_fl );
              }
              if (i0==N-1)  B->cg_flag |= cut_list->gvarm & c_cg_cutr;
            }
        }
      i0=seci0->px;            //᫥���騩 �� ��⨬��쭮� ���
    }

  //�饬 ���� cell �ࠢ�� ib

  ib=r->left+(cut_list+ib)->x;
  for (B=LC->next; B != RC && lefter(B,ib); B=B->next);

  return B;
}

/*-----------------05-24-95 11:06am---------------------------------
  locate  ��।���� ����� ᥣ����, ���஬� �ਭ������� cell B;
          ����� ᥣ���� ࠢ�� ������ ��� �ࠢ��� �祭�� �� ᯨ᪠
          cut_list N �祭��
-------------------------------------------------------------------*/
static int16_t locate(cell *B, struct cut_elm *cut_list, int16_t N, int16_t left)
{
  int16_t i,ip;  //⥪�饥 � �।��饥 �祭�� �� ��⨬��쭮� ���
  struct cut_elm *seci; //����⥫� i-�� �祭��

  ip=i=N-1;
  while ( i>0 )
  {
    seci=&cut_list[i];
    if ( !lefter(B,seci->x+left) ) break;
    ip=i;
    i=seci->px;            //᫥���騩 �� ��⨬��쭮� ���
  }
  return ip;
}

/*--------------------------------------------------------------------
  set_type  �ਢ���� ⨯ cell'� C (dust - �� dust) � ⨯� vers
--------------------------------------------------------------------*/
/*
static void set_type(cell * C, SVERS *vers)
{
  char new_dust=dust(vers);

  if( dust(C) && !new_dust && C->h>sMALL_SIZE)   //���塞 dust �� �� dust
  {
    cell *LC=C->prev;
    (C->next)->prev=LC; LC->next=C->next; // DELETE
    set_bad(C);
    set_bad_cell(C);
    insert_cell1(C,LC);
  }
  else
    if( !dust(C) && new_dust )         //���塞 �� dust �� dust
    {
      set_dust(C);
      set_bad_cell(C);
      C->prevl->nextl=C->nextl;
      C->nextl->prevl=C->prevl;
      err_pnlet(C);  // AL 940318 make err links
    }
}
*/
/*--------------------------------------------------------------------
   is_stick  �����頥� 1, �᫨ cell B - stick, � 0 � ��⨢��� ��砥
--------------------------------------------------------------------*/
static int16_t is_stick(cell *B)
{
#define hist_len RASTER_HEIGHT*2/3+1
  int16_t i,j;
  uchar hist[hist_len],*hp,*he;  //���⮣ࠬ��
  char emax;          //�ࠢ� �ࠩ ���⮣ࠬ�� + 1
  uchar hmax,hmax2;    //���� ��ࡠ � 1/2
  int16_t  w;             //�ਭ� ��ࡠ
  int16_t s0,s1;          //���頤� ��ࡠ � ��⠫쭮� ���
  c_comp *cp;         //envelope C
  lnhead *lp;         //⥪��� �����
  struct int_s *intp; //⥪�騩 ���ࢠ�

  if ( !B->env || B->h < my_bases.ps-(my_bases.ps>>2) ||
       ( B->w+(B->w>>1) > B->h ) )  return 0;

  //��ந� ���⮣ࠬ�� � ����塞 hmax

  memset(hist,0,hist_len); hmax=0; emax=0; cp=B->env;
  lp=(lnhead *)((char *)cp+cp->lines+2);  //蠯�� ��ࢮ� �����
  for ( i=0; i<cp->nl; i++ )
  {
    intp=(struct int_s *)(lp+1);
    for ( j=0; j<lp->h; j++,intp++ )
    {
      he=hist+(intp->e-intp->l); emax=MAX(emax,intp->e);
      for ( hp=hist+(intp->e-1); hp>=he; hp-- )
      {
        (*hp)++;
        hmax=MAX(hmax,*hp);
      }
    }
    lp=(lnhead *)((char *)lp+lp->lth);
  }

  //����塞 ���頤�

  if ( (hmax2=hmax>>1)==0 )  return 0;
  he=hist+emax; hp=hist; s0=s1=w=0;
  while ( *hp<=hmax2 )  s1 += *hp++;
  while ( *hp>hmax2 ) { s0 += *hp; w++; hp++; }
  while ( hp<he )       s1 += *hp++;
  if ( (w<<1)+(w>>1) > hmax )  return 0;
  s1 += (B->h*w-s0);                  //���� �� ���宩 ���
  if ( s1<<1 > s0 )  return 0;
  det_snap(B,"bring sticks");
  return 1;
#undef hist_len
}

/*-----------------05-04-95 05:35pm-----------------
   paste  ᮡ�ࠥ� '�' � '�' �� �ᥩ ��ப�
--------------------------------------------------*/
static void paste()
{
  cell *B,*BC,*EC;
  uchar p,c2,c3;
  int16_t bdiff;
  char dflag;
  s_glue GL;
  uchar expect;   //�������� १����
  int16_t  inc;      //��।����� ������

  BC=cell_f();
  while (1)
  {
    BC=BC->nextl;
    if (!BC) break;
    EC=BC->nextl;
    if (!EC) break;                      // BC = dummy cell at end
    if (!EC->next) break;                // EC = dummy cell at end
    if (!tsimple(BC) || !may_glue(BC)) continue; //"glued" earlier or never
    if (!tsimple(EC) || !may_glue(EC)) continue; //"glued" earlier or never
    if (BC->nvers && EC->nvers)  // recovering after stick cut
    {
      c2=BC->vers[0].let;
      c3=EC->vers[0].let;
      if (
		  !is_turkish_language(language) && // 21.05.2002 E.P.
		  glue_to_o (c2,c3,BC,EC)
		  )
        expect=(uchar)'�';
      else
        if(memchr("윚",c2,3) &&
			(memchr("/1!()�",c3,6) || c3==liga_exm) // 10.09.2000 E.P.
			&&
           abs(BC->h - EC->h) < 4 && abs(BC->row - EC->row) < 4 &&
           EC->r_col - (BC->r_col+BC->w) < BC->h/10 + 4
          ) /*to paste � */
          expect = is_lower(c2) ? (uchar)'�' : (uchar)'�';
        else
          continue;

      bdiff=BC->bdiff; dflag = BC->difflg & 0xf0;
      GL.ncell=2; GL.ngluc=1;
      GL.complist[0]=BC->env;  GL.celist[0]=BC;
      GL.complist[1]=EC->env;  GL.celist[1]=EC;
      GL.complist[2]=NULL;     GL.celist[2]=NULL;
      inc = erection_compose_inc(2, GL.celist);
      B=comp_to_cell(BC,GL.complist,2,(char)bdiff,dflag);
      GL.var=GL.fres=GL.arg=0; GL.flarg=GFsrc;
      if ( (B->flg&c_f_dust) || crecell(B,&GL,3)<0 )  //ᮢ��㯭� ���� - � BOX'�, B �� ���������
      {
        del_cell(B); BC=EC; continue;
      }
      set_erection(B, inc);
      compose_cg_flags(B,BC,EC);
      dmBOX(B,&GL);

      p= (B->nvers) ? B->vers[0].prob : 0;
      if (expect==(uchar)'�')
      {
        if (!memchr("oO0��",B->vers[0].let,5))
        {
          del_cell(B); BC=EC; continue;
        }
      }
      else    // �
        promote('a',B,expect,60);
      if (p>70)
      {
        del_cell(BC); del_cell(EC);
        BC=B;
      }
      else
      {
        del_cell(B); BC=EC;
      }
    }
  }
}

/*-----------------------------------------------------------------------
  compose_cg_flags ��⠭�������� cg_flag � cg_flag_fine cell'� B, � �।-
                   ���������, �� �� ��ନ஢�� �� L(����) � R(�ࠢ�)
------------------------------------------------------------------------*/
static void compose_cg_flags(cell *B, cell *L, cell *R)
{
  B->cg_flag = B->cg_flag & ~(c_cg_cutl | c_cg_cutr) |
               L->cg_flag & c_cg_cutl | R->cg_flag & c_cg_cutr;
  B->cg_flag_fine = L->cg_flag_fine &
                    ( c_cg_cut_tl | c_cg_cut_ml | c_cg_cut_bl | c_cg_cut_fl )
                    |
                    R->cg_flag_fine &
                    ( c_cg_cut_tr | c_cg_cut_mr | c_cg_cut_br | c_cg_cut_fr );
}

/*-----------------02-15-95 05:29pm--------------------------------------
  full_recog �ᯮ����� cell B � �����頥� 1, �᫨ १���� ����
             ��ண� tol � 0, �᫨ ���;
             �ᯮ�������� �� ��⠬ �ந��������, �᫨ trs>0;
             �ᯮ�������� �� 3x5:
               - ��易⥫쭮 �� trs==0,
               - ��� �ࠢ��� �� trs>0,
               - �᫨ ⥪�騩 १���� �㦥 abs(trs) �� trs<0;
             ������� gl0, �᫨ ������, �ᯮ������ ���ਬ����ࠬ�;
------------------------------------------------------------------------*/
int16_t full_recog (cell *B1, s_glue *gl0, int16_t trs, int16_t tol)
{
  SVERS svown,svorg;
  s_glue  sg;
  s_glue  *GL=&sg;
 uchar c, c_sacc, ct, svarg;
 int16_t flag_m, flg_own;
 uchar  p1, pans, porg;
 char wrk[32];
 char *pmsg,madeBOX;
 extern int16_t best_answer_BOX;
 extern char db_pass;

 static char ms220ready[]={"m > 220 --> ready"};
 static char msw3ready[]={"w <= 3  --> ready"};
 static char msf170bef[]={"f supports i,j"};
 static char msnat220[]={"native >220 --> ready"};
 static char ms29500[]={"stick > 29500"};
 static char msingl[]={"single nondiscrim ready"};

  if (dust(B1))  return 0;

#ifdef PROTOCOL
//  rec_count++;
#endif

  //if (pass4_in)
  //{
  //  estletter(B1,gl0); goto retp;
  //}

  if(pass4_in)
  {// OLEG : SERBIAN PASS4
  int16_t ret =estletter(B1,gl0);
  if (B1->vers[0].let==0)  B1->vers[0].let=bad_char;
  if( language==LANG_RUSSIAN && langSer &&
      (B1->flg&c_f_let) && B1->nvers && (B1->vers[0].let==SERB_j||B1->vers[0].let=='j') &&
      ij_dot(B1)>0 )
    ret -= 100;
  goto retp;
  //return ret;
  }

  GL->ncell=1; GL->ngluc=1;                //����� ��� crecell
  GL->celist[0]=B1; GL->complist[0]=B1->env;
  GL->celist[1]=NULL; GL->complist[1]=NULL;
//  if ( cut(B1) ) GL->flarg=GFcut;          //��१���� �������� �� 3x5
//  else           GL->flarg=0;
//  GL->flarg=0; GL->ncell=n;
  GL->arg=0;  GL->flarg=0;
  if (trs==0)      GL->arg |= GABOXR;     //��易⥫쭮 �� 3x5
  else if (trs>0)
       {           GL->arg |= GAtigr;     //�� ��⠬
                   GL->flarg=GFcut;       //�� 3x5, ��� �ࠢ���,
       }
       else           trs = -trs;         //�� 3x5 �� ����室�����
//  if (trs<0)  trs = -trs;
//  else
//  {
//    GL->arg |= GABOXR;
//    if (trs>0)  GL->arg |= GAtigr;
//  }

 flg_own = 1;   // self-made value (no 5-box estimate)
 GL->var=0;
 if ( GL->arg & GAtigr )       // request for full estimation
 {   recog_cell(B1);
  if (db_status && (db_trace_flag & 2))
    est_snap(db_pass,B1,"events");
 }
 c = B1->vers[0].let;
 p1= B1->vers[0].prob;
 if ( let(B1) && p1<trs ) let_to_bad(B1);

//   send to BOX: bad patterns;
//                miltiple version;
//                all 'm' letters   (m, n, u)
//                all cutten letters with simple structure

 if ((B1->flg & c_f_let) && (B1->nvers==1))
 {
   if (language==LANG_RUSSIAN && strchr("���3",c))  goto estiBOX;
   if (
	(language != LANG_ENGLISH) &&
	(memchr ("aoeu",B1->vers[0].let,4))
      )
      { accent(B1);
	if (B1->nvers != 1)
	  goto  not_a_single;
      }
   save_vers(B1,&svown);   // save the whole vector
   abris_reset();
   GL->celist[0]=B1; GL->celist[1]=NULL; GL->ncell=1;

   // 'm' is reliable in events; just test against legs

   flag_m=0;
   c_sacc = let_sans_acc[c];
   if (c_sacc=='m') flag_m=1;	// 'm'
   if (c_sacc=='n') flag_m=2;	// 'n'
   if (c_sacc==liga_ri) flag_m=2;    // "ri"
   if (c_sacc=='u') flag_m=4;	// 'u'
   if (flag_m)
   {
     madeBOX=1;
     atlant(B1,GL,3,flag_m);
     p1=B1->vers[0].prob;
     if (db_status && (db_trace_flag & 2))
       est_snap(db_pass,B1,"atlant");
     if (c_sacc != 'm')
     {
       rest_vers(B1,&svown);              // native u,n,rt -
       goto deciBOX;
     }
     if (p1 >= trs)           // legs OK - ready answer if 'm',
     {
       B1->flg &= ~c_f_bad;  B1->flg |= c_f_let;
       save_vers(B1,&svown);
       pans = p1;
       pmsg = ms220ready;
       goto test_BOX_request;
     }
     //  'm'-legs NOT OK
     B1->flg &= ~c_f_let; B1->flg |= c_f_bad;
     save_vers(B1,&svown);
     pans = p1;
     pmsg = NULL;
     goto test_BOX_request;
   }

   if (B1->w < minw)
//   if (B1->w <= 3)
   {
     pans=B1->vers[0].prob;
     pmsg = msw3ready;
     goto test_BOX_request;
   }
   if  (c==liga_rt)    // 'rt'
   { p1 -= 82; if (p1 < 2) p1= 2; B1->vers[0].prob=p1; goto argBOX_ready; }
   if  ((c=='h') || (c=='b'))
   { p1=(uchar)h_filt(B1,p1,GL,c);  goto deciBOX; }
      c_sacc = let_sans_acc[c];
   if ( (memchr("sSaoO0QGDMNHURdxq6<>cCkwWBEA",c_sacc,28)) ||
        ( c_sacc == ss_deaf_sound && language != LANG_RUSSIAN) ) // Vademar 2.2.93
    {  p1=(uchar)abris(GL,B1,c_sacc,p1); goto deciBOX; }

   if ( memchr("rtfTJ()<>[]LI1il!/F7⒣��|",c,26) &&
   		  !is_russian_turkish_conflict(c) // 21.05.2002 E.P.
		||   // 07.01.1993 (see S_TOOLS.C)
        ( language != LANG_RUSSIAN && (c==liga_i  ||                                   // FARA REDACTION

	!is_baltic_language(language) && 	// 17.07.2001 E.P.
	!is_turkish_language(language) &&
		(	// 21.05.2002 E.P.
		c==i_right_accent ||
		c==i_roof_accent  ||
		c==II_right_accent||
		c==II_roof_accent ||

		// ����������� ���� 07.09.2000 E.P.
		!is_cen_language(language) && (
			c==i_left_accent  ||
			c==i_2dot_accent  ||
			c==II_left_accent ||
			c==II_2dot_accent
		) ) ||

		is_baltic_palka(c) ||	// ���������� �����. 10.07.2001 E.P.
		is_turkish_palka(c)		// �������� �����. 21.05.2002 E.P.
	) ) )
   {
     stick_center_study(B1,GL,1);      // Oleg : 07.08.92.
     p1 = B1->vers[0].prob;
   deciBOX:
     if (db_status && (db_trace_flag & 2))
     {
       sprintf (wrk,"single discrim; p=%d", p1);
       est_snap(db_pass,B1,wrk);
     }
     if (p1 <= 2) p1=2;
     if (p1 < trs)        // single version to be cut
     {
       if ((c=='f') && (p1 >= 170))
       { cell *b1n;
         ct=(b1n=B1->nextl)->vers[0].let;
         if (((ct=='i') || (ct=='j')) /*****  && (b1n->cg_flag & c_cg_rqdot) ****/)
         {
           pans=B1->vers[0].prob;   // weak 'f' provides dot to i,j
           pmsg = msf170bef;
           goto test_BOX_request;
         }
       }
       if (p1 < 170)             // BOX in full ?
       {
         B1->vers[0].prob=0;
         B1->nvers = 0;
         B1->flg = c_f_bad;
         if (db_status && (db_trace_flag & 2))
           est_snap(db_pass,B1,"p<170 to BOX FULL");
         goto estiBOX;
       }
       // not solid, but not too bad  (170 -- trs)
       rest_vers(B1,&svown);
    argBOX_ready:
       if (db_status && (db_trace_flag & 2))
         est_snap(db_pass,B1,"170 < p1 < 220 --> to BOXes");
       goto estiBOX;
     }
     // reliable single >= trs after all discrim.
     if (GL->flarg & GFcut)
     {
       rest_vers(B1,&svown);
 // PERSONAL DISCRIMINATIONS: unreliable cutten letters
       if (c==liga_rt)    // 'rt'
       {  p1-=50; if (p1<2) p1=2; B1->vers[0].prob = p1; }
       if (db_status && (db_trace_flag & 2))
         est_snap(db_pass,B1,"cutten p>220 --> to BOXes");
       goto estiBOX;        // artifact - estimate by BOX with events' value
     }
     B1->vers[0].prob=p1;
     save_vers(B1,&svown);
     pans=p1;
     pmsg = msnat220;
     goto test_BOX_request;
   }  // a stick;

   // cutten letter (stick & others) --> BOX
   if (GL->flarg & GFcut)
   {
      if (db_status && (db_trace_flag & 2))
        est_snap(db_pass,B1,"CUT/GLUE to BOX");
      goto estiBOX;    // cutten version - estimate by BOX
   }
	 if (memchr("1lI()[]{}!",c,10) || c==liga_i ||
		  language == LANG_TURKISH &&  // 30.05.2002 E.P.
			(c==i_sans_accent||c==II_dot_accent) ||
		 c==liga_exm
		)
    if (signif_line_count(B1) == 1)
    {
     svarg=GL->arg;
     if (db_status && (db_trace_flag & 2))
       est_snap(db_pass,B1,"stick, 1 big line -- test 29500");
     GL->arg |= GABOXs;         // direct request
     crepat(B1,GL,0,0);
     crecell(B1,GL,3);         // make raster, box
     dmBOX(B1,GL);
     SBOX_to_static();
     SBOX_to_GL(GL);
     GL->arg=svarg;
     if (best_answer_BOX >= 29500)  // original stick supported by BOX
     {
       rest_vers(B1,&svown);      // retain events' value
       pans=B1->vers[0].prob;
       pmsg = ms29500;
       goto test_BOX_request;
     }
     // weak stick
     B1->flg = c_f_bad;  B1->vers[0].prob=0;  B1->vers[0].let=bad_char;
     if (db_status && (db_trace_flag & 2))
       est_snap(db_pass,B1,"p < 29500 - BOX FULL");
     goto estiBOX;
   }
   // usual native letter (unknown as to be tested against some discrim. rule
   /******** ATEMPT  to confirm single by BOXes ***********  93.07.08 ***
     svarg=GL->arg;
     GL->arg |= GABOXs;         // direct request
     crepat(B1,GL,0,0);
     crecell(B1,GL,3);         // make raster, box
     dmBOX(B1,GL);
     SBOX_to_static();
     SBOX_to_GL(GL);
     GL->arg=svarg;
     rest_vers(B1,&svown);   // back to events
     if (best_answer_BOX < 29200)  // original not confirmed
     // 29200 --> 50; (50 + 254)/2 --> 152
     {
       if (db_status && (db_trace_flag & 2))
         est_snap(db_pass,B1,"p<30000 to BOXes");
       goto estiBOX;
     }
*** END OF ATTEMPT    93.07.08 ***********************************************/
   pans=B1->vers[0].prob;
   pmsg = msingl;
 test_BOX_request:
   if (pmsg && db_status && (db_trace_flag & 2))
     est_snap(db_pass,B1,pmsg);
 if ((GL->arg & GABOXR) == 0)
   goto retp;  // no direct request (for boxes ?)
 }  // let & single
 // not a single version
 not_a_single:
 if ((B1->flg & c_f_bad)      ||        // bad answer
      (B1->nvers > 1)         ||        // multiple versions
      (GL->arg & GABOXR)                // BOX explicitly requested
    )
   {
 estiBOX:
    madeBOX=1;
    if (db_status && (db_trace_flag & 2))
      est_snap(db_pass,B1,"to make all BOXes");
    flg_own = 0;   // estimated by 5-box
    if ( crecell(B1,GL,3) < 0 )            //���� � BOX'�, B �� ���������
      return 0;
    if ( gl0 )  GL=gl0;
//    if ( composed(B1) )  GL->ncell=2;        //��� diffrv
    save_vers(B1,&svorg);
    if ( B1->nvers ) porg=B1->vers[0].prob;
    else             porg=0;
    dmBOX(B1,GL);
    if ( B1->nvers ) pans=B1->vers[0].prob;
    else             pans=0;
    if ( !pans && porg )  rest_vers(B1,&svorg);
   }

   if ( language != LANG_RUSSIAN )
     if ( B1->nvers )
     {
/*
       if ( cut(B1) )
         if ( (c=B1->vers[0].let)=='l' || c=='I' )
         {
           int16_t p=(int16_t)B1->vers[0].prob-trs2-2;
           if ( p>0 )  discr_vers(B1,p,1);   //��� ��� ���ᨩ -p1
         }
*/
     }
     else
       if ( bad(B1) && !cut(B1)
            && B1->env && (B1->env->nl==1) && 3*B1->w<=B1->h  )
       {
         int16_t d=my_bases.ps>>2, bottom=B1->row+B1->h;

         if ( is_stick(B1)
              && B1->row>=my_bases.b2-d && B1->row<=my_bases.b2+d
              &&  bottom>=my_bases.b3-d &&  bottom<=my_bases.b3+d
            )
         {                                  //'i' ��� �窨 �� �ᯮ�������
           B1->vers[0].prob=I_wo_point;
           B1->nvers=1; B1->vers[0].let=liga_i;
         }
         else
           set_bad_cell(B1);
       }

retp:
  if ( B1->nvers ) pans=B1->vers[0].prob;
  else             pans=0;
  if (pans >= tol)
    { B1->flg &= ~c_f_bad;  B1->flg |= c_f_let; }
  else
    { B1->flg &= ~c_f_let; B1->flg |= c_f_bad; }
  glsnap('a',B1,"");
  return pans>tol;
}

static void corr_cut()
{ cell *b1, *b2, *b3;
  uchar c1,c2,c3;

  b2=cell_f();
  while ( (b2=b2->nextl)->nextl )
  {
    if ( !b2->nvers )
    {
      if ( language == LANG_RUSSIAN
          && (b2->env && (b2->env->nl==1) || 3*b2->w<=b2->h) )
      {                               //����⪠ �ᯮ����� "!"
        stick_center_study(b2,NULL,1);
        det_snap(b2,"bring sticks");
      }
      continue;
    }
    if ( bad(b2) && b2->vers[0].prob>MINlet ) set_let(b2);
    if ( language != LANG_RUSSIAN )
    {
      b1=b2->prevl; b3=b2->nextl;
      c2=b2->vers[0].let;
      c1=b1->vers[0].let;
      c3=b3->vers[0].let;
       // try to recover "stick_to_something" producing 'k', 'd', 'b'
       if (memchr("lI1/J)!",c2,7) ||         // stick character ?
           c2==liga_i ||
		   language == LANG_TURKISH &&  // 30.05.2002 E.P.
			(c2==i_sans_accent||c2==II_dot_accent) ||
		   c2==liga_exm
		   )
         // in combinations  (vI) (WI) (uI)  promote  't' in place of 'I'
         if ((b2->cg_flag & c_cg_cutr) && ((c1=='v') || (c3=='w') || (c3=='u')))
         {
           promote('a',b2,'t',16);
           continue;
         }
       if ((c1=='r') && (c2=='t') && have_upper_dot(b1))
         promote ('a',b1,'i',16);
     }
  }   // while all cells
}

static int16_t have_upper_dot(cell *c)
{
 cell *cc;
 int16_t H;
 H=my_bases.ps;
 cc=c->prev;
 if ((cc->flg & c_f_dust) &&
      (c->w>4 && cc->h>=2 && cc->w>=2 &&
	(abs(cc->h-cc->w)<=H/6 || cc->h<cc->w && cc->w-cc->h<=H/4) &&
	cc->col+1>=c->col && cc->col+cc->w-5<=c->col+c->w ||
       c->w<=4 && abs(c->col-cc->col+(c->w-cc->w)/2)<=2) &&
      cc->row+cc->h-2<=my_bases.b2)
   return 1;
 cc=c->next;
 if ((cc->flg & c_f_dust) &&
      (c->w>4 && cc->h>=2 && cc->w>=2 &&
	(abs(cc->h-cc->w)<=H/6 || cc->h<cc->w && cc->w-cc->h<=H/4) &&
	cc->col+1>=c->col && cc->col+cc->w-5<=c->col+c->w ||
       c->w<=4 && abs(c->col-cc->col+(c->w-cc->w)/2)<=2) &&
      cc->row+cc->h-2<=my_bases.b2)
   return 1;
 return 0;
}

void set_bad_vers(SVERS *c)
 {
 c->nvers=0;
 c->source = 0;
 c->vers[0].let=bad_char;
 c->vers[0].prob=0;
 c->vers[1].let=0;
 if (c->flg & (c_f_let+c_f_bad)) c->flg=c_f_bad;
 }

static int16_t forbid_stick_cut(cell *c, SVERS *vers, int16_t h, cut_pos *cpos,
                            int16_t edge)
 {
 uchar l, prob;
 int16_t  wd, bm3, bm, b3;
 int16_t  row1, row2;

  l = vers->vers[0].let;
  prob = vers->vers[0].prob;

  // is it a stick ?
  if ( (l!='i') && (l!='l') && (l!='1') && (l!='I') && (l!=liga_i) &&
		!(language == LANG_TURKISH &&  // 30.05.2002 E.P.
			(l==i_sans_accent||l==II_dot_accent)
		 )
	 )
	 return 0;

  wd = (row2=cpos->row2)-(row1=cpos->row1);  // cut width
  if (wd > h-2)  // full cut
    return 1;
  //93.01.28 allow the non_full cut for strong 'i' (with dot)
  if ((l=='i') && (prob >= 200)) return 0;
  b3=my_bases.b3 + c->bdiff;
  ///////////
  if (2*(b3-row2)-1 > my_bases.ps) return 0;  // above middle line (b2/b3)
  ///////////
  bm3=(my_bases.bm+my_bases.b3)/2+c->bdiff;  bm = my_bases.bm + c->bdiff;

  // first try to retain, then to abort;
  // before 01.06 the 2 lines of code were after 'thickness' check
  ///////
  ///////   LOOK ABOVE
  ///////   if (row2 <= bm) return 0;
  ///////   the cut is entirely upper than middle
  ///////
  if (row1 >= bm3) return 0;     // or lower than (middle--b3)
  if (cpos->flag & 8) return 1;  // thick nonsimple cut
  if (cpos->flag & 4) return 0;  // sophisticated cut (double, horizontal)
  if (3*wd > h)    return 1;  // simple thick
  /////// // 93/01/25 very thin cut --> not a 'k' -- NOT INVENTED
  ////////   NOT INVENTED :  if (5*wd < c->h) return 0;
  ////////   stda17/31  making  ki--> ld
  ////////   stdc8/29   dunkle  kl--> ld
  if (row2 >= (b3-2)) return 0;  //  till very bottom
  if (edge == 1)  return 1;      // at right side

  // SO BEFORE 01.06 stick cuts were:
  //    thick - anywhere
  //    starting and ending 'in the middle' AND at right side of stick
  // AFTER 01.06 stick cut is:
  //
  return 0;
//
}

cell * create_my_cell(MN * mn, cell * ci, char bdiff, char dflag)
{
 cell * C = create_cell1(mn,ci,bdiff,dflag);

 //���塞 �� dust �� dust, �᫨ ᬥ饭 �� ����

 if( !dust(C) && ( C->row>my_bases.bm || C->row+C->h<my_bases.b2 )
	 // Nick - 17.05.99
	 && (!p2_active || C->h <= CCOM_DUST_SIZE || C->w <= CCOM_DUST_SIZE) )
 {
   set_dust(C);  set_bad_cell(C);
   C->prevl->nextl=C->nextl;
   C->nextl->prevl=C->prevl;
   err_pnlet(C);  // AL 940318 make err links
 }

 return C;
}
