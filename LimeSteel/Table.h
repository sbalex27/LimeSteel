#pragma once

class TableFile
{
public:
	/// <summary>
	/// Create the physical table
	/// </summary>
	virtual void up() = 0;
	/// <summary>
	/// Drop the physical table
	/// </summary>
	virtual void down() = 0;

	void refresh() {
		this->down();
		this->up();
	}
};
