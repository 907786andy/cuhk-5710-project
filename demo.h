
// HXH1155238049View.h: CHXH1155238049View 类的接口
//

#pragma once


struct Vertex {
	float x, y, z;
};

// 2. 面结构体
struct Facet {
	Vertex normal;          // 法向量
	std::vector<Vertex> vertices;  // 顶点列表（3个顶点）
};
class CHXH1155238049View : public CView
{
protected: // 仅从序列化创建
	CHXH1155238049View() noexcept;
	DECLARE_DYNCREATE(CHXH1155238049View)

// 特性
public:
	CHXH1155238049Doc* GetDocument() const;

// 操作
public:


	HGLRC m_hRC;
	int Cx;
	int Cy;
	bool SetupPixelFormat();
	void SetupViewingFrustum(double aspect);
	void SetupViewingTransform();
	std::vector<Facet> m_facets;
	void RenderScene();
	bool m_bWireframe = false; // 线框模式开关（默认实体模式）

	CPoint current_point;
	CPoint last_point;

	float modelCenterX;
	float modelCenterY;
	float modelCenterZ;
	bool isCenterCalculated;


	float xtrans = 0.0f;
	float ytrans = 0.0f;
	float ztrans = 0.0f;
	float xrot = 0.0f;
	float yrot = 0.0f;
	float zrot = 0.0f;
	float m_fScale = 1.0f;


	// 相机当前朝向向量（平移时看向前方的方向）
	float forwardX = 0.0f;
	float forwardY = 0.0f;
	float forwardZ = -1.0f;  // 初始指向模型（z轴负方向）

	// 状态标志：是否正在旋转（左键按下时为true）
	bool isRotating = false;

	void Lighting();
	void Material();



// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CHXH1155238049View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTranslationXp();
	afx_msg void OnTranslationXm();
	afx_msg void OnTranslationYp();
	afx_msg void OnTranslationYm();
	afx_msg void OnTranslationZp();
	afx_msg void OnTranslationZm();
	afx_msg void OnRotationXp();
	afx_msg void OnRotationXm();
	afx_msg void OnRotationYp();
	afx_msg void OnRotationYm();
	afx_msg void OnRotationZp();
	afx_msg void OnRotationZm();
	afx_msg void OnZoomIn();
	afx_msg void OnZoomOut();
	afx_msg void OnDisplayShaded();
	afx_msg void OnDisplayWireframe();
	afx_msg void OnRxp();
	afx_msg void OnRxm();
	afx_msg void OnRyp();
	afx_msg void OnRym();
	afx_msg void OnRzp();
	afx_msg void OnRzm();
	afx_msg void OnTxp();
	afx_msg void OnTxm();
	afx_msg void OnTyp();
	afx_msg void OnTym();
	afx_msg void OnTzp();
	afx_msg void OnTzm();
	afx_msg void OnRectangle();
	afx_msg void OnCuboid();
	afx_msg void OnZp();
	afx_msg void OnZm();
	afx_msg void OnR();
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFileOpen();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // HXH1155238049View.cpp 中的调试版本
inline CHXH1155238049Doc* CHXH1155238049View::GetDocument() const
   { return reinterpret_cast<CHXH1155238049Doc*>(m_pDocument); }
#endif

