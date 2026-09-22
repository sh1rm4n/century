//
// Created by sh1rm4n on 8/28/26.
//

#ifndef CENTURY_INETMESSAGE_HPP
#define CENTURY_INETMESSAGE_HPP

#pragma once

#include "bitbuf.hpp"

#include "../core/memory.hpp"

class CNetChan;

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

class CNetMessage {
public:
    CNetMessage() {
	    reliable = false;
    	netChannel = nullptr;
    }

    virtual ~CNetMessage() {}

    bool	  reliable;	// true if message should be send reliable
    CNetChan* netChannel;
	void*	  messageHandler;
};

class CLC_Move : public CNetMessage {
public:
	CLC_Move() {
		reliable = false;
	}

	void Init() noexcept {
		// adding 0x10 cuz we need vtable funcs, not the info (gcc/clang)
		static auto vtable = reinterpret_cast<void**>(
			memory::ResolveGOT(memory::GetAbsAddress(memory::FindPattern("engine_client.so", "48 8B 05 ? ? ? ? C6 85 ? ? ? ? ? 44 8B 35"))) + 0x10);

		*reinterpret_cast<void***>(this) = vtable;
	}

	int				backupCommands; // 32
	int				newCommands; // 36
	int				length; // 40
	bf_read			dataIn; // 48
	bf_write		dataOut; // 80
};

#endif //CENTURY_INETMESSAGE_HPP
