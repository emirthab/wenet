#ifndef EVENT_CONNECTION_REQUEST_H
#define EVENT_CONNECTION_REQUEST_H

#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include "../event_base.h"

namespace godot
{
    class EventConnectionRequest : public EventBase
    {
        GDCLASS(EventConnectionRequest, EventBase)

    private:
    protected:
        static void _bind_methods();

    public:
        EventConnectionRequest(){};
        ~EventConnectionRequest(){};
        
        virtual void _handle(Array packet, Client *client) override;
        String header = "CON_REQ";
    };
}
#endif
