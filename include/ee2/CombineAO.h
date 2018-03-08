#pragma once

#include <ee0/AtomicOP.h>

#include <memory>
#include <vector>

namespace ee2
{

class CombineAO : public ee0::AtomicOP
{
public:
	virtual void Undo() override;
	virtual void Redo() override;

	void Add(const std::shared_ptr<ee0::AtomicOP>& ao);

private:
	std::vector<std::shared_ptr<ee0::AtomicOP>> m_atomics;

}; // CombineAO

}