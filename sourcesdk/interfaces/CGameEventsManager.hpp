//
// Created by sh1rm4n on 7/9/26.
//

#ifndef CENTURY_CGAMEEVENTSMANAGER_HPP
#define CENTURY_CGAMEEVENTSMANAGER_HPP

class bf_read;
class bf_write;

class IGameEvent {
public:
    virtual ~IGameEvent() {};

    virtual const char *GetName() const = 0;

    virtual bool  IsReliable() const = 0;
    virtual bool  IsLocal() const = 0;
    virtual bool  IsEmpty(const char *keyName = NULL) = 0;

    virtual bool  GetBool( const char *keyName = NULL, bool defaultValue = false ) = 0;
    virtual int   GetInt( const char *keyName = NULL, int defaultValue = 0 ) = 0;
    virtual float GetFloat( const char *keyName = NULL, float defaultValue = 0.0f ) = 0;
    virtual const char *GetString( const char *keyName = NULL, const char *defaultValue = "" ) = 0;

    virtual void SetBool( const char *keyName, bool value ) = 0;
    virtual void SetInt( const char *keyName, int value ) = 0;
    virtual void SetFloat( const char *keyName, float value ) = 0;
    virtual void SetString( const char *keyName, const char *value ) = 0;
};

class IGameEventListener2 {
public:
    virtual	~IGameEventListener2( void ) {};

    virtual void FireGameEvent( IGameEvent *event ) = 0;
};

class CGameEventsManager {
public:
    virtual void constructor () = 0;
    virtual void destructor () = 0;

    virtual int LoadEventsFromFile( const char *filename ) = 0;
    virtual void  Reset() = 0;
    virtual bool AddListener( IGameEventListener2 *listener, const char *name, bool bServerSide ) = 0;
    virtual bool FindListener( IGameEventListener2 *listener, const char *name ) = 0;
    virtual void RemoveListener( IGameEventListener2 *listener) = 0;
    virtual IGameEvent *CreateEvent( const char *name, bool bForce = false ) = 0;
    virtual bool FireEvent( IGameEvent *event, bool bDontBroadcast = false ) = 0;
    virtual bool FireEventClientSide( IGameEvent *event ) = 0;
    virtual IGameEvent *DuplicateEvent( IGameEvent *event ) = 0;
    virtual void FreeEvent( IGameEvent *event ) = 0;
    virtual bool SerializeEvent( IGameEvent *event, bf_write *buf ) = 0;
    virtual IGameEvent *UnserializeEvent( bf_read *buf ) = 0; // create new KeyValues, must be deleted
};

#endif //CENTURY_CGAMEEVENTSMANAGER_HPP
