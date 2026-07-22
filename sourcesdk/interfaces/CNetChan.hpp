//
// Created by sh1rm4n on 6/5/26.
//

#ifndef CENTURY_CNETCHAN_HPP
#define CENTURY_CNETCHAN_HPP

class CNetChan
{
public:
    virtual int GetName(void) = 0; // 0
    virtual const char* GetAddress(void) = 0; // 1
    virtual int GetTime(void) = 0; // 2
    virtual int GetTimeConnected(void) = 0; // 3
    virtual int GetBufferSize(void) = 0; // 4
    virtual int GetDataRate(void) = 0; // 5
    virtual int IsLoopback(void) = 0; // 6
    virtual int IsTimingOut(void) = 0; // 7
    virtual int IsPlayback(void) = 0; // 8
    virtual float GetLatency(int) = 0; // 9
    virtual int GetAvgLatency(int) = 0; // 10
    virtual int GetAvgLoss(int) = 0; // 11
    virtual int GetAvgChoke(int) = 0; // 12
    virtual int GetAvgData(int) = 0; // 13
    virtual int GetAvgPackets(int) = 0; // 14
    virtual int GetTotalData(int) = 0; // 15
    virtual int GetSequenceNr(int) = 0; // 16
    virtual int IsValidPacket(int,int) = 0; // 17
    virtual int GetPacketTime(int,int) = 0; // 18
    virtual int GetPacketBytes(int,int,int) = 0; // 19
    virtual int GetStreamProgress(int,int *,int *) = 0; // 20
    virtual int GetTimeSinceLastReceived(void) = 0; // 21
    virtual int GetCommandInterpolationAmount(int,int) = 0; // 22
    virtual int GetPacketResponseLatency(int,int,int *,int *) = 0; // 23
    virtual int GetRemoteFramerate(float *,float *) = 0; // 24
    virtual int GetTimeoutSeconds(void) = 0; // 25
    virtual void constructor() = 0; // 26
    virtual void destructor() = 0; // 27
    virtual int SetDataRate(float) = 0; // 28
    virtual void RegisterMessage(void**) = 0; // 29
    virtual int StartStreaming(std::uint32_t) = 0; // 30
    virtual int ResetStreaming(void) = 0; // 31
    virtual int SetTimeout(float) = 0; // 32
    virtual void SetDemoRecorder(void**) = 0; // 33
    virtual int SetChallengeNr(std::uint32_t) = 0; // 34
    virtual int Reset(void) = 0; // 35
    virtual int Clear(void) = 0; // 36
    virtual int Shutdown(char const*) = 0; // 37
    virtual int ProcessPlayback(void) = 0; // 38
    virtual int ProcessStream(void) = 0; // 39
    virtual int ProcessPacket(void**,bool) = 0; // 40
    virtual void SendNetMsg(void* msg, bool forceReliable, bool voice) = 0; // 41
    virtual int SendData(void*&,bool) = 0; // 42
    virtual int SendFile(char const*,std::uint32_t) = 0; // 43
    virtual int DenyFile(std::uint32_t) = 0; // 44
    virtual int RequestFile_OLD(char const*,std::uint32_t) = 0; // 45
    virtual void SetChoked() = 0; // 46
    virtual int SendDatagram(void**) = 0; // 47
    virtual int Transmit(bool) = 0; // 48
    virtual int GetRemoteAddress(void) = 0; // 49
    virtual int GetMsgHandler(void) = 0; // 50
    virtual int GetDropNumber(void) = 0; // 51
    virtual int GetSocket(void) = 0; // 52
    virtual int GetChallengeNr(void) = 0; // 53
    virtual int GetSequenceData(int &,int &,int &) = 0; // 54
    virtual int SetSequenceData(int,int,int) = 0; // 55
    virtual int UpdateMessageStats(int,int) = 0; // 56
    virtual int CanPacket(void) = 0; // 57
    virtual int IsOverflowed(void) = 0; // 58
    virtual int IsTimedOut(void) = 0; // 59
    virtual int HasPendingReliableData(void) = 0; // 60
    virtual int SetFileTransmissionMode(bool) = 0; // 61
    virtual int SetCompressionMode(bool) = 0; // 62
    virtual int RequestFile(int,unsigned int) = 0; // 63
    virtual int SetMaxBufferSize(bool,int,bool) = 0; // 64
    virtual int IsNull(void) = 0; // 65
    virtual int GetNumBitsWritten(bool) = 0; // 66
    virtual int SetInterpolationAmount(float) = 0; // 67
    virtual int SetRemoteFramerate(float,float) = 0; // 68
    virtual int SetMaxRoutablePayloadSize(int) = 0; // 69
    virtual int GetMaxRoutablePayloadSize(void) = 0; // 70
    virtual int GetProtocolVersion(void) = 0; // 71

    int unknown;

    int			outSequenceNumber;
    int			inSequenceNumber;
    int			outSequenceNumberAck;
    int			outReliableState;
    int			inReliableState;
    int			chokedPackets;
};

#endif //CENTURY_CNETCHAN_HPP
