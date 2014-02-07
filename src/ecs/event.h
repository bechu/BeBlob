/////////////////////////////////////////////////////////////////////////
/// BeBlob
/// Copyright (C) 2014 Jérôme Béchu
///
/// This program is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.
///
/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.
///
/// You should have received a copy of the GNU General Public License
/// along with this program.  If not, see <http://www.gnu.org/licenses/>.
/////////////////////////////////////////////////////////////////////////

#ifndef ECS_EVENT_H
#define ECS_EVENT_H

#include <memory>
#include <list>
#include <map>
#include <queue>
#include <unordered_map>
#include <cstdint>

#include "simplesignal.h"
#include "classid.h"

namespace ecs
{

class BaseEvent
{
public:
    typedef std::shared_ptr<BaseEvent> Ptr;
    virtual ~BaseEvent() {}
protected:
};

typedef std::queue<BaseEvent> Events;

typedef Simple::Signal<void (const BaseEvent*)> EventSignal;
typedef std::shared_ptr<EventSignal> EventSignalPtr;
typedef std::weak_ptr<EventSignal> EventSignalWeakPtr;

template <typename Derived>
class Event : public BaseEvent
{
public:
    typedef std::shared_ptr<Event<Derived>> Ptr;

    static TypeId getTypeId()
    {
        return ClassTypeId<BaseEvent>::getTypeId<Derived>();
    }

};

class BaseReceiver {
public:
    virtual ~BaseReceiver() {
        for (auto connection : connections_) {
            auto &ptr = connection.first;
            if (!ptr.expired()) {
                ptr.lock()->disconnect(connection.second);
            }
        }
    }

    int connected_signals() const {
        size_t size = 0;
        for (auto connection : connections_) {
            if (!connection.first.expired()) {
                size++;
            }
        }
        return size;
    }

private:
    friend class EventManager;
    std::list<std::pair<EventSignalWeakPtr, size_t>> connections_;
};


class EventManager
        : public std::enable_shared_from_this<EventManager>
{
public:
    typedef std::shared_ptr<EventManager> Ptr;

    template <typename E, typename Receiver>
    void subscribe(Receiver &receiver)
    {
        void (Receiver::*receive)(const E &) = &Receiver::receive;
        auto sig = signal_for(E::getTypeId());
        auto  wrapper = EventCallbackWrapper<E>(std::bind(receive, &receiver, std::placeholders::_1));
        auto connection = sig->connect(wrapper);
        static_cast<BaseReceiver&>(receiver).connections_.push_back(
                    std::make_pair(EventSignalWeakPtr(sig), connection));
    }

    template <typename E, typename ... Args>
    void emit(Args && ... args)
    {
        E event(std::forward<Args>(args) ...);
        auto sig = signal_for(E::getTypeId());
        sig->emit(static_cast<BaseEvent*>(&event));
    }

    template <typename E>
    void emit(E event)
    {
        auto sig = signal_for(E::getTypeId());
        sig->emit(static_cast<BaseEvent*>(&event));
    }

    void emit(TypeId type, std::shared_ptr<BaseEvent> event)
    {
        auto sig = signal_for(type);
        sig->emit(event.get());
    }

    EventManager() {}
private:

    EventSignalPtr signal_for(int id) {
        auto it = handlers_.find(id);
        if (it == handlers_.end()) {
            EventSignalPtr sig(std::make_shared<EventSignal>());
            handlers_.insert(std::make_pair(id, sig));
            return sig;
        }
        return it->second;
    }

    template <typename E>
    struct EventCallbackWrapper
    {
        EventCallbackWrapper(std::function<void(const E &)> callback) : callback(callback) {}
        void operator()(const BaseEvent* event) { callback(*(static_cast<const E*>(event))); }
        std::function<void(const E &)> callback;
    };

    std::unordered_map<int, EventSignalPtr> handlers_;
};

class Receiver
        : public BaseReceiver
{
public:
    virtual ~Receiver() {}

    template <typename E, typename ... Args>
    void emit(Args && ... args)
    {
        E event(std::forward<Args>(args) ...);
        if(event_manager_)
        {
            event_manager_->emit<E>(event);
        }
        else throw ;
    }

    void emit(TypeId type, std::shared_ptr<BaseEvent> event)
    {
        if(event_manager_)
        {
            event_manager_->emit(type, event);
        }
        else throw ;
    }

    //private:
    friend class EventManager;
    std::shared_ptr<EventManager> event_manager_;
};

typedef Receiver Emitter;

}

#endif // ECS_EVENT_H
