#include "CKeyboard.h"

bool CKeyboard::KeyIsPressed(unsigned char keycode) const noexcept
{
    return keystates[keycode];
}

CKeyboard::Event CKeyboard::ReadKey() noexcept
{
    if (keybuffer.size() > 0u)
    {
        CKeyboard::Event e = keybuffer.front();
        keybuffer.pop();
        return e;
    }
    else
        return CKeyboard::Event();
}

bool CKeyboard::KeyIsEmpty() noexcept
{
    return keybuffer.empty();
}

void CKeyboard::FlushKey() noexcept
{
    keybuffer = std::queue<Event>();
}

char CKeyboard::ReadChar() noexcept
{
    if (charbuffer.size() > 0u)
    {
        unsigned char charcode = charbuffer.front();
        charbuffer.pop();
        return charcode;
    }
    else
        return 0;
}

bool CKeyboard::CharIsEmpty() noexcept
{
    return charbuffer.empty();
}

void CKeyboard::FlushChar() noexcept
{
    charbuffer = std::queue<char>();
}

void CKeyboard::Flush() noexcept
{
    FlushKey();
    FlushChar();
}

void CKeyboard::EnableAutorepeat() noexcept
{
    autorepeatEnabled = true;
}

void CKeyboard::DisableAutorepeat() noexcept
{
    autorepeatEnabled = false;
}

bool CKeyboard::AutorepeatIsEnabled() const noexcept
{
    return autorepeatEnabled;
}

void CKeyboard::OnKeyPressed(unsigned char keycode) noexcept
{
    keystates[keycode] = true;
    keybuffer.push(CKeyboard::Event(CKeyboard::Event::Type::Release, keycode));
    TrimBuffer(keybuffer);
}

void CKeyboard::OnKeyReleased(unsigned char keycode) noexcept
{
    keystates[keycode] = false;
    keybuffer.push(CKeyboard::Event(CKeyboard::Event::Type::Release, keycode));
    TrimBuffer(keybuffer);
}

void CKeyboard::OnChar(char character) noexcept
{
    charbuffer.push(character);
    TrimBuffer(charbuffer);
}

void CKeyboard::ClearState() noexcept
{
    keystates.reset();
}

template<typename T>
inline void CKeyboard::TrimBuffer(std::queue<T>& buffer) noexcept
{
    while (buffer.size() > bufferSize)
    {
        buffer.pop();
    }
}
