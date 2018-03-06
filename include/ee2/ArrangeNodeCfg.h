#pragma once

namespace ee2
{

class ArrangeNodeCfg
{
public:
	bool is_auto_align_open;

	bool is_deform_open;
	bool is_offset_open;
	bool is_rotate_open;

	ArrangeNodeCfg() {
		is_auto_align_open = true;
		is_deform_open = true;
		is_offset_open = true;
		is_rotate_open = true;
	}

	ArrangeNodeCfg(bool auto_align, bool deform, bool offset, bool rotate) {
		is_auto_align_open = auto_align;
		is_deform_open = deform;
		is_offset_open = offset;
		is_rotate_open = rotate;
	}

	static const float CTRL_NODE_RADIUS;
	static const float MAX_CTRL_NODE_RADIUS;

}; // ArrangeNodeCfg

}