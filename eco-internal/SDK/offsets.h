#pragma once

namespace offsets
{
	inline int static_padding = 0xB8;

	namespace Player {
		int update_RVA = 0xB64340;
		int toggle_fly_RVA = 0xB64150;
		int set_position_RVA = 0xB63B10;
		int get_motor_RVA = 0xB65B80;
	}
}