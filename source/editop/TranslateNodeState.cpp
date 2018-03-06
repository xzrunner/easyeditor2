#include "TranslateNodeState.h"
#include "TranslateSpriteAOP.h"
#include "SpriteSelection.h"
#include "panel_msg.h"
#include "EditSprMsg.h"

namespace ee2
{

TranslateNodeState::TranslateNodeState(SpriteSelection* selection, 
										   const sm::vec2& first_pos)
	: m_dirty(false)
{
	m_selection = selection;
	m_selection->AddReference();

	m_first_pos = m_last_pos = first_pos;
}

TranslateNodeState::~TranslateNodeState()
{
	m_selection->RemoveReference();
}

void TranslateNodeState::OnMousePress(int x, int y)
{
	m_first_pos = m_last_pos = pos;
}

void TranslateNodeState::OnMouseRelease(int x, int y)
{
	m_last_pos.MakeInvalid();
	if (!m_dirty) {
		return;
	}

	m_dirty = false;

	if (m_selection) {
		sm::vec2 offset = pos - m_first_pos;
		AtomicOP* aop = new TranslateSpriteAOP(*m_selection, offset);
		EditAddRecordSJ::Instance()->Add(aop);

		EditSprMsg::Translate(*m_selection, offset);
	}
}

bool TranslateNodeState::OnMouseDrag(int x, int y)
{
	if (m_selection->IsEmpty() || !m_last_pos.IsValid()) {
		return false;
	}

	m_dirty = true;
	Translate(pos - m_last_pos);
	m_last_pos = pos;

	return true;
}

bool TranslateNodeState::OnDirectionKeyDown(int type)
{
	if (m_selection->IsEmpty()) return false;

	sm::vec2 offset;
	float len = (type & KEY_SHIFT) ? 10 : 1;
	switch (type & 0xf)
	{
	case KEY_LEFT:
		offset.Set(-len, 0);
		break;
	case KEY_RIGHT:
		offset.Set(len, 0);
		break;
	case KEY_DOWN:
		offset.Set(0, -len);
		break;
	case KEY_UP:
		offset.Set(0, len);
		break;
	}

	if (offset.x == 0 && offset.y == 0) {
		return false;
	}

	Translate(offset);

	if (m_selection) {
		EditAddRecordSJ::Instance()->Add(new TranslateSpriteAOP(*m_selection, offset));
		EditSprMsg::Translate(*m_selection, offset);
	}

	return true;
}

void TranslateNodeState::Translate(const sm::vec2& offset)
{
	m_selection->Traverse(TranslateVisitor(offset));
}

//////////////////////////////////////////////////////////////////////////
// class TranslateNodeState::TranslateVisitor
//////////////////////////////////////////////////////////////////////////

void TranslateNodeState::TranslateVisitor::
Visit(const SprPtr& spr, bool& next)
{
	spr->Translate(m_offset);
	next = true;
}

}