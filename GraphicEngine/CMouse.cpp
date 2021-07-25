#include "CMouse.h"
#include "CWin.h"

std::pair<int, int> CMouse::GetPos() const noexcept
{
    return {x, y};
}

int CMouse::GetPosX() const noexcept
{
    return x;
}

int CMouse::GetPosY() const noexcept
{
    return y;
}

bool CMouse::IsInWindow() const noexcept
{
    return isInWindow;
}

bool CMouse::LeftIsPressed() const noexcept
{
    return leftIsPressed;
}

bool CMouse::RightIsPressed() const noexcept
{
    return rightIsPressed;
}

CMouse::Event CMouse::Read() noexcept
{
    if (buffer.size() > 0u)
    {
        CMouse::Event e = buffer.front();
        buffer.pop();
        return e;
    }
    else
        return CMouse::Event();
}

void CMouse::Flush() noexcept
{
    buffer = std::queue<Event>();
}

void CMouse::OnMouseMove(int newx, int newy) noexcept
{
    x = newx;
    y = newy;

    buffer.push(CMouse::Event(CMouse::Event::Type::Move, *this));
    TrimBuffer();
}

void CMouse::OnMouseEnter() noexcept
{
    isInWindow = true;

    buffer.push(CMouse::Event(CMouse::Event::Type::Enter, *this));
    TrimBuffer();
}

void CMouse::onMouseLeave() noexcept
{
    isInWindow = false;

    buffer.push(CMouse::Event(CMouse::Event::Type::Leave, *this));
    TrimBuffer();
}

void CMouse::OnLeftPressed(int x, int y) noexcept
{
    leftIsPressed = true;

    buffer.push(CMouse::Event(CMouse::Event::Type::LPress, *this));
    TrimBuffer();
}

void CMouse::OnLeftReleased(int x, int y) noexcept
{
    leftIsPressed = false;

    buffer.push(CMouse::Event(CMouse::Event::Type::LRelease, *this));
    TrimBuffer();
}

void CMouse::OnRightPressed(int x, int y) noexcept
{
    rightIsPressed = true;

    buffer.push(CMouse::Event(CMouse::Event::Type::RPress, *this));
    TrimBuffer();
}

void CMouse::OnRightReleased(int x, int y) noexcept
{
    rightIsPressed = false;

    buffer.push(CMouse::Event(CMouse::Event::Type::RRelease, *this));
    TrimBuffer();
}

void CMouse::OnWheelUp(int x, int y) noexcept
{
    buffer.push(CMouse::Event(CMouse::Event::Type::WheelUp, *this));
    TrimBuffer();
}

void CMouse::OnWheelDown(int x, int y) noexcept
{
    buffer.push(CMouse::Event(CMouse::Event::Type::WheelDown, *this));
    TrimBuffer();
}

void CMouse::OnWheelDelta(int x, int y, int delta) noexcept
{
    // The delta was set to 120 to allow Microsoft or other vendors to build finer-resolution wheels (a freely-rotating wheel with no notches)
    // to send more messages per rotation, but with a smaller value in each message.

    wheelDeltaCarry += delta;
    // Generate events for every 120
    while (wheelDeltaCarry >= WHEEL_DELTA)
    {
        wheelDeltaCarry -= WHEEL_DELTA;
        OnWheelUp(x, y);
    }
    while (wheelDeltaCarry <= -WHEEL_DELTA)
    {
        wheelDeltaCarry += WHEEL_DELTA;
        OnWheelDown(x, y);
    }
}

void CMouse::TrimBuffer() noexcept
{
    while (buffer.size() > bufferSize)
    {
        buffer.pop();
    }
}
