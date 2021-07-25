#pragma once
#include <queue>
#include <bitset>
class CKeyboard
{
	friend class CWindow;
public:
	// Internal Event Class
	class Event
	{
	public:
		enum class Type
		{
			Press,
			Release, 
			Invalid
		};
	private:
		Type type;
		unsigned char code;
	public:
		Event() noexcept : type(Type::Invalid), code(0u) {}
		Event( Type type, unsigned char code) noexcept : type(type), code(code) {}

		bool IsPress() const noexcept { return type == Type::Press; }
		bool IsRelease() const noexcept { return type == Type::Release; }
		bool IsInvalid() const noexcept { return type == Type::Invalid; }
		unsigned char GetCode() const noexcept { return code; }

	};

	CKeyboard() = default;
	CKeyboard(const CKeyboard&) = delete;
	CKeyboard& operator= (const CKeyboard&) = delete;
	// KeyEvent
	bool KeyIsPressed(unsigned char keycode) const noexcept;
	Event ReadKey() noexcept;
	bool KeyIsEmpty() noexcept;
	void FlushKey() noexcept;
	// CharEvent
	char ReadChar() noexcept;
	bool CharIsEmpty() noexcept;
	void FlushChar() noexcept;
	void Flush() noexcept;
	// Autorep control
	void EnableAutorepeat() noexcept;
	void DisableAutorepeat() noexcept;
	bool AutorepeatIsEnabled() const noexcept;
private:
	void OnKeyPressed(unsigned char keycode) noexcept;
	void OnKeyReleased(unsigned char keycode) noexcept;
	void OnChar(char character) noexcept;
	void ClearState() noexcept;
	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer) noexcept;

	static constexpr unsigned int nKeys = 256u;
	static constexpr unsigned int bufferSize = 16u;
	bool autorepeatEnabled = false;
	std::bitset<nKeys> keystates;
	std::queue<Event> keybuffer;
	std::queue<char> charbuffer;
};
