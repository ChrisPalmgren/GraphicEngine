#pragma once
#include <queue>
class CMouse
{
	friend class CWindow;
public:
	class Event
	{
	public:
		enum class Type
		{
			LPress,
			LRelease,
			RPress,
			RRelease,
			WheelUp,
			WheelDown,
			Move,
			Enter,
			Leave,
			Invalid
		};
	private:
		Type type;
		bool leftIsPressed;
		bool rightIsPressed;
		int x, y;
	public:
		Event() noexcept 
			: type(Type::Invalid), leftIsPressed(false), rightIsPressed(false), x(0), y(0) {}
		Event(Type type, const CMouse& parent) noexcept 
			: type(type), leftIsPressed(parent.leftIsPressed), rightIsPressed(parent.rightIsPressed),
			x(parent.x), y(parent.y) {}

		bool IsValid() const noexcept { return type != Type::Invalid; }
		Type GetType() const noexcept { return type; }
		std::pair<int, int> GetPos() const noexcept { return { x, y }; }
		int GetPosX() const noexcept { return x; }
		int GetPosY() const noexcept { return y; }
		bool LeftIsPressed() const noexcept { return leftIsPressed; }
		bool RightIsPressed() const noexcept { return rightIsPressed; }
	};
public:
	CMouse() = default;
	CMouse(const CMouse&) = delete;
	CMouse& operator= (const CMouse&) = delete;

	std::pair<int, int> GetPos() const noexcept;
	int GetPosX() const noexcept;
	int GetPosY() const noexcept;
	bool IsInWindow() const noexcept;
	bool LeftIsPressed() const noexcept;
	bool RightIsPressed() const noexcept;
	CMouse::Event Read() noexcept;
	bool IsEmpty() const noexcept { return buffer.empty(); }
	void Flush() noexcept;

private:
	void OnMouseMove(int newx, int newy) noexcept;
	void OnMouseEnter() noexcept;
	void onMouseLeave() noexcept;
	void OnLeftPressed(int x, int y) noexcept;
	void OnLeftReleased(int x, int y) noexcept;
	void OnRightPressed(int x, int y) noexcept;
	void OnRightReleased(int x, int y) noexcept;
	void OnWheelUp(int x, int y) noexcept;
	void OnWheelDown(int x, int y) noexcept;
	void OnWheelDelta(int x, int y, int delta) noexcept;
	void TrimBuffer() noexcept;

	static constexpr unsigned int bufferSize = 16u;
	int x;
	int y;
	bool isInWindow = false;
	bool leftIsPressed = false;
	bool rightIsPressed = false;
	int wheelDeltaCarry = 0;
	std::queue<Event> buffer;
};

