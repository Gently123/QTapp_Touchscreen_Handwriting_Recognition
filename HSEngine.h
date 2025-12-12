/*
*****************************************************************************
   Handwriting Recognition Engine Header
*****************************************************************************
*/

#define UINT unsigned int
#define WORD unsigned short
#define BOOL int

#ifdef __cplusplus
extern "C"    {    
#endif                         


#define DllImport	__declspec( dllimport )
#define DllExport	__declspec( dllexport )

//#define DllImport
//#define DllExport

#define ADDRESS_POINTER_TAG		0x01
#define FILENAME_POINTER_TAG	0x02
/*
Function:		Recognition Engine Initialization
Usage:			call this function before using other functions in handwritting recognition engine
Return value:	TRUE	success
				FALSE	fail
Comment:		pPathOrPtr represent the file name if uTag is ADDRESS_POINTER_TAG or memory pointer if uTag is FILENAME_POINTER_TAG
				uTag is a tag figure out which pointer is pFileOrPtr
*/
DllExport BOOL HSStartCharRecogEngine(const void* pFileOrPtr, UINT uTag);

/*
Function:		Recognition Engine End
Usage:			call this function after using other functions handwritting recognition kernel no more
Return value :	TRUE	success
				FALSE	fail
*/
DllExport BOOL HSEndCharRecogEngine(void);


#define  MAX_CANDIDATE_NUM    10

#define SHUZI_MATCH_RANGE			0x0001	// 10个数字
#define DAXIEZIMU_MATCH_RANGE		0x0002	// 26个英文大写字母
#define XIAOXIEZIMU_MATCH_RANGE		0x0004	// 26个英文小写字母
#define FUHAO_MATCH_RANGE			0x0008	// 31个标点符号
#define BISHI_MATCH_RANGE			0x0010	// 笔势符号
#define GUOBIAOYIJI_MATCH_RANGE		0x0020	// 3755个国标一级汉字
#define GUOBIAOERJI_MATCH_RANGE		0x0040	// 3008个国标二级汉字
#define FANTIZIFU_MATCH_RANGE		0x0080	// Big5一级繁体汉字（不包括与国标一二级重合的简体汉字）
#define GBKZIFU_MATCH_RANGE			0x0100	// 常用简繁体汉字之外的所有GBK汉字
#define XUEXIZIFU_MATCH_RANGE		0x1000	// 自学习字符

#define ZIMU_MATCH_RANGE			(DAXIEZIMU_MATCH_RANGE | XIAOXIEZIMU_MATCH_RANGE)
#define JIANTIZIFU_MATCH_RANGE		(GUOBIAOYIJI_MATCH_RANGE | GUOBIAOERJI_MATCH_RANGE)
#define HANZI_MATCH_RANGE			(GUOBIAOYIJI_MATCH_RANGE | GUOBIAOERJI_MATCH_RANGE | FANTIZIFU_MATCH_RANGE | GBKZIFU_MATCH_RANGE)
#define ALL_MATCH_RANGE				(SHUZI_MATCH_RANGE | ZIMU_MATCH_RANGE | FUHAO_MATCH_RANGE | JIANTIZIFU_MATCH_RANGE | FANTIZIFU_MATCH_RANGE | GBKZIFU_MATCH_RANGE | XUEXIZIFU_MATCH_RANGE)
/*
Function:		Main Recognition Function
Comment:		Return value is recognition result number, its valid value are [-1, MAX_CANDIDATE_NUM]
				pnCharBuffer is handwritting buffer of Chinese character. (x, y) make up one point, and x and y are short type,
				their valid value are from -1 to 32767. (-1, 0) is end flag of stroke, (-1, -1) is end flag of character.
				pwResultBuffer is recognition result buffer, we recommend 推荐its size are MAX_CANDIDATE_NUM+1
				uMatchRange is one of several kinds of match_range or combination of them. Note: it is not null(zero)
				moreover, uMatchRange may include ADAPTATION_MATCH_RANGE only after calling 'HZInitAdaptation' function
*/
DllExport int HSCharRecognize(short* pnCharBuffer, WORD* pwResultBuffer, UINT uMatchRange);

/*
Function:		Adaptation function initialization
Usage:			call this function before using other functions in adaptation function
Return value:	TRUE	success
				FALSE	fail
Comment:		szFileName1 and szFileName2 are two adaptation function file name
				the format of them should be all path name
*/
DllExport BOOL HSInitAdaptation(const char* szFileName1, const char* szFileName2, const char* szFileName3);

/*
Function:		Adaptation function exit
Usage:			call this function before using other functions in adaptatio function no more
Return value:	TRUE	success
//				FALSE	fail
*/
DllExport BOOL HSExitAdaptation(const char* szFileName1, const char* szFileName2, const char* szFileName3);

/*
Function:		Character Adaptation function
Return value:	TRUE	success
				FALSE	fail
Comment:		Param usage same as 'HSCharacterRecognize' function
*/
DllExport BOOL HSCharacterAdaptation(WORD wCode, short* pnCharBuffer);

/*
Return value:	Adaptation Character Numbers
*/
DllExport int HSGetAdaptationCharacterNumber();

/*
Return value:	Character handwriting length in short
*/
DllExport int HSGetAdaptationCharacterBufferLength(int iIndex);

/*
Return value:	Character code
Comment:		iIndex is index value in adaptation dictionary
				pnCharBuffer is handwritting buffer of character.
*/
DllExport WORD HSGetAdaptationCharacter(int iIndex, short* pnCharBuffer);

/*
Comment:		iIndex is index value in adaptation dictionary
*/
DllExport void HSDeleteAdaptationCharacter(int iIndex);

#ifdef __cplusplus
}
#endif 
