#include "DBManager.h"
#include "Function.h"


CDBManager::CDBManager()
{
}


CDBManager::~CDBManager()
{
}


CString CDBManager::getPath(CString inode){
	CString rootinode=_T("00 00 00 01");
	CString pinode;
	CString path=_T(""),pathtemp=_T("");
	//connect
	ADOConn m_AdoConn;
	m_AdoConn.OnInitADOConn();
	//set SELECT
	_bstr_t getpinode,getname;
	CString sql;
	//excute SELECT
	_RecordsetPtr m_pRecordPinode, m_pRecordName;
	getpinode = _T("select * from iNodeconnection where iNode='" + inode + "'");
	
	pathtemp = "/";
	path = path + pathtemp;
	while (inode != rootinode){
		m_pRecordPinode = m_AdoConn.GetRecordSet(getpinode);  //save result to m_pRecordset
		if (m_pRecordPinode->adoEOF)  //not existing
		{
			pathtemp = inode;
			return path;
		}
		else
		{
			pinode = m_pRecordPinode->GetCollect("piNode"); //read a row from m_pRecordset
			getname = _T("select * from packetname where packetiNode='" + inode + "'");
			
			m_pRecordName = m_AdoConn.GetRecordSet(getname);  //save result to m_pRecordset
			
			if (m_pRecordName->adoEOF){
				pathtemp = inode;
			}
			else{
				pathtemp = m_pRecordName->GetCollect("packetname");
			}
			inode = pinode;
		}
		path = pathtemp + path;
		path = _T("/") + path;
		getpinode = _T("select * from iNodeconnection where iNode='" + inode + "'");
	}
	m_AdoConn.ExitConnect();
	return path;
}

void CDBManager::newfile(CString pinode,CString inode, CString filename){
	//connect
	ADOConn m_AdoConn;
	m_AdoConn.OnInitADOConn();
	//set SELECT
	_bstr_t pushnewfile, pushnewconnection,checkinode;
	//CString sql;
	//excute SELECT
	_RecordsetPtr filetest;
	checkinode = _T("select * from filename where fileiNode='" + inode + "'");
	pushnewfile = _T("insert into filename values ('" + inode + "','" + filename + "')");
	pushnewconnection = _T("insert into iNodeconnection values ('" + pinode + "','" + inode + "')");
	filetest = m_AdoConn.GetRecordSet(checkinode);  //save result to m_pRecordset
	if (filetest->adoEOF)  //not existing
	{
		m_AdoConn.ExecuteSQL(pushnewfile);
		m_AdoConn.ExecuteSQL(pushnewconnection);
	}
	m_AdoConn.ExitConnect();
}

void CDBManager::newfolder(CString pinode,CString inode, CString foldername){
	ADOConn m_AdoConn;
	m_AdoConn.OnInitADOConn();
	//set SELECT
	_bstr_t pushnewfile, pushnewconnection, checkinode;
	//CString sql;
	//excute SELECT
	_RecordsetPtr filetest;
	checkinode = _T("select * from packetname where packetiNode='" + inode + "'");
	pushnewfile = _T("insert into packetname values ('" + inode + "','" + foldername + "')");
	pushnewconnection = _T("insert into iNodeconnection values ('" + pinode + "','" + inode + "')");
	filetest = m_AdoConn.GetRecordSet(checkinode);  //save result to m_pRecordset
	if (filetest->adoEOF)  //not existing
	{
		m_AdoConn.ExecuteSQL(pushnewfile);
		m_AdoConn.ExecuteSQL(pushnewconnection);
	}
	m_AdoConn.ExitConnect();
}

CString CDBManager::getfilecompletePath(CString inode){
	CString filecompletepath=_T("");
	CString rootinode = _T("00 00 00 01");
	CString pinode;
	CString pathtemp = _T("");
	//connect
	ADOConn m_AdoConn;
	m_AdoConn.OnInitADOConn();
	//set SELECT
	_bstr_t getname, getpath,getpinode;
	CString sql;
	//excute SELECT
	_RecordsetPtr m_pRecordPinode, m_pRecordPath,m_pRecordName;
	getname = _T("select * from filename where fileiNode='" + inode + "'");
	m_pRecordName = m_AdoConn.GetRecordSet(getname);
	if (m_pRecordName->adoEOF)  //not existing
	{
		pathtemp = inode;
		return filecompletepath;
	}
	else
	{
		pathtemp = m_pRecordName->GetCollect("filename"); //read a row from m_pRecordset
		filecompletepath = filecompletepath + pathtemp;
	}
	getpinode = _T("select * from iNodeconnection where iNode='" + inode + "'");
	while (inode != rootinode){
		m_pRecordPinode = m_AdoConn.GetRecordSet(getpinode);  //save result to m_pRecordset
		if (m_pRecordPinode->adoEOF)  //not existing
		{
			pathtemp = inode;
			return filecompletepath;
		}
		else
		{
			pinode = m_pRecordPinode->GetCollect("piNode"); //read a row from m_pRecordset
			
			getpath = _T("select * from packetname where packetiNode='" + pinode + "'");
			m_pRecordName = m_AdoConn.GetRecordSet(getpath);  //save result to m_pRecordset
			if (m_pRecordName->adoEOF){
				pathtemp = inode;
			}
			else{
				pathtemp = m_pRecordName->GetCollect("packetname");
			}
			inode = pinode;
		}
		
		filecompletepath = _T("/") + filecompletepath;
		filecompletepath = pathtemp + filecompletepath;
		getpinode = _T("select * from iNodeconnection where iNode='" + inode + "'");
	}
	m_AdoConn.ExitConnect();
	return filecompletepath;
}

void CDBManager::setfilename(CString inode, CString name){
	ADOConn m_AdoConn;
	m_AdoConn.OnInitADOConn();
	//set SELECT
	_bstr_t setfilename,selecttest;
	//CString sql;
	//excute SELECT
	_RecordsetPtr filetest;
	selecttest = _T("select * from filename where fileiNode='" + inode + "'");
	setfilename = _T("update filename set filename='" + name + "' where fileiNode='" + inode + "'");
	filetest = m_AdoConn.GetRecordSet(selecttest);  //save result to m_pRecordset
	if (filetest->adoEOF)  //not existing
	{
		;
	}
	else{
		m_AdoConn.ExecuteSQL(setfilename);
	}
	m_AdoConn.ExitConnect();
}

void CDBManager::setfoldername(CString inode, CString name){
	ADOConn m_AdoConn;
	m_AdoConn.OnInitADOConn();
	//set SELECT
	_bstr_t setfoldername, selecttest;
	//CString sql;
	//excute SELECT
	_RecordsetPtr filetest;
	selecttest = _T("select * from packetname where packetiNode='" + inode + "'");
	setfoldername = _T("update packetname set packetname='" + name + "' where packetiNode='" + inode + "'");
	filetest = m_AdoConn.GetRecordSet(selecttest);  //save result to m_pRecordset
	if (filetest->adoEOF)  //not existing
	{
		;
	}
	else{
		m_AdoConn.ExecuteSQL(setfoldername);
	}
	m_AdoConn.ExitConnect();
}

void CDBManager::setconnect(CString pinode, CString inode){
	ADOConn m_AdoConn;
	m_AdoConn.OnInitADOConn();
	//set SELECT
	_bstr_t setconnect, selecttest;
	//CString sql;
	//excute SELECT
	_RecordsetPtr filetest;
	selecttest = _T("select * from iNodeconnection where iNode='" + inode + "'");
	setconnect = _T("update iNodeconnection set piNode='" + pinode + "' where iNode='" + inode + "'");
	filetest = m_AdoConn.GetRecordSet(selecttest);  //save result to m_pRecordset
	if (filetest->adoEOF)  //not existing
	{
		;
	}
	else{
		m_AdoConn.ExecuteSQL(setconnect);
	}
	m_AdoConn.ExitConnect();
}

CString CDBManager::findPindoe(CString inode)
{
	//connect
	CString pinodeFind;
	ADOConn m_AdoConn;
	m_AdoConn.OnInitADOConn();
	//set SELECT
	_bstr_t getpinode;
	CString sql;
	//excute SELECT
	_RecordsetPtr m_pRecordPinode;
	getpinode = _T("select * from iNodeconnection where iNode='" + inode + "'");
	m_pRecordPinode = m_AdoConn.GetRecordSet(getpinode);
	if (m_pRecordPinode->adoEOF)  //not existing
	{
		pinodeFind = inode;
	}
	else
	{
		pinodeFind = m_pRecordPinode->GetCollect("piNode"); //read a row from m_pRecordset
	}
	return pinodeFind;
}

CString CDBManager::findfilename(CString inode)
{
	CString filename;
	ADOConn m_AdoConn;
	m_AdoConn.OnInitADOConn();
	//set SELECT
	_bstr_t getname;
	CString sql;
	//excute SELECT
	_RecordsetPtr m_pRecordName;
	getname = _T("select * from filename where fileiNode='" + inode + "'");
	m_pRecordName = m_AdoConn.GetRecordSet(getname);
	if (m_pRecordName->adoEOF)  //not existing
	{
		filename = inode;
	}
	else
	{
		filename = m_pRecordName->GetCollect("filename"); //read a row from m_pRecordset
	}
	return filename;
}