#include "os_core.h"

String os_key_to_string(const OS_Key key)
{
    switch(key)
    {
    case OS_Key_LeftMouseButton:
        return string_lit("MB1");
    case OS_Key_RightMouseButton:
        return string_lit("MB2");
    case OS_Key_MiddleMouseButton:
        return string_lit("MB3");
    case OS_Key_BackMouseButton:
        return string_lit("MB4");
    case OS_Key_ForwardMouseButton:
        return string_lit("MB5");
    case OS_Key_0:
        return string_lit("0");
    case OS_Key_1:
        return string_lit("1");
    case OS_Key_2:
        return string_lit("2");
    case OS_Key_3:
        return string_lit("3");
    case OS_Key_4:
        return string_lit("4");
    case OS_Key_5:
        return string_lit("5");
    case OS_Key_6:
        return string_lit("6");
    case OS_Key_7:
        return string_lit("7");
    case OS_Key_8:
        return string_lit("8");
    case OS_Key_9:
        return string_lit("9");
    case OS_Key_A:
        return string_lit("A");
    case OS_Key_B:
        return string_lit("B");
    case OS_Key_C:
        return string_lit("C");
    case OS_Key_D:
        return string_lit("D");
    case OS_Key_E:
        return string_lit("E");
    case OS_Key_F:
        return string_lit("F");
    case OS_Key_G:
        return string_lit("G");
    case OS_Key_H:
        return string_lit("H");
    case OS_Key_I:
        return string_lit("I");
    case OS_Key_J:
        return string_lit("J");
    case OS_Key_K:
        return string_lit("K");
    case OS_Key_L:
        return string_lit("L");
    case OS_Key_M:
        return string_lit("M");
    case OS_Key_N:
        return string_lit("N");
    case OS_Key_O:
        return string_lit("O");
    case OS_Key_P:
        return string_lit("P");
    case OS_Key_Q:
        return string_lit("Q");
    case OS_Key_R:
        return string_lit("R");
    case OS_Key_S:
        return string_lit("S");
    case OS_Key_T:
        return string_lit("T");
    case OS_Key_U:
        return string_lit("U");
    case OS_Key_V:
        return string_lit("V");
    case OS_Key_W:
        return string_lit("W");
    case OS_Key_X:
        return string_lit("X");
    case OS_Key_Y:
        return string_lit("Y");
    case OS_Key_Z:
        return string_lit("Z");
    case OS_Key_Semicolon:
        return string_lit("SEMICOLON");
    case OS_Key_Quote:
        return string_lit("QUOTE");
    case OS_Key_Comma:
        return string_lit("COMMA");
    case OS_Key_Period:
        return string_lit("PERIOD");
    case OS_Key_Slash:
        return string_lit("SLASH");
    case OS_Key_LeftBracket:
        return string_lit("LEFT BRACKET");
    case OS_Key_RightBracket:
        return string_lit("RIGHT BRACKET");
    case OS_Key_BackSlash:
        return string_lit("BACK SLASH");
    case OS_Key_Backspace:
        return string_lit("BACKSPACE");
    case OS_Key_BackTick:
        return string_lit("BACKTICK");
    case OS_Key_Minus:
        return string_lit("MINUS");
    case OS_Key_Equal:
        return string_lit("EQUAL");
    case OS_Key_Tab:
        return string_lit("TAB");
    case OS_Key_Return:
        return string_lit("RETURN");
    case OS_Key_Shift:
        return string_lit("SHIFT");
    case OS_Key_Ctrl:
        return string_lit("CTRL");
    case OS_Key_Alt:
        return string_lit("ALT");
    case OS_Key_Pause:
        return string_lit("PAUSE");
    case OS_Key_CapsLock:
        return string_lit("CAPS LOCK");
    case OS_Key_Esc:
        return string_lit("ESC");
    case OS_Key_Space:
        return string_lit("SPACE");
    case OS_Key_PageUp:
        return string_lit("PAGE UP");
    case OS_Key_PageDown:
        return string_lit("PAGE DOWN");
    case OS_Key_End:
        return string_lit("END");
    case OS_Key_Home:
        return string_lit("HOME");
    case OS_Key_Select:
        return string_lit("SELECT");
    case OS_Key_Insert:
        return string_lit("INSERT");
    case OS_Key_Delete:
        return string_lit("DELETE");
    case OS_Key_ScrollLock:
        return string_lit("SCROLL LOCK");
    case OS_Key_Left:
        return string_lit("LEFT ARROW");
    case OS_Key_Right:
        return string_lit("RIGHT ARROW");
    case OS_Key_Up:
        return string_lit("UP ARROW");
    case OS_Key_Down:
        return string_lit("DOWN ARROW");
    case OS_Key_Num0:
        return string_lit("NUMPAD 0");
    case OS_Key_Num1:
        return string_lit("NUMPAD 1");
    case OS_Key_Num2:
        return string_lit("NUMPAD 2");
    case OS_Key_Num3:
        return string_lit("NUMPAD 3");
    case OS_Key_Num4:
        return string_lit("NUMPAD 4");
    case OS_Key_Num5:
        return string_lit("NUMPAD 5");
    case OS_Key_Num6:
        return string_lit("NUMPAD 6");
    case OS_Key_Num7:
        return string_lit("NUMPAD 7");
    case OS_Key_Num8:
        return string_lit("NUMPAD 8");
    case OS_Key_Num9:
        return string_lit("NUMPAD 9");
    case OS_Key_NumLock:
        return string_lit("NUM LOCK");
    case OS_Key_NumSlash:
        return string_lit("NUM SLASH");
    case OS_Key_NumStar:
        return string_lit("NUM STAR");
    case OS_Key_NumMinus:
        return string_lit("NUM MINUS");
    case OS_Key_NumPlus:
        return string_lit("NUM PLUS");
    case OS_Key_NumPeriod:
        return string_lit("NUM PERIOD");
    case OS_Key_F1:
        return string_lit("F1");
    case OS_Key_F2:
        return string_lit("F2");
    case OS_Key_F3:
        return string_lit("F3");
    case OS_Key_F4:
        return string_lit("F4");
    case OS_Key_F5:
        return string_lit("F5");
    case OS_Key_F6:
        return string_lit("F6");
    case OS_Key_F7:
        return string_lit("F7");
    case OS_Key_F8:
        return string_lit("F8");
    case OS_Key_F9:
        return string_lit("F9");
    case OS_Key_F10:
        return string_lit("F10");
    case OS_Key_F11:
        return string_lit("F11");
    case OS_Key_F12:
        return string_lit("F12");
    case OS_Key_F13:
        return string_lit("F13");
    case OS_Key_F14:
        return string_lit("F14");
    case OS_Key_F15:
        return string_lit("F15");
    case OS_Key_F16:
        return string_lit("F16");
    case OS_Key_F17:
        return string_lit("F17");
    case OS_Key_F18:
        return string_lit("F18");
    case OS_Key_F19:
        return string_lit("F19");
    case OS_Key_F20:
        return string_lit("F20");
    case OS_Key_F21:
        return string_lit("F21");
    case OS_Key_F22:
        return string_lit("F22");
    case OS_Key_F23:
        return string_lit("F23");
    case OS_Key_F24:
        return string_lit("F24");
    case OS_Key_Misc0:
        return string_lit("MISC 0");
    case OS_Key_Misc1:
        return string_lit("MISC 1");
    case OS_Key_Misc2:
        return string_lit("MISC 2");
    case OS_Key_Misc3:
        return string_lit("MISC 3");
    case OS_Key_Misc4:
        return string_lit("MISC 4");
    case OS_Key_Misc5:
        return string_lit("MISC 5");
    case OS_Key_Misc6:
        return string_lit("MISC 6");
    case OS_Key_Misc7:
        return string_lit("MISC 7");
    case OS_Key_Misc8:
        return string_lit("MISC 8");
    case OS_Key_Misc9:
        return string_lit("MISC 9");
    case OS_Key_Misc10:
        return string_lit("MISC 10");
    case OS_Key_Misc11:
        return string_lit("MISC 11");
    case OS_Key_Misc12:
        return string_lit("MISC 12");
    case OS_Key_Misc13:
        return string_lit("MISC 13");
    case OS_Key_Misc14:
        return string_lit("MISC 14");
    case OS_Key_Misc15:
        return string_lit("MISC 15");
    case OS_Key_Misc16:
        return string_lit("MISC 16");
    case OS_Key_Misc17:
        return string_lit("MISC 17");
    case OS_Key_Misc18:
        return string_lit("MISC 18");
    case OS_Key_Misc19:
        return string_lit("MISC 19");
    case OS_Key_Misc20:
        return string_lit("MISC 20");
    case OS_Key_Misc21:
        return string_lit("MISC 21");
    case OS_Key_Misc22:
        return string_lit("MISC 22");
    case OS_Key_Misc23:
        return string_lit("MISC 23");
    case OS_Key_Misc24:
        return string_lit("MISC 24");
    case OS_Key_Misc25:
        return string_lit("MISC 25");
    case OS_Key_Misc26:
        return string_lit("MISC 26");
    case OS_Key_Misc27:
        return string_lit("MISC 27");
    case OS_Key_Misc28:
        return string_lit("MISC 28");
    case OS_Key_Misc29:
        return string_lit("MISC 29");
    case OS_Key_Null:
    default:
        return string_lit("Unkown Key");

    }
}

String os_event_type_to_string(const OS_EventType event_type)
{
    switch(event_type)
    {
    case OS_EventType_WindowClose:
        return string_lit("Window Close");
    case OS_EventType_WindowResize:
        return string_lit("Window Resize");
    case OS_EventType_WindowFocus:
        return string_lit("Window Focus");
    case OS_EventType_WindowLostFocus:
        return string_lit("Window Lost Focus");
    case OS_EventType_WindowMoved:
        return string_lit("Window Moved");
    case OS_EventType_KeyPressed:
        return string_lit("Key Pressed");
    case OS_EventType_KeyReleased:
        return string_lit("Key Released");
    case OS_EventType_KeyTyped:
        return string_lit("Key Typed");
    case OS_EventType_MouseButtonPressed:
        return string_lit("Mouse Button Pressed");
    case OS_EventType_MouseButtonReleased:
        return string_lit("Mouse Button Released");
    case OS_EventType_MouseMoved:
        return string_lit("Mouse Moved");
    case OS_EventType_MouseScrolled:
        return string_lit("Mouse Scrolled");
    case OS_EventType_Null:
    default:
        return string_lit("Unkown Event");

    }
}

String os_event_to_string(Arena *arena, const OS_Event *const event)
{
    String out = {};
    switch(event->type)
    {
    case OS_EventType_WindowClose:
        out = string_lit("Window Close");
        break;
    case OS_EventType_WindowResize: // TODO: dimensions
    {
        return string_lit("Window Resize");
        break;
    }
    case OS_EventType_WindowFocus:
        return string_lit("Window Focus");
    case OS_EventType_WindowLostFocus:
        return string_lit("Window Lost Focus");
    case OS_EventType_WindowMoved: // TODO: position
    {
        return string_lit("Window Moved");
        break;
    }
    case OS_EventType_KeyPressed:
    {
        String key = os_key_to_string(event->key);
        out = string_format(arena, "Key Pressed %S", key);
        break;
    }
    case OS_EventType_KeyReleased:
    {
        String key = os_key_to_string(event->key);
        out = string_format(arena, "Key Released %S", key);
        break;
    }
    case OS_EventType_KeyTyped:
    {
        out = string_format(arena, "Key Typed %c", event->character);
        break;
    }
    case OS_EventType_MouseButtonPressed:
    {
        String key = os_key_to_string(event->key);
        out = string_format(arena, "Mouse Pressed %S", key);
        break;
    }
    case OS_EventType_MouseButtonReleased:
    {
        String key = os_key_to_string(event->key);
        out = string_format(arena, "Mouse Released %S", key);
        break;
    }
    case OS_EventType_MouseMoved:
    {
        out = string_format(arena, "Mouse Moved (%.0f, %.0f)", event->pos.x, event->pos.y);
        break;
    }
    case OS_EventType_MouseScrolled:
    {
        out = string_format(arena, "Mouse Scrolled (%.0f, %.0f)", event->delta.x, event->delta.y);
        break;
    }
    case OS_EventType_Null:
    default:
        return string_lit("Unkown Event");
    }

    return out;
}

OS_Event *os_push_event(Arena **arena, OS_State *os_state, OS_EventType eventType, OS_WindowID window_id)
{
    OS_Event *event = nullptr;
    for(OS_Window *itr = os_state->first_window; itr; itr = itr->next)
    {
        if (itr->id == window_id)
        {
            os_mutex_lock(itr->mutex);

            u32 idx = itr->write_index;
            event = (OS_Event *)arena_alloc_align(arena[itr->write_index], sizeof(OS_Event), sizeof(OS_Event));

            linked_list_push_back(itr->event_queue[idx].first, itr->event_queue[idx].last, event);
            event->type = eventType;
            itr->event_queue[idx].count += 1;

            os_mutex_unlock(itr->mutex);
            break;
        }
    }

    return event;
}

// TODO: Refactor with window specic event queues in mind
OS_Event *os_pop_event()
{
    ASSERT(os_event_queue.first != nullptr);

    OS_Event *event;

    event = os_event_queue.first;
    if (os_event_queue.count == 1)
    {
        os_event_queue.first = nullptr;
        os_event_queue.last = nullptr;
    }
    else
    {
        os_event_queue.first = event->next;
    }

    os_event_queue.count -= 1;
    return event;
}


OS_WindowID os_generate_window_id()
{
    static OS_WindowID window_id_counter = 0;

    return window_id_counter++;
}

OS_Window *os_get_window_from_id(OS_WindowID window_id)
{
    for(OS_Window *itr = global_os_state().first_window; itr; itr = itr->next)
    {
        if (itr->id == window_id)
        {
            return itr;
        }
    }

    return nullptr;
}

OS_State &global_os_state()
{
    static OS_State global_os_state = {};

    return global_os_state;
}