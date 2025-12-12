// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 CHARRECOSVR_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// CHARRECOSVR_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef CHARRECOSVR_EXPORTS
#define CHARRECOSVR_API __declspec(dllexport)
#else
#define CHARRECOSVR_API __declspec(dllimport)
#endif

#include "windows.h"


// 定义笔迹中的结束标记
#define		X_STROKE_END_FLAG	(-1)
#define		Y_STROKE_END_FLAG	( 0)
#define		X_CHAR_END_FLAG		(-1)
#define		Y_CHAR_END_FLAG		(-1)

#define  MAX_POINT_NUM          1000  // 一次最多能识别的点数（点数到达 1024 之后，调用 HSCharRecognize 时，识别核心崩溃）
#define  MAX_STROKE_NUM         500   // 最多的笔画数（包含结束标志的笔画至少占两点，故只需最多点数的一半即可）

#define  GAP_PERCENT          30    // 文字拆分最小间隙百分比（相对于文字大小）
#define  MAX_CANDIDATE_NUM    10

// 此类是从 CharReco.dll 导出的
class CHARRECOSVR_API CCharRecoSvr {
public:
	CCharRecoSvr(void);
	~CCharRecoSvr();
	// TODO: 在此添加您的方法。


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
// 	CHAR m_szHSAdapt[3][MAX_PATH];

    unsigned short m_pCharater[MAX_CANDIDATE_NUM + 1]; // 识别结果缓冲区（为字符串结束标志预留一个点）
	//	LONG m_pDistance[MAX_CANDIDATE_NUM + 1]; // 识别权重缓冲区（为权值串结束标志预

public:


#if 1

    int Recognize(short m_pPoint[MAX_POINT_NUM * 2 + 2],int &m_iPoint,  stStroke m_pStroke[MAX_STROKE_NUM],	int &m_iStroke,unsigned short m_pCharater[MAX_CANDIDATE_NUM + 1], bool m_bMatchMulti,/*LONG m_pDistance[MAX_CANDIDATE_NUM + 1],*/ unsigned int m_uMatchRange);

#endif

};
