//
// Created by sh1rm4n on 7/4/26.
//

#ifndef CENTURY_BF_READ_HPP
#define CENTURY_BF_READ_HPP

#define BITBUF_INLINE inline

#include "math.hpp"
#include "typedefs.hpp"

class bf_read
{
public:
	bf_read();

	// nMaxBits can be used as the number of bits in the buffer.
	// It must be <= nBytes*8. If you leave it at -1, then it's set to nBytes * 8.
	bf_read( const void *pData, int nBytes, int nBits = -1 );
	bf_read( const char *pDebugName, const void *pData, int nBytes, int nBits = -1 );

	// Start reading from the specified buffer.
	// pData's start address must be dword-aligned.
	// nMaxBits can be used as the number of bits in the buffer.
	// It must be <= nBytes*8. If you leave it at -1, then it's set to nBytes * 8.
	void			StartReading( const void *pData, int nBytes, int iStartBit = 0, int nBits = -1 );

	// Restart buffer reading.
	void			Reset();

	// Enable or disable assertion on overflow. 99% of the time, it's a bug that we need to catch,
	// but there may be the occasional buffer that is allowed to overflow gracefully.
	void			SetAssertOnOverflow( bool bAssert );

	// This can be set to assign a name that gets output if the buffer overflows.
	const char*		GetDebugName() const { return m_pDebugName; }
	void			SetDebugName( const char *pName );

	void			ExciseBits( int startbit, int bitstoremove );


// Bit functions.
public:

	// Returns 0 or 1.
	int				ReadOneBit();


protected:

	unsigned int	CheckReadUBitLong(int numbits);		// For debugging.
	int				ReadOneBitNoCheck();				// Faster version, doesn't check bounds and is inlined.
	bool			CheckForOverflow(int nBits);


public:

	// Get the base pointer.
	const unsigned char*	GetBasePointer() { return m_pData; }

	BITBUF_INLINE int TotalBytesAvailable( void ) const
	{
		return m_nDataBytes;
	}

	// Read a list of bits in.
	void            ReadBits(void *pOut, int nBits);
	// Read a list of bits in, but don't overrun the destination buffer.
	// Returns the number of bits read into the buffer. The remaining
	// bits are skipped over.
	int             ReadBitsClamped_ptr(void *pOut, size_t outSizeBytes, size_t nBits);
	// Helper 'safe' template function that infers the size of the destination
	// array. This version of the function should be preferred.
	// Usage: char databuffer[100];
	//        ReadBitsClamped( dataBuffer, msg->m_nLength );
	template <typename T, size_t N>
	int             ReadBitsClamped( T (&pOut)[N], size_t nBits )
	{
		return ReadBitsClamped_ptr( pOut, N * sizeof(T), nBits );
	}

	float			ReadBitAngle( int numbits );

	unsigned int	ReadUBitLong( int numbits );
	unsigned int	ReadUBitLongNoInline( int numbits );
	unsigned int	PeekUBitLong( int numbits );
	int				ReadSBitLong( int numbits );

	// reads an unsigned integer with variable bit length
	unsigned int	ReadUBitVar();
	unsigned int	ReadUBitVarInternal( int encodingType );

	// reads a varint encoded integer
	std::uint32_t			ReadVarInt32();
	std::uint64_t			ReadVarInt64();
	std::int32_t			ReadSignedVarInt32();
	std::int64_t			ReadSignedVarInt64();

	// You can read signed or unsigned data with this, just cast to
	// a signed int if necessary.
	unsigned int	ReadBitLong(int numbits, bool bSigned);

	float			ReadBitCoord();
	float			ReadBitCoordMP( bool bIntegral, bool bLowPrecision );
	float			ReadBitFloat();
	float			ReadBitNormal();
	void			ReadBitVec3Coord( Vector& fa );
	void			ReadBitVec3Normal( Vector& fa );
	void			ReadBitAngles( Angle& fa );

	// Faster for comparisons but do not fully decode float values
	unsigned int	ReadBitCoordBits();
	unsigned int	ReadBitCoordMPBits( bool bIntegral, bool bLowPrecision );

// Byte functions (these still read data in bit-by-bit).
public:

	BITBUF_INLINE int	ReadChar() { return (char)ReadUBitLong(8); }
	BITBUF_INLINE int	ReadByte() { return ReadUBitLong(8); }
	BITBUF_INLINE int	ReadShort() { return (short)ReadUBitLong(16); }
	BITBUF_INLINE int	ReadWord() { return ReadUBitLong(16); }
	BITBUF_INLINE std::int32_t ReadLong() { return ReadUBitLong(32); }
	std::int64_t			ReadLongLong();
	float			ReadFloat();
	bool			ReadBytes(void *pOut, int nBytes);

	// Returns false if bufLen isn't large enough to hold the
	// string in the buffer.
	//
	// Always reads to the end of the string (so you can read the
	// next piece of data waiting).
	//
	// If bLine is true, it stops when it reaches a '\n' or a null-terminator.
	//
	// pStr is always null-terminated (unless bufLen is 0).
	//
	// pOutNumChars is set to the number of characters left in pStr when the routine is
	// complete (this will never exceed bufLen-1).
	//
	bool			ReadString( char *pStr, int bufLen, bool bLine=false, int *pOutNumChars=NULL );

	// Reads a string and allocates memory for it. If the string in the buffer
	// is > 2048 bytes, then pOverflow is set to true (if it's not NULL).
	char*			ReadAndAllocateString( bool *pOverflow = 0 );

	// Returns nonzero if any bits differ
	int				CompareBits( bf_read *  other, int bits ) ;
	int				CompareBitsAt( int offset, bf_read *  other, int otherOffset, int bits ) ;

// Status.
public:
	int				GetNumBytesLeft();
	int				GetNumBytesRead();
	int				GetNumBitsLeft() ;
	int				GetNumBitsRead() const;

	// Has the buffer overflowed?
	inline bool		IsOverflowed() const {return m_bOverflow;}

	inline bool		Seek(int iBit);					// Seek to a specific bit.
	inline bool		SeekRelative(int iBitDelta);	// Seek to an offset from the current position.

	// Called when the buffer is overflowed.
	void			SetOverflowFlag() ;


public:

	// The current buffer.
	const unsigned char*  m_pData;
	int						m_nDataBytes;
	int						m_nDataBits;

	// Where we are in the buffer.
	int				m_iCurBit;


private:
	// Errors?
	bool			m_bOverflow;

	// For debugging..
	bool			m_bAssertOnOverflow;

	const char		*m_pDebugName;
};

//-----------------------------------------------------------------------------
// Inlines.
//-----------------------------------------------------------------------------

inline int bf_read::GetNumBytesRead()
{
	return BitByte(m_iCurBit);
}

inline int bf_read::GetNumBitsLeft()
{
	return m_nDataBits - m_iCurBit;
}

inline int bf_read::GetNumBytesLeft()
{
	return GetNumBitsLeft() >> 3;
}

inline int bf_read::GetNumBitsRead() const
{
	return m_iCurBit;
}

inline bool bf_read::Seek(int iBit)
{
	if(iBit < 0 || iBit > m_nDataBits)
	{
		SetOverflowFlag();
		m_iCurBit = m_nDataBits;
		return false;
	}
	else
	{
		m_iCurBit = iBit;
		return true;
	}
}

// Seek to an offset from the current position.
inline bool	bf_read::SeekRelative(int iBitDelta)
{
	return Seek(m_iCurBit+iBitDelta);
}

inline bool bf_read::CheckForOverflow(int nBits)
{
	if( m_iCurBit + nBits > m_nDataBits )
	{
		SetOverflowFlag();
	}

	return m_bOverflow;
}

inline int bf_read::ReadOneBitNoCheck()
{
#if VALVE_LITTLE_ENDIAN
	unsigned int value = ((std::uint32_t * )m_pData)[m_iCurBit >> 5] >> (m_iCurBit & 31);
#else
	unsigned char value = m_pData[m_iCurBit >> 3] >> (m_iCurBit & 7);
#endif
	++m_iCurBit;
	return value & 1;
}

inline int bf_read::ReadOneBit()
{
	if( GetNumBitsLeft() <= 0 )
	{
		SetOverflowFlag();
		return 0;
	}
	return ReadOneBitNoCheck();
}

inline float bf_read::ReadBitFloat()
{
	union { std::uint32_t u; float f; } c = { ReadUBitLong(32) };
	return c.f;
}

BITBUF_INLINE unsigned int bf_read::ReadUBitVar()
{
	// six bits: low 2 bits for encoding + first 4 bits of value
	unsigned int sixbits = ReadUBitLong(6);
	unsigned int encoding = sixbits & 3;
	if ( encoding )
	{
		// this function will seek back four bits and read the full value
		return ReadUBitVarInternal( encoding );
	}
	return sixbits >> 2;
}

BITBUF_INLINE unsigned int bf_read::ReadUBitLong( int numbits )
{
	if ( GetNumBitsLeft() < numbits )
	{
		m_iCurBit = m_nDataBits;
		SetOverflowFlag();
		return 0;
	}

	unsigned int iStartBit = m_iCurBit & 31u;
	int iLastBit = m_iCurBit + numbits - 1;
	unsigned int iWordOffset1 = m_iCurBit >> 5;
	unsigned int iWordOffset2 = iLastBit >> 5;
	m_iCurBit += numbits;

#if __i386__
	unsigned int bitmask = (2 << (numbits-1)) - 1;
#else
	extern std::uint32_t g_ExtraMasks[33];
	unsigned int bitmask = g_ExtraMasks[numbits];
#endif

	unsigned int dw1 = LoadLittleDWord( (std::uint32_t* )m_pData, iWordOffset1 ) >> iStartBit;
	unsigned int dw2 = LoadLittleDWord( (std::uint32_t* )m_pData, iWordOffset2 ) << (32 - iStartBit);

	return (dw1 | dw2) & bitmask;
}

BITBUF_INLINE int bf_read::CompareBits( bf_read *  other, int numbits ) 
{
	return (ReadUBitLong(numbits) != other->ReadUBitLong(numbits));
}

#endif //CENTURY_BF_READ_HPP
