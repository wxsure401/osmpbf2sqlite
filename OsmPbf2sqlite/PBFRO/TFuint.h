#pragma once

namespace PBFRO {

template <class T>
class TFuint : public CField
{
public:
	TFuint(const T &t)
		:m_val(t.m_val)
		,CField(t)

	{

	}

	TFuint(void)
		:m_val(0)
	{

	}

	virtual ~TFuint(void)
	{

	}

	
	T m_val;

	/// Читаем из потока
	virtual bool Read(CInputStream *pis)
	{
		return pis->ReadVariant(m_val); 
	}
	virtual void Clear()
	{
		CField::Clear();
		m_val=0;
	}

	/*
class uintF (Aint,Buint) :  Field
{
	static const WireType wireType_= WireType.WIRETYPE_VARINT;

	this( uint field_number )
	{
		super(field_number);
		def_=val_.init;

	}
	
	this( uint field_number, Aint def )
	{
		super(field_number,true);
		def_=def;

	}

	override @safe nothrow void Clear()
	{
		super.Clear();
		val_=def_;

	}

	uintF opAssign2(in uintF a)
	{
		if(a.Has)
			Val(a.val_);
		else
			Clear();
		return this;

	}


	@safe @property pure nothrow final Aint Val()const
	{
		return val_;
	}

	@safe @property  nothrow final void Val(in Aint  val)
	{
		val_=val;
		if(!IsDefault())
			Has=true;
	}

	override bool Read(CodedInputStream input)
	{
		Buint temp;
		if (!input.ReadVarint(temp)) 
			return false;
		Val=temp;
		return true;
	}
	//TODO
	override bool Write (CodedOutputStream output)const
	{ 
		if(!output.WriteVarint( WireFormatLite.MakeTag(field_number_, wireType_)))
			return false;
		Buint v=val_;
		return output.WriteVarint(v);

	}

	override @safe @property uintF dup()const
	{
		uintF m=new uintF(field_number_);
		if(Has)
		{
			m.val_=val_;
		}
		m.def_=def_;
		return m;
	}

	override void CheckTypeAndMergeFrom(in Field from)
	{
		uintF v=cast(uintF) from;
		if(v)
		{
			Clear();
			if(v.Has)
				val_=v.val_;

		}

	}

	override pure nothrow WireType GetWireType()const
	{return wireType_;}

	alias Val this;
private:
	Aint val_;
	Aint def_;

}

	*/
};

typedef TFuint<__int32>  Fint32;
typedef TFuint<__int64>  Fint64;
typedef TFuint<unsigned __int32>  Fuint32;


class Fbool : public CField
{
public:

	Fbool(void)
		:m_val(false)
	{
	}

	virtual ~Fbool(void)
	{
	}
	
	bool m_val;

	/// Читаем из потока
	virtual bool Read(CInputStream *pis)
	{
		unsigned u;
		if(!pis->ReadVariant(u))
			return false;
		m_val=u!=0;
		return true;
	}
	virtual void Clear()
	{
		CField::Clear();
		m_val=false;
	}
};

}//namespace PBFRO 
