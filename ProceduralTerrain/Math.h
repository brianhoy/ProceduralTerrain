#pragma once
class Math {
public:
	static int uid() {
		static int s_id = 0;
		return ++s_id;
	}
};