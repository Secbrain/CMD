// ADOConn.cpp: implementation of the ADOConn class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ADOConn.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ADOConn::ADOConn()
{

}

ADOConn::~ADOConn()
{

}

// connect
void  ADOConn::OnInitADOConn()
{
	// initialize OLE/COM 
	::CoInitialize(NULL);
  
	try
	{
		// create Connection 
		m_pConnection.CreateInstance("ADODB.Connection");
		// set string, BSTR or _bstr_t
		_bstr_t strConnect = "Provider=SQLOLEDB; Server=DESKTOP-8U6MU1N; Database=jffs2filesystem; Integrated Security= SSPI;";
		m_pConnection->Open(strConnect,"","",adModeUnknown);
	}
	// catch
	catch(_com_error e)
	{
		// show
		AfxMessageBox(e.Description());
	}
}

// excute
_RecordsetPtr&  ADOConn::GetRecordSet(_bstr_t bstrSQL)
{
	try
	{
		// connect
		if(m_pConnection==NULL)
			OnInitADOConn();
		// create
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		// read
		m_pRecordset->Open(bstrSQL,m_pConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
	}
	// catch
	catch(_com_error e)
	{
		// show
		AfxMessageBox(e.Description());
	}
	// result
	return m_pRecordset;
}

 // excute SQL£¬Insert Update _variant_t
BOOL ADOConn::ExecuteSQL(_bstr_t bstrSQL)
{
//	_variant_t RecordsAffected;
	try
	{
		// if connect
		if(m_pConnection == NULL)
			OnInitADOConn();
		// Execute:(_bstr_t CommandText, 
		// VARIANT * RecordsAffected, long Options ) 
		// CommandText, SQL commands
		// RecordsAffected
		// Options means CommandText type£ºadCmdText-text£»adCmdTable-table
		// adCmdProc-£»adCmdUnknown-
		m_pConnection->Execute(bstrSQL,NULL,adCmdText);
		return true;
	}
	catch(_com_error e)
	{
		AfxMessageBox(e.Description());
		return false;
	}
}

void ADOConn::ExitConnect()
{
	// close
	if (m_pRecordset != NULL)
		m_pRecordset->Close();
	m_pConnection->Close();
	// 
	::CoUninitialize();
}