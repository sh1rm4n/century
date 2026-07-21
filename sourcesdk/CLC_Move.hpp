//
// Created by sh1rm4n on 7/9/26.
//

#ifndef CENTURY_CLC_MOVE_HPP
#define CENTURY_CLC_MOVE_HPP

#include "tier1/bitbuf.h"

#define NETMSG_TYPE_BITS	6

#define NUM_NEW_COMMAND_BITS		4
#define MAX_NEW_COMMANDS			((1 << NUM_NEW_COMMAND_BITS)-1)

// Max number of history commands to send ( 2 by default ) in case of dropped packets
#define NUM_BACKUP_COMMAND_BITS		3
#define MAX_BACKUP_COMMANDS			((1 << NUM_BACKUP_COMMAND_BITS)-1)

class INetMessage {
public:
    virtual	~INetMessage() {};

    virtual void	        SetNetChannel(void* netchan) = 0; // netchannel this message is from/for
    virtual void	        SetReliable( bool state ) = 0;	// set to true if it's a reliable message
    virtual bool	        Process( void ) = 0; // calles the recently set handler to process this message
    virtual	bool	        ReadFromBuffer( bf_read &buffer ) = 0; // returns true if parsing was OK
    virtual	bool	        WriteToBuffer( bf_write &buffer ) = 0;	// returns true if writing was OK
    virtual bool	        IsReliable( void ) const = 0;  // true, if message needs reliable handling
    virtual int				GetType( void ) const = 0; // returns module specific header tag eg svc_serverinfo
    virtual int				GetGroup( void ) const = 0;	// returns net message group of this message
    virtual const char*		GetName( void ) const = 0;	// returns network message name, eg "svc_serverinfo"
    virtual void*       	GetNetChannel( void ) const = 0;
    virtual const char*		ToString( void ) const = 0; // returns a human readable string about message content
};

class INetChannelInfo {
public:
    enum {
        GENERIC = 0,	// must be first and is default group
        LOCALPLAYER,	// bytes for local player entity update
        OTHERPLAYERS,	// bytes for other players update
        ENTITIES,		// all other entity bytes
        SOUNDS,			// game sounds
        EVENTS,			// event messages
        USERMESSAGES,	// user messages
        ENTMESSAGES,	// entity messages
        VOICE,			// voice data
        STRINGTABLE,	// a stringtable update
        MOVE,			// client move cmds
        STRINGCMD,		// string command
        SIGNON,			// various signondata
        TOTAL,			// must be last and is not a real group
    };
};

class CNetMessage : public INetMessage
{
public:
    CNetMessage() {	m_bReliable = true;
        m_NetChannel = 0; }
    virtual ~CNetMessage() {};

    virtual int		GetGroup() const { return INetChannelInfo::GENERIC; }
    void*   		GetNetChannel() const { return m_NetChannel; }
    virtual void	SetReliable( bool state) {m_bReliable = state;};
    virtual bool	IsReliable() const { return m_bReliable; };
    virtual void    SetNetChannel(void* netchan) { m_NetChannel = netchan; }
    virtual bool	Process() { return false; };	// no handler set

protected:
    bool				m_bReliable;	// true if message should be send reliable
    void*			    m_NetChannel;	// netchannel this message is from/for
};

class CLC_Move : public CNetMessage {
public:
    CLC_Move() {
        m_bReliable = false;
    }

    int GetGroup() const override {
        return INetChannelInfo::MOVE;
    }

    int GetType() const override {
        return 9; // clc_Move id is 9
    }

    const char* GetName() const override {
        return "clc_Move";
    }

    const char* ToString() const override {
        utils::Warn("clc_Move ToString");
        return "";
    }

    bool ReadFromBuffer(bf_read& buffer) override {
        m_nNewCommands = buffer.ReadUBitLong( NUM_NEW_COMMAND_BITS );
        m_nBackupCommands = buffer.ReadUBitLong( NUM_BACKUP_COMMAND_BITS );
        m_nLength = buffer.ReadWord();
        m_DataIn = 	buffer;
        return buffer.SeekRelative( m_nLength );
    }

    bool WriteToBuffer(bf_write& buffer) override {
        buffer.WriteUBitLong( GetType(), NETMSG_TYPE_BITS );
        m_nLength = m_DataOut.GetNumBitsWritten();

        buffer.WriteUBitLong( m_nNewCommands, NUM_NEW_COMMAND_BITS );
        buffer.WriteUBitLong( m_nBackupCommands, NUM_BACKUP_COMMAND_BITS );

        buffer.WriteWord( m_nLength );

        return buffer.WriteBits( m_DataOut.GetData(), m_nLength );
    }

    bool Process() override {
        return true;
    }

    int      m_nBackupCommands{};
    int      m_nNewCommands{};
    int      m_nLength{};
    bf_read  m_DataIn;
    bf_write m_DataOut;
};

#endif //CENTURY_CLC_MOVE_HPP
