//
// Created by sh1rm4n on 5/16/26.
//

#ifndef CENTURY_HLCLIENT_HPP
#define CENTURY_HLCLIENT_HPP

#include "../../core/memory.hpp"

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

struct RecvProp;
class RecvTable {
public:

    // Properties described in a table.
    RecvProp*       m_pProps;
    int				m_nProps;

    // The decoder. NOTE: this covers each RecvTable AND all its children (ie: its children
    // will have their own decoders that include props for all their children).
    void*	*m_pDecoder;

    const char		*m_pNetTableName;	// The name matched between client and server.


private:

    bool			m_bInitialized;
    bool			m_bInMainList;
};

enum class SendPropType : int
{
    INT = 0,
    FLOAT,
    VECTOR,
    VECTOR2D,
    STRING,
    ARRAY,
    DATATABLE,
    INT64,
    SENDPROPTYPEMAX
};

struct RecvProp
{
    char* varName;
    SendPropType recvType;
    int flags;
    int stringBufferSize;
    bool insideArray;
    const void* extraData;
    RecvProp* arrayProp;
    void* arrayLengthProxyFn;
    void* proxyFn;
    void* dataTableProxyFn;
    RecvTable* dataTable;
    int offset;
    int elementStride;
    int elements;
    const char* parentArrayPropName;
};

class ClientClass
{
public:
    void* m_pCreateFn;
    void* m_pCreateEventFn;	// Only called for event objects.
    int unk;
    const char *m_pNetworkName;
    RecvTable *m_pRecvTable;
    ClientClass *m_pNext;
    int	m_ClassID;	// Managed by the engine.

    constexpr const char* GetNetworkedName() noexcept {
        return *reinterpret_cast<const char **>(std::uint64_t(this) + 0x10);
    }

    constexpr RecvTable* GetRecvTable() noexcept {
        return *reinterpret_cast<RecvTable**>(std::uint64_t(this) + 0x18);
    }

    constexpr ClientClass* GetNext() noexcept {
        return *reinterpret_cast<ClientClass**>(std::uint64_t(this) + 0x20);
    }
};

class HLClient {
public:
    constexpr ClientClass* GetAllClasses() noexcept {
        return memory::Call<ClientClass*>(this, 8);
    }
};

#endif //CENTURY_HLCLIENT_HPP
