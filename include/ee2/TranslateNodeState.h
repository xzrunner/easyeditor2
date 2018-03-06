#pragma once

#include <ee0/EditOpState.h>
#include <ee0/SelectionSet.h>

#include <SM_Vector.h>
#include <node0/typedef.h>

namespace pt2 { class Camera; }

namespace ee2
{

class TranslateNodeState : public ee0::EditOpState
{
public:
	TranslateNodeState(pt2::Camera& cam, 
		const ee0::SelectionSet<n0::SceneNode>& selection, 
		const sm::vec2& first_pos);

	virtual bool OnMousePress(int x, int y) override;
	virtual bool OnMouseRelease(int x, int y) override;
	virtual	bool OnMouseDrag(int x, int y) override;

	virtual bool OnDirectionKeyDown(int type) override;

private:
	void Translate(const sm::vec2& offset);

private:
	pt2::Camera& m_cam;

	const ee0::SelectionSet<n0::SceneNode>& m_selection;

	sm::vec2 m_first_pos, m_last_pos;

	bool m_dirty;
	
}; // TranslateState

}