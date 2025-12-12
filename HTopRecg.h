
#ifndef U8
#define U8 unsigned char
#define P_U8 U8 *
#endif

#ifndef U16
#define U16 unsigned short
#define P_U16 U16 *  
#endif

#ifndef byte
#define byte unsigned char
#endif

#ifndef WORD
#define WORD unsigned short
#endif

#ifndef DWORD
#define DWORD unsigned long
#endif

#ifndef true
#define true 1
#define false 0
#endif

/*
////////////////////////////////////////////////////////////////
// for PDA
// 
typedef struct {
	U8 x;
	U8 y;
}PDAPOINT;

typedef struct {
	U8 top;
	U8 bottom;
	U8 left;
	U8 right;
}PDARECT;

typedef struct {
	short itop;
	short ibottom;
	short ileft;
	short iright;
}SHORTRECT;
*/

///////////////////////////////////////////////////////////////////////////////
// define by HenTop 

#define ALC_NUMERIC				0x0001	/* 0..9								*/
#define ALC_LCALPHA				0x0002	/* a..z								*/
#define ALC_UCALPHA				0x0004	/* A..Z								*/
#define	ALC_PUNCTUATION			0x0008	/* !",:;?、。'()…《》				*/
#define	ALC_SYMBOLS				0x0010	/* #$%&*+-./<=>@￥					*/
#define ALC_CHINESE_COMMON		0x0020	/* Commonly used Chinese characters	*/
#define ALC_CHINESE_RARE		0x0040	/* Rarely used Chinese characters	*/
#define ALC_CHINESE_VARIANT		0x0080	/* Variant Chinese characters		*/
//Add by hefeng 2009.05.21
#define ALC_NUMERIC_SERIAL		0x0100	/* 数字串的识别		                */

//拆字模式
#define AWORD_SPLIT            0    //普通的单字拆分
#define WORDS_NORMAL_SPLIT     1    //原稿普通的多字拆分
#define WORDS_OVERLAP_SPLIT    2    //原稿放宽条件(可有重叠部分)的多字拆分
//手写字在手写格内的位置，为识别特殊常用的标点符号，。.
#define WORD_POS_CENTER        0    //通常位置           
#define WORD_POS_UP            1    //标点符号:'" "'
#define WORD_POS_DOWN          2    //标点符号:，。、,.
#define WORD_POS_SAMLLSIZE     3    //小尺寸字符，用于区分汉字和数字、英文、标点符号
#define WORD_POS_SPLITWORD    10    //进行拆分识别

extern  "C" void PASCAL EXPORT SetHTCoreBuff(WORD *,void *);
extern  "C" WORD PASCAL EXPORT HandWritingRecognise(short *,WORD,char *,WORD,WORD);
extern "C" void PASCAL EXPORT FreeLearnBuff();//清除识别自学习缓存区
extern "C" void PASCAL EXPORT LearnDbAdd(U16 nWordCode);//增加一个自学习字迹
extern "C" void PASCAL EXPORT LearnDbDel(U16 nWordCode);//删除当前的自学习字迹
extern "C" void *PASCAL EXPORT GetLearnInk(U16 nWordCode);//得到当前自学习字迹的指针，如不存在返回NULL

//extern "C"  CSplitParam* PASCAL EXPORT SplitWords(int nSplitMode,short *strokelist,short *psplitsum);
//extern "C" CJudgeOfRecg* PASCAL EXPORT GetRecgResults(short *pstrokesum,WORD *presultsum);
//extern "C" CInInkStroke * PASCAL EXPORT GetFirstInStroke();
//extern "C" void PASCAL EXPORT SetDebugHzId(void *pdebugdata,byte *pdebughz,byte *plibhz);
//
//extern "C" bool PASCAL EXPORT CompRecgResult(CResultOfRecg *plastresult,CResultOfRecg *pcurrresult);
