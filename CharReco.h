// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 CHARRECO_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// CHARRECO_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef CHARRECO_EXPORTS
#define CHARRECO_API __declspec(dllexport)
#else
#define CHARRECO_API __declspec(dllimport)
#endif

//#include "d:\\MySrc\\NewHandwriter\\HandWriter\\CharRecoSvr\\CharRecoSvr.h"
//#pragma comment(lib, "D:\\MySrc\\NewHandwriter\\HandWriter\\Debug\\CharRecoSvr.lib")


// #ifdef _USE_HTOPRECG
// #include "HTopRecg.h"
// #pragma comment(lib, "HTopRecg.lib")
// #endif
// 
// #include "HSEngine.h"
// #pragma comment(lib, "HSEngine.lib")

//#include <DirFile.h>

#include <windows.h>


// 定义笔迹中的结束标记
#define		X_STROKE_END_FLAG	(-1)
#define		Y_STROKE_END_FLAG	( 0)
#define		X_CHAR_END_FLAG		(-1)
#define		Y_CHAR_END_FLAG		(-1)

#define  MAX_POINT_NUM          1000  // 一次最多能识别的点数（点数到达 1024 之后，调用 HSCharRecognize 时，识别核心崩溃）
#define  MAX_STROKE_NUM         500   // 最多的笔画数（包含结束标志的笔画至少占两点，故只需最多点数的一半即可）

#define  GAP_PERCENT          30    // 文字拆分最小间隙百分比（相对于文字大小）

#define  MAX_CANDIDATE_NUM    10

    struct stStroke
	{
		int   iBegin;    // 笔画开始点位置
		int   iEnd;      // 笔画结束点位置
		RECT  rect;      // 笔画外接矩形
	};
// 此类是从 CharReco.dll 导出的
 class CHARRECO_API CCharReco {
public:
  CCharReco(void);
	~CCharReco();
	// TODO: 在此添加您的方法。

//	CCharRecoSvr m_CharRecoSvr;
private:
  stStroke m_stroke;				// 当前笔画
  int m_iPoint;                   // 坐标点数
  int m_iStroke;                  // 笔画数
  short     m_pPoint[MAX_POINT_NUM * 2 + 2];  // 坐标点缓冲区（为汉字结束标志预留一个点）
  stStroke  m_pStroke[MAX_STROKE_NUM];        // 笔画索引指针

  // 文字识别及消息传递
  // 	HWND m_hWndReceiver;
  // 	UINT m_uMsg;
  unsigned int m_uMatchRange;
  bool m_bMatchMulti;
  char m_szHSAdapt[3][256];

//	WORD m_pCharater[MAX_CANDIDATE_NUM + 1]; // 识别结果缓冲区（为字符串结束标志预留一个点）
	//	LONG m_pDistance[MAX_CANDIDATE_NUM + 1]; // 识别权重缓冲区（为权值串结束标志预

public:

	void GetPara(	short     m_pPoint_fromApp[MAX_POINT_NUM * 2 + 2],  // 坐标点缓冲区（为汉字结束标志预留一个点）
		int &m_iPoint_fromApp,                   // 坐标点数
		stStroke  m_pStroke_fromApp[MAX_STROKE_NUM],        // 笔画索引指针
		int &m_iStroke_fromApp                 // 笔画数
		);
		void Clear();
#if 1
	// 笔画进行中
    bool Stroking(POINT point);
	// 笔画结束
    bool StrokeEnd();
	// 笔画结束
    bool StrokeEnd(stStroke m_pStroke[MAX_STROKE_NUM],int m_iStroke,stStroke m_stroke);
	// 文字初始化

// 	BOOL Recognize(short m_pPoint[MAX_POINT_NUM * 2 + 2],int &m_iPoint,  stStroke m_pStroke[MAX_STROKE_NUM],	int &m_iStroke,WORD m_pCharater[MAX_CANDIDATE_NUM + 1], BOOL m_bMatchMulti,/*LONG m_pDistance[MAX_CANDIDATE_NUM + 1],*/ UINT m_uMatchRange);
// 	BOOL Recognize();
// 	DWORD GetCharacter();
// 	DWORD GetCandidate(WORD pBuff[]);
	// 文字识别及消息传递
	// 	void GetReceiver(HWND &hWnd, UINT &uMsg);
	// 	void SetReceiver(HWND hWnd, UINT uMsg);
// 	void GetRecognize(UINT &uMatchRange, BOOL &bMatchMulti);
// 	void SetRecognize(UINT uMatchRange, BOOL bMatchMulti);
// 	void SetLearnWeight(int iValue);
private:
// 	void SplitChar(CUIntArray &aIndex, BOOL m_bMatchMulti,stStroke m_pStroke[MAX_STROKE_NUM],int m_iStroke);	
// 	int _Recognize(short* pnCharBuffer, WORD* pwResultBuffer, long* plResultDistance, int iMaxCandNum, UINT uMatchRange);
// 
// 
//	void CombinRect(RECT &rt, RECT rtAdd);
//    bool IsOtherRect(CRect rt, CRect rtNext, int GapPercent);
#endif

};

extern  CHARRECO_API BOOL CharRecoStroking(POINT point);
// 笔画结束
extern __declspec(dllimport) BOOL CharRecoStrokeEnd();
