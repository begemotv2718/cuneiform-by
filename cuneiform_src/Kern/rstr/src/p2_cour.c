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

//#define _SAVE_INVALID_CLU_

#include <stdlib.h>
#include <string.h>
#include <memory.h>
/*#include <io.h>*/


#include "struct.h"
#include "status.h"
#include "func.h"
#include "lang.h"
#include "cstr.h" // OLEG
#include "fon.h"
#include "std.h"
//#include "linear.h"

#include "msk.h"
//#include "r35.h"
#include "leo.h"
#include "ctb.h"
#include "linutil.h"
extern  char    alphabet[256];
extern  uchar    decode_ASCII_to_[256][4];
static  int            HndTab=0, HndTabInc=0;
static  char    alpha_str[]="��������������������������������/0123456789��������������������������������#%^()";
static  char    alpha_str_lat[]="QWERTYUIOPASDFGHJKLZXCVBNM/0123456789qwertyuiopasdfghjklzxcvbnm#%^()";
static  char    alpha_str_cap[]="��������������������������������";
static  char    alpha_str_lat_cap[]="QWERTYUIOPASDFGHJKLZXCVBNM";
static  char    alpha_str_all_cap[]="��������������������������������QWERTYUIOPASDFGHJKLZXCVBNM";
static  char    alphabet1[256];

void set_alphabet(unsigned char *alpha,unsigned char *letters)
{
    memset(alpha,0,256);

    while( *letters )
    {
        alpha[ *letters++ ] = 1;
    }
    return;
}

static void print_method(FILE *fp,RecVersions *res,char *s,int ansi)
{
    int ii;
    fprintf(fp, "%s", s);
    if( !res->lnAltCnt )
    {
        fprintf(fp,"-\n");
        return;
    }
    fprintf(fp,"%c(%d)",ansi?stdAnsiToAscii(res->Alt[0].Code):res->Alt[0].Code,res->Alt[0].Prob);
    for(ii=1;ii<res->lnAltCnt;ii++)
    {
        fprintf(fp,",%c(%d)",ansi?stdAnsiToAscii(res->Alt[ii].Code):res->Alt[ii].Code,res->Alt[ii].Prob);
    }
    fprintf(fp,"\n");
    return;
}

Bool32 TestFontProtocol(void)
{
#ifdef _USE_LEO_
    int32_t          name;
    int            i; //,j;
    int            nClust;
    int            numCourier,nC;
    FILE          *fp;
    LeoFieldSetup  fs={0};
    RecRaster   rec;
    RecObject   ro={0};
    LeoPageSetup ps={0};
    int jj;

    if( _access("c:\\met.ini",0)==-1 )
        return FALSE;

    nClust=FONGetClustCount();

    set_alphabet(alphabet1, alpha_str);

    LEOSetPlatform(586);
    fs.nStyle = LS_PRINT;
    memcpy(fs.AlphaTable,alphabet1,256);
    LEOSetupField(&fs);
    CTB_unlink("CLUST2");

    fp=fopen(".\\clu_met.pro","wt");
    for(i=0,nC=numCourier=0;i<nClust;i++)
    {
        for(jj=0;jj<3;jj++)
        {
            name=0;
            FONGetClusterAsBW(&name,i,jj*25,&rec);
            CTB_AddRecRaster("CLUST2", &rec,decode_ASCII_to_[name][0]);
            memset(&ro,0,sizeof(RecObject));
            memcpy(&ro.recData.recRaster,&rec,REC_MAX_RASTER_SIZE);
            ps.nIdPage=-1;
            ro.recData.lwStatus=0;
            LEOSetupPage(&ps);
            fprintf(fp,"Nclu=%d(%d) let=%c tresh=%d%s\n",i,i*3+jj, name, jj,jj?"":"(default)" );

            LEORecogPrintChar(&ro);
            print_method(fp,&ro.recResults,"         leo : ",1);


            LEORecogPrnMethod( &ro , REC_METHOD_MSK, 1);
            print_method(fp,&ro.recResults,"         msk : ",0);

            LEORecogCharDebug( &ro );
            print_method(fp,&ro.recResults,"     evn+3x5 : ",1);
        } // end treshold
        fprintf(fp,"\n");
    }   // end clusters

    fclose(fp);
#endif
    return TRUE;
}
////////////////
Bool32 TestFontCourier(void)
{
#ifdef _USE_LEO_
    uchar          p;
    int32_t          name;
    RecRaster      recRast;
    int            i,j;
    int            nClust;
    int            porog=0; // test !!!
    int            numCourier,nC;

    RecVersions    MSKres;
    ClustInfo      clustinfo;
    int32_t          nf=FONGetFontCount();

    if( nf<1 || nf>2)
        return FALSE;
    nClust=FONGetClustCount();
    if(nClust < 20 )
        return FALSE;

    if(!HndTab)
        return FALSE;
    MSKSetAlphabet  ( alphabet );

    for(i=0,nC=numCourier=0;i<nClust;i++)
    {
        name=0;
        j=FONGetClusterAsBW(&name,i,porog,&recRast);
        if(j<0)
            continue;

        if( name>'9' && name<128 )
            continue;
        clustinfo.let = 0;  // from all clusters
        FONGetClustInfo( &clustinfo, i+1 );
        if( clustinfo.weight<3 )
            continue;
        if(0)
            CTB_AddRecRaster("CLUST2", &recRast,decode_ASCII_to_[name][0]);
        MSKres.lnAltCnt=2;
        MSKres.lnAltMax=REC_MAX_VERS;
        MSKres.Alt[0].Code=(uchar)to_upper((uchar)name);
        MSKres.Alt[0].Prob=255;
        MSKres.Alt[1].Code=(uchar)to_lower((uchar)name);
        MSKres.Alt[1].Prob=255;

        if(!MSKRecogCharExp( HndTab, &recRast,       &MSKres )  )
            continue;
        p = MAX(MSKres.Alt[0].Prob,MSKres.Alt[1].Prob);
        if( p> 150)
            numCourier++;
        nC++;
    }

    if(numCourier*3 > nC*2)
        return TRUE;
#endif
    return FALSE;
}
////////////////////////
static int curNumFile=0;
static uchar hasNearSame[]="\xa7\xed";  // ��
static uchar NearSame[]="\xed\xa7";     // ��
int32_t TestFontClusters(void)
{
#ifdef _USE_LEO_
    int32_t          name;
    int            i,j;
    int            nClust;
    int            porog=50; // test !!!
    int            numInvalid;

    LeoFieldSetup  fs={0};
    RecRaster   rec;
    RecObject   ro={0};
    LeoPageSetup ps={0};
    ClustInfo cluInfo;
    uchar addLet,resLet;

#ifdef _SAVE_INVALID_CLU_
    FILE          *fp;
#endif

    curNumFile++;

    nClust=FONGetClustCount();
    set_alphabet(alphabet1, alpha_str);

#ifdef _SAVE_INVALID_CLU_
    fp=fopen("clust.tst","at");
    fprintf(fp,"file %d\n",curNumFile);
#endif

    LEOSetPlatform(LEOGetCPU());
    fs.nStyle = LS_PRINT;
    memcpy(fs.AlphaTable,alphabet1,256);
    LEOSetupField(&fs);

    for(i=0,numInvalid=0;i<nClust;i++)
    {
        cluInfo.let = 0;
        j=FONGetClustInfo(&cluInfo,i+1);
        if(j<=0)
            continue;

        if(cluInfo.attr & CTB_PRINT_ITALIC)
            continue;

        if(cluInfo.attr & CTB_PRINT_BOLD)
            continue;

        // now - test only russian ASCII letters
        if(cluInfo.let < 128 ||
           cluInfo.let >= 176 && cluInfo.let < 224 ||
           cluInfo.let > 240
           )
            continue;

        addLet=(cluInfo.let < 144 ? cluInfo.let +32 :
                cluInfo.let < 160 ? cluInfo.let +80 :
                cluInfo.let < 176 ? cluInfo.let -32 :
                cluInfo.let - 80
                );


        name=0;
        FONGetClusterAsBW(&name,i,porog,&rec);

        memset(&ro,0,sizeof(RecObject));
        memcpy(&ro.recData.recRaster,&rec,sizeof(RecRaster));
        ps.nIdPage=-1;
        ro.recData.lwStatus=0;
        LEOSetupPage(&ps);

        LEORecogPrintChar(&ro);

        // ������ �������� �� LEO ?
        if( ro.recResults.lnAltCnt <= 0 ||
            ro.recResults.Alt[0].Prob < 150
            )
            continue;

        for(j=0;j<ro.recResults.lnAltCnt;j++)
        {
            resLet = stdAnsiToAscii(ro.recResults.Alt[j].Code);

            if( resLet == cluInfo.let ||
                resLet == addLet )
                break;
        }

        if(j==0)
            continue;

        { char *qq;

            resLet = stdAnsiToAscii(ro.recResults.Alt[0].Code);
            if( !is_lower(resLet) )
                resLet =  to_lower(resLet);
            if( (qq=strchr(hasNearSame,cluInfo.let)) &&
                NearSame[qq-(char*)hasNearSame] == resLet
                )
                continue;
        }

        // �������� ��� ���-�� ���� ?
        // ���� ������ �� ������������ - ������ ('�' � sten91)
        if( j >= ro.recResults.lnAltCnt ||
            ro.recResults.Alt[j].Prob < 180 ||
            ro.recResults.Alt[j].Prob < 220 &&
            ro.recResults.Alt[j].Prob + 25 < ro.recResults.Alt[0].Prob
            )
        {
            FonTestInfo testInfo[MAXCHECKALT];

            // ��������
            resLet = stdAnsiToAscii(ro.recResults.Alt[0].Code);
            j=FONTestChar(&rec,resLet,testInfo,0);

            if( j <=0 || testInfo[0].prob <= 215 )
            {
                resLet=(resLet < 144 ? resLet +32 :
                        resLet < 160 ? resLet +80 :
                        resLet < 176 ? resLet -32 :
                        resLet - 80
                        );
                j=FONTestChar(&rec,resLet,testInfo,0);
            }

            if( j > 0 && testInfo[0].prob > 215 )
            {
                numInvalid++;
                FONSetClusterInvalid(i+1);
#ifdef _SAVE_INVALID_CLU_
                fprintf(fp,"    invalid %d (%c -> %c(%d))\n",i+1,cluInfo.let,
                        stdAnsiToAscii(ro.recResults.Alt[0].Code),
                        ro.recResults.Alt[0].Prob);
#endif
            }
        } // end if j

    }   // end clusters



#ifdef _SAVE_INVALID_CLU_
    fclose(fp);
#endif

    return numInvalid;
#else
    return 0;
#endif
}
////////////////

Bool32 RecogLEOcap(RecRaster *Rs,uchar Language,RecVersions *Vs)
{
#ifdef _USE_LEO_
    LeoFieldSetup  fs={0};
    RecObject      ro={0};
    LeoPageSetup   ps={0};
    char    *      alpha;

    switch(Language)
    {
    case PUMA_LANG_RUSSIAN:
        alpha = alpha_str_cap;
        break;
    case PUMA_LANG_ENGLISH:
        alpha = alpha_str_lat_cap;
        break;
    default:
        alpha = alpha_str_all_cap;
        break;
    }

    set_alphabet(alphabet1, alpha);

    LEOSetPlatform(586);
    fs.nStyle = LS_PRINT;
    memcpy(fs.AlphaTable,alphabet1,256);
    LEOSetupField(&fs);

    memset(&ro,0,sizeof(RecObject));
    memcpy(&ro.recData.recRaster,Rs,REC_MAX_RASTER_SIZE);
    ps.nIdPage=-1;
    ro.recData.lwStatus=0;
    LEOSetupPage(&ps);

    LEORecogPrintChar(&ro);
    memcpy(Vs,&ro.recResults,sizeof(RecVersions));
#endif
    return TRUE;
}

Bool32 RecogLEO(RecRaster *Rs,uchar Language,UniVersions *Us)
{
#ifdef _USE_LEO_
    LeoFieldSetup  fs={0};
    RecObject      ro={0};
    LeoPageSetup   ps={0};
    char    *      alpha;
    uchar          c, cw;
    int32_t          i, up=-1;


    alpha = alpha_str;
    set_alphabet(alphabet1, alpha);

    LEOSetPlatform(586);
    fs.nStyle = LS_PRINT;
    memcpy(fs.AlphaTable,alphabet1,256);
    LEOSetupField(&fs);

    memset(&ro,0,sizeof(RecObject));
    memcpy(&ro.recData.recRaster,Rs,REC_MAX_RASTER_SIZE);
    ps.nIdPage=-1;
    ro.recData.lwStatus=0;
    LEOSetupPage(&ps);

    LEORecogPrintChar(&ro);
    //        memcpy(Vs,&ro.recResults,sizeof(RecVersions));
    if( Us->lnAltCnt )
    {
        c = Us->Alt[0].Liga     ;
        if( is_upper(c) )
            up=1;
        else  if( is_lower(c) )
            up=0;
    }
    Us->lnAltCnt=ro.recResults.lnAltCnt;
    Us->lnAltMax=REC_MAX_VERS;

    for(i=0;i<ro.recResults.lnAltCnt;i++)
    {
        c  = stdAnsiToAscii( ro.recResults.Alt[i].Code);
        switch( up )
        {
                case    1: // upper
                    c=to_upper(c);
                    break;
                case    0: // lower
                    c=to_lower(c);
                    break;
                default:
                    break;
                }
        cw = stdAsciiToAnsi( c );
        Us->Alt[i].Code[0]=cw;
        Us->Alt[i].Code[1]=0;
        Us->Alt[i].Liga=c;
        Us->Alt[i].Charset=CSTR_RUSSIAN_CHARSET;
        Us->Alt[i].Method =REC_METHOD_LEO;
        Us->Alt[i].Prob   = ro.recResults.Alt[i].Prob ;
    }
#endif
    return TRUE;
}

Bool32  p2_msk_init()
{
#ifdef _USE_MSK_
    HndTab = MSKInit( NULL, "rec4cour.dat");
    HndTabInc = MSKInit( NULL, "rec4inc.dat");
#endif
    return HndTab;
}

void    p2_msk_done(void)
{
#ifdef _USE_MSK_
    MSKDone();
#endif
    return;
}

#define MIN_MSK_PROB        207
#define MIN_MSK_PROB_SIM    230
static char similar_letters[]="����Ꭾ��";
Bool32   p2_msk_inc(CSTR_rast    rst)
{
#ifdef _USE_MSK_
    uchar       prob=0, name, lim, code;
    RecRaster   rc;
    RecVersions ver;
    int32_t       i;

    if(!HndTabInc )
        return 0;
    if(language!=PUMA_LANG_RUSSIAN && language!=PUMA_LANG_RUSENG )
        return 0;
    CSTR_GetImage(rst,(uchar*)&rc,CSTR_TYPE_IMAGE_RS);
    if( !rc.lnPixHeight || !rc.lnPixWidth )
        return 0;
    MSKSetAlphabet  ( alphabet );

    CSTR_GetCollection(rst,&ver);
    ver.lnAltCnt=2;
    ver.lnAltMax=REC_MAX_VERS;
    name = ver.Alt[0].Code;
    ver.Alt[0].Code=(uchar)to_upper((uchar)name);
    ver.Alt[0].Prob=255;
    ver.Alt[1].Code=(uchar)to_lower((uchar)name);
    ver.Alt[1].Prob=255;
    if( name=='0' )
    {
        ver.Alt[1].Code=(uchar)'�';
        ver.Alt[2].Code=(uchar)'�';
        ver.Alt[2].Prob=255;
        ver.lnAltCnt=3;
    }
    if( name=='3' )
    {
        ver.Alt[1].Code=(uchar)'�';
        ver.Alt[2].Code=(uchar)'�';
        ver.Alt[2].Prob=255;
        ver.lnAltCnt=3;
    }
    if( !MSKRecogCharExpPuma( HndTabInc, &rc, &ver,1 ) )
        return 0;
    if( !ver.lnAltCnt )
        return 0;

    for(prob=0, i=0;i<ver.lnAltCnt;i++)
    {
        if( prob<ver.Alt[i].Prob)
        {
            prob=ver.Alt[i].Prob ;
            code=ver.Alt[i].Code;
        }
    }
    lim = MIN_MSK_PROB;
    if( strchr(similar_letters,code) )
        lim = MIN_MSK_PROB_SIM;
    return (prob>lim);
#else
    return FALSE;
#endif
}


Bool32 RecogLEO_SetAlphabet(char *letters)
{
    set_alphabet(alphabet1, letters);
    return TRUE;
}

Bool32 RecogLEOall(RecRaster *Rs,RecVersions *Us,int32_t nType)
{
#ifdef _USE_LEO_
    LeoFieldSetup  fs={0};
    RecObject      ro={0};
    LeoPageSetup   ps={0};

    fs.nStyle = LS_PRINT;
    memcpy(fs.AlphaTable,alphabet1,256);
    LEOSetupField(&fs);
    memset(&ro,0,sizeof(RecObject));
    memcpy(&ro.recData.recRaster,Rs,REC_MAX_RASTER_SIZE);
    ps.nIdPage=-1;
    ro.recData.lwStatus=0;
    LEOSetupPage(&ps);

    if( nType==0 )
    {
        LEORecogPrintChar(&ro);
    }

    *Us=ro.recResults;
#endif
    return TRUE;
}
