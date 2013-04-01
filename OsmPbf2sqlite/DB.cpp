#include "stdafx.h"
#include "DB.h"

CDB::CDB(void)
{
}

CDB::~CDB(void)
{
}
void CDB::Init()
{
    boost::filesystem::remove(GetFileNameOut().c_str());
	//::DeleteFileW(GetFileNameOut().c_str());
	 int hr=m_db.Open(  GetFileNameOut().c_str(),  /*SQLITE_OPEN_FULLMUTEX|*/SQLITE_OPEN_NOMUTEX|SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE  );
	 if(hr!=SQLITE_OK)
		err(m_db.errmsg());
	 m_db.Execute("BEGIN EXCLUSIVE TRANSACTION;");
	 CreateTables();
	// m_db.Execute("END TRANSACTION;");

	// m_db.Execute("BEGIN EXLUSIVE TRANSACTION;");
	 PrepareTables();

}
void CDB::Close()
{

	 m_dicUser.Save();
	 m_tkvNode.SaveText();
	 m_tkvWay.SaveText();
	 m_tkvRelation.SaveText();
	 m_dicRelationRole.Save();
	 m_tabNode.Close();
	 m_db.Execute("END TRANSACTION;");


	 m_tabNode.Close();
	 m_tabNodeInfo.Close();
	//CSQLite3Table m_tabUser;

	 m_tabNode_NotVisible.Close();
	 m_tabNodeInfo_NotVisible.Close();

	 m_tabWay.Close();
	 m_tabWayInfo.Close();
	 m_tabWay_NotVisible.Close();
	 m_tabWayInfo_NotVisible.Close();

	 m_tabRelation.Close();
	 m_tabRelationInfo.Close();
	 m_tabRelation_NotVisible.Close();
	 m_tabRelationInfo_NotVisible.Close();
	 m_dicRelationRole.m_tab.Close();

	 m_dicUser.m_tab.Close();

	m_tkvNode.Close();
	m_tkvWay.Close();
	m_tkvRelation.Close();


	 m_db.Close();

}
std::wstring CDB::GetFileNameOut()const
{
	std::wstring sOut;
	sOut.assign(m_pSource,_tcsrchr(  m_pSource,_T('.')));


	return sOut+L".db3";
}

void  CDB::CreateTables()
{
	int ret;
	/*
	ret=m_db.Execute("CREATE TABLE IF NOT EXISTS text ( "
    "id   INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,"
    "text TEXT    NOT NULL "
	");");
	if(ret!=SQLITE_OK)
		err(m_db.errmsg());
*/
	ret=m_db.Execute("CREATE TABLE IF NOT EXISTS Node ( "
    "id   INTEGER PRIMARY KEY NOT NULL UNIQUE,"
    "lat  INTEGER NOT NULL,"
    "lon  INTEGER NOT NULL "
	");");
	if(ret!=SQLITE_OK)
		err(m_db.errmsg());

	ret=m_db.Execute("CREATE TABLE IF NOT EXISTS Node_NotVisible ( "
    "id   INTEGER PRIMARY KEY NOT NULL UNIQUE,"
    "lat  INTEGER NOT NULL,"
    "lon  INTEGER NOT NULL "
	");");
	if(ret!=SQLITE_OK)
		err(m_db.errmsg());


	ret=m_db.Execute("CREATE TABLE IF NOT EXISTS NodeInfo ( "
    "idNode   INTEGER PRIMARY KEY NOT NULL UNIQUE,"
    "timestamp  INTEGER,"
    "idUser  INTEGER,"
    "changeset  INTEGER "
	");");
	if(ret!=SQLITE_OK)
		err(m_db.errmsg());

	ret=m_db.Execute("CREATE TABLE IF NOT EXISTS NodeInfo_NotVisible ( "
    "idNode   INTEGER PRIMARY KEY NOT NULL UNIQUE,"
    "timestamp  INTEGER,"
    "idUser  INTEGER,"
    "changeset  INTEGER "
	");");
	if(ret!=SQLITE_OK)
		err(m_db.errmsg());

	ret=m_db.Execute("CREATE TABLE IF NOT EXISTS User ( "
    "id   INTEGER PRIMARY KEY NOT NULL UNIQUE,"
    "Name  TEXT    NOT NULL"
	");");
	if(ret!=SQLITE_OK)
		err(m_db.errmsg());

	m_tkvNode.Init(&m_db,"Node");
	m_tkvWay.Init(&m_db,"Way");
	m_tkvRelation.Init(&m_db,"Relation");

	ret=m_db.Execute("CREATE TABLE IF NOT EXISTS Way ( "
		"id   INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,"
		"idWay  INTEGER    NOT NULL,"
		"idNode INTEGER    NOT NULL,"
		"Position  INTEGER    NOT NULL"
		");");

	if(ret!=SQLITE_OK)
		err(m_db.errmsg());

	ret=m_db.Execute("CREATE TABLE IF NOT EXISTS Way_NotVisible ( "
		"id   INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,"
		"idWay  INTEGER    NOT NULL,"
		"idNode INTEGER    NOT NULL,"
		"Position  INTEGER    NOT NULL"
		");");
	if(ret!=SQLITE_OK)
		err(m_db.errmsg());

	ret=m_db.Execute("CREATE TABLE IF NOT EXISTS WayInfo ( "
		"idWay   INTEGER PRIMARY KEY NOT NULL UNIQUE,"
		"timestamp  INTEGER,"
		"idUser  INTEGER,"
		"changeset  INTEGER "
		");");
	if(ret!=SQLITE_OK)
		err(m_db.errmsg());

	ret=m_db.Execute("CREATE TABLE IF NOT EXISTS WayInfo_NotVisible ( "
		"idWay   INTEGER PRIMARY KEY NOT NULL UNIQUE,"
		"timestamp  INTEGER,"
		"idUser  INTEGER,"
		"changeset  INTEGER "
		");");

	if(ret!=SQLITE_OK)
		err(m_db.errmsg());


	ret=m_db.Execute("CREATE TABLE IF NOT EXISTS Relation ( "
		"id   INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,"
		"idRelation  INTEGER    NOT NULL,"
		"idRef  INTEGER    NOT NULL,"
		"Type  INTEGER    NOT NULL,"
		"idRole  INTEGER    NOT NULL"
		");");
	if(ret!=SQLITE_OK)
		err(m_db.errmsg());

	ret=m_db.Execute("CREATE TABLE IF NOT EXISTS RelationInfo ( "
		"idRelation   INTEGER PRIMARY KEY NOT NULL UNIQUE,"
		"timestamp  INTEGER,"
		"idUser  INTEGER,"
		"changeset  INTEGER "
		");");

	if(ret!=SQLITE_OK)
		err(m_db.errmsg());

	ret=m_db.Execute("CREATE TABLE IF NOT EXISTS Relation_NotVisible ( "
		"id   INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL UNIQUE,"
		"idRelation  INTEGER    NOT NULL,"
		"idRef  INTEGER    NOT NULL,"
		"Type  INTEGER    NOT NULL,"
		"idRole  INTEGER    NOT NULL"
		");");
	if(ret!=SQLITE_OK)
		err(m_db.errmsg());

	ret=m_db.Execute("CREATE TABLE IF NOT EXISTS RelationInfo_NotVisible ( "
		"idRelation   INTEGER PRIMARY KEY NOT NULL UNIQUE,"
		"timestamp  INTEGER,"
		"idUser  INTEGER,"
		"changeset  INTEGER "
		");");

	if(ret!=SQLITE_OK)
		err(m_db.errmsg());

	ret=m_db.Execute("CREATE TABLE IF NOT EXISTS RelationRole ( "
		"id   INTEGER PRIMARY KEY NOT NULL UNIQUE,"
		"Name  TEXT  NOT NULL"
		");");

	if(ret!=SQLITE_OK)
		err(m_db.errmsg());

}
void  CDB::PrepareTables()
{

	 m_tabNode.m_pDB=&m_db;
	 m_tabNode.prepare(
		 "INSERT INTO Node (id,lat,lon) VALUES (?,?,?);",-1);

	 m_tabNode_NotVisible.m_pDB=&m_db;
	 m_tabNode_NotVisible.prepare(
		 "INSERT INTO Node_NotVisible (id,lat,lon) VALUES (?,?,?);",-1);

	 m_tabNodeInfo.m_pDB=&m_db;
	 m_tabNodeInfo.prepare(
		 "INSERT INTO NodeInfo (idNode,timestamp,idUser,changeset) VALUES (?,?,?,?);",-1);

	 m_tabNodeInfo_NotVisible.m_pDB=&m_db;
	 m_tabNodeInfo_NotVisible.prepare(
		 "INSERT INTO NodeInfo_NotVisible (idNode,timestamp,idUser,changeset) VALUES (?,?,?,?);",-1);

	 //m_tabUser.m_pDB=&m_db;
	// m_tabUser.prepare(
		 //"INSERT INTO User (Name) VALUES (?);",-1);

	 m_dicUser.Init(&m_db);


	 m_tabWay.m_pDB=&m_db;
	 m_tabWay.prepare(
		 "INSERT INTO Way (idWay,idNode,Position) VALUES (?,?,?);",-1);

	 m_tabWay_NotVisible.m_pDB=&m_db;
	 m_tabWay_NotVisible.prepare(
		 "INSERT INTO Way_NotVisible (idWay,idNode,Position) VALUES (?,?,?);",-1);

	 m_tabWayInfo.m_pDB=&m_db;
	 m_tabWayInfo.prepare(
		 "INSERT INTO WayInfo (idWay,timestamp,idUser,changeset) VALUES (?,?,?,?);",-1);

	 m_tabWayInfo_NotVisible.m_pDB=&m_db;
	 m_tabWayInfo_NotVisible.prepare(
		 "INSERT INTO WayInfo_NotVisible (idWay,timestamp,idUser,changeset) VALUES (?,?,?,?);",-1);


	 m_tabRelation.m_pDB=&m_db;
	 m_tabRelation.prepare(
		 "INSERT INTO Relation (idRelation,idRef,Type,idRole) VALUES (?,?,?,?);",-1);

	 m_tabRelation_NotVisible.m_pDB=&m_db;
	 m_tabRelation_NotVisible.prepare(
		 "INSERT INTO Relation_NotVisible (idRelation,idRef,Type,idRole) VALUES (?,?,?,?);",-1);

	 m_tabRelationInfo.m_pDB=&m_db;
	 m_tabRelationInfo.prepare(
		 "INSERT INTO RelationInfo (idRelation,timestamp,idUser,changeset) VALUES (?,?,?,?);",-1);

	 m_tabRelationInfo_NotVisible.m_pDB=&m_db;
	 m_tabRelationInfo_NotVisible.prepare(
		 "INSERT INTO RelationInfo_NotVisible (idRelation,timestamp,idUser,changeset) VALUES (?,?,?,?);",-1);


	 m_dicRelationRole.m_tab.m_pDB=&m_db;
	 m_dicRelationRole.m_tab.prepare(
		 "INSERT INTO RelationRole (id,Name) VALUES (?,?);",-1);

}
