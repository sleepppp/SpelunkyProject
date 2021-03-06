#include "stdafx.h"
#include "ImageManager.h"

#include "Image.h"
#include "Path.h"
#include "StringHelper.h"

#include <io.h>

ImageManager::ImageManager()
	:mWicFactory(nullptr)
{
	//WIC 팩토리 생성
	//Windows Imaging Component
	//Direct2D는 이미지를 로드하는 기능이 없다. 
	//고로 Direct2D는 WIC라는 라이브러리의 이미지로부터 이미지 데이터를 만들 수 있다.
	CoInitialize(NULL);
	HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&mWicFactory));
#ifdef _DEBUG
	assert(SUCCEEDED(hr));
#endif
}

/*********************************************************************
## ImageManager ##
**********************************************************************/
ImageManager::~ImageManager()
{
	this->DeleteAll();
	SafeRelease(mWicFactory);
}
/*********************************************************************
## AddImage ##
**********************************************************************/
Image * ImageManager::AddImage(const string& key, const wstring& file)
{
	Image* image = this->FindImage(key);
	if (image)
		return image;

	ID2D1Bitmap* bitmap = this->CreateD2DBitmapFromFile(file);
	if (bitmap)
	{
		Image::TagLoadedImageInfo loadInfo(key, file);
		image = new Image(bitmap, loadInfo);
		mImageList.insert(make_pair(key, image));
		return image;
	}
	else
		assert(E_FAIL);

	return nullptr;
}
/*********************************************************************
## AddFrameImage ##
**********************************************************************/
Image * ImageManager::AddFrameImage(const string& key,const wstring& file,const int& maxFrameX,const int& maxFrameY)
{
	Image* image = this->FindImage(key);
	if (image)
		return image;

	ID2D1Bitmap* bitmap = CreateD2DBitmapFromFile(file);
	if (bitmap)
	{
		Image::TagLoadedImageInfo loadInfo(key, file);
		image = new Image(bitmap,loadInfo,maxFrameX,maxFrameY);
		this->mImageList.insert(make_pair(key, image));
		return image;
	}

	return nullptr;
}
/*********************************************************************
## FindImage ##
**********************************************************************/
Image * ImageManager::FindImage(const string& key)
{
	ImageIter iter = mImageList.find(key);
	if (iter != mImageList.end())
		return iter->second;
	return nullptr;
}
/*********************************************************************
## CreateD2DBitmapFromFile ##
@@ wstring file : 파일 경로
**********************************************************************/
ID2D1Bitmap * ImageManager::CreateD2DBitmapFromFile(const wstring & file)
{
	//디코더 생성
	IWICBitmapDecoder* ipDecoder = nullptr;
	HRESULT hr;
	hr = mWicFactory->CreateDecoderFromFilename(file.c_str(), NULL, GENERIC_READ,
		WICDecodeMetadataCacheOnDemand, &ipDecoder);
#ifdef _DEBUG
	assert(SUCCEEDED(hr));
#endif
	//디코더에서 프레임얻음
	IWICBitmapFrameDecode* ipFrame = nullptr;
	hr = ipDecoder->GetFrame(0, &ipFrame);
#ifdef _DEBUG
	assert(SUCCEEDED(hr));
#endif
	//프레임을 기반으로 포맷 컨버터 생성
	IWICFormatConverter* convertedSrcBmp = nullptr;
	hr = mWicFactory->CreateFormatConverter(&convertedSrcBmp);
#ifdef _DEBUG
	assert(SUCCEEDED(hr));
#endif
	//컨버터 초기화
	hr = convertedSrcBmp->Initialize
	(
		ipFrame,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		nullptr,
		0.0f,
		WICBitmapPaletteTypeCustom
	);
#ifdef _DEBUG
	assert(SUCCEEDED(hr));
#endif
	//컨버트된 데이터를 기반으로 실제 Direct2D용 비트맵을 생성
	ID2D1Bitmap* ipResult = nullptr;
	hr = D2DRenderer::Get()->GetRenderTarget()->CreateBitmapFromWicBitmap
	(
		convertedSrcBmp,
		nullptr,
		&ipResult
	);
#ifdef _DEBUG
	assert(SUCCEEDED(hr));
#endif

	SafeRelease(convertedSrcBmp);
	SafeRelease(ipFrame);
	SafeRelease(ipDecoder);

	return ipResult;
}
/*********************************************************************
## DeleteAll ##
**********************************************************************/
void ImageManager::DeleteAll()
{
	ImageIter iter = mImageList.begin();
	for (; iter != mImageList.end(); ++iter)
	{
		SafeDelete(iter->second);
	}
	mImageList.clear();
}
/********************************************************************************
## LoadFolder ##
해당 폴더의 모든 png를 찾아 이미지로 등록
@@ const string path : 파일 경로
@@ const string filter : 찾을 파일 필터
********************************************************************************/
void ImageManager::LoadFolder(const string path, const string filter)
{
	std::string searching = path + filter;

	std::vector<std::string> return_;

	_finddata_t fd;
	long handle = _findfirst(searching.c_str(), &fd);  //현재 폴더 내 모든 파일을 찾는다.

	if (handle == -1)return;

	int result = 0;
	do
	{
		string filePath = path + fd.name;

		if (filter == "*.png")
			this->AddImage(Path::GetFileNameWithoutExtension(fd.name), StringHelper::StringToWString(filePath));

		result = _findnext(handle, &fd);
	} while (result != -1);

	_findclose(handle);
}

/********************************************************************************
## LoadAllResource ##
********************************************************************************/
void ImageManager::LoadAllResource()
{
	//끼요오오오오오옷

	this->AddFrameImage("Tile02", PathResources(L"./Tile/Tile02.png"), 8, 8);
	this->AddFrameImage("Items", PathResources(L"./Item/items.png"),25,12);
	this->AddFrameImage("Character0", PathResources(L"./Character/Character0.png"), 13, 13);
	this->AddFrameImage("Character1", PathResources(L"./Character/Character1.png"), 13, 13);
	this->AddFrameImage("rubble", PathResources(L"./Tile/rubble.png"), 8, 8);
	this->AddFrameImage("playerhud", PathResources(L"./UI/playerhud.png"), 1, 2);
	this->AddFrameImage("SelectBulletUI", PathResources(L"./UI/SelectBulletUI.png"), 2, 1);
	this->AddFrameImage("ShotGunFrame", PathResources(L"./Item/ShotGunFrame.png"), 2, 1);
	this->AddFrameImage("Smoke0", PathResources(L"./Effect/Smoke0.png"), 6, 1);
	this->AddFrameImage("SmokeOrange", PathResources(L"./Effect/SmokeOrange.png"), 6, 1);
	
	this->AddFrameImage("Fire01", PathResources(L"./Effect/Fire01.png"), 8, 4);
	this->AddFrameImage("effects", PathResources(L"Effect/effects.png"),8,8);

	this->AddFrameImage("Bomb", PathResources(L"./Item/Bomb.png"), 3, 1);
	this->AddFrameImage("monsters3", PathResources(L"./Monster/monsters3.png"), 13, 13);
	this->AddFrameImage("monsters2", PathResources(L"./Monster/monsters2.png"), 12, 12);
	this->AddFrameImage("monsters", PathResources(L"./Monster/monsters.png"), 13, 13);
	this->AddFrameImage("monstersbig2", PathResources(L"./Monster/monstersbig2.png"), 6,6);
	
	this->AddFrameImage("Revolver", PathResources(L"./Item/Revolver.png"), 2, 1);
	this->AddFrameImage("AK_47", PathResources(L"./Item/AK47.png"), 2, 1);
	this->AddFrameImage("AWP", PathResources(L"./Item/AWP.png"), 2, 1);

	this->AddFrameImage("dmscore", PathResources(L"./UI/dmscore.png"),3,4);
	
	this->LoadFolder("../Resources/Tile/", "*.png");
	this->LoadFolder("../Resources/Object/", "*.png");
	this->LoadFolder("../Resources/UI/", "*.png");
	this->LoadFolder("../Resources/Item/", "*.png");
	this->LoadFolder("../Resources/Bullet/", "*.png");
	this->LoadFolder("../Resources/Title/", "*.png");
}
