/*
Copyright (c) 1993-2008, Cognitive Technologies
All rights reserved.

Разрешается повторное распространение и использование как в виде исходного кода,
так и в двоичной форме, с изменениями или без, при соблюдении следующих условий:

      * При повторном распространении исходного кода должны оставаться указанное
        выше уведомление об авторском праве, этот список условий и последующий
        отказ от гарантий.
      * При повторном распространении двоичного кода в документации и/или в
        других материалах, поставляемых при распространении, должны сохраняться
        указанная выше информация об авторском праве, этот список условий и
        последующий отказ от гарантий.
      * Ни название Cognitive Technologies, ни имена ее сотрудников не могут
        быть использованы в качестве средства поддержки и/или продвижения
        продуктов, основанных на этом ПО, без предварительного письменного
        разрешения.

ЭТА ПРОГРАММА ПРЕДОСТАВЛЕНА ВЛАДЕЛЬЦАМИ АВТОРСКИХ ПРАВ И/ИЛИ ДРУГИМИ ЛИЦАМИ "КАК
ОНА ЕСТЬ" БЕЗ КАКОГО-ЛИБО ВИДА ГАРАНТИЙ, ВЫРАЖЕННЫХ ЯВНО ИЛИ ПОДРАЗУМЕВАЕМЫХ,
ВКЛЮЧАЯ ГАРАНТИИ КОММЕРЧЕСКОЙ ЦЕННОСТИ И ПРИГОДНОСТИ ДЛЯ КОНКРЕТНОЙ ЦЕЛИ, НО НЕ
ОГРАНИЧИВАЯСЬ ИМИ. НИ ВЛАДЕЛЕЦ АВТОРСКИХ ПРАВ И НИ ОДНО ДРУГОЕ ЛИЦО, КОТОРОЕ
МОЖЕТ ИЗМЕНЯТЬ И/ИЛИ ПОВТОРНО РАСПРОСТРАНЯТЬ ПРОГРАММУ, НИ В КОЕМ СЛУЧАЕ НЕ
НЕСЁТ ОТВЕТСТВЕННОСТИ, ВКЛЮЧАЯ ЛЮБЫЕ ОБЩИЕ, СЛУЧАЙНЫЕ, СПЕЦИАЛЬНЫЕ ИЛИ
ПОСЛЕДОВАВШИЕ УБЫТКИ, СВЯЗАННЫЕ С ИСПОЛЬЗОВАНИЕМ ИЛИ ПОНЕСЕННЫЕ ВСЛЕДСТВИЕ
НЕВОЗМОЖНОСТИ ИСПОЛЬЗОВАНИЯ ПРОГРАММЫ (ВКЛЮЧАЯ ПОТЕРИ ДАННЫХ, ИЛИ ДАННЫЕ,
СТАВШИЕ НЕГОДНЫМИ, ИЛИ УБЫТКИ И/ИЛИ ПОТЕРИ ДОХОДОВ, ПОНЕСЕННЫЕ ИЗ-ЗА ДЕЙСТВИЙ
ТРЕТЬИХ ЛИЦ И/ИЛИ ОТКАЗА ПРОГРАММЫ РАБОТАТЬ СОВМЕСТНО С ДРУГИМИ ПРОГРАММАМИ,
НО НЕ ОГРАНИЧИВАЯСЬ ЭТИМИ СЛУЧАЯМИ), НО НЕ ОГРАНИЧИВАЯСЬ ИМИ, ДАЖЕ ЕСЛИ ТАКОЙ
ВЛАДЕЛЕЦ ИЛИ ДРУГОЕ ЛИЦО БЫЛИ ИЗВЕЩЕНЫ О ВОЗМОЖНОСТИ ТАКИХ УБЫТКОВ И ПОТЕРЬ.

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

#define _BAL_VERSION_

#ifdef _BAL_VERSION_

 #ifdef LIN_MAIN
  #define EXTLIN
 #else
  #define EXTLIN extern
 #endif


#define c_df_round  16
#define c_df_twins  32      // has a twin
#define c_df_b1a    64      // agrees to be at  b1
#define c_df_b2a    128     //                  b2

#define c_fdef_cap  16

#define TRSWEAK  70     // treshold to treat letter as doubtful
#define TRSPNLT  120    // penalty to be treated as reason to cut
#define TRSCUT   36     // penalty to be treated as cut

extern uchar db_status;  // snap presence byte
extern uchar line_scale;
extern int16_t  line_number;
extern uchar fax1x2;

void bring_dust(int16_t);
void all_cell_levcut(int16_t);

// параметры строки
EXTLIN int16_t     minrow,mincol, maxrow, maxcol;
// получены БЛ
EXTLIN int16_t     bs_got;     //
// линии
EXTLIN int16_t			 bbs0,bbs1, bbs2, bbs3, bbs4, bbsm;
// bbst,
// параметры для определения dust
EXTLIN int16_t           bsdust_upper, bsdust_lower, bsdust_ps;
// параметры БЛ
EXTLIN int16_t           Nb1, Nb2, Nb3, Nb4, Nbt, Ps, Psf;
// мульти - БЛ
EXTLIN uchar          multi_bas;

EXTLIN int16_t           Ns1,Ns2;        // in Iot.c   only  !!!

EXTLIN uchar          let_to_dust;

EXTLIN char          all_caps;
EXTLIN char          all_diffs_made;
EXTLIN CSTR_line     lin_str;     // pass3.c only
EXTLIN char          dust_in ;


#define cpsu(i)  ((((i)<3) && ((i)>-3))? i : ((i) * 26 / DPs))
#define cpsau(i)  ((i) * 26 / DPs)

#define cpsf(i)  ((((i)<3) && ((i)>-3))? i : ((i) * 26 / DPsf))
#define cpsaf(i)  ((i) * 26 / DPsf)
#define cpsf1(i)   cpsaf(i)+1
#define cpsf2(i)   cpsaf(i)-1

extern uchar     *let_linpos,
                *let_linshape,
                *let_lindef,
				// let_lindef когда знаем точно bs3
				*let_lindef3;

#else   // old version

#define DBG 1
#ifdef LIN_MAIN
#define EXTLIN
#else
#define EXTLIN extern
#endif
typedef struct bas_ln_type
  {int16_t startcol, endcol, startf, endf, ncells;
   int16_t ps, b1, b2, b3, b4, n1, n2, n3, n4; uchar fl_def, fl_ext;
   cell *c1, *c2, *cf1, *cf2; } bas_ln;
//
#define c_df_round  16
#define c_df_twins  32      // has a twin
#define c_df_b1a    64      // agrees to be at  b1
#define c_df_b2a    128     //                  b2

#define c_fdef_cap  16

typedef struct bas_acc_tg {int16_t row, cnt[4];} bas_acc_t;
typedef struct hi_ac_t { int16_t all, b1, a1, b2, a2, bc; } hi_ac;
#define TRSBAD   10     // treshold to treat letter as bad
#define TRSWEAK  70     // treshold to treat letter as doubtful
#define TRSPNLT  120    // penalty to be treated as reason to cut
#define TRSCUT   36     // penalty to be treated as cut
#define TRSINFL  18     // influence by small/cap type of solid cell
#define PROP_MAX 20	// VERY TALL PATTERN criterium


extern uchar db_status;  // snap presence byte
extern uchar line_scale;
extern int16_t line_number;
extern uchar fax1x2;

int16_t defbas(int16_t filter);
void diffs_by_vers();
void diffs_by_hist();
void discrim_by_dust();
void lpreset(int16_t);
void lpmima(int16_t);
void bring_dust(int16_t);
void interdif(cell *P, cell *N, cell *B1);
void stand_bas();
void complete_bas(char *);
void dbreset(int16_t);
int16_t  fincells();
int16_t  dbsum(int16_t filter);
void fincell();
void all_cell_levcut(int16_t);
void mutual_influence();
void cut_sunk_let ();
void all_cell_ledust();
int16_t  dust_to_let();
void histb(int16_t x1, int16_t x2, int16_t flg, uchar *begs);
void set_basarr(bas_ln * bs, int16_t x, int16_t l);
int16_t  multi_hist(int16_t p);
uchar get_let_tb(uchar);
void insert_basar(int16_t x1, int16_t x2);
int16_t  get_maxmax(uchar *begs);
int16_t  cells_for_base (int16_t base);
cell *def_init_cell();
int16_t  multi_hist();
int16_t  same_int(cell *, uchar);
void extend_int();
void cell_bases();
void set_int();
void set_basint();
void make_intpairs();
void histes();
void diffs_by_cells();
int16_t  comp_Llet(uchar, cell*);
int16_t  comp_Lbad(cell *);
int16_t  comp_Slet(uchar, cell *);
int16_t  comp_Sbad(cell *);
void complete_int(bas_ln *bp);
void cell_analyze(cell *BC);
int16_t  calc_base();
int16_t  doubt_bas();
void make_difbas(cell *BC, int16_t i);
void histo_analyze(cell *BC);
int16_t  types_of_vers(cell *BC);
int16_t  types_byBOX(cell *BC);
void set_difflg(cell *B1, int16_t filter);
#define f_cut 1
#define f_retain 0xc0

EXTLIN hi_ac  hi_beg[192];
EXTLIN uchar   lin_begs[192],
              loc_begs[192],
              lin_ends[192],
              lin_segs[192],
              tmp_beg[192],
              int_beg[192];

// EXTLIN  int16_t  mindef_col, maxdef_col;
EXTLIN  int16_t  minrow, mincol, maxrow, maxcol, midcol,  minold,
             sum_maxrow, max_height, min_crow, max_crow,
             old_minrow, old_maxrow, out_of_minmax,
             ncut_sticks, ncut_vers, ntot_cells, ncut_mult, killed_box, ncut_box, ncut_cells, bs_got, sum_ans;
EXTLIN  int16_t  ncut_old, nmult_old, ndead_old, nvers_old, ntot_old;
EXTLIN char fl_fail, fl_artifact, all_caps, fl_defb,
            b2_solid, dust_in, diff_curv,
            all_diffs_made, diffs_made, histofl, histiter, lin_pass;
EXTLIN int16_t  oldPs2, oldPs1, oldPs4, oldn1, oldn2, oldn4, oldmult,
            sbs1, sbs2, sbs3, sbs4, sbst,
            Ns1, Ns2, Ns3, Ns4, Nst,
            sum_cellth, sum_letlth,
            nctot, ncletrs, ncbs, nab1, sbsu, sbsd;
EXTLIN int16_t  cut_by_lines, change_vote, cells_inln,
            bbs0, bbs1, bbs2, bbs3, bbs4, bbst, bbsm,
            bsdust_upper, bsdust_lower, bsdust_ps,
            Nb1, Nb2, Nb3, Nb4, Nbt, Ps, Psf;
EXTLIN CSTR_line lin_str;
EXTLIN int16_t  wrk_dup, wrk_ddn;
EXTLIN int16_t  bs_int_no, trans_total;
EXTLIN int16_t  obs1, obs2, obs3, obs4, obsm,
            oNb1, oNb2, oNb3, oNb4, oPs, oPsf, obs2m, obsm3;
EXTLIN int16_t  cur_up, cur_t, cur_b2, cur_fl, cur_ps1, cur_ps2,
            cur_dn, cur_dup, cur_ddn, cur_h;
EXTLIN int16_t  int_upper, int_lower, int_up, int_dn, int_dup, int_ddn,
            int_supper, int_slower,
            int_ini, int_fid, int_fis, int_ps1, int_ps2, pen_up, pen_dn,
            int_sps1, int_sps2,
            int_sbs1,  int_sbs2, int_sbst, int_sfbd, int_sfbs,
            int_smin2, int_smax2, int_smin1, int_smax1, int_smid1, int_smid2,
            int_ssumb1, int_ssumb2, int_snb1, int_snb2, int_snf,
            int_bs1,  int_bs2, int_bst, int_fbd, int_fbs,
            int_min2, int_max2, int_min1, int_max1, int_mid1, int_mid2,
            int_sumb1, int_sumb2, int_nb1, int_nb2, int_nf;
EXTLIN cell *RUndef, *LUndef;
EXTLIN cell *Curr_cell, *Start_cell, *End_cell, *Fail_Scell, *Fail_Ecell, *Fail_cell;
EXTLIN int16_t  krit_hist, krit_loc;
EXTLIN bas_acc_t bas_acc[8];
EXTLIN int16_t  first_max, second_max;
EXTLIN int16_t forbiset, all_doubts;
EXTLIN uchar linpos_arg, multi_bas, hist_done;
EXTLIN uchar let_to_dust;
EXTLIN bas_ln all_bases[32];

#define cpsu(i)  ((((i)<3) && ((i)>-3))? i : ((i) * 26 / DPs))
#define cpsau(i)  ((i) * 26 / DPs)

#define cpsf(i)  ((((i)<3) && ((i)>-3))? i : ((i) * 26 / DPsf))
#define cpsaf(i)  ((i) * 26 / DPsf)
#define cpsf1(i)   cpsaf(i)+1
#define cpsf2(i)   cpsaf(i)-1

extern uchar     *let_linpos,    *let_lindef,
                *let_lincomp,   *let_linshape,
                *let_sans_acc,  *let_lindef3;


#endif
