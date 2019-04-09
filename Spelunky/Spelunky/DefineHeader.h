#pragma once

/*********************************************************************************
## PathResource ##
Resources폴더 경로
*********************************************************************************/
#define PathResources(n) (wstring(L"../Resources/").append(n))
/*********************************************************************************
## CastingInt ##
int로 static_cast
*********************************************************************************/
#define CastingInt(n) static_cast<int>(n)
/*********************************************************************************
## CastingFloat ##
float으로 static_cast
*********************************************************************************/
#define CastingFloat(n) static_cast<float>(n)

#define CastingByType(type,value) ((type)value)
/*********************************************************************************
## Safe - Relase,Delete,DeleteArray ##
*********************************************************************************/
#define SafeRelease(p){ if(p){ (p)->Release(); (p) = NULL; } }
#define SafeDelete(p){ if(p){ delete (p); (p) = NULL; } }
#define SafeDeleteArray(p){ if(p){ delete [] (p); (p) = NULL; } }
/*********************************************************************************
## BlockAssign  ##
대입 생성 및 복사 생성을 막는다
*********************************************************************************/
#define BlockAssign(ClassName)\
public:\
ClassName(const ClassName& t) = delete;\
const ClassName& operator=(const ClassName& t) = delete;
/*********************************************************************************
## Synthesize ##
@@ VarType : 자료형
@@ VarName : 변수 이름
@@ FuncName : 함수 이름

변수,접근자 설정자 매크로 전처리기
*********************************************************************************/
#define Synthesize(VarType, VarName, FuncName)	\
protected:	VarType VarName;	\
public:	inline VarType Get##FuncName(void) const { return VarName; }	\
public:	inline void Set##FuncName(VarType value) { VarName = value; }
/*********************************************************************************
## Assert Checking ##
안정섬 검사 문들
*********************************************************************************/
#if defined(DEBUG) | defined(_DEBUG)
#ifndef HResult
#define HResult(x)                                       \
   {                                             \
      HRESULT hr = (x);                              \
      if(FAILED(hr))                                 \
      {                                          \
         LPWSTR output;                              \
         FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS |            \
         FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),   \
            (LPTSTR)&output, 0, NULL);                                             \
            MessageBox(NULL, output, L"Error", MB_OK);                                 \
      }                                                                     \
   }
#endif

#else
#ifndef HResult
#define HResult(x) (x)
#endif
#endif 

#ifndef Assert
#if defined(DEBUG) || defined(_DEBUG)
#define Assert(b) do {if (!(b)) {OutputDebugStringA("Assert : " #b "\n"); }} while (0)
#else
#define Assert(b)
#endif   //   DEBUG || _DEBUG
#endif

/********************************************************
## DefaultSingleton ##
*********************************************************/
#define DefaultSingletonHeader(ClassName)	\
public:								\
	static ClassName* Get();		\
	static void Create();			\
	static void Delete();			\
private:							\
	ClassName();					\
	~ClassName();					\
	static ClassName* instance;		

#define DefaultSingletonCpp(ClassName)		\
ClassName* ClassName::instance = nullptr;	\
ClassName * ClassName::Get()				\
{											\
	assert(instance != nullptr);			\
	return instance;						\
}											\
void ClassName::Create()					\
{											\
	assert(instance == nullptr);			\
	instance = new ClassName();				\
}											\
void ClassName::Delete()					\
{											\
	SafeDelete(instance);					\
}			
