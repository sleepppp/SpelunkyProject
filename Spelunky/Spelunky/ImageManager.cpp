#include "stdafx.h"
#include "ImageManager.h"

#include "Image.h"
#include "Path.h"
#include "StringHelper.h"

#include <io.h>

ImageManager::ImageManager()
	:mWicFactory(nullptr)
{
	//WIC ���丮 ����
	//Windows Imaging Component
	//Direct2D�� �̹����� �ε��ϴ� ����� ����. 
	//��� Direct2D�� WIC��� ���̺귯���� �̹����κ��� �̹��� �����͸� ���� �� �ִ�.
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
@@ wstring file : ���� ���
**********************************************************************/
ID2D1Bitmap * ImageManager::CreateD2DBitmapFromFile(const wstring & file)
{
	//���ڴ� ����
	IWICBitmapDecoder* ipDecoder = nullptr;
	HRESULT hr;
	hr = mWicFactory->CreateDecoderFromFilename(file.c_str(), NULL, GENERIC_READ,
		WICDecodeMetadataCacheOnDemand, &ipDecoder);
#ifdef _DEBUG
	assert(SUCCEEDED(hr));
#endif
	//���ڴ����� �����Ӿ���
	IWICBitmapFrameDecode* ipFrame = nullptr;
	hr = ipDecoder->GetFrame(0, &ipFrame);
#ifdef _DEBUG
	assert(SUCCEEDED(hr));
#endif
	//�������� ������� ���� ������ ����
	IWICFormatConverter* convertedSrcBmp = nullptr;
	hr = mWicFactory->CreateFormatConverter(&convertedSrcBmp);
#ifdef _DEBUG
	assert(SUCCEEDED(hr));
#endif
	//������ �ʱ�ȭ
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
	//����Ʈ�� �����͸� ������� ���� Direct2D�� ��Ʈ���� ����
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
�ش� ������ ��� png�� ã�� �̹����� ���
@@ const string path : ���� ���
@@ const string filter : ã�� ���� ����
********************************************************************************/
void ImageManager::LoadFolder(const string path, const string filter)
{
	std::string searching = path + filter;

	std::vector<std::string> return_;

	_finddata_t fd;
	long handle = _findfirst(searching.c_str(), &fd);  //���� ���� �� ��� ������ ã�´�.

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
	this->AddFrameImage("Tile02", PathResources(L"./Tile/Tile02.png"), 8, 8);
	this->AddFrameImage("Items", PathResources(L"./Item/items.png"),25,12);
	this->LoadFolder("../Resources/Tile/", "*.png");
	this->LoadFolder("../Resources/Object/", "*.png");
}
