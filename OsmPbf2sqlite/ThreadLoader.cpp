#include "stdafx.h"
#include "DLIB/SQLite3Table.h"
#include "DLIB/Stopwatch.h"

#include "ThreadLoader.h"

ATOMIC_INT CThreadLoader::m_nCount;

CThreadLoader::CThreadLoader(void)

{

}

CThreadLoader::~CThreadLoader(void)
{
	//::DeleteFile(GetFileNameOut().c_str());
}
void CThreadLoader::Start(CThreadUnit** pTasks, int countTasks)
{

	 if(m_nCount >0)
	 {
		 assert(false);
	 }

	 boost::timer::nanosecond_type const secunda(1000000000LL);
	 boost::timer::cpu_timer uTimer;
	 //unsigned uTime=0;
	 if(m_nThredNumber==0)
	 {
		 uTimer.start();

	 }


	m_pprimblock=new OSMPBF::PrimitiveBlock;

	 std::vector<char> buffer_blob_header;
	 std::vector<char>  buffer(OSMPBF::max_uncompressed_blob_size);
	 std::vector<unsigned char> unpack_buffer(OSMPBF::max_uncompressed_blob_size);

	 const char* pbWork;

	 for(;;)
	 {
		 // storage of size, used multiple times
		 __int32 sz;
		 OSMPBF::BlobHeader blobheader;

		 {
			 //Начало паралельной работы
			boost::lock_guard<boost::mutex> l(*m_pcs);
			if(feof(m_fp))
				break;


			 // read the first 4 bytes of the file, this is the size of the blob-header
			 if(fread(&sz, sizeof(sz), 1, m_fp) != 1)
				 break; // end of file reached

			 // convert the size from network byte-order to host byte-order
			 sz = ntohl(sz);

			 // ensure the blob-header is smaller then MAX_BLOB_HEADER_SIZE
			 if(sz > OSMPBF::max_blob_header_size)
				 err("blob-header-size is bigger then allowed (%u > %u)", sz, OSMPBF::max_blob_header_size);

			 buffer_blob_header.resize(sz);
			 // read the blob-header from the file
			 if(fread(&buffer_blob_header[0], sz, 1, m_fp) != 1)
				 err("unable to read blob-header from file");


			 // parse the blob-header from the read-buffer
			 if(!blobheader.ParseFromArray(&buffer_blob_header[0], sz))
				 err("unable to parse blob header");

			 // size of the following blob
			 sz = blobheader.m_datasize.m_val;

			 // ensure the blob is smaller then MAX_BLOB_SIZE
			 if(sz > OSMPBF::max_uncompressed_blob_size)
				 err("blob-size is bigger then allowed (%u > %u)", sz, OSMPBF::max_uncompressed_blob_size);

			 // read the blob from the file
			 if(fread(&buffer[0], sz, 1, m_fp) != 1)
				 err("unable to read blob from file");

			 //Отсюда можно работать паралельно

		 }
		// ++m_nCount; continue;


		 // parse the blob from the read-buffer
		 OSMPBF::Blob blob;
		 if(!blob.ParseFromArray(&buffer[0], sz))
			 err("unable to parse blob");

		 // set when we find at least one data stream
		 bool found_data = false;

		 // if the blob has uncompressed data
		 if(!blob.m_raw.empty()) {
			 // we have at least one datastream
			 found_data = true;

			 // size of the blob-data
			 sz = blob.m_raw.size();

			 // check that raw_size is set correctly
			 if(sz != blob.m_raw_size.m_val)
				 warn("  reports wrong raw_size: %u bytes", blob.m_raw_size.m_val);

			 // copy the uncompressed data over to the unpack_buffer
			 //memcpy(&unpack_buffer[0], &buffer[0], sz);

			 pbWork=&buffer[0];
		 }

		 // if the blob has zlib-compressed data
		 if(!blob.m_zlib_data.empty())
		 {
			 // issue a warning if there is more than one data steam, a blob may only contain one data stream
			 if(found_data)
				 warn("  contains several data streams");

			 // we have at least one datastream
			 found_data = true;

			 // the size of the compressesd data
			 sz = blob.m_zlib_data.size();

			 // zlib information
			 z_stream z;

			 // next byte to decompress
			 z.next_in   = (z_const Bytef *) blob.m_zlib_data.m_pBegin;

			 // number of bytes to decompress
			 z.avail_in  = sz;

			 // place of next decompressed byte
			  z.next_out  =  &unpack_buffer[0];
			  pbWork= (const char*) &unpack_buffer[0];


			 // space for decompressed data
			 z.avail_out = blob.m_raw_size.m_val;

			 // misc
			 z.zalloc    = Z_NULL;
			 z.zfree     = Z_NULL;
			 z.opaque    = Z_NULL;

			 if(inflateInit(&z) != Z_OK) {
				 err("  failed to init zlib stream");
			 }
			 if(inflate(&z, Z_FINISH) != Z_STREAM_END) {
				 err("  failed to inflate zlib stream");
			 }
			 if(inflateEnd(&z) != Z_OK) {
				 err("  failed to deinit zlib stream");
			 }

			 // unpacked size
			 sz = z.total_out;

		 }

		 // if the blob has lzma-compressed data
		 if(!blob.m_lzma_data.empty()) {
			 // issue a warning if there is more than one data steam, a blob may only contain one data stream
			 if(found_data)
				 warn("  contains several data streams");

			 // we have at least one datastream
			 found_data = true;

			 // issue a warning, lzma compression is not yet supported
			 err("  lzma-decompression is not supported");
		 }
		 // check we have at least one data-stream
		 if(!found_data)
			 err("  does not contain any known data stream");


		 // switch between different blob-types
		 if(blobheader.m_type.compareString("OSMHeader")==0)
		 {

			 OSMPBF::Headerblock headerblock ;
			 // parse the HeaderBlock from the blob
			 if(!headerblock.ParseFromArray(pbWork, sz))
				 err("unable to parse header block");

			 // tell about the bbox
		 /*
		 if(headerblock.m_bbox.IsInitialized()) {
				 OSMPBF::HeaderBBox bbox = headerblock.m_bbox;

				 debug("    bbox: %.7f,%.7f,%.7f,%.7f",
					 (double)bbox.m_left.m_val / OSMPBF::lonlat_resolution,
					 (double)bbox.m_bottom.m_val / OSMPBF::lonlat_resolution,
					 (double)bbox.m_right.m_val / OSMPBF::lonlat_resolution,
					 (double)bbox.m_top.m_val / OSMPBF::lonlat_resolution);
			 }

			 // tell about the required features
			 for(int i = 0, l = headerblock.m_required_features.size(); i < l; i++)
				 debug("    required_feature: %s", headerblock.m_required_features[i].toString().c_str());

			 // tell about the optional features
			 for(int i = 0, l = headerblock.m_optional_features.size(); i < l; i++)
				 debug("    required_feature: %s", headerblock.m_optional_features[i].toString().c_str());

			 // tell about the writing program
			 if(!headerblock.m_writingprogram.empty())
				 debug("    writingprogram: %s", headerblock.m_writingprogram.toString().c_str());

			 // tell about the source
			 if(!headerblock.m_source.empty())
				 debug("    source: %s", headerblock.m_source.toString().c_str());
		 */
		 }
		 else if(blobheader.m_type.compareString("OSMData")==0)
		 {

			 m_pprimblock->Clear();



			 // parse the PrimitiveBlock from the blob
			 if(!m_pprimblock->ParseFromArray(pbWork, sz))
				 err("unable to parse primitive block");
/*

			 // tell about the block's meta info
			 debug("    granularity: %u", primblock.m_granularity.m_val);
			 debug("    lat_offset: %u", primblock.m_lat_offset.m_val);
			 debug("    lon_offset: %u", primblock.m_lon_offset.m_val);
			 debug("    date_granularity: %u", primblock.m_date_granularity.m_val);


			 // tell about the stringtable
			 debug("    stringtable: %u items", primblock.m_stringtable.m_s.size());
			 debug("    stringtable:last %s", primblock.m_stringtable.m_s.back().toString().c_str());
			 // number of PrimitiveGroups
			 debug("    primitivegroups: %u groups", primblock.m_primitivegroup.size());
*/
			 // iterate over all PrimitiveGroups
			 for(int i = 0, l = m_pprimblock->m_primitivegroup.size(); i < l; i++)
			 {
				 // one PrimitiveGroup from the the Block
				 OSMPBF::PrimitiveGroup& pg = m_pprimblock->m_primitivegroup[i];

				 bool found_items=false;

				 // tell about nodes
				 if(pg.m_nodes.size() > 0)
				 {
					//CComCritSecLock<CComAutoCriticalSection> l(m_pDB->m_cs);
					 found_items = true;
					 for(size_t i=0;i<pg.m_nodes.size();++i)
						AddNode(pg.m_nodes[i]);

					 /*
					 debug("      nodes: %d", pg.m_nodes.size());
					 if(!pg.m_nodes[0].m_info.empty())
						 debug("        with meta-info");
						 */
				 }

				 // tell about dense nodes
				 if(!pg.m_dense.empty()) {
					 found_items = true;
					 AddDense(pg.m_dense);
/*
					 debug("      dense nodes: %d", pg.m_dense.m_id.size());
					 if(!pg.m_dense.m_denseinfo.empty())
						 debug("        with meta-info");
						 */
				 }

				 // tell about ways
				 if(pg.m_ways.size() > 0) {
					 found_items = true;
 					 //CComCritSecLock<CComAutoCriticalSection> l(m_pDB->m_cs);

					 for(size_t i=0;i<pg.m_ways.size();++i)
						AddWay(pg.m_ways[i]);
/*
					 debug("      ways: %d", pg.m_ways.size());
					 if(!pg.m_ways[0].m_info.empty())
						 debug("        with meta-info");
						 */
				 }

				 // tell about relations
				 if(pg.m_relations.size() > 0) {
				 //CComCritSecLock<CComAutoCriticalSection> l(m_pDB->m_cs);
					found_items = true;

					for(size_t i=0;i<pg.m_relations.size();++i)
						AddRelations(pg.m_relations[i]);

					 /*
					 debug("      relations: %d", pg.m_relations.size());
					 if(pg.m_relations[0].IsInitialized())
						 debug("        with meta-info");
						 */
				 }

				 if(!found_items)
					 warn("      contains no items");

			 }
		 }

		 else {
			 // unknown blob type
			 warn("  unknown blob type: %s", blobheader.m_type.toString().c_str());
		 }

		 //////////////////////////////////////////////////////////////////////////
		 LONG l =INTERLOCKED_INCREMENT(m_nCount);
		 if(m_nThredNumber==0)
		 {
			 boost::timer::cpu_times const elapsed_times(uTimer.elapsed());
		     boost::timer::nanosecond_type elapsed(elapsed_times.system
				 + elapsed_times.user);
			 if(elapsed >= secunda)
			 {
				 info("Bloks=%d",l);
				 //uTimer.stop();
				 uTimer.start();
			 }
		 }
	 }
	//sqlite3_close(pDb);

	 delete m_pprimblock;
	 m_pprimblock=NULL;




}
void CThreadLoader::AddNode(const OSMPBF::Node& n)
{


	int ret;
	CSQLite3Table *pTabN, *pTabNI;
	if(n.m_info.m_visible.m_val)
	{
		pTabN=&m_pDB->m_tabNode;
		pTabNI=&m_pDB->m_tabNodeInfo;
	}
	else
	{
		pTabN=&m_pDB->m_tabNode_NotVisible;
		pTabNI=&m_pDB->m_tabNodeInfo_NotVisible;
	}

	CtabKeyValue::SarInts arInts;
	m_pDB->m_tkvNode.Prepare(m_pprimblock->m_stringtable,
		n.m_keys,n.m_vals,&arInts);

	const PBFRO::FBytes &s=m_pprimblock->m_stringtable.m_s[n.m_info.m_user_sid.m_val];
	int nUser=m_pDB->m_dicUser.GetID(n.m_info.m_uid.m_val, s.m_pBegin,s.size());

	boost::lock_guard<boost::mutex> l(m_pDB->m_cs);


	//Таблица Node
	{
    	// CComCritSecLock<CComAutoCriticalSection> l(pTabN->m_cs);
		ret = pTabN->reset();
		if(ret!=SQLITE_OK)
			err(m_pDB->m_db.errmsg());
		ret = pTabN->bind_int64(1,n.m_id.m_val);
		if(ret!=SQLITE_OK)
			err(m_pDB->m_db.errmsg());
		pTabN->bind_int64(2,n.m_lat.m_val);
		pTabN->bind_int64(3,n.m_lon.m_val);
		ret = pTabN->step();
		if(ret!= SQLITE_DONE)
			err(m_pDB->m_db.errmsg());
	}

	//Таблица NodeInfo
	{
    	//CComCritSecLock<CComAutoCriticalSection> l(pTabNI->m_cs);
		ret = pTabNI->reset();
		if(ret!=SQLITE_OK)
			err(m_pDB->m_db.errmsg());
		ret = pTabNI->bind_int64(1,n.m_id.m_val);
		if(ret!=SQLITE_OK)
			err(m_pDB->m_db.errmsg());


		pTabNI->bind_int64(2,n.m_info.m_timestamp.m_val);


		pTabNI->bind_int(3,nUser);
		pTabNI->bind_int64(4,n.m_info.m_changeset.m_val);
		ret = pTabNI->step();

	}

	//m_dicUser.GetID(n.m_info.m_uid.m_val, s.m_pBegin,s.size());
	if(ret!= SQLITE_DONE)
		err(m_pDB->m_db.errmsg());

	m_pDB->m_tkvNode.SaveId(n.m_id.m_val,arInts);
	/*

	{
    	//CComCritSecLock<CComAutoCriticalSection> l(m_pDB->m_tkvNode.m_tabKeyVal.m_cs);
		for(unsigned i=0;i<n.m_keys.size();++i)
		{
			m_pDB->m_tkvNode.Add(n.m_id.m_val,&m_pprimblock->m_stringtable.m_s[n.m_keys[i].m_val],&m_pprimblock->m_stringtable.m_s[n.m_vals[i].m_val]);
		}
	}
	*/


}
void CThreadLoader::AddDense(const OSMPBF::DenseNodes& d)
{
	size_t lastkv=0;
	__int64 lastid=0;
	__int64 lastlat=0;
	__int64 lastlon=0;
	__int64 lasttimestamp=0;
	__int64 lastchangeset=0;
	int lastuid=0;
	unsigned lastuser_sid=0;

	//CStopwatch st(true);


	for(size_t i=0;i<d.m_id.size();++i)
	{
		OSMPBF::Node node;
		node.SetMemory(&m_pprimblock->m_Memory);
		lastid=node.m_id.m_val=d.m_id[i].m_val+lastid;
		lastlat=node.m_lat.m_val=d.m_lat[i].m_val+lastlat;
		lastlon=node.m_lon.m_val=d.m_lon[i].m_val+lastlon;

		node.m_info.m_version=d.m_denseinfo.m_version[i];
		lasttimestamp=node.m_info.m_timestamp.m_val=d.m_denseinfo.m_timestamp[i].m_val+lasttimestamp;
		lastchangeset=node.m_info.m_changeset.m_val=d.m_denseinfo.m_changeset[i].m_val+lastchangeset;
		lastuid=node.m_info.m_uid.m_val=d.m_denseinfo.m_uid[i].m_val+lastuid;
		lastuser_sid=node.m_info.m_user_sid.m_val=d.m_denseinfo.m_user_sid[i].m_val+lastuser_sid;
		if(i<d.m_denseinfo.m_visible.size())
			node.m_info.m_visible=d.m_denseinfo.m_visible[i];
		else
			node.m_info.m_visible.m_val=true;

		if (lastkv<d.m_keys_vals.size())
		{

			while(d.m_keys_vals[lastkv].m_val)
			{
				PBFRO::Fuint32 u;
				u.m_val=d.m_keys_vals[lastkv++].m_val;
				node.m_keys.push_back(u);
				u.m_val=d.m_keys_vals[lastkv++].m_val;
				node.m_vals.push_back(u);
			}
			++lastkv;
		}



		AddNode(node);
/*
		if(i==100)
		{
			st.Stop("10add=");
			int a=0;
			a++;
		}
*/
	}
}
void CThreadLoader::AddWay(const OSMPBF::Way& w)
{
	const PBFRO::FBytes &s=m_pprimblock->m_stringtable.m_s[w.m_info.m_user_sid.m_val];
	int nUser=m_pDB->m_dicUser.GetID(w.m_info.m_uid.m_val, s.m_pBegin,s.size());

	int ret;
	CSQLite3Table *pTabW, *pTabWI;
	if(w.m_info.m_visible.m_val)
	{
		pTabW=&m_pDB->m_tabWay;
		pTabWI=&m_pDB->m_tabWayInfo;
	}
	else
	{
		pTabW=&m_pDB->m_tabWay_NotVisible;
		pTabWI=&m_pDB->m_tabWayInfo_NotVisible;
	}

	CtabKeyValue::SarInts arInts;
	m_pDB->m_tkvNode.Prepare(m_pprimblock->m_stringtable,
		w.m_keys,w.m_vals,&arInts);

	boost::lock_guard<boost::mutex> l(m_pDB->m_cs);

	//Таблица Way
	{
		//CComCritSecLock<CComAutoCriticalSection> l(pTabW->m_cs);
		__int64 idw=0;
		for(unsigned i=0;i<w.m_refs.size();++i)
		{
			ret = pTabW->reset();
			ret = pTabW->bind_int64(1,w.m_id.m_val);
			idw+=w.m_refs[i].m_val;
			pTabW->bind_int64(2,idw);
			pTabW->bind_int(3,i);
			ret = pTabW->step();
		}
	}



	//Таблица NodeInfo

	{
		//CComCritSecLock<CComAutoCriticalSection> l(pTabWI->m_cs);
		ret = pTabWI->reset();
		if(ret!=SQLITE_OK)
			err(m_pDB->m_db.errmsg());
		ret = pTabWI->bind_int64(1,w.m_id.m_val);
		if(ret!=SQLITE_OK)
			err(m_pDB->m_db.errmsg());
		pTabWI->bind_int64(2,w.m_info.m_timestamp.m_val);
		pTabWI->bind_int(3,nUser);
		pTabWI->bind_int64(4,w.m_info.m_changeset.m_val);
		ret = pTabWI->step();
	}


	//m_dicUser.GetID(n.m_info.m_uid.m_val, s.m_pBegin,s.size());
	if(ret!= SQLITE_DONE)
		err(m_pDB->m_db.errmsg());

	m_pDB->m_tkvWay.SaveId(w.m_id.m_val,arInts);
/*
	{
		//CComCritSecLock<CComAutoCriticalSection> l(m_pDB->m_tkvWay.m_tabKeyVal.m_cs);
		for(unsigned i=0;i<w.m_keys.size();++i)
		{
			m_pDB->m_tkvWay.Add(w.m_id.m_val,&m_pprimblock->m_stringtable.m_s[w.m_keys[i].m_val],
				&m_pprimblock->m_stringtable.m_s[w.m_vals[i].m_val]);
		}
	}
	*/
}
void CThreadLoader::AddRelations(const OSMPBF::Relation& r)
{
	const PBFRO::FBytes &s=m_pprimblock->m_stringtable.m_s[r.m_info.m_user_sid.m_val];
	int UserId=m_pDB->m_dicUser.GetID(r.m_info.m_uid.m_val, s.m_pBegin,s.size());

	int ret;
	CSQLite3Table *pTabR, *pTabRI;
	if(r.m_info.m_visible.m_val)
	{
		pTabR=&m_pDB->m_tabRelation;
		pTabRI=&m_pDB->m_tabRelationInfo;
	}
	else
	{
		pTabR=&m_pDB->m_tabRelation_NotVisible;
		pTabRI=&m_pDB->m_tabRelationInfo_NotVisible;
	}

	CtabKeyValue::SarInts arInts;
	m_pDB->m_tkvNode.Prepare(m_pprimblock->m_stringtable,
		r.m_keys,r.m_vals,&arInts);

	boost::lock_guard<boost::mutex> l(m_pDB->m_cs);

	//Таблица Way
	{
		//CComCritSecLock<CComAutoCriticalSection> l(pTabR->m_cs);
		__int64 idw=0;
		for(unsigned i=0;i<r.m_memids.size();++i)
		{
			pTabR->reset();
			pTabR->bind_int64(1,r.m_id.m_val);
			idw+=r.m_memids[i].m_val;
			pTabR->bind_int64(2,idw);
			pTabR->bind_int(3,r.m_types_N[i].m_val);
			pTabR->bind_int(4,m_pDB->m_dicRelationRole.GetIdText(&m_pprimblock->m_stringtable.m_s[r.m_roles_sid[i].m_val]));
			pTabR->step();
		}
	}



	//Таблица NodeInfo
	{
		//CComCritSecLock<CComAutoCriticalSection> l(pTabRI->m_cs);

		ret = pTabRI->reset();
		if(ret!=SQLITE_OK)
			err(m_pDB->m_db.errmsg());
		ret = pTabRI->bind_int64(1,r.m_id.m_val);
		if(ret!=SQLITE_OK)
			err(m_pDB->m_db.errmsg());


		pTabRI->bind_int64(2,r.m_info.m_timestamp.m_val);


		pTabRI->bind_int(3,UserId);
		pTabRI->bind_int64(4,r.m_info.m_changeset.m_val);



		ret = pTabRI->step();
	}


	//m_dicUser.GetID(n.m_info.m_uid.m_val, s.m_pBegin,s.size());
	if(ret!= SQLITE_DONE)
		err(m_pDB->m_db.errmsg());


	m_pDB->m_tkvRelation.SaveId(r.m_id.m_val,arInts);
/*
	{
		//CComCritSecLock<CComAutoCriticalSection> l(m_pDB->m_tkvRelation.m_tabKeyVal.m_cs);

		for(unsigned i=0;i<r.m_keys.size();++i)
		{
			m_pDB->m_tkvRelation.Add(r.m_id.m_val,&m_pprimblock->m_stringtable.m_s[r.m_keys[i].m_val],
				&m_pprimblock->m_stringtable.m_s[r.m_vals[i].m_val]);
		}
	}
	*/

}
