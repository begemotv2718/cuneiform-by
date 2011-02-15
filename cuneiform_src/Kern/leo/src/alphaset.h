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

#define	ALPH_ALL 	0x00
#define	ALPH_RUS 	0x01
#define	ALPH_DIG	0x02
#define	ALPH_ROM	0x04
#define	ALPH_R_E	0x08
#define	ALPH_ENG    0x10
#define	ALPH_RALL   0x20
//
// 1 - Capital Russian letters
// 2 - Digital
// 4 - Roma digit
//
static uchar typ_of_alpha[256] = {
     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//                                                         last 0f
     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//                                                         last 1f
     0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,
//     ! " # $ % & ' ( ) * + , - . /           last = 2f
     2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,
//   0 1 2 3 4 5 6 7 8 9 : ; < = > ? @         last = 40
     8,8,8,8,8,8,8,8,4,8,8,8,8,8,8,8,8,8,8,8,8,4,8,4,8,8,0,0,0,0,0,0,
//   A B C D E F G H I J K L M N O P Q R S T U V W X Y Z [ \ ] ^ _ `                                 last: 60
     8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,0,0,0,0,0,
//   a b c d e f g h i j k l m n o p q r s t u v w x y z
     1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
//   � � � � � � � � � � � � � � � �
     1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
//   � � � � � � � � � � � � � � � �
     32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
//   �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �
     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//
     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//
     32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,
//   �  �  �  �  �  �  �  �  �  �  �  �  �  �  �  �
     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
//
   };


static uchar up_of_alpha[256] = {
     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//                                                         last 0f
     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//                                                         last 1f
     0,0,0,'#',0,'%',0,0,0,0,0,0,0,0,0,'/',
//     ! "  #  $  %  & ' ( ) * + , - . /           last = 2f
     (uchar)'0',(uchar)'1',(uchar)'2',(uchar)'3',(uchar)'4',(uchar)'5',(uchar)'6',(uchar)'7',(uchar)'8',(uchar)'9',0,0, 0,0, 0, 0, 0,
//    0   1   2   3   4   5   6   7   8   9 : ; <  =  >  ?  @         last = 40
     'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z', 0,0, 0,0,0,0,
//   A B C D E F G H I J K L M N O P Q R S T U V W X Y Z [  \  ] ^ _ `                                 last: 60
     'a','b','C','d','e','f','g','h','i','j','k','l','m','n','O','P','q','r','S','t','u','V','W','X','Y','Z',0,(uchar)'|',0,0,0,
//   a b c d e f g h i j k l m n o p q r s t u v w x y z {  |  } ~
     (uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',
//    �   �   �   �   �   �   �   �   �   �   �   �   �   �   �   �
     (uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',
//    �   �   �   �   �   �   �   �   �   �   �   �   �   �   �   �
     (uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',
//    �   �   �   �   �   �   �   �   �   �   �   �   �   �   �   �
     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//
     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//
     (uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',(uchar)'�',
//    �   �   �   �   �   �   �   �   �   �   �   �   �   �   �   �
     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
//
   };

static uchar leo_accepted[256]={
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//                                                         last 0f
     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//                                                         last 1f
     0,0, 0, 0, 0,0,0,0, 0, 0,0, 0,0,0,0, 0,
//     !  " #  $  % & ' (   ) *  + , - . /           last = 2f
     1,1,1,1,1,1,1,1,1,1,0,0, 0,0, 0, 0, 0,
//   0 1 2 3 4 5 6 7 8 9 : ; <  =  >  ?  @         last = 40
     1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1, 1, 1,1,1,1,1, 1,1, 1,1,1, 0,0, 0,0,0,0,
//   A B C D E F G H I J  K L M N O P  Q  R S T U V  W  X Y Z [  \  ] ^ _ `                                 last: 60
     1,1,1,1,1, 1, 1,1,1,1,1,1,1,1,1, 1,1,1,1, 1,1,1,1,1,1, 1,0,0,0,0,0,
//   a b c d e f  g  h i j k l m n o p  q r s t  u v w x y z
     1,1,1,1, 1,1,1,1,1,1,1,1,1,1,1,1,
//   � � � � �  � � � � � � � � � � �
     1,1,1,1,1,1, 1,1,1, 1,1,1,1,1,1, 1,
//   � � � � � � �  � � �  � � � � � �
     1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
//   � � � �  � � � � �  � � � � � �
     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//
     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//
     1,1,1,1,1,1, 1,1,1, 1,1,1,1,1,1,1,
//   � � � � � � �  � � �  � � � � � �
     0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
//
};
