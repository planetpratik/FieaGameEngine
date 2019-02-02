#pragma once
namespace FieaGameEngine
{
	class Bar
	{
	public:
		explicit Bar(int data = 0);
		~Bar() = default;
		int data() const;
		void SetData(int data);
		bool operator==(const Bar& t_rhs) const;
		bool operator!=(const Bar& t_rhs) const;
	private:
		int m_data;
	};
}