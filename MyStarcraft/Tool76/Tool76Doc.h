
// Tool76Doc.h : CTool76Doc Ŭ������ �������̽�
//


#pragma once


class CTool76Doc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CTool76Doc();
	DECLARE_DYNCREATE(CTool76Doc)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// �����Դϴ�.
public:
	virtual ~CTool76Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};


