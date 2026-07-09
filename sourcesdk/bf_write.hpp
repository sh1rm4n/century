//
// Created by sh1rm4n on 7/4/26.
//

#ifndef CENTURY_BF_WRITE_HPP
#define CENTURY_BF_WRITE_HPP

#include <cstdint>
#include <cstring>

#include "math.hpp"
#include "typedefs.hpp"

#define BITBUF_INLINE inline

class bf_write
{
public:
	bf_write();

					// nMaxBits can be used as the number of bits in the buffer.
					// It must be <= nBytes*8. If you leave it at -1, then it's set to nBytes * 8.
	bf_write( void *pData, int nBytes, int nMaxBits = -1 );
	bf_write( const char *pDebugName, void *pData, int nBytes, int nMaxBits = -1 );

	// Start writing to the specified buffer.
	// nMaxBits can be used as the number of bits in the buffer.
	// It must be <= nBytes*8. If you leave it at -1, then it's set to nBytes * 8.
	void			StartWriting( void *pData, int nBytes, int iStartBit = 0, int nMaxBits = -1 );

	// Restart buffer writing.
	void			Reset();

	// Get the base pointer.
	unsigned char*	GetBasePointer() { return (unsigned char*) m_pData; }

	// Enable or disable assertion on overflow. 99% of the time, it's a bug that we need to catch,
	// but there may be the occasional buffer that is allowed to overflow gracefully.
	void			SetAssertOnOverflow( bool bAssert );

	// This can be set to assign a name that gets output if the buffer overflows.
	const char*		GetDebugName();
	void			SetDebugName( const char *pDebugName );


// Seek to a specific position.
public:

	void			SeekToBit( int bitPos );


// Bit functions.
public:

	void			WriteOneBit(int nValue);
	void			WriteOneBitNoCheck(int nValue);
	void			WriteOneBitAt( int iBit, int nValue );

	// Write signed or unsigned. Range is only checked in debug.
	void		    WriteUBitLong( unsigned int data, int numbits, bool bCheckRange=true );
	void			WriteSBitLong( int data, int numbits );

	// Tell it whether or not the data is unsigned. If it's signed,
	// cast to unsigned before passing in (it will cast back inside).
	void			WriteBitLong(unsigned int data, int numbits, bool bSigned);

	// Write a list of bits in.
	bool			WriteBits(const void *pIn, int nBits);

	// writes an unsigned integer with variable bit length
	void			WriteUBitVar( unsigned int data );

	// writes a varint encoded integer
	void			WriteVarInt32( std::uint32_t data );
	void			WriteVarInt64( std::uint64_t data );
	void			WriteSignedVarInt32( std::int32_t data );
	void			WriteSignedVarInt64( std::int64_t data );
	int				ByteSizeVarInt32( std::uint32_t data );
	int				ByteSizeVarInt64( std::uint64_t data );
	int				ByteSizeSignedVarInt32( std::int32_t data );
	int				ByteSizeSignedVarInt64( std::int64_t data );

	// Copy the bits straight out of pIn. This seeks pIn forward by nBits.
	// Returns an error if this buffer or the read buffer overflows.
	bool			WriteBitsFromBuffer( class bf_read *pIn, int nBits );

	void			WriteBitAngle( float fAngle, int numbits );
	void			WriteBitCoord (const float f);
	void			WriteBitCoordMP( const float f, bool bIntegral, bool bLowPrecision );
	void			WriteBitFloat(float val);
	void			WriteBitVec3Coord( const Vector& fa );
	void			WriteBitNormal( float f );
	void			WriteBitVec3Normal( const Vector& fa );
	void			WriteBitAngles( const Angle& fa );


// Byte functions.
public:

	void			WriteChar(int val);
	void			WriteByte(int val);
	void			WriteShort(int val);
	void			WriteWord(int val);
	void			WriteLong(std::int32_t val);
	void			WriteLongLong(std::int64_t val);
	void			WriteFloat(float val);
	bool			WriteBytes( const void *pBuf, int nBytes );

	// Returns false if it overflows the buffer.
	bool			WriteString(const char *pStr);


// Status.
public:

	// How many bytes are filled in?
	int				GetNumBytesWritten() const;
	int				GetNumBitsWritten() const;
	int				GetMaxNumBits();
	int				GetNumBitsLeft();
	int				GetNumBytesLeft();
	unsigned char*	GetData();
	const unsigned char*	GetData() const;

	// Has the buffer overflowed?
	bool			CheckForOverflow(int nBits);
	inline bool		IsOverflowed() const {return m_bOverflow;}

	void			SetOverflowFlag();


public:
	// The current buffer.
	std::uint32_t*  m_pData;
	int				m_nDataBytes;
	int				m_nDataBits;

	// Where we are in the buffer.
	int				m_iCurBit;

private:

	// Errors?
	bool			m_bOverflow;

	bool			m_bAssertOnOverflow;
	const char		*m_pDebugName;
};


//-----------------------------------------------------------------------------
// Inlined methods
//-----------------------------------------------------------------------------

// How many bytes are filled in?
inline int bf_write::GetNumBytesWritten() const
{
	return BitByte(m_iCurBit);
}

inline int bf_write::GetNumBitsWritten() const
{
	return m_iCurBit;
}

inline int bf_write::GetMaxNumBits()
{
	return m_nDataBits;
}

inline int bf_write::GetNumBitsLeft()
{
	return m_nDataBits - m_iCurBit;
}

inline int bf_write::GetNumBytesLeft()
{
	return GetNumBitsLeft() >> 3;
}

inline unsigned char* bf_write::GetData()
{
	return (unsigned char*) m_pData;
}

inline const unsigned char* bf_write::GetData()	const
{
	return (unsigned char*) m_pData;
}

BITBUF_INLINE bool bf_write::CheckForOverflow(int nBits)
{
	if ( m_iCurBit + nBits > m_nDataBits )
	{
		SetOverflowFlag();
	}

	return m_bOverflow;
}

BITBUF_INLINE void bf_write::SetOverflowFlag()
{
#ifdef DBGFLAG_ASSERT
	if ( m_bAssertOnOverflow )
	{
		Assert( false );
	}
#endif
	m_bOverflow = true;
}

BITBUF_INLINE void bf_write::WriteOneBitNoCheck(int nValue)
{
#if __i386__
	if(nValue)
		m_pData[m_iCurBit >> 5] |= 1u << (m_iCurBit & 31);
	else
		m_pData[m_iCurBit >> 5] &= ~(1u << (m_iCurBit & 31));
#else
	extern std::uint32_t g_LittleBits[32];
	if(nValue)
		m_pData[m_iCurBit >> 5] |= g_LittleBits[m_iCurBit & 31];
	else
		m_pData[m_iCurBit >> 5] &= ~g_LittleBits[m_iCurBit & 31];
#endif

	++m_iCurBit;
}

inline void bf_write::WriteOneBit(int nValue)
{
	if( m_iCurBit >= m_nDataBits )
	{
		SetOverflowFlag();
		return;
	}
	WriteOneBitNoCheck( nValue );
}


inline void	bf_write::WriteOneBitAt( int iBit, int nValue )
{
	if( iBit >= m_nDataBits )
	{
		SetOverflowFlag();
		return;
	}

#if __i386__
	if(nValue)
		m_pData[iBit >> 5] |= 1u << (iBit & 31);
	else
		m_pData[iBit >> 5] &= ~(1u << (iBit & 31));
#else
	extern std::uint32_t g_LittleBits[32];
	if(nValue)
		m_pData[iBit >> 5] |= g_LittleBits[iBit & 31];
	else
		m_pData[iBit >> 5] &= ~g_LittleBits[iBit & 31];
#endif
}

BITBUF_INLINE void bf_write::WriteUBitLong( unsigned int curData, int numbits, bool bCheckRange )
{
#ifdef _DEBUG
	// Make sure it doesn't overflow.
	if ( bCheckRange && numbits < 32 )
	{
		if ( curData >= (uint32)(1 << numbits) )
		{
			CallErrorHandler( BITBUFERROR_VALUE_OUT_OF_RANGE, GetDebugName() );
		}
	}
	Assert( numbits >= 0 && numbits <= 32 );
#endif

	if ( GetNumBitsLeft() < numbits )
	{
		m_iCurBit = m_nDataBits;
		SetOverflowFlag();
		return;
	}

	int iCurBitMasked = m_iCurBit & 31;
	int iDWord = m_iCurBit >> 5;
	m_iCurBit += numbits;

	// Mask in a dword.
    std::uint32_t * pOut = &m_pData[iDWord];

	// Rotate data into dword alignment
	curData = (curData << iCurBitMasked) | (curData >> (32 - iCurBitMasked));

	// Calculate bitmasks for first and second word
	unsigned int temp = 1 << (numbits-1);
	unsigned int mask1 = (temp*2-1) << iCurBitMasked;
	unsigned int mask2 = (temp-1) >> (31 - iCurBitMasked);

	// Only look beyond current word if necessary (avoid access violation)
	int i = mask2 & 1;
    std::uint32_t dword1 = LoadLittleDWord( pOut, 0 );
    std::uint32_t dword2 = LoadLittleDWord( pOut, i );

	// Drop bits into place
	dword1 ^= ( mask1 & ( curData ^ dword1 ) );
	dword2 ^= ( mask2 & ( curData ^ dword2 ) );

	// Note reversed order of writes so that dword1 wins if mask2 == 0 && i == 0
	StoreLittleDWord( pOut, i, dword2 );
	StoreLittleDWord( pOut, 0, dword1 );
}

// writes an unsigned integer with variable bit length
BITBUF_INLINE void bf_write::WriteUBitVar( unsigned int data )
{
	/* Reference:
	if ( data < 0x10u )
		WriteUBitLong( 0, 2 ), WriteUBitLong( data, 4 );
	else if ( data < 0x100u )
		WriteUBitLong( 1, 2 ), WriteUBitLong( data, 8 );
	else if ( data < 0x1000u )
		WriteUBitLong( 2, 2 ), WriteUBitLong( data, 12 );
	else
		WriteUBitLong( 3, 2 ), WriteUBitLong( data, 32 );
	*/
	// a < b ? -1 : 0 translates into a CMP, SBB instruction pair
	// with no flow control. should also be branchless on consoles.
	int n = (data < 0x10u ? -1 : 0) + (data < 0x100u ? -1 : 0) + (data < 0x1000u ? -1 : 0);
	WriteUBitLong( data*4 + n + 3, 6 + n*4 + 12 );
	if ( data >= 0x1000u )
	{
		WriteUBitLong( data >> 16, 16 );
	}
}

// write raw IEEE float bits in little endian form
BITBUF_INLINE void bf_write::WriteBitFloat(float val)
{
	std::int32_t intVal;

	std::memcpy( &intVal, &val, sizeof(intVal));
	WriteUBitLong( intVal, 32 );
}

#endif //CENTURY_BF_WRITE_HPP
