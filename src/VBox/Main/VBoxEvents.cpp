/* $Id$ */
/** @file
 * VirtualBox-specific event classes implementation
 */

/*
 * Copyright (C) 2010 Oracle Corporation
 *
 * This file is part of VirtualBox Open Source Edition (OSE), as
 * available from http://www.virtualbox.org. This file is free software;
 * you can redistribute it and/or modify it under the terms of the GNU
 * General Public License (GPL) as published by the Free Software
 * Foundation, in version 2 as it comes in the "COPYING" file of the
 * VirtualBox OSE distribution. VirtualBox OSE is distributed in the
 * hope that it will be useful, but WITHOUT ANY WARRANTY of any kind.
 */
#include "EventImpl.h"

/**
 * @todo: Technically, this file better be autogenerated,
 *        as contains almost no semantic meat. We use aggregation
 *        of VBoxEvent to simplify multiple inheritance ambiguity.
 */

// @todo: add more elements to COM map too
#define EVENT_CLASS_TRIVIA(klazz, iface) \
  public:                                \
    DECLARE_NOT_AGGREGATABLE(klazz)      \
                                         \
    DECLARE_PROTECT_FINAL_CONSTRUCT()    \
                                         \
    BEGIN_COM_MAP(klazz)                                \
        COM_INTERFACE_ENTRY(iface)                      \
        COM_INTERFACE_ENTRY(IEvent)                     \
        COM_INTERFACE_ENTRY(IDispatch)                  \
    END_COM_MAP()                                       \
                                                        \
    klazz () {}                                         \
    virtual ~klazz () {}                                \
                                                        \
    HRESULT FinalConstruct()                            \
    {                                                   \
        return mEvent.createObject();                   \
    }                                                   \
    void FinalRelease() {                               \
        mEvent->FinalRelease();                         \
    }                                                   \
    STDMETHOD(COMGETTER(Type)) (VBoxEventType_T *aType) \
    {                                                   \
        return mEvent->COMGETTER(Type) (aType);                 \
    }                                                           \
    STDMETHOD(COMGETTER(Source)) (IEventSource * *aSource)      \
    {                                                           \
        return mEvent->COMGETTER(Source) (aSource);             \
    }                                                           \
    STDMETHOD(COMGETTER(Waitable)) (BOOL *aWaitable)            \
    {                                                           \
        return mEvent->COMGETTER(Waitable) (aWaitable);         \
    }                                                           \
    STDMETHOD(SetProcessed)()                                   \
    {                                                           \
        return mEvent->SetProcessed();                          \
    }                                                           \
    STDMETHOD(WaitProcessed)(LONG aTimeout, BOOL *aResult)      \
    {                                                           \
        return mEvent->WaitProcessed(aTimeout, aResult);        \
    }                                                           \
private:                                                        \
    ComObjPtr<VBoxEvent>      mEvent;

//    operator VBoxEvent&() { return mEvent; }
//    operator IEvent&()    { return mEvent; }


class ATL_NO_VTABLE MachineStateChangeEvent :
    public VirtualBoxBase,
    VBOX_SCRIPTABLE_IMPL(IMachineStateChangeEvent)
{
    EVENT_CLASS_TRIVIA(MachineStateChangeEvent, IMachineStateChangeEvent)

public:
    HRESULT init (IEventSource* aSource, VBoxEventType_T aType, IN_BSTR aId, MachineState_T aState)
    {
        HRESULT rc;

        rc = mEvent->init(aSource, aType, FALSE);
        if (SUCCEEDED(rc))
        {
            mMachineId = aId;
            mState = aState;
        }
        return rc;
    }

    void uninit()
    {
        mEvent->uninit();
    }

    // IMachineEvent properties
    STDMETHOD(COMGETTER(MachineId)) (BSTR *aMachineId)
    {
        mMachineId.cloneTo(aMachineId);
        return S_OK;
    }
    // IMachineStateChangeEvent methods
    STDMETHOD(COMGETTER(State))(MachineState_T * aState)
    {
        *aState = mState;
        return S_OK;
    }

  private:
    Bstr           mMachineId;
    MachineState_T mState;
};

class ATL_NO_VTABLE MachineDataChangeEvent :
    public VirtualBoxBase,
    VBOX_SCRIPTABLE_IMPL(IMachineDataChangeEvent)
{
    EVENT_CLASS_TRIVIA(MachineDataChangeEvent, IMachineDataChangeEvent)

public:
    HRESULT init (IEventSource* aSource, VBoxEventType_T aType, IN_BSTR aId)
    {
        HRESULT rc;

        rc = mEvent->init(aSource, aType, FALSE);
        if (SUCCEEDED(rc))
            mMachineId = aId;

        return rc;
    }

    void uninit()
    {
        mEvent->uninit();
    }

    // IMachineEvent properties
    STDMETHOD(COMGETTER(MachineId)) (BSTR *aMachineId)
    {
        mMachineId.cloneTo(aMachineId);
        return S_OK;
    }

  private:
    Bstr           mMachineId;
};

class ATL_NO_VTABLE MachineRegisteredEvent :
    public VirtualBoxBase,
    VBOX_SCRIPTABLE_IMPL(IMachineRegisteredEvent)
{
    EVENT_CLASS_TRIVIA(MachineRegisteredEvent, IMachineRegisteredEvent)

public:
    HRESULT init (IEventSource* aSource, VBoxEventType_T aType, IN_BSTR aId, BOOL aRegistered)
    {
        HRESULT rc;

        rc = mEvent->init(aSource, aType, FALSE);
        if (SUCCEEDED(rc))
        {
            mMachineId = aId;
            mRegistered = aRegistered;
        }

        return rc;
    }

    void uninit()
    {
        mEvent->uninit();
    }

    // IMachineEvent properties
    STDMETHOD(COMGETTER(MachineId)) (BSTR *aMachineId)
    {
        mMachineId.cloneTo(aMachineId);
        return S_OK;
    }

    // IMachineRegisteredEvent properties
    STDMETHOD(COMGETTER(Registered))(BOOL * aRegistered)
    {
        *aRegistered = mRegistered;
        return S_OK;
    }

  private:
    Bstr           mMachineId;
    BOOL           mRegistered;
};

class ATL_NO_VTABLE MachineSessionStateEvent :
    public VirtualBoxBase,
    VBOX_SCRIPTABLE_IMPL(IMachineSessionStateEvent)
{
    EVENT_CLASS_TRIVIA(MachineSessionStateEvent, IMachineSessionStateEvent)

public:
    HRESULT init (IEventSource* aSource, VBoxEventType_T aType, IN_BSTR aId, SessionState_T aState)
    {
        HRESULT rc;

        rc = mEvent->init(aSource, aType, FALSE);
        if (SUCCEEDED(rc))
        {
            mMachineId = aId;
            mState = aState;
        }

        return rc;
    }

    void uninit()
    {
        mEvent->uninit();
    }

    // IMachineEvent properties
    STDMETHOD(COMGETTER(MachineId)) (BSTR *aMachineId)
    {
        mMachineId.cloneTo(aMachineId);
        return S_OK;
    }

    // IMachineSessionStateEvent properties
    STDMETHOD(COMGETTER(State))(SessionState_T * aState)
    {
        *aState = mState;
        return S_OK;
    }

  private:
    Bstr           mMachineId;
    SessionState_T mState;
};

class ATL_NO_VTABLE GuestPropertyChangeEvent :
    public VirtualBoxBase,
    VBOX_SCRIPTABLE_IMPL(IGuestPropertyChangeEvent)
{
    EVENT_CLASS_TRIVIA(GuestPropertyChangeEvent, IGuestPropertyChangeEvent)

public:
    HRESULT init (IEventSource* aSource, VBoxEventType_T aType,
                  IN_BSTR aMachineId, IN_BSTR aName, IN_BSTR aValue, IN_BSTR aFlags)
    {
        HRESULT rc;

        rc = mEvent->init(aSource, aType, FALSE);
        if (SUCCEEDED(rc))
        {
            mMachineId = aMachineId;
            mName = aName;
            mValue = aValue;
            mFlags = aFlags;
        }

        return rc;
    }

    void uninit()
    {
        mEvent->uninit();
    }

    // IMachineEvent properties
    STDMETHOD(COMGETTER(MachineId)) (BSTR *aMachineId)
    {
        mMachineId.cloneTo(aMachineId);
        return S_OK;
    }

    // IGuestPropertyChangeEvent properties
    STDMETHOD(COMGETTER(Name))(BSTR * aName)
    {
        mName.cloneTo(aName);
        return S_OK;
    }
    STDMETHOD(COMGETTER(Value))(BSTR * aValue)
    {
        mValue.cloneTo(aValue);
        return S_OK;
    }
    STDMETHOD(COMGETTER(Flags))(BSTR * aFlags)
    {
        mFlags.cloneTo(aFlags);
        return S_OK;
    }
  private:
    Bstr           mMachineId, mName, mValue, mFlags;
};

class ATL_NO_VTABLE SnapshotTakenEvent :
    public VirtualBoxBase,
    VBOX_SCRIPTABLE_IMPL(ISnapshotTakenEvent)
{
    EVENT_CLASS_TRIVIA(SnapshotTakenEvent, ISnapshotTakenEvent)

public:
    HRESULT init (IEventSource* aSource, VBoxEventType_T aType, IN_BSTR aMachineId,  IN_BSTR aSnapshotId)
    {
        HRESULT rc;

        rc = mEvent->init(aSource, aType, FALSE);
        if (SUCCEEDED(rc))
        {
            mMachineId = aMachineId;
            mSnapshotId = aSnapshotId;
        }

        return rc;
    }

    void uninit()
    {
        mEvent->uninit();
    }

    // IMachineEvent properties
    STDMETHOD(COMGETTER(MachineId)) (BSTR *aMachineId)
    {
        mMachineId.cloneTo(aMachineId);
        return S_OK;
    }

    // ISnapshotEvent properties
    STDMETHOD(COMGETTER(SnapshotId)) (BSTR *aSnapshotId)
    {
        mSnapshotId.cloneTo(aSnapshotId);
        return S_OK;
    }

  private:
    Bstr           mMachineId, mSnapshotId;
};

class ATL_NO_VTABLE SnapshotChangeEvent :
    public VirtualBoxBase,
    VBOX_SCRIPTABLE_IMPL(ISnapshotChangeEvent)
{
    EVENT_CLASS_TRIVIA(SnapshotChangeEvent, ISnapshotChangeEvent)

public:
    HRESULT init (IEventSource* aSource, VBoxEventType_T aType, IN_BSTR aMachineId,  IN_BSTR aSnapshotId)
    {
        HRESULT rc;

        rc = mEvent->init(aSource, aType, FALSE);
        if (SUCCEEDED(rc))
        {
            mMachineId = aMachineId;
            mSnapshotId = aSnapshotId;
        }

        return rc;
    }

    void uninit()
    {
        mEvent->uninit();
    }

    // IMachineEvent properties
    STDMETHOD(COMGETTER(MachineId)) (BSTR *aMachineId)
    {
        mMachineId.cloneTo(aMachineId);
        return S_OK;
    }

    // ISnapshotEvent properties
    STDMETHOD(COMGETTER(SnapshotId)) (BSTR *aSnapshotId)
    {
        mSnapshotId.cloneTo(aSnapshotId);
        return S_OK;
    }

  private:
    Bstr           mMachineId, mSnapshotId;
};

class ATL_NO_VTABLE SnapshotDeletedEvent :
    public VirtualBoxBase,
    VBOX_SCRIPTABLE_IMPL(ISnapshotDeletedEvent)
{
    EVENT_CLASS_TRIVIA(SnapshotDeletedEvent, ISnapshotDeletedEvent)

public:
    HRESULT init (IEventSource* aSource, VBoxEventType_T aType, IN_BSTR aMachineId,  IN_BSTR aSnapshotId)
    {
        HRESULT rc;

        rc = mEvent->init(aSource, aType, FALSE);
        if (SUCCEEDED(rc))
        {
            mMachineId = aMachineId;
            mSnapshotId = aSnapshotId;
        }

        return rc;
    }

    void uninit()
    {
        mEvent->uninit();
    }

    // IMachineEvent properties
    STDMETHOD(COMGETTER(MachineId)) (BSTR *aMachineId)
    {
        mMachineId.cloneTo(aMachineId);
        return S_OK;
    }

    // ISnapshotEvent properties
    STDMETHOD(COMGETTER(SnapshotId)) (BSTR *aSnapshotId)
    {
        mSnapshotId.cloneTo(aSnapshotId);
        return S_OK;
    }

  private:
    Bstr           mMachineId, mSnapshotId;
};

class ATL_NO_VTABLE ExtraDataChangeEvent :
    public VirtualBoxBase,
    VBOX_SCRIPTABLE_IMPL(IExtraDataChangeEvent)
{
    EVENT_CLASS_TRIVIA(ExtraDataChangeEvent, IExtraDataChangeEvent)

public:
    HRESULT init (IEventSource* aSource, VBoxEventType_T aType, IN_BSTR aMachineId,  IN_BSTR aKey,  IN_BSTR aValue)
    {
        HRESULT rc;

        rc = mEvent->init(aSource, aType, FALSE);
        if (SUCCEEDED(rc))
        {
            mMachineId = aMachineId;
            mKey = aKey;
            mValue = aValue;
        }

        return rc;
    }

    void uninit()
    {
        mEvent->uninit();
    }

    // IExtraDataChangeEvent properties
    STDMETHOD(COMGETTER(MachineId)) (BSTR *aMachineId)
    {
        mMachineId.cloneTo(aMachineId);
        return S_OK;
    }

    STDMETHOD(COMGETTER(Key)) (BSTR *aKey)
    {
        mKey.cloneTo(aKey);
        return S_OK;
    }

    STDMETHOD(COMGETTER(Value)) (BSTR *aValue)
    {
        mValue.cloneTo(aValue);
        return S_OK;
    }

  private:
    Bstr           mMachineId, mKey, mValue;
};


#ifdef VBOX_WITH_XPCOM
NS_DECL_CLASSINFO(VBoxEvent)
NS_IMPL_THREADSAFE_ISUPPORTS1_CI(VBoxEvent, IEvent)
NS_DECL_CLASSINFO(MachineStateChangeEvent)
NS_IMPL_THREADSAFE_ISUPPORTS3_CI(MachineStateChangeEvent, IMachineStateChangeEvent, IMachineEvent, IEvent)
NS_DECL_CLASSINFO(MachineDataChangeEvent)
NS_IMPL_THREADSAFE_ISUPPORTS3_CI(MachineDataChangeEvent, IMachineDataChangeEvent, IMachineEvent, IEvent)
NS_DECL_CLASSINFO(MachineRegisteredEvent)
NS_IMPL_THREADSAFE_ISUPPORTS3_CI(MachineRegisteredEvent, IMachineRegisteredEvent, IMachineEvent, IEvent)
NS_DECL_CLASSINFO(MachineSessionStateEvent)
NS_IMPL_THREADSAFE_ISUPPORTS3_CI(MachineSessionStateEvent, IMachineSessionStateEvent, IMachineEvent, IEvent)
NS_DECL_CLASSINFO(GuestPropertyChangeEvent)
NS_IMPL_THREADSAFE_ISUPPORTS3_CI(GuestPropertyChangeEvent, IGuestPropertyChangeEvent, IMachineEvent, IEvent)
NS_DECL_CLASSINFO(SnapshotTakenEvent)
NS_IMPL_THREADSAFE_ISUPPORTS4_CI(SnapshotTakenEvent, ISnapshotTakenEvent, ISnapshotEvent, IMachineEvent, IEvent)
NS_DECL_CLASSINFO(SnapshotDeletedEvent)
NS_IMPL_THREADSAFE_ISUPPORTS4_CI(SnapshotDeletedEvent, ISnapshotDeletedEvent, ISnapshotEvent, IMachineEvent, IEvent)
NS_DECL_CLASSINFO(SnapshotChangeEvent)
NS_IMPL_THREADSAFE_ISUPPORTS4_CI(SnapshotChangeEvent, ISnapshotChangeEvent, ISnapshotEvent, IMachineEvent, IEvent)
NS_DECL_CLASSINFO(ExtraDataChangeEvent)
NS_IMPL_THREADSAFE_ISUPPORTS2_CI(ExtraDataChangeEvent, IExtraDataChangeEvent, IEvent)
#endif

HRESULT VBoxEventDesc::init(IEventSource* source, VBoxEventType_T aType, ...)
{
    va_list args;
    va_start(args, aType);
    switch (aType)
    {
        case VBoxEventType_OnMachineStateChange:
        {
            ComObjPtr<MachineStateChangeEvent> obj; obj.createObject();
            BSTR aId = va_arg(args, BSTR);
            MachineState_T aState = va_arg(args, MachineState_T);
            obj->init(source, aType, aId, aState);
            obj.queryInterfaceTo(mEvent.asOutParam());
            break;
        }
        case VBoxEventType_OnMachineDataChange:
        {
            ComObjPtr<MachineDataChangeEvent> obj; obj.createObject();
            BSTR aId = va_arg(args, BSTR);
            obj->init(source, aType, aId);
            obj.queryInterfaceTo(mEvent.asOutParam());
            break;
        }
        case VBoxEventType_OnMachineRegistered:
        {
            ComObjPtr<MachineRegisteredEvent> obj; obj.createObject();
            BSTR aId = va_arg(args, BSTR);
            BOOL aRegistered = va_arg(args, BOOL);
            obj->init(source, aType, aId, aRegistered);
            obj.queryInterfaceTo(mEvent.asOutParam());
            break;
        }
        case VBoxEventType_OnSessionStateChange:
        {
            ComObjPtr<MachineSessionStateEvent> obj; obj.createObject();
            BSTR aId = va_arg(args, BSTR);
            SessionState_T aState = va_arg(args, SessionState_T);
            obj->init(source, aType, aId, aState);
            obj.queryInterfaceTo(mEvent.asOutParam());
            break;
        }
        case VBoxEventType_OnGuestPropertyChange:
        {
            ComObjPtr<GuestPropertyChangeEvent> obj; obj.createObject();
            BSTR aMachineId = va_arg(args, BSTR);
            BSTR aName = va_arg(args, BSTR);
            BSTR aValue = va_arg(args, BSTR);
            BSTR aFlags = va_arg(args, BSTR);
            obj->init(source, aType, aMachineId, aName, aValue, aFlags);
            obj.queryInterfaceTo(mEvent.asOutParam());
            break;
        }

        case VBoxEventType_OnSnapshotTaken:
        {
            ComObjPtr<SnapshotTakenEvent> obj; obj.createObject();
            BSTR aMachineId = va_arg(args, BSTR);
            BSTR aSnapshotId = va_arg(args, BSTR);
            obj->init(source, aType, aMachineId, aSnapshotId);
            obj.queryInterfaceTo(mEvent.asOutParam());
            break;
        }
        case VBoxEventType_OnSnapshotDeleted:
        {
            ComObjPtr<SnapshotDeletedEvent> obj; obj.createObject();
            BSTR aMachineId = va_arg(args, BSTR);
            BSTR aSnapshotId = va_arg(args, BSTR);
            obj->init(source, aType, aMachineId, aSnapshotId);
            obj.queryInterfaceTo(mEvent.asOutParam());
            break;
        }
        case VBoxEventType_OnSnapshotChange:
        {
            ComObjPtr<SnapshotChangeEvent> obj; obj.createObject();
            BSTR aMachineId = va_arg(args, BSTR);
            BSTR aSnapshotId = va_arg(args, BSTR);
            obj->init(source, aType, aMachineId, aSnapshotId);
            obj.queryInterfaceTo(mEvent.asOutParam());
            break;
        }
        case VBoxEventType_OnExtraDataChange:
        {
            ComObjPtr<ExtraDataChangeEvent> obj; obj.createObject();
            BSTR aMachineId = va_arg(args, BSTR);
            BSTR aKey = va_arg(args, BSTR);
            BSTR aValue = va_arg(args, BSTR);
            obj->init(source, aType, aMachineId, aKey, aValue);
            obj.queryInterfaceTo(mEvent.asOutParam());
            break;
        }

        default:
            if (0) AssertFailed();
    }
    va_end(args);

    return S_OK;
}

void VBoxEventDesc::getEvent(IEvent ** aEvent)
{
    mEvent.queryInterfaceTo(aEvent);
}