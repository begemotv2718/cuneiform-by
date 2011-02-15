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

/*------------------------------------------------------------------------------------------------------------
   Written by Shahverdiev  Alik
   This file cration date: 29.05.00

   frmtallignment.cpp

---------------------------------------------------------------------------------------------------------------*/

//#include <afxtempl.h>
//#include <afxwin.h>
#include <stdlib.h>
#include "stdafx.h"
#include <cstring>
#include "globus.h"
#include "creatertf.h"
#include "dpuma.h"

extern float  Twips;
extern uint32_t FlagMode;
extern Bool32 FlagLineTransfer;
extern Bool32 FlagDebugAlign;

/*------------------------------------------------------------------------------------------------------------
   RTF_TP_LEFT_ALLIGN                   0   -  ������������� �� ������ ����
 	 RTF_TP_RIGHT_ALLIGN                  1   -  ������������ �� ������� ����
   RTF_TP_LEFT_ALLIGN&TP_RIGHT_ALLIGN   2   -  ������������ �� ������
   RTF_TP_CENTER                        3   -  ������������ �� ������
   RTF_TP_ONE                           4   -  ������ ������ ������� �� ������ �����, ������������ ������ ����
   RTF_TP_TYPE_LINE                     5   -  ������
---------------------------------------------------------------------------------------------------------------*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 SetFragmentAlignment
void CRtfFragment::SetFragmentAlignment(RtfSectorInfo* SectorInfo)
{

 if(ProcessingUseNoneMode()==FALSE)
 {

  Init(SectorInfo);

  if(ProcessingOverLayedFragment(SectorInfo)==FALSE)
  {
   if(DeterminationOfMixedFragment(SectorInfo)==FALSE)
   {
    if(DeterminationOfLeftRightJustification(0, m_wStringsCount)==FALSE)
    {
     if(DeterminationOfListType(0, m_wStringsCount)==FALSE)
     {
      if(DeterminationOfLeftJustification(0, m_wStringsCount, 0)==FALSE)
      {
       if(DeterminationOfCentreJustification(0, m_wStringsCount)==FALSE)
       {
        if(DeterminationOfRightJustification(0, m_wStringsCount)==FALSE)
        {
         DeterminationOfLeftJustification(0, m_wStringsCount, 1);
        }
       }
      }
     }
    }
   }
   Done();
  }

 }

}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                     ProcessingUseNoneMode

Bool CRtfFragment::ProcessingUseNoneMode(void)
{
 CRtfString  *pRtfString;
 int ns;

  if(!(FlagMode & USE_NONE))
   return FALSE;

  for( ns=0; ns<m_wStringsCount; ns++ )
	 {
   pRtfString=(CRtfString*)m_arStrings[ns];

   if(!ns) pRtfString->m_wFlagBeginParagraph = TRUE;
   else				pRtfString->m_wFlagBeginParagraph = FALSE;

			if(ns == m_wStringsCount-1) pRtfString->m_bLineTransfer = FALSE;
			else                        pRtfString->m_bLineTransfer = TRUE;

   pRtfString->m_wAlignment = RTF_TP_LEFT_ALLIGN;
			pRtfString->m_wLeftIndent = pRtfString->m_wFirstIndent = pRtfString->m_wRightIndent = 0;
  }

	 return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                            Init

void CRtfFragment::Init(RtfSectorInfo* SectorInfo)
{
	int         LeftDif, RightDif, CentreDif;
	CRtfString  *pRtfStringPrev, *pRtfString;
	CRtfWord    *pRtfWord;
	CRtfChar    *pRtfCharFirst, *pRtfCharLast;
	int32_t       CountChars  = 0;
	int32_t       LengthChars = 0;
	int ns;

	m_l_fragment = 32000;
	m_r_fragment = 0;

  //  I. �����:       �����(m_l_fragment) � ������(m_r_fragment) ������ ���������
  //  II.����������:  m_max_dist, ������� ������������ ��� ������ ������ ������
	for(ns=0; ns<m_wStringsCount; ns++ )
	{
		pRtfString = (CRtfString*)m_arStrings[ns];

		if(!ns)	{pRtfString->m_wSpaceBefore = SectorInfo->VerticalOffsetFragmentInColumn;
			pRtfString->m_wFlagBeginParagraph = TRUE;}
		else    pRtfString->m_wSpaceBefore = 0;

		pRtfWord      = (CRtfWord*)pRtfString->m_arWords[0];
		pRtfCharFirst = (CRtfChar*)pRtfWord->m_arChars[0];
		pRtfString->m_FirstChar = pRtfCharFirst->m_chrVersions[0].m_bChar;

		pRtfWord      = (CRtfWord*)pRtfString->m_arWords[pRtfString->m_wWordsCount-1];
		pRtfCharLast  = (CRtfChar*)pRtfWord->m_arChars[pRtfWord->m_wCharsCount-1];
		pRtfString->m_LastChar = pRtfCharLast->m_chrVersions[0].m_bChar;

		pRtfString->m_LeftBorder  = pRtfCharFirst->m_Idealrect.left;
		pRtfString->m_RightBorder = pRtfCharLast->m_Idealrect.right;

		CalculationLengthAndCount(pRtfString, &CountChars, &LengthChars);
		m_l_fragment  = MIN(m_l_fragment, (int16_t)pRtfCharFirst->m_Idealrect.left);
		m_r_fragment  = MAX(m_r_fragment, (int16_t)pRtfCharLast->m_Idealrect.right);

		if(pRtfCharLast->m_chrVersions[0].m_bChar=='-' && pRtfCharLast->m_bFlg_spell_nocarrying)
			pRtfString->m_FlagCarry=TRUE;
	}

	if(CountChars) m_max_dist = (int16_t)(LengthChars/CountChars);
	else           m_max_dist = 10;

  // ����������� ������(m_wLeftIndent, m_wRightIndent) ������ �� ����� ��������� � ����� ������
	for( ns=0; ns < m_wStringsCount; ns++ )
	{
		pRtfString = (CRtfString*)m_arStrings[ns];
		pRtfString->m_wLeftIndent  = (int16_t)(pRtfString->m_LeftBorder - m_l_fragment);
		pRtfString->m_wRightIndent = (int16_t)(m_r_fragment - pRtfString->m_RightBorder);
		pRtfString->m_wCentre      = (int16_t)(pRtfString->m_LeftBorder + pRtfString->m_RightBorder)/2;
	}

  // ������������� �������� ��������� ������ � �������� �������� �����
	for( ns=1; ns < m_wStringsCount; ns++ )
	{
		pRtfStringPrev = (CRtfString*)m_arStrings[ns-1];
		pRtfString     = (CRtfString*)m_arStrings[ns];

		LeftDif   = pRtfString->m_wLeftIndent  - pRtfStringPrev->m_wLeftIndent;
		RightDif  = pRtfString->m_wRightIndent - pRtfStringPrev->m_wRightIndent;
  		CentreDif = pRtfString->m_wCentre      - pRtfStringPrev->m_wCentre;

		if( abs(LeftDif) <= m_max_dist )
		{
 			pRtfString->m_wLeftBorderEqual = TRUE;
			m_CountLeftEqual++;
			if(ns==1)
				{pRtfStringPrev->m_wLeftBorderEqual = TRUE;
				m_CountLeftEqual++;
			}
		}

		if( abs(RightDif) <= m_max_dist )
		{
			pRtfString->m_wRightBorderEqual = TRUE;
			m_CountRightEqual++;
			if(pRtfString->m_wLeftBorderEqual==TRUE)
				m_CountLeftRightEqual++;

			if(ns==1)
				{pRtfStringPrev->m_wRightBorderEqual = TRUE;
				m_CountRightEqual++;
				m_CountLeftRightEqual++;
			}
		}

		if(((abs(CentreDif) < m_max_dist) &&
			((LeftDif<=0 && RightDif<=0) || (LeftDif>0 && RightDif>0)) &&
			(abs(LeftDif) > m_max_dist/2)  && (abs(RightDif) > m_max_dist/2))  ||
			((abs(CentreDif) < 2*m_max_dist) && (abs(RightDif-LeftDif) < 3*m_max_dist) && (abs(LeftDif) > 5*m_max_dist) && (abs(RightDif) > 5*m_max_dist)) )
		{
			pRtfString->m_wCentreEqual = TRUE;
			m_CountCentreEqual++;

			if(ns==1)
				{pRtfStringPrev->m_wCentreEqual = TRUE;
				m_CountCentreEqual++;
			}
		}

	}

	PrintTheResult("\n ================== Init ================== \n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 ProcessingOverLayedFragment

// ���� ������� �������� ����������������, �� ��� ��� ����� �������� ��� frames

Bool CRtfFragment::ProcessingOverLayedFragment(RtfSectorInfo* SectorInfo)
{
 CRtfString  *pRtfStringPrev;
 CRtfString  *pRtfStringNext;
 CRtfString  *pRtfString;

	if(!(SectorInfo->FlagOverLayed)) return FALSE;

 int ns(0);
 for(ns=0; ns<m_wStringsCount; ns++ )
	{
		pRtfString = (CRtfString*)m_arStrings[ns];
	 pRtfString->m_wAlignment          = RTF_TP_LEFT_AND_RIGHT_ALLIGN;
		pRtfString->m_wFlagBeginParagraph = FALSE;
		pRtfString->m_wRightIndent        = 0;
	}

 
 for(ns=0; ns<m_wStringsCount; ns++ )
	{
		pRtfString = (CRtfString*)m_arStrings[ns];

		if(ns==0)
		{
			if(pRtfString->m_wLeftIndent>m_max_dist/2)
				pRtfString->m_wFirstIndent = (uint16_t)(m_max_dist*Twips);
			else
				pRtfString->m_wFirstIndent = 0;

   pRtfString->m_wLeftIndent = 0;
			pRtfString->m_wFlagBeginParagraph = TRUE;
		 continue;
		}

		pRtfStringPrev = (CRtfString*)m_arStrings[ns-1];

		if(ns==m_wStringsCount-1)
		{
			if((pRtfString->m_wLeftIndent-pRtfStringPrev->m_wLeftIndent)>(m_max_dist/2))
			{
    pRtfString->m_wLeftIndent         = 0;
				pRtfString->m_wFirstIndent        = (uint16_t)(m_max_dist*Twips);
 	 	pRtfString->m_wFlagBeginParagraph = TRUE;
			}
		}
		else
		{
 		pRtfStringNext = (CRtfString*)m_arStrings[ns+1];
   if( ((pRtfString->m_wLeftIndent - pRtfStringPrev->m_wLeftIndent)>(m_max_dist/2)) &&
				   ((pRtfString->m_wLeftIndent - pRtfStringNext->m_wLeftIndent)>(m_max_dist/2)) )
				{
     pRtfString->m_wLeftIndent         = 0;
					pRtfString->m_wFirstIndent        = (uint16_t)(m_max_dist*Twips);
 	 		pRtfString->m_wFlagBeginParagraph = TRUE;
				}
  }
 }

 return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 DeterminationOfLeftRightJustification

Bool CRtfFragment::DeterminationOfLeftRightJustification(int beg, int end)
{

 if(!CheckLeftRightJustification(beg, end))  return FALSE;

 SetParagraphAlignment(beg, end, RTF_TP_LEFT_AND_RIGHT_ALLIGN);
 SetFlagBeginParagraphForLeftRightJustification(beg, end);
 CorrectIndents(beg, end);

	PrintTheResult("\n ================== DeterminationOfLeftRightJustification ================== \n");

 return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 CheckLeftRightJustification

Bool CRtfFragment::CheckLeftRightJustification(int beg, int end)
{
 CRtfString  *pRtfString;
 int          Count = 0;
 uint16_t         CountLeftRightEqual=0;
 uint16_t         CountCentreEqual=0;
 uint16_t         CountLeftEqual=0;

 GetCountEqual(beg, end, &CountCentreEqual, RTF_TP_CENTER);
 GetCountEqual(beg, end, &CountLeftEqual, RTF_TP_LEFT_ALLIGN);

 if(CountCentreEqual==(end-beg))
  return FALSE;

 GetCountEqual(beg, end, &CountLeftRightEqual, RTF_TP_LEFT_AND_RIGHT_ALLIGN);
 m_FlagCarry=GetFlagCarry(beg, end);
 m_FlagLeft=GetFlagLeft(beg, end);
 m_FlagStrongLeft=GetFlagStrongLeft(beg, end);
 m_FlagRight=GetFlagRight(beg, end);
 m_FlagBigSpace=GetFlagBigSpace(beg,end);

 if(m_FlagStrongLeft==TRUE)
  return FALSE;

 if(m_FlagCarry==FALSE && end-beg<=2 && CountLeftRightEqual==0)
  return FALSE;

	if(m_FlagCarry==FALSE && m_FlagBigSpace==FALSE && (CountLeftRightEqual < (end-beg)/3))
  return FALSE;

 if(m_FlagRight==TRUE && end-beg<=4)
  return FALSE;

 if((CountLeftEqual>=(end-beg-1)) && m_FlagBigSpace==FALSE)
 {
  if(((end-beg)<5) && (CountLeftEqual>CountLeftRightEqual))
   return FALSE;

  if(CountLeftRightEqual<=(end-beg)/2)
   return FALSE;
 }

 for(int ns=beg+1; ns<end; ns++ )
	{
  pRtfString = (CRtfString*)m_arStrings[ns];

  if(pRtfString->m_wLeftBorderEqual && pRtfString->m_wRightBorderEqual)
   continue;

  if(CheckStringForLeftRightJustification(ns))
		 Count++;
	}

 if(m_FlagLeft && ((Count + CountLeftRightEqual) < 4*(end-beg-1)/5))
  return FALSE;

 if(m_FlagCarry)
 {
  if((Count + CountLeftRightEqual) < (end-beg-1)/3)
   return FALSE;
 }
 else
 {
  if((Count + CountLeftRightEqual) < 4*(end-beg-1)/5)
   return FALSE;

  GetCountEqual(beg, end, &CountCentreEqual, RTF_TP_CENTER);
  if((Count + CountLeftRightEqual) < CountCentreEqual)
   return FALSE;
 }
 return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 GetCountEqual

void CRtfFragment::GetCountEqual(int beg, int end, uint16_t* Count, int AlignType)
{
 CRtfString  *pRtfString;

 for(int i=beg; i<end; i++)
 {
  pRtfString = (CRtfString*)m_arStrings[i];

  switch(AlignType)
  {
  case RTF_TP_LEFT_ALLIGN:
                                    if(pRtfString->m_wLeftBorderEqual==TRUE)
                                     (*Count)++;
                                    break;
  case RTF_TP_RIGHT_ALLIGN:
                                    if(pRtfString->m_wRightBorderEqual==TRUE)
                                     (*Count)++;
                                    break;
  case RTF_TP_LEFT_AND_RIGHT_ALLIGN:
                                    if((pRtfString->m_wLeftBorderEqual==TRUE) && (pRtfString->m_wRightBorderEqual==TRUE))
                                     (*Count)++;
                                    break;
  case RTF_TP_CENTER:
                                    if(pRtfString->m_wCentreEqual==TRUE)
                                     (*Count)++;
                                    break;

  default:
                                    break;
  }

 }

 if(*Count==1) (*Count)++;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                       CheckStringForLeftRightJustification

Bool CRtfFragment::CheckStringForLeftRightJustification(int ns)
{
 CRtfString *pRtfString;
 CRtfString *pRtfStringPrev;
 CRtfString *pRtfStringNext;
 int16_t       LeftFragm, RightFragm;
 int16_t       LeftDif,RightDif;

 if(m_Attr)
 {
  LeftFragm = m_l_fragmentLocal; RightFragm = m_r_fragmentLocal;
  LeftDif = m_l_fragmentLocal-m_l_fragment;
  RightDif= m_r_fragment-m_r_fragmentLocal;
 }
 else
 {LeftFragm  = m_l_fragment; RightFragm = m_r_fragment; LeftDif=0; RightDif=0;}

 pRtfString = (CRtfString*)m_arStrings[ns];

 if((pRtfString->m_wLeftIndent-LeftDif)>m_max_dist && ns<m_wStringsCount-1 )
 {
  pRtfStringNext = (CRtfString*)m_arStrings[ns+1];
  if(((pRtfString->m_wLeftIndent-LeftDif) < (RightFragm-LeftFragm)/2) &&
     ((pRtfString->m_wRightIndent-RightDif) < m_max_dist)              &&
     ((pRtfStringNext->m_wLeftIndent-LeftDif) < m_max_dist))
   return TRUE;
 }

 if((pRtfString->m_wLeftIndent-LeftDif)<m_max_dist && ns>1)
 {
  pRtfStringPrev = (CRtfString*)m_arStrings[ns-1];
  if((pRtfStringPrev->m_wRightIndent-RightDif)<m_max_dist)
   return TRUE;
 }

return FALSE;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 SetFlagBeginParagraphForLeftRightJustification

void CRtfFragment::SetFlagBeginParagraphForLeftRightJustification(int beg, int end)
{
 CRtfString  *pRtfStringPrev;
 CRtfString  *pRtfString;

 for(int ns=beg; ns<end; ns++ )
	{
		pRtfString = (CRtfString*)m_arStrings[ns];
		if(ns==beg) {	pRtfString->m_wFlagBeginParagraph = TRUE; continue;	}

		pRtfStringPrev = (CRtfString*)m_arStrings[ns-1];

  if(
     ((pRtfString->m_wLeftIndent      >  2*m_max_dist) && (abs(pRtfString->m_wLeftIndent-pRtfStringPrev->m_wLeftIndent)>m_max_dist)) ||
     (pRtfStringPrev->m_wRightIndent  >  10*m_max_dist)                                                                         ||
     ((pRtfStringPrev->m_wRightIndent >  5*m_max_dist) && (pRtfStringPrev->m_LastChar==';' || pRtfStringPrev->m_LastChar=='.')) ||
     (CheckNumber(pRtfString->m_FirstChar) && (pRtfStringPrev->m_LastChar==';' || pRtfStringPrev->m_LastChar=='.'))             ||
     ((pRtfString->m_wLeftIndent      >  3*m_max_dist/2) && (pRtfStringPrev->m_LastChar=='.' || pRtfString->m_FirstChar==TIRE)) ||
     (pRtfStringPrev->m_LastChar=='.' && (pRtfString->m_FirstChar=='-'  || pRtfString->m_FirstChar==TIRE))                      ||
     (pRtfStringPrev->m_LastChar=='?' && (pRtfString->m_FirstChar=='-'  || pRtfString->m_FirstChar==TIRE))                      ||
     (pRtfStringPrev->m_LastChar==':' && (pRtfString->m_FirstChar=='-'  || pRtfString->m_FirstChar==TIRE))                      ||
     (pRtfStringPrev->m_wRightIndent  >  2*m_max_dist && pRtfString->m_FirstChar==TIRE)
    )
			pRtfString->m_wFlagBeginParagraph = TRUE;
 }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 CheckNumber

Bool CRtfFragment::CheckNumber(uchar FirstChar)
{
 const char* result=NULL;
 const char* MasNumber="0123456789";

	 result = strchr( MasNumber, FirstChar );
  if(result==NULL)
   return FALSE;

 return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 CorrectIndents

void CRtfFragment::CorrectIndents(int beg, int end)
{
 CRtfString*  pRtfString;
 int16_t        MinLeftIndent;
 int16_t        MinRightIndent;

 	 int i(0);
     for(i=beg; i<end; i++ )
		 {
    pRtfString      = (CRtfString*)m_arStrings[i];
    if(i==beg)
    {
  	  MinLeftIndent  = pRtfString->m_wLeftIndent;
		   MinRightIndent = pRtfString->m_wRightIndent;
     continue;
    }
    if(pRtfString->m_wFlagBeginParagraph==TRUE)
    {
     MinLeftIndent   = MIN(pRtfString->m_wLeftIndent,  MinLeftIndent);
     MinRightIndent  = MIN(pRtfString->m_wRightIndent, MinRightIndent);
    }
		 }

 	 for(i=beg; i<end; i++)
		 {
    pRtfString = (CRtfString*)m_arStrings[i];
    if(pRtfString->m_wFlagBeginParagraph==TRUE)
    {
     if(abs(pRtfString->m_wLeftIndent-MinLeftIndent) < m_max_dist)
      pRtfString->m_wLeftIndent = MinLeftIndent;
    }
		 }

}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 DeterminationOfLeftJustification

Bool CRtfFragment::DeterminationOfLeftJustification(int beg, int end, Bool direct)
{

 if(!direct && !CheckLeftJustification(beg, end))  return FALSE;

 SetParagraphAlignment(beg, end, RTF_TP_LEFT_ALLIGN);

 if(GetFlagCarry(beg, end)==FALSE || GetFlagLeft(beg, end))
		SetLineTransfer(beg, end-1);

 SetFlagBeginParagraphForLeftJustification(beg, end);
 CorrectIndents(beg, end);

	PrintTheResult("\n ================== DeterminationOfLeftJustification ================== \n");

 return TRUE;

}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 CheckLeftJustification

Bool CRtfFragment::CheckLeftJustification(int beg, int end)
{
 uint16_t CountLeftEqual=0;
 uint16_t CountRightEqual=0;
 uint16_t CountLeftRightEqual=0;
 uint16_t CountCentreEqual=0;

 GetCountEqual(beg, end, &CountLeftEqual,      RTF_TP_LEFT_ALLIGN);
 GetCountEqual(beg, end, &CountRightEqual,     RTF_TP_RIGHT_ALLIGN);
 GetCountEqual(beg, end, &CountLeftRightEqual, RTF_TP_LEFT_AND_RIGHT_ALLIGN);
 GetCountEqual(beg, end, &CountCentreEqual,    RTF_TP_CENTER);

	if((CountLeftEqual < (end-beg)/2         ||
     CountLeftEqual < CountRightEqual     ||
     CountLeftEqual < CountLeftRightEqual ||
     CountLeftEqual < CountCentreEqual)   &&
    (CountRightEqual+ CountLeftRightEqual + CountCentreEqual>0 ))
  return FALSE;

 return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 SetFlagBeginParagraphForLeftJustification

void CRtfFragment::SetFlagBeginParagraphForLeftJustification(int beg, int end)
{
 CRtfString  *pRtfStringPrev;
 CRtfString  *pRtfString;
 uchar        FlagStringParagraph=FALSE;
 uchar        FlagStringParagraphSoft=FALSE;
 uint16_t        Count=0;
 int16_t       LeftFragm, RightFragm;
 int16_t       LeftDif,RightDif;

 if(m_Attr)
 {
  LeftFragm = m_l_fragmentLocal; RightFragm = m_r_fragmentLocal;
  LeftDif = m_l_fragmentLocal-m_l_fragment;
  RightDif= m_r_fragment-m_r_fragmentLocal;
 }
 else
 {LeftFragm  = m_l_fragment; RightFragm = m_r_fragment; LeftDif=0; RightDif=0;}

 int ns(0);
 for(ns=beg; ns<end; ns++ )
	{
		pRtfString = (CRtfString*)m_arStrings[ns];
  if(pRtfString->m_LastChar=='.')
   Count++;
 }

 if(Count>(4*(end-beg)/5))
  FlagStringParagraph=TRUE;

 if(Count>=((end-beg)/3))
  FlagStringParagraphSoft=TRUE;

 for(ns=beg; ns<end; ns++ )
	{
		pRtfString = (CRtfString*)m_arStrings[ns];
		if(ns==beg) {	pRtfString->m_wFlagBeginParagraph = TRUE; continue;	}

		pRtfStringPrev = (CRtfString*)m_arStrings[ns-1];

  if(((pRtfString->m_wLeftIndent-LeftDif) > 2*m_max_dist)                                                                          ||
     ((pRtfStringPrev->m_wRightIndent-RightDif) > (RightFragm-LeftFragm)/3)                                                        ||
     ((pRtfString->m_wLeftIndent > m_max_dist) && (pRtfString->m_FirstChar=='�' || pRtfString->m_FirstChar==TIRE))                 ||
     (CheckNumber(pRtfString->m_FirstChar) && (pRtfStringPrev->m_LastChar==';' || pRtfStringPrev->m_LastChar=='.'))                ||
     (pRtfStringPrev->m_LastChar=='.' && FlagStringParagraphSoft==TRUE && (pRtfStringPrev->m_wRightIndent-RightDif)>5*m_max_dist ) ||
     (pRtfStringPrev->m_LastChar=='.' && FlagStringParagraph==TRUE))
  {
   pRtfStringPrev->m_bLineTransfer = FALSE;
			pRtfString->m_wFlagBeginParagraph = TRUE;
  }
 }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 DeterminationOfCentreJustification

Bool CRtfFragment::DeterminationOfCentreJustification(int beg, int end)
{
 CRtfString  *pRtfString;

 uint16_t CountLeftEqual=0;
 uint16_t CountRightEqual=0;
 uint16_t CountLeftRightEqual=0;
 uint16_t CountCentreEqual=0;

 GetCountEqual(beg, end, &CountLeftEqual,      RTF_TP_LEFT_ALLIGN);
 GetCountEqual(beg, end, &CountRightEqual,     RTF_TP_RIGHT_ALLIGN);
 GetCountEqual(beg, end, &CountLeftRightEqual, RTF_TP_LEFT_AND_RIGHT_ALLIGN);
 GetCountEqual(beg, end, &CountCentreEqual,    RTF_TP_CENTER);

	if((CountCentreEqual < (end-beg)/2         ||
     CountCentreEqual < CountRightEqual     ||
     CountCentreEqual < CountLeftRightEqual ||
     CountCentreEqual < CountLeftEqual)   &&
    (CountRightEqual+ CountLeftRightEqual + CountLeftEqual>0 ))
  return FALSE;

 SetParagraphAlignment(beg, end, RTF_TP_CENTER);
 SetLineTransfer( beg,  end);

	pRtfString = (CRtfString*)m_arStrings[beg];
	pRtfString->m_wFlagBeginParagraph = TRUE;

	PrintTheResult("\n ================== DeterminationOfCentreJustification ================== \n");

return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 DeterminationOfRightJustification

Bool CRtfFragment::DeterminationOfRightJustification(int beg, int end)
{
 CRtfString  *pRtfStringPrev;
 CRtfString  *pRtfString;

 uint16_t CountLeftEqual=0;
 uint16_t CountRightEqual=0;
 uint16_t CountLeftRightEqual=0;
 uint16_t CountCentreEqual=0;

  m_FlagCarry=GetFlagCarry(beg, end);
  if(m_FlagCarry && m_FlagRight==FALSE) return FALSE;

  GetCountEqual(beg, end, &CountLeftEqual,      RTF_TP_LEFT_ALLIGN);
  GetCountEqual(beg, end, &CountRightEqual,     RTF_TP_RIGHT_ALLIGN);
  GetCountEqual(beg, end, &CountLeftRightEqual, RTF_TP_LEFT_AND_RIGHT_ALLIGN);
  GetCountEqual(beg, end, &CountCentreEqual,    RTF_TP_CENTER);

	 if(CountRightEqual < (end-beg)/2 )
   return FALSE;

  if((CountRightEqual < CountCentreEqual ||
      CountRightEqual < CountLeftRightEqual ||
      CountRightEqual < CountLeftEqual)   &&
     (CountCentreEqual+ CountLeftRightEqual + CountLeftEqual>0 ))
  return FALSE;

  SetParagraphAlignment(beg, end, RTF_TP_RIGHT_ALLIGN);

  for(int ns=beg; ns < end; ns++ )
	 {
	 	pRtfString = (CRtfString*)m_arStrings[ns];
   if(ns==beg) {pRtfString->m_wFlagBeginParagraph = TRUE; pRtfString->m_bLineTransfer = TRUE; continue;}

   pRtfString->m_bLineTransfer = TRUE;

	 	pRtfStringPrev = (CRtfString*)m_arStrings[ns-1];
   if(pRtfStringPrev->m_LastChar=='.')
   {
	 	 pRtfString->m_wFlagBeginParagraph = TRUE;
    pRtfStringPrev->m_bLineTransfer = FALSE;
   }
  }

	 PrintTheResult("\n ================== DeterminationOfRightJustification ================== \n");

return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 DeterminationOfListType

Bool CRtfFragment::DeterminationOfListType(int beg, int end)
{
 CRtfString  *pRtfString;
	uchar        FlagListParagraph = 0;
 int32_t       MinLeft,MaxLeft,MaxRight;
 int32_t       CountMinLeft=0, CountMaxLeft=0, CountMaxRight=0;
 uint16_t        CountCentreEqual=0;

  GetCountEqual(beg, end, &CountCentreEqual, RTF_TP_CENTER);

  if(CountCentreEqual==(end-beg))
   return FALSE;

  pRtfString = (CRtfString*)m_arStrings[0];
  MinLeft=MaxLeft=pRtfString->m_LeftBorder;
  MaxRight=pRtfString->m_RightBorder;

  int ns(0);
	 //����� ���� ������
  for(ns=beg; ns<end; ns++ )
	 {
	 	pRtfString = (CRtfString*)m_arStrings[ns];
   MinLeft  = MIN(MinLeft,  pRtfString->m_LeftBorder);
   MaxLeft  = MAX(MaxLeft,  pRtfString->m_LeftBorder);
   MaxRight = MAX(MaxRight, pRtfString->m_RightBorder);
  }

  if((MaxLeft-MinLeft)>(MaxRight-MaxLeft)/2)
   return FALSE;

  if((MaxLeft-MinLeft)<m_max_dist)
   return FALSE;

  for(ns=beg; ns<end; ns++ )
	 {
	 	pRtfString = (CRtfString*)m_arStrings[ns];

   if((abs(MinLeft-pRtfString->m_LeftBorder)>5*m_max_dist) &&
      (abs(MaxLeft-pRtfString->m_LeftBorder)>5*m_max_dist))
    return FALSE;

   if(abs(MinLeft-pRtfString->m_LeftBorder)<m_max_dist)
    CountMinLeft++;

   if(abs(MaxLeft-pRtfString->m_LeftBorder)<m_max_dist)
    CountMaxLeft++;

   if(abs(MaxRight-pRtfString->m_RightBorder)<m_max_dist)
    CountMaxRight++;
  }

  if((CountMinLeft>CountMaxLeft)  ||
    ((CountMinLeft+CountMaxLeft)< 4*(end-beg)/5)  ||
    (CountMaxRight<(end-beg)/2))
   return FALSE;

  SetParagraphAlignment(beg, end, RTF_TP_TYPE_LINE);

  for(ns=beg; ns<end; ns++ )
	 {
	 	pRtfString = (CRtfString*)m_arStrings[ns];

   if((ns==beg) || abs(MinLeft-pRtfString->m_LeftBorder)<m_max_dist)
    pRtfString->m_wFlagBeginParagraph=TRUE;
  }

 	PrintTheResult("\n ================== DeterminationOfListType ================== \n");

return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 DeterminationOfMixedFragment

Bool CRtfFragment::DeterminationOfMixedFragment(RtfSectorInfo* SectorInfo)
{
 int32_t beg=0,end;
 Bool  Flag=TRUE;

 if(m_Attr==FALSE)
  return FALSE;

 while(Flag)
 {
  GetNextFragmentBegEnd(&beg, &end, &Flag);
  ReInit(SectorInfo, beg, end);
  if(DeterminationOfLeftRightJustification(beg, end)==FALSE)
  {
   if(DeterminationOfListType(beg, end)==FALSE)
   {
    if(DeterminationOfLeftJustification(beg, end, 0)==FALSE)
    {
     if(DeterminationOfCentreJustification(beg, end)==FALSE)
     {
      if(DeterminationOfRightJustification(beg, end)==FALSE)
      {
       DeterminationOfLeftJustification(beg, end, 1);
      }
     }
    }
   }
  }
  beg=end;
 }

	PrintTheResult("\n ================== DeterminationOfMixedFragment ================== \n");
 return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                            ReInit

void CRtfFragment::ReInit(RtfSectorInfo* SectorInfo, int beg, int end)
{
	int         LeftDif, RightDif, CentreDif,top,bottom;
	CRtfString  *pRtfStringPrev, *pRtfString;
	CRtfWord    *pRtfWord;
	CRtfChar    *pRtfCharFirst, *pRtfCharLast;
    int ns(0);

	m_l_fragmentLocal = 32000;
	m_r_fragmentLocal = 0;

	m_CountLeftEqual=0;
	m_CountRightEqual=0;
	m_CountLeftRightEqual=0;
	m_CountCentreEqual=0;
	for(ns=beg; ns<end; ns++ )
	{
		pRtfString = (CRtfString*)m_arStrings[ns];
		pRtfString->m_wLeftBorderEqual  = FALSE;
		pRtfString->m_wRightBorderEqual = FALSE;
		pRtfString->m_wCentreEqual      = FALSE;
	}
  //  I. �����:       �����(m_l_fragmentLocal) � ������(m_r_fragmentLocal) ������ ���������
	for(ns=beg; ns<end; ns++ )
	{
		pRtfString = (CRtfString*)m_arStrings[ns];

		if(ns==beg)
		{
			if(!ns) pRtfString->m_wSpaceBefore = SectorInfo->VerticalOffsetFragmentInColumn;
			else
			{
				pRtfStringPrev=(CRtfString*)m_arStrings[ns-1];

  				pRtfWord      = (CRtfWord*)pRtfStringPrev->m_arWords[0];
				pRtfCharFirst = (CRtfChar*)pRtfWord->m_arChars[0];
				top = pRtfCharFirst->m_Idealrect.bottom;

  				pRtfWord      = (CRtfWord*)pRtfString->m_arWords[0];
				pRtfCharFirst = (CRtfChar*)pRtfWord->m_arChars[0];
				bottom = pRtfCharFirst->m_Idealrect.top;
				pRtfString->m_wSpaceBefore =(uint16_t)(bottom - top);
			}
			pRtfString->m_wFlagBeginParagraph = TRUE;
		}
		else    pRtfString->m_wSpaceBefore = 0;

		pRtfWord      = (CRtfWord*)pRtfString->m_arWords[0];
		pRtfCharFirst = (CRtfChar*)pRtfWord->m_arChars[0];
		pRtfString->m_FirstChar = pRtfCharFirst->m_chrVersions[0].m_bChar;

		pRtfWord      = (CRtfWord*)pRtfString->m_arWords[pRtfString->m_wWordsCount-1];
		pRtfCharLast  = (CRtfChar*)pRtfWord->m_arChars[pRtfWord->m_wCharsCount-1];
		pRtfString->m_LastChar = pRtfCharLast->m_chrVersions[0].m_bChar;

		pRtfString->m_LeftBorder  = pRtfCharFirst->m_Idealrect.left;
		pRtfString->m_RightBorder = pRtfCharLast->m_Idealrect.right;

		m_l_fragmentLocal  = MIN(m_l_fragmentLocal, (int16_t)pRtfCharFirst->m_Idealrect.left);
		m_r_fragmentLocal  = MAX(m_r_fragmentLocal, (int16_t)pRtfCharLast->m_Idealrect.right);

	}

  // ������������� �������� ��������� ������ � �������� �������� �����
	for(ns=beg+1; ns < end; ns++ )
	{
		pRtfStringPrev = (CRtfString*)m_arStrings[ns-1];
		pRtfString     = (CRtfString*)m_arStrings[ns];

		LeftDif   = pRtfString->m_wLeftIndent  - pRtfStringPrev->m_wLeftIndent;
		RightDif  = pRtfString->m_wRightIndent - pRtfStringPrev->m_wRightIndent;
  		CentreDif = pRtfString->m_wCentre      - pRtfStringPrev->m_wCentre;

		if( abs(LeftDif) <= m_max_dist )
		{
 			pRtfString->m_wLeftBorderEqual = TRUE;
			m_CountLeftEqual++;
			if(ns==beg)
				{pRtfStringPrev->m_wLeftBorderEqual = TRUE;
				m_CountLeftEqual++;
			}
		}

		if( abs(RightDif) <= m_max_dist )
		{
			pRtfString->m_wRightBorderEqual = TRUE;
			m_CountRightEqual++;
			if(pRtfString->m_wLeftBorderEqual==TRUE)
				m_CountLeftRightEqual++;

			if(ns==beg)
				{pRtfStringPrev->m_wRightBorderEqual = TRUE;
				m_CountRightEqual++;
				m_CountLeftRightEqual++;
			}
		}

		if( (abs(CentreDif) < m_max_dist) &&
			((LeftDif<=0 && RightDif<=0) || (LeftDif>0 && RightDif>0)) &&
			(abs(LeftDif) > m_max_dist/2)  && (abs(RightDif) > m_max_dist/2))
		{
			pRtfString->m_wCentreEqual = TRUE;
			m_CountCentreEqual++;

			if(ns==beg)
				{pRtfStringPrev->m_wCentreEqual = TRUE;
				m_CountCentreEqual++;
			}
		}
	}

	PrintTheResult("\n ================== ReInit ================== \n");
}



////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 GetNextFragmentBegEnd

void CRtfFragment::GetNextFragmentBegEnd(int32_t* beg, int32_t* end, Bool* Flag)
{
 CRtfString *pRtfString;
 int i;

 *end=*beg+1;
 for(i=*end; i<m_wStringsCount; i++)
 {
  pRtfString = (CRtfString*)m_arStrings[i];
  if(pRtfString->m_Attr==TRUE)
  {*end=i; break;}
 }

 if((*end >= (int32_t)m_wStringsCount) || (i >= (int32_t)m_wStringsCount))
 {
  *end= (int32_t)m_wStringsCount;
  *Flag=FALSE;
 }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                 Done

void CRtfFragment::Done(void)
{
	CheckOnceAgainImportancesFlagBeginParagraph();
	SetFirstLeftAndRightIndentOfParagraph();
	DefineLineTransfer();
}


//-------------------------------------------------------------------------------------------------
//---------------------------------  HELPER FUNCTIONS  --------------------------------------------
//-------------------------------------------------------------------------------------------------
void CRtfFragment::CalculationLengthAndCount(CRtfString* pRtfString, int32_t* CountChars, int32_t* LengthChars)
{
 CRtfWord    *pRtfWord;
	CRtfChar    *pRtfChar;
	uint16_t         CountWords;
	uint16_t         WCountChars;

  CountWords = pRtfString->m_wWordsCount;
  for(int i=0; i<CountWords; i++)
  {
   pRtfWord   = (CRtfWord*)pRtfString->m_arWords[i];
   WCountChars = pRtfWord->m_wCharsCount;
   for(int j=0; j<WCountChars; j++)
   {
    pRtfChar = (CRtfChar*)pRtfWord->m_arChars[j];
    (*LengthChars) += MAX(0,pRtfChar->m_Idealrect.right-pRtfChar->m_Idealrect.left);
    (*CountChars)++;
   }
  }

}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                 CheckOnceAgainImportancesFlagBeginParagraph

void CRtfFragment::CheckOnceAgainImportancesFlagBeginParagraph()
{
	CRtfString  *pRtfString;
	CRtfString  *pRtfStringPrev;
	CRtfWord    *pRtfWordPrev;
	CRtfWord    *pRtfWord;
	CRtfChar    *pRtfChar;
	uint16_t         CountWords;
	uint16_t         CountChars;

    int ns(0);
	for(ns=1; ns < m_wStringsCount; ns++ )
	{
		pRtfStringPrev  = (CRtfString*)m_arStrings[ns-1];
		pRtfString      = (CRtfString*)m_arStrings[ns];

   // ���� ���. ������, �� ���������� ������ ����� ��������
		if( pRtfString->m_wAlignment != pRtfStringPrev->m_wAlignment )
		{
			pRtfStringPrev->m_bLineTransfer = FALSE;
			pRtfString->m_wFlagBeginParagraph = TRUE;
		}
	}

	for(ns=1; ns < m_wStringsCount; ns++ )
	{
		pRtfStringPrev  = (CRtfString*)m_arStrings[ns-1];
		pRtfString      = (CRtfString*)m_arStrings[ns];

 		pRtfWordPrev    = (CRtfWord*)pRtfStringPrev->m_arWords[0];
 		pRtfWord        = (CRtfWord*)pRtfString->m_arWords[0];

		if(pRtfString->m_wAlignment !=RTF_TP_CENTER && abs(pRtfWord->m_wRealFontPointSize - pRtfWordPrev->m_wRealFontPointSize)>1 )
		{
			pRtfStringPrev->m_bLineTransfer = FALSE;
			pRtfString->m_wFlagBeginParagraph = TRUE;
		}
	}

	for(ns=1; ns < m_wStringsCount; ns++ )
	{
		pRtfStringPrev  = (CRtfString*)m_arStrings[ns-1];
		pRtfString      = (CRtfString*)m_arStrings[ns];

		if( pRtfString->m_wFlagBeginParagraph == TRUE )
		{
			CountWords = pRtfStringPrev->m_wWordsCount;
  			pRtfWord   = (CRtfWord*)pRtfStringPrev->m_arWords[CountWords-1];
			CountChars = pRtfWord->m_wCharsCount;
			pRtfChar   = (CRtfChar*)pRtfWord->m_arChars[CountChars-1];
			if( pRtfChar->m_chrVersions[0].m_bChar=='-' && pRtfChar->m_bFlg_spell_nocarrying )
			{
				if( pRtfString->m_wAlignment == pRtfStringPrev->m_wAlignment	)
 					pRtfString->m_wFlagBeginParagraph = FALSE;
				else
				if(pRtfStringPrev->m_wAlignment == RTF_TP_LEFT_AND_RIGHT_ALLIGN && pRtfString->m_wAlignment == RTF_TP_LEFT_ALLIGN)
				{
	 				pRtfString->m_wAlignment = RTF_TP_LEFT_AND_RIGHT_ALLIGN;
 					pRtfStringPrev->m_bLineTransfer = FALSE;
					pRtfString->m_wFlagBeginParagraph = FALSE;
				}
			}
		}
	}

	PrintTheResult("\n ================== CheckOnceAgainImportancesFlagBeginParagraph ================== \n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                          SetFirstLeftAndRightIndentOfParagraph

void CRtfFragment::SetFirstLeftAndRightIndentOfParagraph()
{
 CRtfString  *pRtfString;
 CRtfString  *pRtfStringNext;
 int16_t       MinLeftIndent;
 int16_t       MinRightIndent;
 int         i;
 int         ns(0);
 int16_t       twp_dist;
 int16_t       Dif=0;

 twp_dist = (int16_t)(3*m_max_dist * Twips);
	for(ns=0; ns<m_wStringsCount; ns++ )
	{
		pRtfString                         =(CRtfString*)m_arStrings[ns];
		pRtfString->m_LengthStringInTwips  =pRtfString->GetRealStringSize();
  pRtfString->m_wLeftIndent          =(int16_t)((int16_t)(pRtfString->m_wLeftIndent  * Twips)+m_LeftOffsetFragmentFromVerticalColumn);
  pRtfString->m_wRightIndent         =(int16_t)((int16_t)(pRtfString->m_wRightIndent * Twips)+m_RightOffsetFragmentFromVerticalColumn);
		pRtfString->m_wRightIndent         =MIN(pRtfString->m_wRightIndent,
			m_WidthVerticalColumn - (pRtfString->m_LengthStringInTwips + pRtfString->m_wLeftIndent + pRtfString->m_wRightIndent));
	}

	for(ns=0; ns<m_wStringsCount; ns++ )
	{
		pRtfString  = (CRtfString*)m_arStrings[ns];
  if(pRtfString->m_wFlagBeginParagraph == TRUE)
		{

			if(pRtfString->m_wAlignment == RTF_TP_LEFT_ALLIGN )
			{
				Dif = MAX(0,m_WidthVerticalColumn - pRtfString->m_LengthStringInTwips);

				MinLeftIndent  = pRtfString->m_wLeftIndent;
 		 for( i=ns+1; i<m_wStringsCount; i++ )
				{
   		pRtfStringNext  = (CRtfString*)m_arStrings[i];
     if(pRtfStringNext->m_wFlagBeginParagraph == TRUE || pRtfStringNext->m_wAlignment != RTF_TP_LEFT_ALLIGN)
						break;
     MinLeftIndent   = MIN(pRtfStringNext->m_wLeftIndent,  MinLeftIndent);
				}

				if(m_WidthVerticalColumn > pRtfString->m_LengthStringInTwips)
				{
			  pRtfString->m_wFirstIndent  = pRtfString->m_wLeftIndent - MinLeftIndent;
     if(pRtfString->m_wFirstIndent<(twp_dist/3))
      pRtfString->m_wFirstIndent=0;

     pRtfString->m_wLeftIndent   = MIN(Dif,	MinLeftIndent);
				}
				else
				{
     pRtfString->m_wLeftIndent  = 0;
     pRtfString->m_wFirstIndent = 0;
				}

    pRtfString->m_wRightIndent = 0;

    if(pRtfString->m_wLeftIndent<(twp_dist/2))
     pRtfString->m_wLeftIndent  = 0;

    if(i==(ns+1))
    { pRtfString->m_wFirstIndent = pRtfString->m_wLeftIndent; pRtfString->m_wLeftIndent = 0; }

				continue;
			}

			if(pRtfString->m_wAlignment == RTF_TP_RIGHT_ALLIGN )
			{
				pRtfString->m_wLeftIndent  = 0;
				pRtfString->m_wFirstIndent = 0;
    pRtfString->m_wRightIndent = 0;
				continue;
			}

   if(pRtfString->m_wAlignment == RTF_TP_TYPE_LINE)
			{
	   if( ns+1< m_wStringsCount)
				{
   		pRtfStringNext = (CRtfString*)m_arStrings[ns+1];
					if( pRtfStringNext->m_wFlagBeginParagraph == FALSE && pRtfStringNext->m_wAlignment == RTF_TP_TYPE_LINE )
	 			{
						pRtfString->m_wFirstIndent = pRtfStringNext->m_wLeftIndent-pRtfString->m_wLeftIndent;
      pRtfString->m_wLeftIndent  = pRtfStringNext->m_wLeftIndent;
					}
     else
						pRtfString->m_wFirstIndent = 0;
				}
				else
     pRtfString->m_wFirstIndent = 0;
				continue;
			}

   if(pRtfString->m_wAlignment == RTF_TP_LEFT_AND_RIGHT_ALLIGN)
			{
	   MinLeftIndent  = pRtfString->m_wLeftIndent;
				MinRightIndent = pRtfString->m_wRightIndent;

 		 for( i=ns+1; i<m_wStringsCount; i++ )
				{
   		pRtfStringNext  = (CRtfString*)m_arStrings[i];
     if(pRtfStringNext->m_wFlagBeginParagraph == TRUE ||
        pRtfStringNext->m_wAlignment != RTF_TP_LEFT_AND_RIGHT_ALLIGN)
						break;
     MinLeftIndent   = MIN(pRtfStringNext->m_wLeftIndent,  MinLeftIndent);
     MinRightIndent  = MIN(pRtfStringNext->m_wRightIndent, MinRightIndent);
				}

    if(MinLeftIndent<(twp_dist/3))
     MinLeftIndent=0;

				pRtfString->m_wFirstIndent = pRtfString->m_wLeftIndent - MinLeftIndent;

    if(pRtfString->m_wFirstIndent<(twp_dist/3))
     pRtfString->m_wFirstIndent=0;

				if(MinLeftIndent < twp_dist)
     pRtfString->m_wLeftIndent = 0;
				else
     pRtfString->m_wLeftIndent  = MinLeftIndent;

				if(MinRightIndent < twp_dist)
					pRtfString->m_wRightIndent = 0;

    if(i==(ns+1))
    {
     if(MinLeftIndent>((2*twp_dist)/3))
      pRtfString->m_wLeftIndent = MinLeftIndent;

     pRtfString->m_wFirstIndent = pRtfString->m_wLeftIndent; pRtfString->m_wLeftIndent = 0; pRtfString->m_wRightIndent = 0;}
				continue;
			}

			if(pRtfString->m_wAlignment == RTF_TP_CENTER)
			{
    MinLeftIndent  = pRtfString->m_wLeftIndent;
				MinRightIndent = pRtfString->m_wRightIndent;

			 for( i=ns; i<m_wStringsCount; i++ )
				{
   		pRtfStringNext  = (CRtfString*)m_arStrings[i];
     MinLeftIndent   = MIN(pRtfStringNext->m_wLeftIndent,  MinLeftIndent);
     MinRightIndent  = MIN(pRtfStringNext->m_wRightIndent, MinRightIndent);
  			if(pRtfString->m_wAlignment != RTF_TP_CENTER)
						break;
				}

    if(abs(MinLeftIndent-MinRightIndent)<(2*m_max_dist))
     MinLeftIndent=MinRightIndent=0;
    pRtfString->m_wFirstIndent = 0;
				pRtfString->m_wLeftIndent =  MinLeftIndent;
    pRtfString->m_wRightIndent = MinRightIndent;
			}

		}
	}
	PrintTheResult("\n ================== SetFirstLeftAndRightIndentOfParagraph ================== \n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                     DefineLineTransfer

void CRtfFragment::DefineLineTransfer()
{
 CRtfString  *pRtfString;
 int          Count;

	for( int ns=0; ns<m_wStringsCount; ns++ )
	{
		pRtfString = (CRtfString*)m_arStrings[ns];
  if(pRtfString->m_wFlagBeginParagraph == TRUE  &&
			 (FlagLineTransfer || pRtfString->m_wAlignment == RTF_TP_CENTER ))
		{
   Count = GetCountLine(ns);
			SetLineTransfer(ns,ns+Count);
		}
	}

	PrintTheResult("\n ================== DefineLineTransfer ================== \n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                     GetFlagLeft

Bool CRtfFragment::GetFlagLeft(int beg, int end)
{
 CRtfString*  pRtfString;
 int          Count=0;
 uint16_t         CountLeftEqual=0;
 Bool         PriznakLeft=FALSE;


	for( int ns=beg; ns<end; ns++ )
	{
		pRtfString = (CRtfString*)m_arStrings[ns];
  if(pRtfString->m_LastChar==',')
  {
   Count++;
   if(pRtfString->m_wRightIndent > (pRtfString->m_RightBorder - pRtfString->m_LeftBorder)/4)
    PriznakLeft=TRUE;
  }
	}

 if(Count>1 && PriznakLeft)
  return TRUE;

 GetCountEqual(beg, end, &CountLeftEqual, RTF_TP_LEFT_ALLIGN);

 if(CountLeftEqual==(end-beg))
  return TRUE;

 m_FlagBigSpace=GetFlagBigSpace(beg,end);
 m_FlagCarry=GetFlagCarry(beg, end);

 if(!m_FlagBigSpace && !m_FlagCarry)
  return TRUE;

 return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                     GetFlagStrongLeft

Bool CRtfFragment::GetFlagStrongLeft(int beg, int end)
{
 int         Count=0;
 CRtfString  *pRtfString;

	for( int ns=beg; ns<end; ns++ )
	{
		pRtfString = (CRtfString*)m_arStrings[ns];
  if(pRtfString->m_LastChar=='.' || pRtfString->m_LastChar==',')
   Count++;
	}

 if(Count==end-beg)
  return TRUE;

 if((end-beg<5) && Count>2 )
  return TRUE;

 return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                     GetFlagRight

Bool CRtfFragment::GetFlagRight(int beg, int end)
{
 CRtfString  *pRtfString;

    int ns(0);
	for(ns=beg; ns<end; ns++ )
	{
		pRtfString = (CRtfString*)m_arStrings[ns];
  if(pRtfString->m_wRightIndent > m_max_dist)
   return FALSE;
	}

	for(ns=beg; ns<end; ns++ )
	{
		pRtfString = (CRtfString*)m_arStrings[ns];
  if(pRtfString->m_wLeftIndent > ((m_r_fragment-m_l_fragment)/2))
   return TRUE;
	}

 return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                     GetFlagCarry

Bool CRtfFragment::GetFlagCarry(int beg, int end)
{
 CRtfString  *pRtfString;

	for( int ns=beg; ns<end; ns++ )
	{
		pRtfString = (CRtfString*)m_arStrings[ns];
  if(pRtfString->m_FlagCarry == TRUE)
   return TRUE;
	}

 return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                     GetFlagBigSpace

Bool CRtfFragment::GetFlagBigSpace(int beg, int end)
{
 CRtfString*  pRtfString;
 CRtfWord*    pRtfWordPrev,*pRtfWordCur;
 CRtfChar*    pRtfCharPrev,*pRtfCharCur;
 int          FlagBigSpase=0;
 int          CountCharInPrevWord;

  for(int ns=beg; ns<end; ns++ )
  {
	  pRtfString = (CRtfString*)m_arStrings[ns];

   for(int i=1; i<pRtfString->m_wWordsCount; i++)
   {
		  pRtfWordPrev = (CRtfWord*)pRtfString->m_arWords[i-1];
		  pRtfWordCur  = (CRtfWord*)pRtfString->m_arWords[i];

    CountCharInPrevWord = pRtfWordPrev->m_wCharsCount;

    pRtfCharPrev = (CRtfChar*)pRtfWordPrev->m_arChars[CountCharInPrevWord-1];
    pRtfCharCur  = (CRtfChar*)pRtfWordCur->m_arChars[0];
    if((pRtfCharCur->m_Idealrect.left - pRtfCharPrev->m_Idealrect.right)>2*m_max_dist)
     FlagBigSpase=1;
   }
  }

 return FlagBigSpase;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//                                   SetParagraphAlignment

void CRtfFragment::SetParagraphAlignment(int beg, int end, int AlignType)
{
	CRtfString  *pRtfString;
	int         i;

  beg = MAX(0,beg);

	 for(i=beg; i<end; i++)
		{
			pRtfString = (CRtfString*)m_arStrings[i];
   pRtfString->m_wAlignment = AlignType;
		}

}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                     GetCountLine

int CRtfFragment::GetCountLine(int beg)
{
 CRtfString  *pRtfString;
 int          Count=0;

	for( int ns=beg+1; ns<m_wStringsCount; ns++ )
	{
		pRtfString = (CRtfString*)m_arStrings[ns];
  if(pRtfString->m_wFlagBeginParagraph == TRUE)
			break;
		else
			Count++;
	}
	return Count;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//                                     SetLineTransfer

void CRtfFragment::SetLineTransfer(int beg, int end)
{
 CRtfString  *pRtfString;

	for( int ns=beg; ns<end; ns++ )
	{
		pRtfString = (CRtfString*)m_arStrings[ns];
  pRtfString->m_bLineTransfer = TRUE;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                   PrintTheResult

void CRtfFragment::PrintTheResult(const char* header_str)
{
/*	CRtfString  *pRtfString;
	CRtfWord    *pRtfWord;
	CRtfChar    *pRtfCharFirst;
	CString     str;
	char* str;
	uint16_t        CountWord,CountChar;

	if( FlagDebugAlign )
	{
		LDPUMA_Console(header_str);
		for(int ns=0; ns < m_wStringsCount; ns++ )
		{
			str.Empty();
			pRtfString = (CRtfString*)m_arStrings[ns];
			CountWord  = pRtfString->m_wWordsCount;
			for(int i1=0; i1<CountWord; i1++)
			{
 				pRtfWord  = pRtfString->m_arWords[i1];
				CountChar = pRtfWord->m_wCharsCount;
				for(int i2=0; i2<CountChar; i2++)
				{
					if(!i2)
						str+=' ';
					pRtfCharFirst = (CRtfChar*)pRtfWord->m_arChars[i2];
					str+=pRtfCharFirst->m_chrVersions[0].m_bChar;
				}
			}
			str+='\n';
			LDPUMA_Console(str);
 			str.Format("N#=%4d , Alignment= %d, BeginParagraph= %d,FI=%4d, LI=%4d, RI=%4d, LBorderEqual=%d, RBorderEqual=%d, CentreEqual=%d, dist=%3d \n",
															ns, pRtfString->m_wAlignment, pRtfString->m_wFlagBeginParagraph,
															pRtfString->m_wFirstIndent,pRtfString->m_wLeftIndent,	pRtfString->m_wRightIndent,
			pRtfString->m_wLeftBorderEqual, pRtfString->m_wRightBorderEqual, pRtfString->m_wCentreEqual,
															m_max_dist);
			LDPUMA_Console(str);
		}
	}
*/}
