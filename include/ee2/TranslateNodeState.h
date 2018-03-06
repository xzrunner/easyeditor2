#ifndef _EASYEDITOR_TRANSLATE_SPRITE_STATE_H_
#define _EASYEDITOR_TRANSLATE_SPRITE_STATE_H_

#include "ArrangeNodeState.h"
#include "Visitor.h"
#include "Sprite.h"

namespace ee2
{

class SpriteSelection;

class TranslateNodeState : public ee0::EditOpState
{
public:
	TranslateNodeState(SpriteSelection* selection, const sm::vec2& first_pos);
	virtual ~TranslateNodeState();

	virtual bool OnMousePress(int x, int y) override;
	virtual bool OnMouseRelease(int x, int y) override;
	virtual	bool OnMouseDrag(int x, int y) override;

	virtual bool OnDirectionKeyDown(int type) override;

protected:
	virtual void Translate(const sm::vec2& offset);

protected:
	SpriteSelection* GetSelection() { return m_selection; } 

private:
	class TranslateVisitor : public RefVisitor<Sprite>
	{
	public:
		TranslateVisitor(const sm::vec2& offset) : m_offset(offset) {}
		virtual void Visit(const SprPtr& spr, bool& next) override;
	private:
		sm::vec2 m_offset;
	}; // TranslateVisitor

private:
	SpriteSelection* m_selection;

	sm::vec2 m_first_pos, m_last_pos;

	bool m_dirty;
	
}; // TranslateState

}

#endif // _EASYEDITOR_TRANSLATE_SPRITE_STATE_H_