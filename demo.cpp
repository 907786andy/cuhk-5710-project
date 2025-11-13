
// HXH1155238049View.cpp: CHXH1155238049View 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "HXH1155238049.h"
#endif

#include "HXH1155238049Doc.h"
#include "HXH1155238049View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
ifstream in;
ofstream out;
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

// CHXH1155238049View

IMPLEMENT_DYNCREATE(CHXH1155238049View, CView)

BEGIN_MESSAGE_MAP(CHXH1155238049View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_TRANSLATION_XP, &CHXH1155238049View::OnTranslationXp)
	ON_COMMAND(ID_TRANSLATION_XM, &CHXH1155238049View::OnTranslationXm)
	ON_COMMAND(ID_TRANSLATION_YP, &CHXH1155238049View::OnTranslationYp)
	ON_COMMAND(ID_TRANSLATION_YM, &CHXH1155238049View::OnTranslationYm)
	ON_COMMAND(ID_TRANSLATION_ZP, &CHXH1155238049View::OnTranslationZp)
	ON_COMMAND(ID_TRANSLATION_ZM, &CHXH1155238049View::OnTranslationZm)
	ON_COMMAND(ID_ROTATION_XP, &CHXH1155238049View::OnRotationXp)
	ON_COMMAND(ID_ROTATION_XM, &CHXH1155238049View::OnRotationXm)
	ON_COMMAND(ID_ROTATION_YP, &CHXH1155238049View::OnRotationYp)
	ON_COMMAND(ID_ROTATION_YM, &CHXH1155238049View::OnRotationYm)
	ON_COMMAND(ID_ROTATION_ZP, &CHXH1155238049View::OnRotationZp)
	ON_COMMAND(ID_ROTATION_ZM, &CHXH1155238049View::OnRotationZm)
	ON_COMMAND(ID_ZOOM_IN, &CHXH1155238049View::OnZoomIn)
	ON_COMMAND(ID_ZOOM_OUT, &CHXH1155238049View::OnZoomOut)
	ON_COMMAND(ID_DISPLAY_SHADED, &CHXH1155238049View::OnDisplayShaded)
	ON_COMMAND(ID_DISPLAY_WIREFRAME, &CHXH1155238049View::OnDisplayWireframe)
	ON_COMMAND(ID_RXP, &CHXH1155238049View::OnRxp)
	ON_COMMAND(ID_RXM, &CHXH1155238049View::OnRxm)
	ON_COMMAND(ID_RYP, &CHXH1155238049View::OnRyp)
	ON_COMMAND(ID_RYM, &CHXH1155238049View::OnRym)
	ON_COMMAND(ID_RZP, &CHXH1155238049View::OnRzp)
	ON_COMMAND(ID_RZM, &CHXH1155238049View::OnRzm)
	ON_COMMAND(ID_TXP, &CHXH1155238049View::OnTxp)
	ON_COMMAND(ID_TXM, &CHXH1155238049View::OnTxm)
	ON_COMMAND(ID_TYP, &CHXH1155238049View::OnTyp)
	ON_COMMAND(ID_TYM, &CHXH1155238049View::OnTym)
	ON_COMMAND(ID_TZP, &CHXH1155238049View::OnTzp)
	ON_COMMAND(ID_TZM, &CHXH1155238049View::OnTzm)
	ON_COMMAND(ID_Rectangle, &CHXH1155238049View::OnRectangle)
	ON_COMMAND(ID_Cuboid, &CHXH1155238049View::OnCuboid)
	ON_COMMAND(ID_ZP, &CHXH1155238049View::OnZp)
	ON_COMMAND(ID_ZM, &CHXH1155238049View::OnZm)
	ON_COMMAND(ID_R, &CHXH1155238049View::OnR)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_OPEN, &CHXH1155238049View::OnFileOpen)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CHXH1155238049View 构造/析构

CHXH1155238049View::CHXH1155238049View() noexcept
{
	// TODO: 在此处添加构造代码
	m_hRC = 0;
	Cx = 0;
	Cy = 0;

	xtrans = 0.0;
	ytrans = 0.0;
	ztrans = 0.0;
	xrot = 0.0;
	yrot = 0.0;
	m_fScale = 1.0;

	modelCenterX = 0.0f;
	modelCenterY = 0.0f;
	modelCenterZ = 0.0f;
	isCenterCalculated = false;  // 中心是否已计算

	static float modelCenterY = 0.0f;
	static float modelCenterZ = 0.0f;
	static bool isCenterCalculated = false;

}

CHXH1155238049View::~CHXH1155238049View()
{
}

BOOL CHXH1155238049View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CHXH1155238049View 绘图

void CHXH1155238049View::OnDraw(CDC* pDC)
{
	CHXH1155238049Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	wglMakeCurrent(pDC->m_hDC, m_hRC);
	//draw here 
	glViewport(0, 0, Cx, Cy);
	SetupViewingFrustum((double)Cx / (double)Cy);
	SetupViewingTransform();
	Lighting();
	Material();
	RenderScene();
	wglMakeCurrent(pDC->m_hDC, NULL);

	// TODO: 在此处为本机数据添加绘制代码
}


// CHXH1155238049View 打印

BOOL CHXH1155238049View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CHXH1155238049View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CHXH1155238049View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CHXH1155238049View 诊断

#ifdef _DEBUG
void CHXH1155238049View::AssertValid() const
{
	CView::AssertValid();
}

void CHXH1155238049View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CHXH1155238049Doc* CHXH1155238049View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHXH1155238049Doc)));
	return (CHXH1155238049Doc*)m_pDocument;
}
#endif //_DEBUG


// CHXH1155238049View 消息处理程序

void CHXH1155238049View::OnTranslationXp()
{
	// TODO: 在此添加命令处理程序代码
	//AfxMessageBox("You have pressed the ‘X+’ selection in the menu.");
	xtrans += 1;
	InvalidateRect(nullptr, FALSE);
}

void CHXH1155238049View::OnTranslationXm()
{
	// TODO: 在此添加命令处理程序代码
	//AfxMessageBox("You have pressed the ‘X-’ selection in the menu.");
	xtrans -= 1;
	InvalidateRect(nullptr, FALSE);
}

void CHXH1155238049View::OnTranslationYp()
{
	// TODO: 在此添加命令处理程序代码
	//AfxMessageBox("You have pressed the ‘Y+’ selection in the menu.");
	ytrans += 1;
	InvalidateRect(nullptr, FALSE);

}

void CHXH1155238049View::OnTranslationYm()
{
	// TODO: 在此添加命令处理程序代码
	ytrans -= 1;
	InvalidateRect(nullptr, FALSE);
	//AfxMessageBox("You have pressed the ‘Y-’ selection in the menu.");
}

void CHXH1155238049View::OnTranslationZp()
{
	// TODO: 在此添加命令处理程序代码
	//AfxMessageBox("You have pressed the ‘Z+’ selection in the menu.");
	ztrans += 1;
	InvalidateRect(nullptr, FALSE);
}

void CHXH1155238049View::OnTranslationZm()
{
	// TODO: 在此添加命令处理程序代码
	//AfxMessageBox("You have pressed the ‘Z-’ selection in the menu.");
	ztrans -= 1;
	InvalidateRect(nullptr, FALSE);
}

void CHXH1155238049View::OnRotationXp()
{
	// TODO: 在此添加命令处理程序代码
	//AfxMessageBox("You have pressed the ‘X+’ selection in the menu.");
	xrot += 1.5;
	InvalidateRect(nullptr, FALSE);
	
}

void CHXH1155238049View::OnRotationXm()
{
	// TODO: 在此添加命令处理程序代码
	//AfxMessageBox("You have pressed the ‘X-’ selection in the menu.");
	xrot -= 1.5;
	InvalidateRect(nullptr, FALSE);
}

void CHXH1155238049View::OnRotationYp()
{
	// TODO: 在此添加命令处理程序代码
	//AfxMessageBox("You have pressed the ‘Y+’ selection in the menu.");
	yrot += 1.5;
	InvalidateRect(nullptr, FALSE);
}

void CHXH1155238049View::OnRotationYm()
{
	// TODO: 在此添加命令处理程序代码
	//AfxMessageBox("You have pressed the ‘Y-’ selection in the menu.");
	yrot -= 1.5;
	InvalidateRect(nullptr, FALSE);
}

void CHXH1155238049View::OnRotationZp()
{
	// TODO: 在此添加命令处理程序代码
	//AfxMessageBox("You have pressed the ‘Z+’ selection in the menu.");
	zrot += 1.5;
	InvalidateRect(nullptr, FALSE);
}

void CHXH1155238049View::OnRotationZm()
{
	// TODO: 在此添加命令处理程序代码
	//AfxMessageBox("You have pressed the ‘Z-’ selection in the menu.");
	zrot -= 1.5;
	InvalidateRect(nullptr, FALSE);
}

void CHXH1155238049View::OnZoomIn()
{
	// TODO: 在此添加命令处理程序代码
	//AfxMessageBox("You have pressed the ‘In’ selection in the menu.");
	m_fScale += 0.1;
	InvalidateRect(nullptr, FALSE);
}

void CHXH1155238049View::OnZoomOut()
{
	// TODO: 在此添加命令处理程序代码
	//AfxMessageBox("You have pressed the ‘Out’ selection in the menu.");
	m_fScale -= 0.1;
	InvalidateRect(nullptr, FALSE);
}

void CHXH1155238049View::OnDisplayShaded()
{
	// TODO: 在此添加命令处理程序代码
	//AfxMessageBox("You have pressed the ‘Shaded’ selection in the menu.");
	m_bWireframe = false;
	InvalidateRect(nullptr, FALSE);
}

void CHXH1155238049View::OnDisplayWireframe()
{
	// TODO: 在此添加命令处理程序代码
	//AfxMessageBox("You have pressed the ‘Wireframe’ selection in the menu.");
	m_bWireframe = true;
	InvalidateRect(nullptr, FALSE);
}

void CHXH1155238049View::OnRxp()
{
	// TODO: 在此添加命令处理程序代码
	//AfxMessageBox("You have pressed the ‘Rx+’ icon on Toolbar.");
	xrot += 1.5;
	InvalidateRect(nullptr, FALSE);
}

void CHXH1155238049View::OnRxm()
{
	// TODO: 在此添加命令处理程序代码
	//AfxMessageBox("You have pressed the ‘Rx-’ icon on Toolbar.");
	xrot -= 1.5;
	InvalidateRect(nullptr, FALSE);
}

void CHXH1155238049View::OnRyp()
{
	// TODO: 在此添加命令处理程序代码
	//AfxMessageBox("You have pressed the ‘Ry+’ icon on Toolbar.");
	yrot += 1.5;
	InvalidateRect(nullptr, FALSE);
}

void CHXH1155238049View::OnRym()
{
	// TODO: 在此添加命令处理程序代码
	//AfxMessageBox("You have pressed the ‘Ry-’ icon on Toolbar.");
	yrot -= 1.5;
	InvalidateRect(nullptr, FALSE);
}

void CHXH1155238049View::OnRzp()
{
	// TODO: 在此添加命令处理程序代码
	//AfxMessageBox("You have pressed the ‘Rz+’ icon on Toolbar.");
	zrot += 1.5;
	InvalidateRect(nullptr, FALSE);
}

void CHXH1155238049View::OnRzm()
{
	// TODO: 在此添加命令处理程序代码
	//AfxMessageBox("You have pressed the ‘Rz-’ icon on Toolbar.");
	zrot -= 1.5;
	InvalidateRect(nullptr, FALSE);
}

void CHXH1155238049View::OnTxp()
{
	// TODO: 在此添加命令处理程序代码
	//AfxMessageBox("You have pressed the ‘Tx+’ icon on Toolbar.");
	xtrans += 1;
	InvalidateRect(nullptr, FALSE);
}

void CHXH1155238049View::OnTxm()
{
	// TODO: 在此添加命令处理程序代码
	//AfxMessageBox("You have pressed the ‘Tx-’ icon on Toolbar.");
	xtrans -= 1;
	InvalidateRect(nullptr, FALSE);
}

void CHXH1155238049View::OnTyp()
{
	// TODO: 在此添加命令处理程序代码
	//AfxMessageBox("You have pressed the ‘Ty+’ icon on Toolbar.");
	ytrans += 1;
	InvalidateRect(nullptr, FALSE);
}

void CHXH1155238049View::OnTym()
{
	// TODO: 在此添加命令处理程序代码
	//AfxMessageBox("You have pressed the ‘Ty-’ icon on Toolbar.");
	ytrans -= 1;
	InvalidateRect(nullptr, FALSE);
}

void CHXH1155238049View::OnTzp()
{
	// TODO: 在此添加命令处理程序代码
	//AfxMessageBox("You have pressed the ‘Tz+’ icon on Toolbar.");
	ztrans += 1;
	InvalidateRect(nullptr, FALSE);
}

void CHXH1155238049View::OnTzm()
{
	// TODO: 在此添加命令处理程序代码
	//AfxMessageBox("You have pressed the ‘Tz-’ icon on Toolbar.");
	ztrans -= 1;
	InvalidateRect(nullptr, FALSE);
}

void CHXH1155238049View::OnRectangle()
{
	// TODO: 在此添加命令处理程序代码
	//AfxMessageBox("You have pressed the ‘Rectangle’ icon on Toolbar.");
	m_bWireframe = false;
	InvalidateRect(nullptr, FALSE);
}

void CHXH1155238049View::OnCuboid()
{
	// TODO: 在此添加命令处理程序代码
	//AfxMessageBox("You have pressed the ‘Cuboid’ icon on Toolbar.");
	m_bWireframe = true;
	InvalidateRect(nullptr, FALSE);
}

void CHXH1155238049View::OnZp()
{
	// TODO: 在此添加命令处理程序代码
	//AfxMessageBox("You have pressed the ‘Z+’ icon on Toolbar.");
	m_fScale += 0.1;
	InvalidateRect(nullptr, FALSE);
}

void CHXH1155238049View::OnZm()
{
	// TODO: 在此添加命令处理程序代码
	//AfxMessageBox("You have pressed the ‘Z-’ icon on Toolbar.");
	m_fScale -= 0.1;
	InvalidateRect(nullptr, FALSE);
}

void CHXH1155238049View::OnR()
{
	// TODO: 在此添加命令处理程序代码
	// 重置平移参数（回到初始位置）
	xtrans = 0.0f;
	ytrans = 0.0f;
	ztrans = 0.0f;

	// 重置旋转角度（回到初始朝向）
	xrot = 0.0f;
	yrot = 0.0f;

	// 重置缩放比例（回到100%大小）
	m_fScale = 1.0f;

	// 重绘场景，生效复位
	InvalidateRect(nullptr, FALSE);
}

bool CHXH1155238049View::SetupPixelFormat()
{
	static PIXELFORMATDESCRIPTOR pfd =
	{
			sizeof(PIXELFORMATDESCRIPTOR), // Size of this pfd
			1, // Version number
			PFD_DRAW_TO_WINDOW | // Support window
			PFD_SUPPORT_OPENGL | // Support OpenGL
			PFD_TYPE_RGBA, // RGBA type
			32, // 32-bit color depth
			0, 0, 0, 0, 0, 0, // Color bits ignored
			0, // No alpha buffer
			0, // Shift bit ignored
			0, // No accumulation buffer
			0, 0, 0, 0, // Accum bits ignored
			16, // 16-bit z-buffer
			0, // No stencil buffer
			0, // No auxiliary buffer
			PFD_MAIN_PLANE, // Main layer
			0, // Reserved
			0, 0, 0 // Layer masks ignored
	};
	int pixelformat;
	CClientDC clientDC(this); //get device context
	// match pixel format supported by a device context to given spec
	if ((pixelformat = ::ChoosePixelFormat(clientDC.m_hDC, &pfd)) == 0)
		return false;
	//apply pixel format to view
	if (!(::SetPixelFormat(clientDC.m_hDC, pixelformat, &pfd)))
		return false;
	// bridge OpenGL rendering context with device context
	m_hRC = wglCreateContext(clientDC.m_hDC);
	return true;
}



void CHXH1155238049View::OnDestroy()
{
	CView::OnDestroy();

	// TODO: Add your message handler code here
	::wglDeleteContext(m_hRC);
}

int CHXH1155238049View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	if (!SetupPixelFormat())
		return -1;
	return 0;
}

void CHXH1155238049View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	Cx = cx;
	Cy = cy;
	InvalidateRect(0, FALSE);
	// TODO: Add your message handler code here
}



// 去除字符串两端空白的trim函数
void trim(std::string& str) {
	str.erase(0, str.find_first_not_of(" \t\n\r"));
	if (!str.empty()) {
		str.erase(str.find_last_not_of(" \t\n\r") + 1);
	}
}

void CHXH1155238049View::OnFileOpen()
{
	// 设置文件对话框
	CString szFilter = _T("STL Files|*.stl|All Files|*.*||");
	CFileDialog FD(TRUE, _T("stl"), _T("*.stl"), OFN_HIDEREADONLY, szFilter);

	if (FD.DoModal() == IDOK)
	{
		CString filename = FD.GetPathName();
		std::ifstream in(filename.GetString());  // 打开STL文件

		if (!in.is_open())  // 更严谨的文件打开判断
		{
			AfxMessageBox(_T("文件打开失败！"));
			return;
		}

		std::string line;
		m_facets.clear();  // 清空历史数据

		// 逐行读取文件
		while (std::getline(in, line)) {
			trim(line);

			if (line.find("facet normal") != std::string::npos) {
				float nx, ny, nz;
				std::istringstream stream(line);
				std::string temp1, temp2;

				stream >> temp1 >> temp2;  // 跳过"facet"和"normal"
				if (!(stream >> nx >> ny >> nz)) {
					AfxMessageBox(_T("法向量解析失败！"));
					continue;
				}

				Facet currFacet;
				currFacet.normal = { nx, ny, nz };

				// 处理"outer loop"
				if (!std::getline(in, line)) {
					AfxMessageBox(_T("未找到outer loop行！"));
					break;
				}
				trim(line);
				if (line != "outer loop") {
					AfxMessageBox(_T("格式错误：缺少outer loop！"));
					continue;
				}

				// 读取3个顶点
				bool vertexReadOK = true;
				for (int i = 0; i < 3; ++i) {
					do {
						if (!std::getline(in, line)) {
							AfxMessageBox(_T("顶点数量不足，文件提前结束！"));
							vertexReadOK = false;
							break;
						}
						trim(line);
					} while (line.find("vertex") == std::string::npos);

					if (!vertexReadOK) break;

					std::istringstream vertexStream(line);
					std::string vertexLabel;
					float x, y, z;
					if (!(vertexStream >> vertexLabel >> x >> y >> z)) {
						AfxMessageBox(_T("顶点解析失败！"));
						vertexReadOK = false;
						break;
					}

					currFacet.vertices.push_back({ x, y, z });
				}

				if (!vertexReadOK) continue;

				// 处理"endloop"
				if (!std::getline(in, line)) {
					AfxMessageBox(_T("未找到endloop行！"));
					continue;
				}
				trim(line);
				if (line != "endloop") {
					AfxMessageBox(_T("格式错误：缺少endloop！"));
					continue;
				}

				// 处理"endfacet"
				if (!std::getline(in, line)) {
					AfxMessageBox(_T("未找到endfacet行！"));
					continue;
				}
				trim(line);
				if (line != "endfacet") {
					AfxMessageBox(_T("格式错误：缺少endfacet！"));
					continue;
				}

				m_facets.push_back(currFacet);
			}
		}

		in.close();

		// 模型加载后，计算并应用居中预处理
		if (!m_facets.empty()) {
			// 计算模型中心
			float totalX = 0.0f, totalY = 0.0f, totalZ = 0.0f;
			int vertexCount = 0;
			for (const auto& facet : m_facets) {
				for (const auto& vertex : facet.vertices) {
					totalX += vertex.x;
					totalY += vertex.y;
					totalZ += vertex.z;
					vertexCount++;
				}
			}
			float centerX = totalX / vertexCount;
			float centerY = totalY / vertexCount;
			float centerZ = totalZ / vertexCount;

			// 对所有顶点做居中预处理（减去中心坐标）
			for (auto& facet : m_facets) {
				for (auto& vertex : facet.vertices) {
					vertex.x -= centerX;
					vertex.y -= centerY;
					vertex.z -= centerZ;
				}
			}

			// 重置模型中心为原点（因为已居中）
			modelCenterX = 0.0f;
			modelCenterY = 0.0f;
			modelCenterZ = 0.0f;
			isCenterCalculated = true;  // 标记为已计算

			CString successMsg;
			successMsg.Format(_T("读取成功！共读取%d个面数据，已居中处理。"), (int)m_facets.size());
			AfxMessageBox(successMsg);
		}
		else {
			AfxMessageBox(_T("未读取到任何面数据！"));
		}
	}
	InvalidateRect(nullptr, FALSE);
}



void CHXH1155238049View::SetupViewingFrustum(double aspect)
{
	glMatrixMode(GL_PROJECTION);// 设置当前矩阵模式为投影矩阵
	glLoadIdentity(); // 重置当前矩阵
	gluPerspective(60.0, aspect, 1, 400.0);// 设置透视投影
}

void CHXH1155238049View::SetupViewingTransform()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// 步骤1：先处理世界坐标系平移（固定轴，不随旋转变化）
	glTranslatef(xtrans, ytrans, ztrans);  // 平移方向始终是世界X/Y/Z轴

	// 步骤2：相机后退，留出旋转空间
	const float cameraDistance = 120.0f;
	glTranslatef(0, 0, -cameraDistance);

	// 步骤3：旋转围绕模型中心（核心：确保旋转围绕模型中心）
	glRotatef(yrot, 0, 1, 0);  // Y轴旋转（左右转）
	glRotatef(xrot, 1, 0, 0);  // X轴旋转（上下转）
	glRotatef(zrot, 0, 0, 1);  // z轴旋转（上下转）


	// 步骤4：缩放
	float invScale =  m_fScale;
	glScalef(invScale, invScale, invScale);
}

void CHXH1155238049View::RenderScene()
{
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);

	// 根据线框模式变量设置绘制方式
	if (m_bWireframe) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // 线框模式
	}
	else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // 实体模式
	}

	glPushMatrix();

	glBegin(GL_TRIANGLES);
	for (const auto& f : m_facets) {
		if (f.vertices.size() < 3) continue;

		// 为每个三角形的每个顶点设置颜色
		//使用固定的颜色（红色）
		glColor3f(1.0f, 0.0f, 0.0f); // 设置红色
		glNormal3f(f.normal.x, f.normal.y, f.normal.z);
		glVertex3f(f.vertices[0].x, f.vertices[0].y, f.vertices[0].z);

		glColor3f(0.0f, 1.0f, 0.0f); // 设置绿色
		glVertex3f(f.vertices[1].x, f.vertices[1].y, f.vertices[1].z);

		glColor3f(0.0f, 0.0f, 1.0f); // 设置蓝色
		glVertex3f(f.vertices[2].x, f.vertices[2].y, f.vertices[2].z);
	}
	glEnd();

	glPopMatrix();
}


void CHXH1155238049View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CView::OnMouseMove(nFlags, point);
	current_point = point;

	if (nFlags == MK_LBUTTON && current_point.y > last_point.y + 2)
		xrot += 1.5;
	else
		if (nFlags == MK_LBUTTON && current_point.y < last_point.y - 2)
			xrot -= 1.5;
	//check the location of cursor in x direction to perform rotation about y axis
	if (nFlags == MK_LBUTTON && current_point.x > last_point.x + 2)
		yrot += 1.5;
	else if (nFlags == MK_LBUTTON && current_point.x < last_point.x - 2)
		yrot -= 1.5;
	last_point = point; //record the last point
	InvalidateRect(0, FALSE);

}

BOOL CHXH1155238049View::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	if (zDelta > 0)
		m_fScale += 0.1f;
	else if (m_fScale > 0.1f)
		m_fScale -= 0.1f;
	InvalidateRect(NULL, FALSE);
	return CView::OnMouseWheel(nFlags, zDelta, pt);

}

void CHXH1155238049View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
	if (nChar == VK_RIGHT) //Right button on keyboard
		xtrans += 1.0;
	if (nChar == VK_LEFT) //
		xtrans -= 1.0;
	if (nChar == VK_UP) //
		ytrans += 1.0;
	if (nChar == VK_DOWN) //
		ytrans -= 1.0;
	if (nChar == VK_PRIOR) //
		ztrans += 1.0;
	if (nChar == VK_NEXT) //
		ztrans -= 1.0;

	// Redraw the scene once
	InvalidateRect(0, FALSE);
}
void CHXH1155238049View::Lighting()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE); // force the length of surface normal to be 1
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_LIGHT0); // Main light (Directional Light)
	glEnable(GL_LIGHT1); // Additional light (Point Light)
	glEnable(GL_LIGHT2); // Additional light (Spotlight)

	// Directional light
	GLfloat light_position0[] = { 1.0f, 0.0f, 1.0f, 0.0f }; // Directional light
	GLfloat light_ambient0[] = { 0.3f, 0.3f, 0.3f, 1.0f };
	GLfloat light_diffuse0[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light_specular0[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular0);

	// Point light (adds depth and realism)
	GLfloat light_position1[] = { 2.0f, 2.0f, 2.0f, 1.0f }; // Point light
	GLfloat light_ambient1[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat light_diffuse1[] = { 1.0f, 0.7f, 0.7f, 1.0f }; // Warm reddish tone
	GLfloat light_specular1[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);

	// Spotlight (focused beam, adds dramatic effect)
	GLfloat light_position2[] = { 0.0f, 5.0f, 5.0f, 1.0f }; // Spotlight position
	GLfloat light_direction2[] = { 0.0f, -1.0f, -1.0f }; // Direction towards the scene
	GLfloat light_ambient2[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	GLfloat light_diffuse2[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light_specular2[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light_cutoff2[] = { 30.0f }; // Spotlight angle
	GLfloat light_exponent2[] = { 10.0f }; // Spotlight sharpness

	glLightfv(GL_LIGHT2, GL_POSITION, light_position2);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, light_direction2);
	glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse2);
	glLightfv(GL_LIGHT2, GL_SPECULAR, light_specular2);
	glLightfv(GL_LIGHT2, GL_SPOT_CUTOFF, light_cutoff2);
	glLightfv(GL_LIGHT2, GL_SPOT_EXPONENT, light_exponent2);
}

void CHXH1155238049View::Material()
{
	glEnable(GL_COLOR_MATERIAL); // Allow interaction with light using glColor3f()
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE); // Allow object's color to interact with light

	// Material properties
	GLfloat mat_diffuse[] = { 0.7f, 0.5f, 0.3f, 1.0f }; // Rich warm color
	GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // Strong specular reflection
	GLfloat mat_shininess[] = { 100.0f }; // High shininess (glossy surface)
	GLfloat mat_emission[] = { 0.1f, 0.1f, 0.1f, 1.0f }; // Subtle glow effect

	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
}