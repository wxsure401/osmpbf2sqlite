#pragma once
#include "TFuint.h"

namespace PBFRO {

template <class Ts, class Tu>
class TFsint : public TFuint<Ts>
{
public:

	TFsint(void){}

	/// Читаем из потока
	virtual bool Read(CInputStream *pis)
	{
		Tu u;
		if(!pis->ReadVariant(u))
			return false;
		this->m_val=ZigZagDecode(u);
		return true;
	}


/*
		override bool Read(CodedInputStream input)
	{
		Buint temp;
		if (!input.ReadVarint(temp))
			return false;
		Val=ZigZagDecode(temp);
		return true;
	}

	override bool Write (CodedOutputStream output)const
	{
		if(!output.WriteVarint( WireFormatLite.MakeTag(field_number_, wireType_)))
			return false;
		Buint v=ZigZagEncode(val_);
		return output.WriteVarint(v);
	}
*/
	static Tu ZigZagEncode(Ts n)
	{
		// Note:  the right-shift must be arithmetic
		return (n << 1) ^ (n >> (sizeof(Ts) * 8 - 1));
	}

	static Ts  ZigZagDecode(Tu n) {
		return (n >> 1) ^ - (Ts)(n & 1);
	}

};

typedef TFsint<__int32,unsigned __int32>  Fsint32;
typedef TFsint<__int64,unsigned __int64>  Fsint64;


}//namespace PBFRO
