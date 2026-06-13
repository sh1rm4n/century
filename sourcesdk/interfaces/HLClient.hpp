//
// Created by sh1rm4n on 5/16/26.
//

#ifndef CENTURY_HLCLIENT_HPP
#define CENTURY_HLCLIENT_HPP

enum ClientFrameStage_t
{
    FRAME_UNDEFINED=-1,			// (haven't run any frames yet)
    FRAME_START,
    // A network packet is being recieved
    FRAME_NET_UPDATE_START,
    // Data has been received and we're going to start calling PostDataUpdate
    FRAME_NET_UPDATE_POSTDATAUPDATE_START,
    // Data has been received and we've called PostDataUpdate on all data recipients
    FRAME_NET_UPDATE_POSTDATAUPDATE_END,
    // We've received all packets, we can now do interpolation, prediction, etc..
    FRAME_NET_UPDATE_END,
    // We're about to start rendering the scene
    FRAME_RENDER_START,
    // We've finished rendering the scene.
    FRAME_RENDER_END
};

class RecvTable
{
public:

    // Properties described in a table.
    void*		*m_pProps;
    int				m_nProps;

    // The decoder. NOTE: this covers each RecvTable AND all its children (ie: its children
    // will have their own decoders that include props for all their children).
    void*	*m_pDecoder;

    const char		*m_pNetTableName;	// The name matched between client and server.


private:

    bool			m_bInitialized;
    bool			m_bInMainList;
};

class ClientClass
{
public:
    void*		m_pCreateFn;
    void*			m_pCreateEventFn;	// Only called for event objects.
    const char				*m_pNetworkName;
    RecvTable				*m_pRecvTable;
    ClientClass				*m_pNext;
    int						m_ClassID;	// Managed by the engine.
};

class HLClient {
    public:
};

#endif //CENTURY_HLCLIENT_HPP
