/***************************************************************************
                          controlobject.h  -  description
                             -------------------
    begin                : Wed Feb 20 2002
    copyright            : (C) 2002 by Tue and Ken Haste Andersen
    email                :
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef CONTROLOBJECT_H
#define CONTROLOBJECT_H

#include <QObject>
#include <QEvent>
#include <QMutex>

#include "configobject.h"
#include "controllers/midi/midimessage.h"
#include "control/control.h"

class ControlObject : public QObject {
    Q_OBJECT
  public:
    ControlObject();
    ControlObject(ConfigKey key,
                  bool bIgnoreNops=true, bool bTrack=false,
                  bool bPersist=false);
    virtual ~ControlObject();

    // Returns a pointer to the ControlObject matching the given ConfigKey
    static ControlObject* getControl(const ConfigKey& key, bool warn = true);
    static inline ControlObject* getControl(const QString& group, const QString& item, bool warn = true) {
        ConfigKey key(group, item);
        return getControl(key, warn);
    }
    static inline ControlObject* getControl(const char* group, const char* item, bool warn = true) {
        ConfigKey key(group, item);
        return getControl(key, warn);
    }

    QString name() const {
        return m_pControl ?  m_pControl->name() : QString();
    }

    void setName(const QString& name) {
        if (m_pControl) {
            m_pControl->setName(name);
        }
    }

    const QString description() const {
        return m_pControl ?  m_pControl->description() : QString();
    }

    void setDescription(const QString& description) {
        if (m_pControl) {
            m_pControl->setDescription(description);
        }
    }

    // Return the key of the object
    inline ConfigKey getKey() const { return m_key; }
    // Returns the value of the ControlObject
    double get() const;
    // Instantly returns the value of the ControlObject
    static double get(const ConfigKey& key);
    // Sets the ControlObject value. May require confirmation by owner.
    void set(double value);
    // Sets the ControlObject value and confirms it.
    void setAndConfirm(double value);
    // Instantly sets the value of the ControlObject
    static void set(const ConfigKey& key, const double& value);
    // Sets the default value
    void reset();

    inline void setDefaultValue(double dValue) {
        if (m_pControl) {
            m_pControl->setDefaultValue(dValue);
        }
    }
    inline double defaultValue() const {
        return m_pControl ? m_pControl->defaultValue() : 0.0;
    }

    // Connects a Qt slot to a signal that is delivered when a new value change
    // request arrives for this control.
    bool connectValueChangeRequest(const QObject* receiver,
                                   const char* method, Qt::ConnectionType type);

  signals:
    void valueChanged(double);
    void valueChangedFromEngine(double);

  public:
    // DEPRECATED: Called to set the control value from the controller
    // subsystem.
    virtual void setValueFromMidi(MidiOpCode o, double v);
    virtual double getMidiParameter() const;

  protected:
    // Key of the object
    ConfigKey m_key;
    QSharedPointer<ControlDoublePrivate> m_pControl;

  private slots:
    void privateValueChanged(double value, QObject* pSetter);

  private:
    void initialize(ConfigKey key, bool bIgnoreNops, bool bTrack,
                    bool bPersist);
    inline bool ignoreNops() const {
        return m_pControl ? m_pControl->ignoreNops() : true;
    }
};

#endif
